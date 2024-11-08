//
// Created by Vetle Hjelle on 30/12/2022.
//

#ifndef RAC1_MULTIPLAYER_PLAYER_H
#define RAC1_MULTIPLAYER_PLAYER_H


#include "lib/types.h"
#include <lib/string.h>
#include <rc1/rc1.h>

#include "multiplayer/SyncedMoby.h"
#include "rc1/multiplayer/Moby/RatchetAttachmentMoby.h"

struct Player {
    static Player& shared() {
        static Player player;
        return player;
    }

    Vec4 respawn_position;
    Vec4 respawn_rotation;

    String username;

    GameState last_game_state;

    void on_tick();
    void on_respawned();
    s32 previous_bolt_count;

    u32 ratchet_animation_duration;

    SyncedMoby* backpack_moby;
    SyncedMoby* backpack_attachment_moby;
    SyncedMoby* helmet_moby;
    SyncedMoby* map_o_matic_moby;
    SyncedMoby* persuader_moby;
    SyncedMoby* bolt_magnetizer_moby;
    SyncedMoby* left_shoe_moby;
    SyncedMoby* right_shoe_moby;

    u8 spawn_id;
private:
    Player() {
        previous_bolt_count = player_bolts;
    }
    Player(Player const&);

    bool moved_since_respawn_;
};


#endif //RAC1_MULTIPLAYER_PLAYER_H
