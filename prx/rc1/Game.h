//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_GAME_H
#define PS3_CKIT_GAME_H

#include <rc1/common.h>

#include "rc1/ui/View.h"
#include "rc1/multiplayer/network/Client.h"
#include "rc1/multiplayer/network/DirectoryClient.h"
#include "rc1/multiplayer/network/GameClient.h"
#include "multiplayer/GameServer.h"
#include "rc1/views/ServerListView.h"
#include "Profiler.h"

typedef void (*ServerQueryCallback)(ServerListView* self, Vector<GameServer*>* servers);

static u8* level_flags1 = (u8*)0xa0ca84;
static u8* level_flags2 = (u8*)0xa0cd1c;

struct Game {
public:
    static Game& shared() {
        static Game game;
        return game;
    }

    View* current_view;
    int userid;

    void start();

    void transition_to(View* view);

    void on_tick();
    void on_render();
    void on_bink_do_frame();
    void on_save_operation(int action, void* savedata);

    void before_player_spawn();

    Client* client();
    Client* connected_client();

    static int query_servers_callback(void* data, size_t len, void* extra);
    void connect_to(char* ip, int port);
    void query_servers(int directory_id, ServerQueryCallback callback);

    void refresh_level_flags();

    void alert(String& message);
    void alert_blocking(String& message);
    void alert_blocking(String& message, int timeout_seconds);

    bool manual_save_enabled;
    bool force_load_save_file;
    bool have_save_file;

    int dialogs_dismissed;
private:
    Game() {
        previous_user_option_camera_left_right_movement = -1;
        previous_user_option_camera_up_down_movement = -1;
        previous_user_option_camera_rotation_speed = -1;
    }
    Game(Game const&);

    Client* client_;

    int last_planet_;

    int last_state_;

    int previous_user_option_camera_left_right_movement;
    int previous_user_option_camera_up_down_movement;
    int previous_user_option_camera_rotation_speed;
    bool restored_camera_options_;

    bool reload_level_flags_;
    u8 level_flags1_[0x10];
    u8 level_flags2_[0x100];

    static ServerQueryCallback server_query_callback_;

    void check_level_flags();

    void save(void* savedata);
    void load();
};


#endif //PS3_CKIT_GAME_H
