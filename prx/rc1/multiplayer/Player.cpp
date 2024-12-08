//
// Created by Vetle Hjelle on 30/12/2022.
//

#include "Player.h"

#include <rc1/bridging.h>
#include <rc1/Game.h>
#include "network/Packet.h"

void Player::on_tick() {
    Client* client = Game::shared().connected_client();

    if (client) {
        if (held_buttons != 0) {
            Packet* controller_input = Packet::make_controller_input(held_buttons, MP_CONTROLLER_FLAGS_HELD);
            client->send(controller_input);
        }

        if (pressed_buttons != 0) {
            Packet* controller_input = Packet::make_controller_input(pressed_buttons, MP_CONTROLLER_FLAGS_PRESSED);
            client->send(controller_input);
        }
    }

    if (!ratchet_moby) {
        return;
    }

    if (!moved_since_respawn_) {
        player_ghost_frames = 30;
    }

    if (held_buttons) {
        moved_since_respawn_ = true;
    }

    Packet* packet = new Packet(sizeof(MPPacketMobyUpdate));

    packet->header->type = MP_PACKET_MOBY_UPDATE;

    MPPacketMobyUpdate* payload = (MPPacketMobyUpdate*)packet->body;
    payload->uuid = 0;  // Player moby is always uuid 0
    payload->o_class = ratchet_moby->o_class;
    payload->animation_id = ratchet_moby != nullptr ? ratchet_moby->animation_id : 0;
    payload->animation_duration = ratchet_moby->o_class == 0 ? ratchet_animation_duration : 10;
    payload->x = player_pos.x;
    payload->y = player_pos.y;
    payload->z = player_pos.z;
    payload->rotX = player_rot.x;
    payload->rotY = player_rot.y;
    payload->rotZ = player_rot.z;
    payload->scale = ratchet_moby->scale;

    Logger::trace("Sent update packet");
    Game::shared().client()->send(packet);

    if (client) {
        if (last_game_state != game_state) {
            Packet* game_state_packet = Packet::make_game_state_changed_packet(game_state);
            client->send(game_state_packet);
        }

        if (previous_bolt_count != player_bolts) {
            s32 bolt_diff = player_bolts - previous_bolt_count;
            client->send(Packet::make_bolt_count_changed_packet(bolt_diff, player_bolts));
            previous_bolt_count = player_bolts;
        }

        if (backpack_moby && backpack_moby->get_vars()->status == SyncedMobyStatusActive) {
            if (backpack_attachment_moby) {
                switch (backpack_attachment_moby->get_vars()->status) {
                    case SyncedMobyStatusReady:
//                        backpack_attachment_moby->get_vars()->parent_uuid = backpack_moby->get_vars()->uuid;
                        backpack_attachment_moby->activate();
                        break;
                    case SyncedMobyStatusActive:
                        backpack_attachment_moby->update();
                        break;
                    default:
                        break;
                }
            }

            backpack_moby->update();
        }

        if (helmet_moby && helmet_moby->get_vars()->status == SyncedMobyStatusActive) {
            helmet_moby->update();
        }

        if (map_o_matic_moby && map_o_matic_moby->get_vars()->status == SyncedMobyStatusActive) {
            map_o_matic_moby->update();
        }

        if (persuader_moby && persuader_moby->get_vars()->status == SyncedMobyStatusActive) {
            persuader_moby->update();
        }

        if (bolt_magnetizer_moby && bolt_magnetizer_moby->get_vars()->status == SyncedMobyStatusActive) {
            bolt_magnetizer_moby->update();
        }

        if (left_shoe_moby && left_shoe_moby->get_vars()->status == SyncedMobyStatusActive) {
            left_shoe_moby->update();
        }

        if (right_shoe_moby && right_shoe_moby->get_vars()->status == SyncedMobyStatusActive) {
            right_shoe_moby->update();
        }
    }

    last_game_state = game_state;
}

void Player::on_respawned() {
    backpack_moby = nullptr;
    backpack_attachment_moby = nullptr;

    spawn_id += 1;

    Logger::info("Respawned: %d", spawn_id);

    moved_since_respawn_ = false;

    if (Game::shared().client()) {
        Client* client = Game::shared().client();

        client->send_ack(Packet::make_player_respawned_packet(spawn_id));
    }
}

extern "C" void _c_set_ratchet_animation_duration(u32 duration) {
    Player::shared().ratchet_animation_duration = duration;
}
