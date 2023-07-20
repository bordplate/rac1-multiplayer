//
// Created by Vetle Hjelle on 30/12/2022.
//

#ifndef RAC1_MULTIPLAYER_PLAYER_H
#define RAC1_MULTIPLAYER_PLAYER_H


#include "lib/types.h"
#include <lib/string.h>
#include <rc1/rc1.h>

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
private:
    Player() {}
    Player(Player const&);
};


#endif //RAC1_MULTIPLAYER_PLAYER_H
