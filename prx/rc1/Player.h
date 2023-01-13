//
// Created by Vetle Hjelle on 30/12/2022.
//

#ifndef RAC1_MULTIPLAYER_PLAYER_H
#define RAC1_MULTIPLAYER_PLAYER_H


struct Player {
    static Player& shared() {
        static Player player;
        return player;
    }

    void on_tick();
private:
    Player() {}
    Player(Player const&);
};


#endif //RAC1_MULTIPLAYER_PLAYER_H
