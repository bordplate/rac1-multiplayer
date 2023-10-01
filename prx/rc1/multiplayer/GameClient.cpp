//
// Created by Vetle Hjelle on 30/12/2022.
//

#include "GameClient.h"

#include <rc1/Game.h>

#include "../Player.h"
#include "MPMoby.h"
#include "rc1/views/StartView.h"

#include "../GoldBolt.h"

GameClient::GameClient(char *ip, int port) : Client(ip, port) {
    mobys_.resize(MAX_MP_MOBYS);
    ip_ = ip;

    for(int i = 0; i <= mobys_.capacity(); i++) {
        mobys_[i] = nullptr;
    }

    connection_complete_ = false;
}

void GameClient::disconnect() {
    Client::disconnect();

    if (ratchet_moby == nullptr && current_planet == 0) {
        game_state = Menu;
        StartView* view = new StartView();
        Game::shared().transition_to(view);
    }
}

void GameClient::reset() {
    waiting_for_connect_ = false;
    connection_complete_ = false;

    moby_delete_all();

    Client::reset();
}

void GameClient::update_moby(MPPacketMobyUpdate* packet) {
    // Check that we're not trying to update a moby beyond our predefined moby space.
    if (packet->uuid > mobys_.capacity()) {
        Logger::error("Tried to update illegal Moby UUID: %d", packet->uuid);
        return;
    }

//    if (ticks_ < 60 * 1) {
//        Logger::trace("Waiting a little before spawning moby. Tick: %d", ticks_);
//        return;
//    }

    if (game_state != PlayerControl) {
        Logger::trace("Moby not updating because game state %d", game_state);
        return;
    }

    if (current_planet != packet->level) {
        return;
    }

    Moby* moby = mobys_[packet->uuid];

    if (moby && moby->state > 0 && moby->oClass != packet->o_class) {
        Logger::error("[%d] Incoming o_class %d did not match o_class %d for existing moby", packet->uuid, packet->o_class, moby->oClass);
        if (((MPMobyVars*)moby->pVars)->sig != 0x4542) {
            Logger::debug("Signature also doesn't match a MP moby");
        }
        delete_moby(moby);
        mobys_[packet->uuid] = nullptr;
        moby = nullptr;
    }

    if (!moby) {
        // Spawn moby
        Logger::info("[%d] Spawning Moby (oClass: %d) at (%f, %f, %f)", packet->uuid, packet->o_class, packet->x, packet->y, packet->z);

        if (packet->flags & MP_MOBY_FLAG_ORIG_UDPATE_FUNC) {
            Logger::debug("Spawned with original update function");
            moby = Moby::spawn(packet->o_class, packet->flags, packet->modeBits);
        } else {
            moby = (Moby*)MPMoby::spawn(packet->uuid, packet->o_class, packet->flags, packet->modeBits);
        }

        mobys_[packet->uuid] = moby;

        if (packet->parent && packet->parent != packet->uuid) {
            Moby* parent_moby = mobys_[packet->uuid];
            if (parent_moby) {
                Logger::debug("Set moby (uid: %d) parent to (uid: %d)\n", packet->uuid, packet->parent);
                moby->field80_0xb8 = parent_moby;
            } else {
                Logger::error("Failed to find moby (uid: %d) to attach to parent (uid: %d)\n", packet->uuid, packet->parent);
            }
        }

        if (!moby) return;
    }

    if (packet->flags & MP_MOBY_FLAG_NO_COLLISION) {
        moby->collision = nullptr;
    }

    moby->position.x = packet->x;
    moby->position.y = packet->y;
    moby->position.z = packet->z;
    moby->rotation.x = packet->rotX;
    moby->rotation.y = packet->rotY;
    moby->rotation.z = packet->rotZ;
    moby->scale = packet->scale;

    moby->alpha = packet->alpha;

    // If this moby uses our custom update func, we can update pVars.
    if (!(packet->flags & MP_MOBY_FLAG_ORIG_UDPATE_FUNC)) {
        MPMobyVars *vars = (MPMobyVars *) (moby->pVars);
        if (vars) {
            vars->next_animation_id = (char) packet->animation_id;
            vars->animation_duration = (int) packet->animation_duration;
        }
    }
}

void GameClient::moby_delete(MPPacketMobyDelete* packet) {
    if (packet->flags & MP_MOBY_DELETE_FLAG_UUID) {
        Moby *moby = mobys_[packet->value];
        if (!moby || moby->state < 0) {
            mobys_[packet->value] = 0;
            Logger::debug("Already deleted moby %d @ 0x%08x", packet->value, moby);
            return;
        }

        Logger::debug("Deleting moby %d", packet->value);

        delete_moby(moby);
        mobys_[packet->value] = nullptr;
    } else if (packet->flags & MP_MOBY_DELETE_FLAG_OCLASS) {
        for (Moby *moby = moby_ptr; moby <= moby_ptr_end; moby++) {
            if (moby->state < 0x7f && moby->oClass == packet->value) {
                moby->state = -1;
                delete_moby(moby);
            }
        }
    } else if (packet->flags & MP_MOBY_DELETE_FLAG_UID) {
        for (Moby *moby = moby_ptr; moby <= moby_ptr_end; moby++) {
            if (moby->state < 0x7f && moby->UID == packet->value) {
                moby->state = -1;
                delete_moby(moby);
            }
        }
    }
}

void GameClient::moby_clear_all() {
    for(int i = 0; i <= mobys_.size(); i++) {
        mobys_[i] = nullptr;
    }
}

void GameClient::moby_delete_all() {
    for(int i = 0; i <= mobys_.size(); i++) {
        Moby* moby = mobys_[i];
        if (moby != nullptr && moby->state >= 0) {
            delete_moby(moby);
        }

        mobys_[i] = nullptr;
    }
}

void GameClient::update_set_state(MPPacketSetState* packet) {
    switch(packet->state_type) {
        case MP_STATE_TYPE_DAMAGE: {
            player_health -= (int)packet->value;
            break;
        }
        case MP_STATE_TYPE_PLAYER: {
            transition_to_movement_state(packet->value, 1);
            break;
        }
        case MP_STATE_TYPE_PLAYER_INPUT: {
            player_state_input = (int)packet->value;
            break;
        }
        case MP_STATE_TYPE_POSITION: {
            if (packet->offset > 8) {
                Logger::error("Server tried to set position at invalid offset %d", packet->offset);
                break;
            }

            ((float*)&player_pos)[packet->offset] = ((MPPacketSetStateFloat*)packet)->value;
            break;
        }
        case MP_STATE_TYPE_PLANET: {
            if ((current_planet != (int)packet->value && destination_planet != (int)packet->value) ||
                ratchet_moby == nullptr) {
                Logger::info("Going to planet %d", (int)packet->value);
                seen_planets[0] = 1;
                seen_planets[packet->value] = 1;

//                Logger::debug("Printing galactic map entries!");
//                for (int i = 0x96c18c; i < 0x96c1a8; i += 0x4) {
//                    if (!packet->offset && *(int*)i == *(int*)0x969C70) *(int*)i = 0; // if the not unlock param is true and the current planet is in the visited list, set it to zero. I intend to figure out whether the pointer will cause issues later.
//                    Logger::debug("Current galactic_map[%d] = 0x%x", i, *(int*)i);
//                }
//                Logger::debug("");
                *(int*)0xa10700 = 1;
                *(int*)0xa10704 = (int)packet->value;
                *(int*)0x969c70 = (int)packet->value;
            }

            break;
        }
        case MP_STATE_TYPE_ITEM: {
            u8 give = (u8)(packet->value  >> 16);
            u16 item = (u16)(packet->value & 0xFFFF);

            itemGivenByServer = 1;
            unlock_item(item, give);

//            if (give) {
//                itemGivenByServer = 1;
//                unlock_item(item, 0);
//            }

            break;
        }
        case MP_STATE_TYPE_SET_RESPAWN:  {
            Logger::debug("Setting respawn %d to %f", packet->offset, ((MPPacketSetStateFloat*)packet)->value);
            if (packet->offset < 3) {
                ((float*)&Player::shared().respawn_position)[packet->offset] = ((MPPacketSetStateFloat*)packet)->value;
            } else if (packet->offset < 6) {
                ((float*)&Player::shared().respawn_rotation)[packet->offset-3] = ((MPPacketSetStateFloat*)packet)->value;
            }

            if (current_planet > 0 && ratchet_moby != nullptr) {
                set_spawn_point(&Player::shared().respawn_position, &Player::shared().respawn_rotation);
            }
            break;
        }
        case MP_STATE_TYPE_BLOCK_BOLT: {
            blocked_bolts[(packet->offset * 4) + packet->value] = 1;

            break;
        }
        case MP_STATE_TYPE_ARBITRARY: {
            *(int*)(packet->offset) = (int)packet->value;
            break;
        }
        case MP_STATE_TYPE_SET_BOLTS: {
            uint32_t value = (uint32_t)packet->value;
            *(int*)0x969CA0 += (int)(((value>>24)&0xff) | // move byte 3 to byte 0
                                         ((value<<8)&0xff0000) | // move byte 1 to byte 2
                                         ((value>>8)&0xff00) | // move byte 2 to byte 1
                                         ((value<<24)&0xff000000)); // byte 0 to byte 3
            break;
        }
        case MP_STATE_TYPE_GIVE_BOLTS: {
            Logger::debug("Changing bolt count, but we shouldn't be here right now.");
            *(uint32_t*)player_bolts += (uint32_t)packet->value;
            break;
        }
        default: {
            Logger::error("Server asked us to set unknown state type %d", packet->state_type);
        }
    }
}

void GameClient::update_set_text(MPPacketSetHUDText* packet) {
    // If we don't have a view in time for this packet, just return
    if (remote_view_ == nullptr) {
        return;
    }

    if (packet->flags & HUDElementDelete) {
        Logger::trace("Deleting HUD element");
        remote_view_->delete_element(packet->id);

        return;
    }

    TextElement* element = remote_view_->get_element(packet->id);

    if (!element) {
        Logger::critical("Unable to create text element for remote view");
        return;
    }

    Logger::trace("Updating text element at %p", element);

    element->x = packet->x;
    element->y = packet->y;

    element->color = packet->color;

    Logger::trace("Before setting text");

    element->text->set(packet->text);
}

void GameClient::toast_message(MPPacketToastMessage* packet) {
    String message = String(packet->message);
    strcpy((char*)(0xa15c8c), message.c_str());
    uint32_t duration = packet->duration;

    if (duration < 20) {
        duration = 20;
    }

    (*((int*)0xa15c80)) = packet->duration;
}

bool GameClient::update(MPPacketHeader *header, void *packet_data) {
    if (!Client::update(header, packet_data)) {
        return false;
    }

    // Don't update if handshake isn't complete
    // We also wait a couple of frames after spawning to mitigate an unknown crash that occurs sometimes when we spawn
    //  mobys too quickly after spawn
    if (!handshake_complete()) {
        return false;
    }

    // Process packet
    switch(header->type) {
        case MP_PACKET_MOBY_UPDATE:
            update_moby((MPPacketMobyUpdate*)packet_data);
            break;
        case MP_PACKET_MOBY_DELETE:
            moby_delete((MPPacketMobyDelete*)packet_data);
            break;
        case MP_PACKET_SET_STATE: {
            // Server can send multiple of these messages in 1 packet to ensure the actions are performed in the right sequence.
            int recvd = 0;
            while (recvd < header->size) {
                MPPacketSetState* state_data = (MPPacketSetState*)((char*)packet_data + recvd);
                update_set_state(state_data);

                recvd += sizeof(MPPacketSetState);
            }

            break;
        }
        case MP_PACKET_SET_HUD_TEXT: {
            update_set_text((MPPacketSetHUDText*)packet_data);
            break;
        }
        case MP_PACKET_TOAST_MESSAGE: {
            toast_message((MPPacketToastMessage*)packet_data);
            break;
        }
        default:
            Logger::error("Received %ld bytes of unknown packet %d:", received(), header->type);
            Logger::error("> Advertised size: %d", sizeof(MPPacketHeader)+header->size);
            Logger::error("> Ack ID: %d. Cycle: %d", header->requires_ack, header->ack_cycle);
            Logger::error("> Flags: %d", header->flags);
            if (header->size > 0 && header->size <= received()-sizeof(MPPacketHeader)) {
                hexDump("> Packet body data", packet_data, header->size);
            } else if (header->size <= 0 && received()-sizeof(MPPacketHeader) <= 0) {
                Logger::error(">!Advertised packet body size is 0, but we received %d more bytes than size of header.", received()-sizeof(MPPacketHeader));
            } else {
                Logger::error(">!Received less bytes than advertised size, not printing data");
            }
    }

    return true;
}

int GameClient::connect_callback(void* packetData, size_t size, void* userdata) {
    GameClient* self = (GameClient*)userdata;
    MPPacketConnectCallback* response = (MPPacketConnectCallback*)packetData;

    if (response->status == 1) {
        self->connection_complete_ = true;

        return 0;
    }

    String message;

    switch (response->status) {
        case 0:
            message = String("An unknown error occurred.");
            break;
        case 2:
            message = String("A user with that username is already connected to this server. Please change to a different username.");
            break;
        case 3:
            message = String("You must've been banned from this server or something. Good job.");
            break;
        case 4:
            message = String("Your multiplayer mod is outdated. Please update your multiplayer mod to connect to this game server.");
            break;
        case 5:
            message = String("Wrong passcode.");
            break;
        default:
            message = String("A super unknown error occurred, something's fucky.");
            break;
    }

    Game::shared().alert(message);
    self->disconnect();

    return 0;
}

void GameClient::on_tick() {
    Client::on_tick();

    if (!remote_view_) {
        remote_view_ = new RemoteView();
        Game::shared().transition_to(remote_view_);
    }

    ticks_ += 1;

    if (connected() && !handshake_complete()) {
        send_handshake();

        // Wait until next game tick to receive handshake response and start doing actual stuff
        return;
    }

    if (!connection_complete_) {
        if (!waiting_for_connect_) {
            Packet *packet = Packet::make_connect_packet(Player::shared().username, Game::shared().userid);
            make_self_referencing_rpc(packet, connect_callback);
            send(packet);
            waiting_for_connect_ = true;
        }

        return;
    }

    Player::shared().on_tick();
}