//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_GAME_H
#define PS3_CKIT_GAME_H

#include <rc1/common.h>

#include "View.h"
#include "multiplayer/Client.h"
#include "multiplayer/DirectoryClient.h"
#include "multiplayer/GameClient.h"
#include "multiplayer/GameServer.h"

typedef void (*ServerQueryCallback)(Vector<GameServer*>* servers);

struct Game {
public:
    static Game& shared() {
        static Game game;
        return game;
    }

    int userid;

    void start();

    void transition_to(View* view);

    void on_tick();
    void on_render();

    void before_player_spawn();

    Client* client();

    static int query_servers_callback(void* data, size_t len, void* extra);
    void connect_to(char* ip, int port);
    void query_servers(int directory_id, ServerQueryCallback callback);
private:
    Game() {
        previous_user_option_camera_left_right_movement = -1;
        previous_user_option_camera_up_down_movement = -1;
        previous_user_option_camera_rotation_speed = -1;
    }
    Game(Game const&);

    View* current_view;

    Client* client_;

    int previous_user_option_camera_left_right_movement;
    int previous_user_option_camera_up_down_movement;
    int previous_user_option_camera_rotation_speed;
    bool restored_camera_options_;

    static ServerQueryCallback server_query_callback_;
};


#endif //PS3_CKIT_GAME_H
