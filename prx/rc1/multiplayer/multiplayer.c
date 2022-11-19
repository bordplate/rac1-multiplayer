#include "multiplayer.h"

#include <netex/net.h>

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

        MULTI_LOG("Acked message %d with size %d\n", header->type, header->size);
    }

    //free(unacked->data);
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
	
	mp_sockaddr.sin_addr.s_addr = inet_addr("10.9.0.212");
	mp_sockaddr.sin_port = htons(2407);
	mp_sockaddr.sin_len = sizeof(mp_sockaddr);
	mp_sockaddr.sin_family = AF_INET;
	
	MULTI_LOG("Socket opened, and ready to blast data...\n");
}

Moby* mp_spawn_moby(u32 uuid, int o_class) {
	// Check that we're not trying to update a moby beyond our predefined moby space. 
	if (uuid > sizeof(mp_mobys)/sizeof(mp_mobys[0])) {
		MULTI_LOG("Tried to spawn illegal Moby UUID: %d\n", uuid);
		return 0;
	}
	
	MULTI_TRACE("Spawning moby oClass: %d\n", o_class);
	
	Moby* moby = spawn_moby(o_class);
	
	MULTI_TRACE("Moby address: %d\n", moby);
	
	moby->pUpdate = (void*)0x704720;

	MPMobyVars* vars = (MPMobyVars*)(moby->pVars);
	vars->uuid = uuid;
    vars->o_class = o_class;

	moby->enabled = 1;
	moby->draw_distance = 0xff;
	moby->update_distance = 0xff;
	moby->state = 0;
	moby->alpha = 0xff;

    if (moby->oClass == 0x47) {
        MULTI_LOG("Debug wrench stuff happening right now\n");
        load_gadget_weapon_model(&moby, -1);
    }

	moby->mode_bits = 0x0;

	//idk(moby);
	
	mp_mobys[uuid] = moby;
	
	MULTI_LOG("Spawned Moby (oClass: %d, uuid: %d)\n", o_class, uuid);
	
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

	Moby* moby = mp_mobys[update_packet->uuid];
	
	if (!moby || moby->state < 0) {
		// Spawn moby
		MULTI_LOG("Spawning Moby (oClass: %d) at (%f, %f, %f)\n", update_packet->o_class, update_packet->x, update_packet->y, update_packet->z);
		moby = mp_spawn_moby(update_packet->uuid, update_packet->o_class);

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
	
	MPMobyVars* vars = (MPMobyVars*)(moby->pVars);
	
    moby->position.x = update_packet->x;
    moby->position.y = update_packet->y;
    moby->position.z = update_packet->z;
    moby->rotation.z = update_packet->rotation;

    if (vars) {
        vars->next_animation_id = (char) update_packet->animation_id;
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
	memset(&mp_mobys, 0, sizeof(mp_mobys));

    mp_on_spawn();
}

void mp_server_reset() {
	MULTI_LOG("Server reset\n");
    memset(&mp_mobys, 0, sizeof(mp_mobys));
    memset(&mp_mobys, 0, sizeof(mp_mobys));

    mp_current_weapon_uuid = 0;
}

char recv_buffer[1024];
// Receive and process updates and events from the server. Typically called once per tick.
void mp_receive_update() {
	int recv = 0;
	do {
		MPPacketHeader packet_header = { 0, 0, 0, 0, 0};
		
		memset(&recv_buffer, 0, sizeof(recv_buffer));
		socklen_t paddrlen = sizeof(struct sockaddr_in);
		recv = recvfrom(mp_sock, &recv_buffer, sizeof(recv_buffer), MSG_DONTWAIT, &mp_sockaddr, &paddrlen);
		
		if (recv > 0) {
			memcpy(&packet_header, &recv_buffer, sizeof(packet_header));
		
			MULTI_TRACE("Received %d bytes from server\n", recv);

            // If a packet requires acknowledgement, it has a value in the requires_ack field.
            // We register that we got the packet and only process the packet if it hasn't been
            // registered already.
            if (packet_header.requires_ack && packet_header.type != MP_PACKET_ACK) {
                // Always acknowledge first.
                mp_send_ack(packet_header.requires_ack, packet_header.ack_cycle);

                // Check the table to see if we've processed this ack ID in this ack cycle.
                if (packet_header.ack_cycle == mp_acked[packet_header.requires_ack]) {
                    continue;
                }

                // We haven't seen this packet before, so we add the cycle value to the acked table.
                mp_acked[packet_header.requires_ack] = packet_header.ack_cycle;
            }

            if (packet_header.size+sizeof(packet_header) > recv) {
                MULTI_LOG("We received %d but only processed %d bytes\n", recv, packet_header.size+sizeof(packet_header));
            }

            // Process packet
			switch(packet_header.type) {
				case MP_PACKET_ACK:
                    if (!packet_header.requires_ack) {
                        handshake_complete = 1;
                        MULTI_LOG("Handshake complete!\n");
                    } else {
                        // Register packet as acknowledged to stop repeating the packet
                        MULTI_LOG("Acking ID %d. Total packet size: %d. Received: %d\n", packet_header.requires_ack, sizeof(packet_header) + packet_header.size, recv);
                        hexDump("Data", &recv_buffer, recv);

                        MULTI_LOG("UUID: %d", ((MPPacketMobyCreate*)&recv_buffer[sizeof(packet_header)])->uuid);

                        mp_ack(&recv_buffer, sizeof(packet_header) + packet_header.size);
                    }
					break;
				case MP_PACKET_IDKU:
					handshake_complete = 0;
					mp_server_reset();
					break;
				case MP_PACKET_MOBY_UPDATE:
					MULTI_TRACE("Updating moby\n");
					mp_update_moby(((MPPacketMobyUpdate*)&recv_buffer[sizeof(packet_header)]));
					MULTI_TRACE("Done updating moby\n");
					break;
				default:
					MULTI_LOG("Received %d bytes of unknown packet %d:\n", recv, packet_header.type);
                    MULTI_LOG("> Advertised size: %d\n", sizeof(packet_header)+packet_header.size);
                    MULTI_LOG("> Ack ID: %d. Cycle: %d\n", packet_header.requires_ack, packet_header.ack_cycle);
                    MULTI_LOG("> Flags: %d\n", packet_header.flags);
                    if (packet_header.size > 0 && packet_header.size <= recv-sizeof(packet_header)) {
                        hexDump("> Packet body data", &recv_buffer[sizeof(packet_header)], packet_header.size);
                    } else if (packet_header.size <= 0 && recv-sizeof(packet_header) <= 0) {
                        MULTI_LOG(">!Advertised packet body size is 0, but we received %d more bytes than size of header.\n", recv-sizeof(packet_header));
                    } else {
                        MULTI_LOG(">!Received less bytes than advertised size, not printing data\n");
                    }
			}
		}
	} while (recv > 0);
}

void mp_player_update() {
    MPPacketHeader header = { 0, 0, 0, 0, 0 };
    header.type = MP_PACKET_MOBY_UPDATE;
    header.size = sizeof(MPPacketMobyUpdate);

    // Send packet body
    MPPacketMobyUpdate update_packet;
    memset(&update_packet, 0, sizeof(update_packet));
    update_packet.uuid = 0;  // Player moby is always uuid 0
    update_packet.enabled = 1;
    update_packet.o_class = 0;
    update_packet.animation_id = ratchet_moby->animationID;
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
    update_packet.enabled = 1;
    update_packet.o_class = item_to_oclass(current_weapon);
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

int last_frame_count = 10000;

void mp_tick() {
	MULTI_TRACE("New tick, new life\n");
	
	if (mp_sock <= 0) {
		mp_connect();
		mp_send_handshake();
		
		// Wait until next game tick to receive handshake response and start doing actual stuff 
		return;
	}
	
	// Receive state from server
	mp_receive_update();
	
	MULTI_TRACE("Server sync complete\n");
	
	if (!handshake_complete) {
		mp_send_handshake();
		
		// Wait until next game tick to receive handshake response and start doing actual stuff 
		return; 
	}

    if (frame_count < last_frame_count) {
        MULTI_LOG("Environment reset\n");
        mp_reset_environment();
    }

    last_frame_count = frame_count;

    // Re-send packets that require acknowledgement
    mp_resend_unacked(mp_ack_id);
	
	// Send current state to server
	// Start by sending packet header.
	MULTI_TRACE("Sending current state to server\n");

	mp_send_update();
	
	MULTI_TRACE("Done sending state to server\n");
}