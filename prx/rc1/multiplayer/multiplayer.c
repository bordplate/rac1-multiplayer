#include "multiplayer.h"

#include <netex/net.h>

#include <rc1/hud.h>

#include "server_list.h"

int handshake_complete = 0;

char waiting_for_weapon = 0;

void mp_start() {
	// Need to initialize networking before we're allowed to open sockets and stuff.
	// You can technically bypass it by syscalling directly, but that brings other 
	// 		bugs, so this is easier. 
	sys_net_initialize_network();
	
	memset(&mp_mobys, 0, sizeof(mp_mobys));
    memset(&mp_acked, 0, sizeof(mp_acked));
    memset(&mp_unacked, 0, sizeof(mp_unacked));

    mp_connection_directory_mode = 0;

    game_server_last_refresh = -1;
}

void mp_send(void* buffer, size_t len) {
    sendto(mp_sock, buffer, len, MSG_DONTWAIT, (const struct sockaddr *)&mp_sockaddr, sizeof(struct sockaddr_in));
}

void mp_resend_unacked(int ack_id) {
    mp_unacked_metadata* unacked = &mp_unacked[ack_id];

    if (unacked->data) {
        // Resend the data
        mp_send(unacked->data, unacked->len);

        MULTI_TRACE("Resent packet %d, len %d, flags %d, cycle %d\n", ((MPPacketHeader*)unacked->data)->requires_ack, unacked->len, ((MPPacketHeader*)unacked->data)->flags, ((MPPacketHeader*)unacked->data)->ack_cycle);
    }

    if (unacked->next_unacked != 0) {
        mp_resend_unacked(unacked->next_unacked);
    }
}

void mp_ack(char* packet, size_t len) {
    MPPacketHeader* header = (MPPacketHeader*)packet;

    unsigned char ack_id = header->requires_ack;

    mp_unacked_metadata* unacked = &mp_unacked[ack_id];

    if (unacked->data) {
        // If the ack packet has data, call the event handler for this packet
        if (header->size > 0 && unacked->ack_cb) {
            unacked->ack_cb(&packet[sizeof(MPPacketHeader)], header->size);
        }

        MULTI_TRACE("Acked message %d with size %d\n", header->type, header->size);
    }

    // FIXME: Don't know how to free data.
    //__free((void*)0x90d7c8, unacked->data);
    unacked->data = 0;

    if (unacked->next_unacked) {
        mp_unacked_metadata* next_unacked = &mp_unacked[unacked->next_unacked];

        if (next_unacked->data == 0 && next_unacked->next_unacked != 0) {
            unacked->next_unacked = next_unacked->next_unacked;
        } else if (next_unacked->data == 0 && next_unacked->next_unacked == 0) {
            unacked->next_unacked = 0;
        }
    }
}

// Open socket and connect to the server. 
void mp_connect() {
	MULTI_LOG("Starting multiplayer...\n");
	MULTI_LOG("Opening socket...\n");
	mp_sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (mp_sock <= 0) {
		MULTI_LOG("Couldn't open socket: %d\n", mp_sock);
	}

    if (!mp_sockaddr.sin_addr.s_addr) {
        mp_sockaddr.sin_addr.s_addr = inet_addr("10.9.0.212");
        mp_sockaddr.sin_port = htons(2407);
        mp_sockaddr.sin_len = sizeof(mp_sockaddr);
        mp_sockaddr.sin_family = AF_INET;
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(mp_sockaddr.sin_addr), ip, INET_ADDRSTRLEN);

	MULTI_LOG("Connecting to %s:%d...\n", ip, mp_sockaddr.sin_port);
}

Moby* mp_spawn_moby(u16 uuid, int o_class, u32 flags) {
    // If the main hero moby isn't spawned yet, we shouldn't try to spawn anything.
    if (!ratchet_moby) {
        return 0;
    }

	// Check that we're not trying to update a moby beyond our predefined moby space. 
	if (uuid > sizeof(mp_mobys)/sizeof(mp_mobys[0])) {
		MULTI_LOG("Tried to spawn illegal Moby UUID: %d\n", uuid);
		return 0;
	}

	MULTI_TRACE("Spawning moby oClass: %d\n", o_class);
	
	Moby* moby = spawn_moby(o_class);
	
	MULTI_TRACE("Moby address: %d\n", moby);

    // Set new moby's update func to our generic MP update func
    if (!(flags & MP_MOBY_FLAG_ORIG_UDPATE_FUNC)) {
        moby->pUpdate = (void *)0x710ed8;
    }

	MPMobyVars* vars = (MPMobyVars*)(moby->pVars);
    if (vars) {
        vars->uuid = uuid;
        vars->o_class = o_class;
    }

	moby->enabled = 1;
	moby->draw_distance = 0xff;
	moby->update_distance = 0xff;
	moby->alpha = 0xff;

	moby->mode_bits = 0x10 | 0x20 | 0x400 | 0x1000 | 0x4000;
	
	mp_mobys[uuid] = moby;
	
	MULTI_LOG("Spawned Moby (oClass: %d, uuid: %d)\n", o_class, uuid);

    idk(moby);
	
	return moby;
}


void mp_update_moby(MPPacketMobyUpdate* update_packet) {
	// Check that we're not trying to update a moby beyond our predefined moby space. 
	if (update_packet->uuid > sizeof(mp_mobys)/sizeof(mp_mobys[0])) {
        MULTI_LOG("Tried to update illegal Moby UUID: %d\n", update_packet->uuid);
        return;
    }
	
	if (game_state != PlayerControl) {
		MULTI_TRACE("Moby not updating because game state %d\n", game_state);
	}

    if (current_planet != update_packet->level) {
        MULTI_TRACE("Server sent update packet for a moby on a different level\n");
        return;
    }

	Moby* moby = mp_mobys[update_packet->uuid];
	
	if (!moby || moby->state < 0) {
		// Spawn moby
		MULTI_LOG("Spawning Moby (oClass: %d) at (%f, %f, %f)\n", update_packet->o_class, update_packet->x, update_packet->y, update_packet->z);
		moby = mp_spawn_moby(update_packet->uuid, update_packet->o_class, update_packet->flags);

        if (update_packet->parent && update_packet->parent != update_packet->uuid) {
            Moby* parent_moby = mp_mobys[update_packet->uuid];
            if (parent_moby) {
                MULTI_LOG("Set moby (uid: %d) parent to (uid: %d)\n", update_packet->uuid, update_packet->parent);
                moby->field80_0xb8 = parent_moby;
            } else {
                MULTI_LOG("Failed to find moby (uid: %d) to attach to parent (uid: %d)\n", update_packet->uuid, update_packet->parent);
            }
        }
		
		if (!moby) return;
	}

    if (update_packet->flags & MP_MOBY_FLAG_NO_COLLISION) {
        moby->collision = 0;
    }
	
    moby->position.x = update_packet->x;
    moby->position.y = update_packet->y;
    moby->position.z = update_packet->z;
    moby->rotation.z = update_packet->rotation;

    MPMobyVars* vars = (MPMobyVars*)(moby->pVars);
    if (vars) {
        vars->next_animation_id = (char)update_packet->animation_id;
        vars->animation_duration = (int)update_packet->animation_duration;
    }
}

void mp_delete_moby(MPPacketMobyCreate *delete_packet) {
    Moby* moby = mp_mobys[delete_packet->uuid];
    if (!moby || moby->state < 0) {
        MULTI_LOG("Not deleting moby %d @ 0x%08x\n", delete_packet->uuid, moby);
        return;
    }

    MULTI_LOG("Deleting moby %d\n", delete_packet->uuid);

    delete_moby(moby);
    mp_mobys[delete_packet->uuid] = 0;
}

void mp_delete_all_mobys() {
    for(int i = 0; i <= sizeof(mp_mobys)/sizeof(mp_mobys[0])-1; i++) {
        Moby* moby = mp_mobys[i];
        if (moby && moby->state > 0) {
            MULTI_LOG("Deleting moby %d", i);
            delete_moby(moby);
            mp_mobys[i] = 0;
        }
    }
}

Moby* mp_get_moby(int uuid) {
    if (uuid > sizeof(mp_mobys)) {
        return 0;
    }

    Moby* moby = mp_mobys[uuid];

    if (moby && moby->state >= 0) {
        return moby;
    }

    return 0;
}

int mp_spawn_weapon_cb(void* data, size_t len) {
    MPPacketMobyCreate* packet = (MPPacketMobyCreate*)data;

    mp_current_weapon_uuid = packet->uuid;

    MULTI_LOG("Updated current weapon uuid: %d\n", packet->uuid);

    waiting_for_weapon = 0;

    return 1;
}

void mp_on_spawn() {

}

void mp_reset_environment() {
    mp_delete_all_mobys();
	memset(&mp_mobys, 0, sizeof(mp_mobys));

    hud_clear();

    mp_on_spawn();
}

void mp_server_reset() {
	MULTI_LOG("Server reset\n");

    mp_delete_all_mobys();

    memset(&mp_mobys, 0, sizeof(mp_mobys));
    memset(&mp_acked, 0, sizeof(mp_acked));
    memset(&mp_unacked, 0, sizeof(mp_unacked));

    mp_current_weapon_uuid = 0;
}

void mp_set_state(MPPacketSetState* packet) {
    switch(packet->state_type) {
        case MP_STATE_TYPE_DAMAGE: {
            player_health -= (int)packet->value;
            break;
        }
        case MP_STATE_TYPE_PLAYER: {
            transition_to_movement_state(packet->value, 1);
            break;
        }
        case MP_STATE_TYPE_POSITION: {
            if (packet->offset > 3) {
                MULTI_LOG("Server tried to set position at invalid offset %d\n", packet->offset);
                break;
            }

            ((float*)&player_pos)[packet->offset] = ((MPPacketSetStateFloat*)packet)->value;
            break;
        }
        case MP_STATE_TYPE_PLANET: {
            if (current_planet != (int)packet->value && destination_planet != (int)packet->value) {
                MULTI_LOG("Going to planet %d\n", (int) packet->value);
                seen_planets[0] = 1;
                seen_planets[packet->value] = 1;
                *(int *) 0xa10700 = 1;
                *(int *) 0xa10704 = (int) packet->value;
                *(int *) 0x969c70 = (int) packet->value;
            }

            break;
        }
        case MP_STATE_TYPE_ITEM: {
            u16 give = (u16)(packet->value  >> 16);
            u16 item = (u16)(packet->value & 0xFFFF);

            if (give) {
                unlock_item(item, 0);
            }

            break;
        }
        default: {
            MULTI_LOG("Server asked us to set unknown state type %d\n", packet->state_type);
        }
    }
}

void mp_draw_text(MPPacketSetHUDText* hud_text) {
    HUDElementText* text_element = hud_get_text_element(hud_text->id);

    if (text_element && (hud_text->flags & HUDElementDelete)) {
        hud_delete_text_element(hud_text->id);

        return;
    }

    if (!text_element) {
        text_element = hud_new_text_element(hud_text->id);
    }

    text_element->element.x = hud_text->x;
    text_element->element.y = hud_text->y;
    text_element->color = hud_text->color;
    text_element->flags = hud_text->flags;

    memcpy(&text_element->text, &hud_text->text, sizeof(text_element->text));
}

char recv_buffer[1024];
// Receive and process updates and events from the server. Typically called once per tick.
void mp_receive_update() {
	long recv = 0;
	do {
		memset(&recv_buffer, 0, sizeof(recv_buffer));
		socklen_t paddrlen = sizeof(struct sockaddr_in);
		recv = recvfrom(mp_sock, &recv_buffer, sizeof(recv_buffer), MSG_DONTWAIT, &mp_sockaddr, &paddrlen);

        if (recv > 0x80000000) {
            //MULTI_LOG("recvfrom encountered error: 0x%08x\n", recv);
            continue;
        }
		
		if (recv >= sizeof(MPPacketHeader) && recv < 60000000) {
			MPPacketHeader* packet_header = (MPPacketHeader*)&recv_buffer;

			MULTI_TRACE("Received %d bytes from server\n", recv);

            // If a packet requires acknowledgement, it has a value in the requires_ack field.
            // We register that we got the packet and only process the packet if it hasn't been
            // registered already.
            if (packet_header->requires_ack > 0 && packet_header->type != MP_PACKET_ACK) {
                // Always acknowledge first.
                mp_send_ack(packet_header->requires_ack, packet_header->ack_cycle);

                // Check the table to see if we've processed this ack ID in this ack cycle.
                if (mp_acked[packet_header->requires_ack] != packet_header->ack_cycle) {
                    MULTI_TRACE("Acked %d, cycle: %d\n", packet_header->requires_ack, packet_header->ack_cycle);

                    // We haven't seen this packet before, so we add the cycle value to the acked table.
                    mp_acked[packet_header->requires_ack] = packet_header->ack_cycle;
                } else {
                    continue;
                }
            }

            if (packet_header->size+sizeof(MPPacketHeader) > recv) {
                MULTI_LOG("We received %d but only processed %d bytes\n", recv, packet_header->size+sizeof(MPPacketHeader));
            }

            // Process packet
			switch(packet_header->type) {
				case MP_PACKET_ACK:
                    if (!packet_header->requires_ack) {
                        handshake_complete = 1;
                        MULTI_LOG("Handshake complete!\n");
                    } else {
                        // Register packet as acknowledged to stop repeating the packet
                        mp_ack(recv_buffer, sizeof(MPPacketHeader) + packet_header->size);
                    }
					break;
				case MP_PACKET_IDKU:
					handshake_complete = 0;
					mp_server_reset();
					break;
				case MP_PACKET_MOBY_UPDATE:
					MULTI_TRACE("Updating moby\n");
					mp_update_moby(((MPPacketMobyUpdate*)&recv_buffer[sizeof(MPPacketHeader)]));
					MULTI_TRACE("Done updating moby\n");
					break;
                case MP_PACKET_MOBY_DELETE:
                    mp_delete_moby((MPPacketMobyCreate*)&recv_buffer[sizeof(MPPacketHeader)]);
                    break;
                case MP_PACKET_SET_STATE: {
                    // Server can send multiple of these messages in 1 packet to ensure the actions are performed in the right sequence.
                    int received = 0;
                    while (received < packet_header->size) {
                        mp_set_state((MPPacketSetState*)&recv_buffer[sizeof(MPPacketHeader) + received]);

                        received += sizeof(MPPacketSetState);
                    }
                    break;
                }
                case MP_PACKET_SET_HUD_TEXT: {
                    mp_draw_text((MPPacketSetHUDText*)&recv_buffer[sizeof(MPPacketHeader)]);
                    break;
                }
				default:
					MULTI_LOG("Received %ld bytes of unknown packet %d:\n", recv, packet_header->type);
                    MULTI_LOG("> Advertised size: %d\n", sizeof(MPPacketHeader)+packet_header->size);
                    MULTI_LOG("> Ack ID: %d. Cycle: %d\n", packet_header->requires_ack, packet_header->ack_cycle);
                    MULTI_LOG("> Flags: %d\n", packet_header->flags);
                    if (packet_header->size > 0 && packet_header->size <= recv-sizeof(MPPacketHeader)) {
                        hexDump("> Packet body data", &recv_buffer[sizeof(MPPacketHeader)], packet_header->size);
                    } else if (packet_header->size <= 0 && recv-sizeof(MPPacketHeader) <= 0) {
                        MULTI_LOG(">!Advertised packet body size is 0, but we received %d more bytes than size of header.\n", recv-sizeof(MPPacketHeader));
                    } else {
                        MULTI_LOG(">!Received less bytes than advertised size, not printing data\n");
                    }
			}
		}
	} while (recv > 0);
}


int last_game_state = 0;

void mp_player_update() {
    MPPacketHeader header = { 0, 0, 0, 0, 0 };
    header.type = MP_PACKET_MOBY_UPDATE;
    header.size = sizeof(MPPacketMobyUpdate);

    // Send packet body
    MPPacketMobyUpdate update_packet;
    memset(&update_packet, 0, sizeof(update_packet));
    update_packet.uuid = 0;  // Player moby is always uuid 0
    update_packet.flags |= ratchet_moby != 0 ? 1 : 0;
    update_packet.o_class = 0;
    update_packet.level = (u16)current_planet;
    update_packet.animation_id = ratchet_moby != 0 ? ratchet_moby->animationID : 0;
    update_packet.animation_duration = player_animation_duration;
    update_packet.x = player_pos.x;
    update_packet.y = player_pos.y;
    update_packet.z = player_pos.z;
    update_packet.rotation = player_rot.z;

    char buffer[sizeof(header) + sizeof(update_packet)];
    memset(&buffer, 0, sizeof(buffer));
    memcpy(&buffer, &header, sizeof(header));
    memcpy(&buffer[sizeof(header)], &update_packet, sizeof(update_packet));

    MULTI_TRACE("Sent update packet with advertised size: %d. Actual size: %d\n", header.size, sizeof(buffer));
    mp_send(&buffer, sizeof(buffer));

    // Send which buttons we're holding, if any.
    if (held_buttons != 0 ) {
        mp_send_controller_input(held_buttons, MP_CONTROLLER_FLAGS_HELD);
    }

    if (pressed_buttons != 0) {
        mp_send_controller_input(pressed_buttons, MP_CONTROLLER_FLAGS_PRESSED);
    }

    if (last_game_state != game_state) {
        mp_send_state(MP_STATE_TYPE_GAME, 0, game_state);
    }

    last_game_state = game_state;
}

void mp_weapon_update() {
    if (!mp_current_weapon_uuid && !waiting_for_weapon) {
        // Get permission from server to spawn current weapon
        MULTI_LOG("No current weapon uuid, requesting uuid from server.\n");
        mp_rpc_spawn_moby(&mp_spawn_weapon_cb);

        waiting_for_weapon = 1;

        return;
    }

    if (!mp_current_weapon_uuid) {
        MULTI_TRACE("Waiting for server to ack weapon spawn\n");
        return;
    }

    MPPacketHeader header = { 0, 0, 0, 0, 0};
    header.type = MP_PACKET_MOBY_UPDATE;
    header.size = sizeof(MPPacketMobyUpdate);

    // Send packet body
    MPPacketMobyUpdate update_packet;
    memset(&update_packet, 0, sizeof(update_packet));
    update_packet.uuid = mp_current_weapon_uuid;
    update_packet.flags |= 1;
    update_packet.o_class = item_to_oclass(current_weapon);
    update_packet.level = (u16)current_planet;
    update_packet.animation_id = 1;
    update_packet.x = player_pos.x;
    update_packet.y = player_pos.y;
    update_packet.z = player_pos.z + 2.0;
    update_packet.rotation = player_rot.z;

    char buffer[sizeof(header) + sizeof(update_packet)];
    memset(&buffer, 0, sizeof(buffer));
    memcpy(&buffer, &header, sizeof(header));
    memcpy(&buffer[sizeof(header)], &update_packet, sizeof(update_packet));

    MULTI_TRACE("Sent update packet with advertised size: %d. Actual size: %d\n", header.size, sizeof(buffer));
    mp_send(&buffer, sizeof(buffer));
}

void mp_send_update() {
    mp_player_update();

    // FIXME: Spawning weapons eventually crashes all other clients on the server.
    //mp_weapon_update();
}

int mp_directory_response_cb(void* data, size_t len) {
    int index = 0;
    int items = 0;

    MULTI_LOG("Response from directory. Size: %d\n", len);

    while (index < len && items < 5) {
        MPPacketQueryResponseServer* packet = (MPPacketQueryResponseServer*)&data[index];

        // Names can't be longer than 50 chars.
        if (packet->name_length > 50) {
            MULTI_LOG("Server name too long: %d\n", packet->name_length);
            index += sizeof(MPPacketQueryResponseServer) + packet->name_length;
            continue;
        }

        GameServer* server = (GameServer*)&game_servers[items];

        server->ip = packet->ip;
        server->port = packet->port;
        server->max_players = packet->max_players;
        server->num_players = packet->player_count;

        snprintf(server->name, packet->name_length+1, "%s", &data[index+sizeof(MPPacketQueryResponseServer)]);

        MULTI_LOG("Server name: %s; Port: %d; Max players: %d\n", &server->name, server->port, server->max_players);

        index += sizeof(MPPacketQueryResponseServer) + packet->name_length;
        items += 1;
    }

    num_game_servers = items;
    should_render_server_list = 1;

    return 0;
}

int last_frame_count = 10000;
int last_planet = 0;
SHK_FUNCTION_DEFINE_STATIC_1(0xccda0, int, show_message, char*, p1);
void mp_tick() {
	MULTI_TRACE("New tick, new life\n");
	if (mp_sock <= 0) {
		mp_connect();

        // We don't send handshake to server directory.
        if (!mp_connection_directory_mode) {
            mp_send_handshake();
        }
		
		// Wait until next game tick to receive handshake response and start doing actual stuff 
		return;
	}

    if (frame_count < last_frame_count) {
        MULTI_LOG("Environment reset\n");
        mp_reset_environment();
    }

    if (last_planet != current_planet) {
        MULTI_LOG("Changed planets\n");
        mp_reset_environment();
    }

    last_frame_count = frame_count;
    last_planet = current_planet;

    // Receive state from server
	mp_receive_update();
	
	MULTI_TRACE("Server sync complete\n");

    // Re-send packets that require acknowledgement
    mp_resend_unacked(mp_ack_id);

    // If we're just browsing a game server directory, we don't send update about ourselves.
    if (mp_connection_directory_mode) {
        // Refresh directory every 10 seconds
        if (game_server_last_refresh < 0 || game_server_last_refresh < frame_count - (10 * 60)) {
            mp_rpc_query_game_servers(0, &mp_directory_response_cb);
            game_server_last_refresh = frame_count;
        }

        return;
    }

    if (!handshake_complete) {
        mp_send_handshake();

        // Wait until next game tick to receive handshake response and start doing actual stuff
        return;
    }
	
	// Send current state to server
	// Start by sending packet header.
	MULTI_TRACE("Sending current state to server\n");

	mp_send_update();
	
	MULTI_TRACE("Done sending state to server\n");
}