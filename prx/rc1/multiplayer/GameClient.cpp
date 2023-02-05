//
// Created by Vetle Hjelle on 30/12/2022.
//

#include "GameClient.h"

#include <rc1/Game.h>

#include "../Player.h"

void GameClient::update_moby(MPPacketMobyUpdate* packet) {
    // Check that we're not trying to update a moby beyond our predefined moby space.
    if (packet->uuid > mobys_.capacity()) {
        Logger::error("Tried to update illegal Moby UUID: %d", packet->uuid);
        return;
    }

    if (ticks_ < 60 * 1) {
        Logger::trace("Waiting a little before spawning moby. Tick: %d", ticks_);
        return;
    }

    if (game_state != PlayerControl) {
        Logger::trace("Moby not updating because game state %d", game_state);
        return;
    }

    if (current_planet != packet->level) {
        return;
    }

    Moby* moby = mobys_[packet->uuid];

    if (!moby || moby->state < 0) {
        // Spawn moby
        Logger::info("Spawning Moby (oClass: %d) at (%f, %f, %f)\n", packet->o_class, packet->x, packet->y, packet->z);
        moby = Moby::spawn(packet->o_class, packet->flags);

        mobys_[packet->uuid] = moby;

        MPMobyVars* vars = (MPMobyVars*)(moby->pVars);

        if (vars) {
            vars->uuid = packet->uuid;
            vars->o_class = packet->o_class;
        }


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
        moby->collision = 0;
    }

    moby->position.x = packet->x;
    moby->position.y = packet->y;
    moby->position.z = packet->z;
    moby->rotation.z = packet->rotation;

    MPMobyVars* vars = (MPMobyVars*)(moby->pVars);
    if (vars) {
        vars->next_animation_id = (char)packet->animation_id;
        vars->animation_duration = (int)packet->animation_duration;
    }
}

void GameClient::moby_delete(MPPacketMobyCreate* packet) {
    Moby* moby = mobys_[packet->uuid];
    if (!moby || moby->state < 0) {
        MULTI_LOG("Not deleting moby %d @ 0x%08x\n", packet->uuid, moby);
        return;
    }

    MULTI_LOG("Deleting moby %d\n", packet->uuid);

    delete_moby(moby);
    mobys_[packet->uuid] = 0;
}

void GameClient::moby_delete_all() {

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
        case MP_STATE_TYPE_POSITION: {
            if (packet->offset > 3) {
                Logger::error("Server tried to set position at invalid offset %d", packet->offset);
                break;
            }

            ((float*)&player_pos)[packet->offset] = ((MPPacketSetStateFloat*)packet)->value;
            break;
        }
        case MP_STATE_TYPE_PLANET: {
            if (current_planet != (int)packet->value && destination_planet != (int)packet->value) {
                Logger::info("Going to planet %d", (int)packet->value);
                seen_planets[0] = 1;
                seen_planets[packet->value] = 1;
                *(int*)0xa10700 = 1;
                *(int*)0xa10704 = (int)packet->value;
                *(int*)0x969c70 = (int)packet->value;
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
            Logger::error("Server asked us to set unknown state type %d", packet->state_type);
        }
    }
}

void GameClient::update_set_text(MPPacketSetHUDText* packet) {
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

bool GameClient::update(MPPacketHeader *header, void *packet_data) {
    if (!Client::update(header, packet_data)) {
        return false;
    }

    // Don't update if handshake isn't complete
    if (!handshake_complete()) {
        return false;
    }

    // Process packet
    switch(header->type) {
        case MP_PACKET_MOBY_UPDATE:
            Logger::trace("Updating moby");
            update_moby((MPPacketMobyUpdate*)packet_data);
            Logger::trace("Done updating moby");
            break;
        case MP_PACKET_MOBY_DELETE:
            moby_delete((MPPacketMobyCreate*)packet_data);
            break;
        case MP_PACKET_SET_STATE: {
            // Server can send multiple of these messages in 1 packet to ensure the actions are performed in the right sequence.
            int recvd = 0;
            while (recvd < header->size) {
                update_set_state((MPPacketSetState*)packet_data);

                recvd += sizeof(MPPacketSetState);
            }

            break;
        }
        case MP_PACKET_SET_HUD_TEXT: {
            update_set_text((MPPacketSetHUDText*)packet_data);
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

void GameClient::on_tick() {
    Client::on_tick();

    ticks_ += 1;

    if (connected() && !handshake_complete()) {
        send_handshake();

        // Wait until next game tick to receive handshake response and start doing actual stuff
        return;
    }

    Player::shared().on_tick();

    if (!remote_view_) {
        remote_view_ = new RemoteView();
        Game::shared().transition_to(remote_view_);
    }

    //if (frame_count < last_frame_count) {
    //    MULTI_LOG("Environment reset\n");
    //    mp_reset_environment();
    //}

    //if (last_planet != current_planet) {
    //    MULTI_LOG("Changed planets\n");
    //    mp_reset_environment();
    //}

    //last_frame_count = frame_count;
    //last_planet = current_planet;

    // Re-send packets that require acknowledgement
    //mp_resend_unacked(mp_ack_id);

    //if (!handshake_complete) {
    //    mp_send_handshake();
//
    //    // Wait until next game tick to receive handshake response and start doing actual stuff
    //    return;
    //}
//
    //// Send current state to server
    //// Start by sending packet header.
    //MULTI_TRACE("Sending current state to server\n");
//
    //mp_send_update();
//
    //MULTI_TRACE("Done sending state to server\n");
}