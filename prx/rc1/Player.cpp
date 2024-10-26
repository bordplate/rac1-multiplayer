//
// Created by Vetle Hjelle on 30/12/2022.
//

#include "Player.h"

#include "Game.h"
#include "multiplayer/Packet.h"

void Player::on_tick() {
    if (!ratchet_moby) {
        return;
    }

    Packet* packet = new Packet(sizeof(MPPacketMobyUpdate));

    packet->header->type = MP_PACKET_MOBY_UPDATE;

    MPPacketMobyUpdate* payload = (MPPacketMobyUpdate*)packet->body;
    payload->uuid = 0;  // Player moby is always uuid 0
    payload->flags |= ratchet_moby != 0 ? 1 : 0;
    payload->o_class = ratchet_moby->oClass;
    payload->level = (u16)current_planet;
    payload->animation_id = ratchet_moby != 0 ? ratchet_moby->animationID : 0;
    payload->animation_duration = 10;
    payload->x = player_pos.x;
    payload->y = player_pos.y;
    payload->z = player_pos.z;
    payload->rotX = player_rot.x;
    payload->rotX = player_rot.y;
    payload->rotZ = player_rot.z;
    payload->scale = ratchet_moby->scale;
    payload->alpha = ratchet_moby->alpha;
    payload->state = (u16)player_state;

    Logger::trace("Sent update packet");
    Game::shared().client()->send(packet);

    // Send which buttons we're holding, if any.
    if (Game::shared().client()) {
        Client *client = Game::shared().client();
        if (held_buttons != 0) {
            Packet *controller_input = Packet::make_controller_input(held_buttons, MP_CONTROLLER_FLAGS_HELD);
            client->send(controller_input);
        }

        if (pressed_buttons != 0) {
            Packet* controller_input = Packet::make_controller_input(pressed_buttons, MP_CONTROLLER_FLAGS_PRESSED);
            client->send(controller_input);
        }

        if (last_game_state != game_state) {
            Packet* game_state_packet = Packet::make_game_state_changed_packet(game_state);
            client->send(game_state_packet);
        }
    }

    last_game_state = game_state;
}

void Player::on_respawned() {
    if (Game::shared().client()) {
        Client* client = Game::shared().client();

        Packet* respawned_packet = Packet::make_player_respawned_packet();
        client->make_ack(respawned_packet, nullptr);
        client->send(respawned_packet);
    }
}