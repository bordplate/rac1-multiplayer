//
// Created by Vetle Hjelle on 30/12/2022.
//

#include "Player.h"

#include <rc1/rc1.h>

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
    payload->o_class = 0;
    payload->level = (u16)current_planet;
    payload->animation_id = ratchet_moby != 0 ? ratchet_moby->animationID : 0;
    payload->animation_duration = 10;
    payload->x = player_pos.x;
    payload->y = player_pos.y;
    payload->z = player_pos.z;
    payload->rotation = player_rot.z;

    Logger::trace("Sent update packet");
    Game::shared().client()->send(packet);

    // Send which buttons we're holding, if any.
    //if (held_buttons != 0 ) {
    //    mp_send_controller_input(held_buttons, MP_CONTROLLER_FLAGS_HELD);
    //}

    //if (pressed_buttons != 0) {
    //    mp_send_controller_input(pressed_buttons, MP_CONTROLLER_FLAGS_PRESSED);
    //}

    //if (last_game_state != game_state) {
    //    mp_send_state(MP_STATE_TYPE_GAME, 0, game_state);
    //}

    //last_game_state = game_state;
}