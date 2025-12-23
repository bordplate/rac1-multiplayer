//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "Game.h"

#include <rc1/rc1.h>

#include <lib/vector.h>
#include <lib/logger.h>
#include <lib/memory.h>

#include <sys/random_number.h>
#include <sysutil/sysutil_msgdialog.h>

#include "views/StartView.h"

#include "rc1/multiplayer/Player.h"
#include "rc1/utils/PersistentStorage.h"
#include "rc1/game/GoldBolt.h"
#include "rc1/views/ServerListView.h"

// For whatever dumb reason I can't get the compiler to include
//  .cpp files under /lib/, so it's defined here.
// Maybe better to just have it here anyway. Idk.
LogLevel Logger::log_level_ = Debug;

ServerQueryCallback Game::server_query_callback_ = NULL;

void Game::start() {
    PersistentStorage storage = PersistentStorage("settings.conf");

    int index = storage.index_of("userid");
    if (index < 0) {
        sys_get_random_number(&userid, sizeof(userid));
        storage.set("userid", userid);
    } else {
        userid = storage.get_int("userid");
    }

    init();
}

void Game::init() {
    // "Temporary" hack for gold bolt stuff
    memset(blocked_bolts, 0, 100);
    memset(metal_detector_bolt_multiplier, 1, 1);
    memset(proxy_item_array, 0, proxy_item_array_size);
    memset(proxy_level_array, 0, proxy_level_array_size);

    force_load_save_file = false;
    have_save_file = false;
    manual_save_enabled = false;

    previous_user_option_camera_left_right_movement = -1;
    previous_user_option_camera_up_down_movement = -1;
    previous_user_option_camera_rotation_speed = -1;
    restored_camera_options_ = false;

    dialogs_dismissed = 0;
}

void Game::reset() {
    init();

    if (client_) {
        if (DataClient::shared()->is_running()) {
            DataClient::stop = true;
        }

        client_->disconnect();
    }
}

static Profiler full_tick_timer_("full tick");
static Profiler tick_timer_("tick");
static Profiler check_level_flags_("check level flags");
void Game::on_tick() {
    Profiler::reset_all();

    Profiler::Scope s(&full_tick_timer_);

    if (client_) {
        {
            Profiler::Scope scope(&tick_timer_);

            client_->on_tick();
        }

        if (ratchet_moby != nullptr) {
            Profiler::Scope scope(&check_level_flags_);
            check_level_flags();
        }
    }

    option_helpdesk_text = false;
    option_helpdesk_voice = false;

    if (previous_user_option_camera_left_right_movement == -1) {
        previous_user_option_camera_left_right_movement = user_option_camera_left_right_movement;
        previous_user_option_camera_up_down_movement = user_option_camera_up_down_movement;
        previous_user_option_camera_rotation_speed = user_option_camera_rotation_speed;
    }

    if (previous_user_option_camera_left_right_movement != user_option_camera_left_right_movement ||
    previous_user_option_camera_up_down_movement != user_option_camera_up_down_movement ||
    previous_user_option_camera_rotation_speed != user_option_camera_rotation_speed) {
        previous_user_option_camera_left_right_movement = user_option_camera_left_right_movement;
        previous_user_option_camera_up_down_movement = user_option_camera_up_down_movement;
        previous_user_option_camera_rotation_speed = user_option_camera_rotation_speed;

        PersistentStorage storage = PersistentStorage("settings.conf");

        storage.set("user_option_camera_left_right_movement", user_option_camera_left_right_movement);
        storage.set("user_option_camera_up_down_movement", user_option_camera_up_down_movement);
        storage.set("user_option_camera_rotation_speed", user_option_camera_rotation_speed);
    }

    // Pass inputs to the current view and send to server
    if (pressed_buttons) {
        if (current_view) {
            Logger::trace("Pressed buttons (%08X) sent to view", pressed_buttons);
            current_view->on_pressed_buttons(pressed_buttons);
        }
    }

    // Player is most likely on intro menu scene. Present and handle multiplayer startup stuff.
    if (current_planet == 0 && ratchet_moby == 0) {
        if (current_view && game_state == 0) {
            delete current_view;
            current_view = 0;
        }

        if (!current_view && game_state == 3) {
            ServerListView* view = new ServerListView();
            this->transition_to(view);
        }
    } else if (ratchet_moby != nullptr && use_custom_player_color) {
        ratchet_moby->color = custom_player_color;
    }

    if (client_) {
        client_->flush();

        if (force_load_save_file && have_save_file && DataClient::shared() != nullptr && DataClient::shared()->is_running()) {
            force_load_save_file = false;
            load();
        }
    }
}

void Game::before_player_spawn() {
    if (client_ && client_->connected()) {
        Player::shared().on_respawned();
        Logger::debug("Player spawn");

        if (!restored_camera_options_) {
            PersistentStorage storage = PersistentStorage("settings.conf");
            if (storage.index_of("user_option_camera_left_right_movement") >= 0)
                user_option_camera_left_right_movement = storage.get_int("user_option_camera_left_right_movement");
            if (storage.index_of("user_option_camera_up_down_movement") >= 0)
                user_option_camera_up_down_movement = storage.get_int("user_option_camera_up_down_movement");
            if (storage.index_of("user_option_camera_rotation_speed") >= 0)
                user_option_camera_rotation_speed = storage.get_int("user_option_camera_rotation_speed");

            restored_camera_options_ = true;
        }

        ((GameClient*)client())->moby_clear_all();
        ((GameClient*)client())->refresh_hybrid_mobys();

        if (last_planet_ != current_planet) {
            last_planet_ = current_planet;

            ((GameClient*)client())->clear_hybrid_mobys();
            ((GameClient*)client())->moby_delete_all();
        }
    }
}

//static Profiler render_timer_ = Profiler("render");
void Game::on_render() {
//    Profiler::Scope scope(&render_timer_);

    // If loading, we shouldn't render anything;
    if (game_state == 6) {
        if (client()) {
            client()->send_ping();
            client()->drop_receive();
        }
        return;
    }

    if (current_view) {
        current_view->render();
    }

    return;
}

void Game::on_bink_do_frame() {
    if (client()) {
        client()->send_ping();
        client()->drop_receive();
    }
}


void Game::transition_to(View *view) {
    Logger::trace("Starting transition to a new view");

    if (current_view) {
        current_view->on_unload();
        delete current_view;
    }

    current_view = view;
    current_view->on_load();

    Logger::trace("Done transitioning");
}

Client* Game::client() {
    return client_;
}

Client* Game::connected_client() {
    if (client_ && client_->connected() && client_->handshake_complete()) {
        return client_;
    }

    return nullptr;
}

int Game::query_servers_callback(void* data, size_t len, void* extra) {
    if (!server_query_callback_) {
        Logger::error("Directory query callback, but server_query_callback_ not set. Bailing on unnecessary work");

        return -1;
    }

    Vector<GameServer*>* servers = new Vector<GameServer*>();

    int index = 0;
    int items = 0;

    Logger::debug("Response from directory. Size: %d", len);

    while (index < len && items < 10) {
        MPPacketQueryResponseServer* packet = (MPPacketQueryResponseServer*)&((char*)data)[index];

        // Names can't be longer than 50 chars.
        if (packet->name_length > 50) {
            Logger::error("Server name too long: %d", packet->name_length);
            index += sizeof(MPPacketQueryResponseServer) + packet->name_length;
            continue;
        }

        GameServer* server = new GameServer();

        server->ip = packet->ip;
        server->port = packet->port;
        server->max_players = packet->max_players;
        server->num_players = packet->player_count;

        char* server_name = (char*)allocate_memory(packet->name_length+1);
        char* description = (char*)allocate_memory(packet->description_length+1);
        char* owner_name = (char*)allocate_memory(packet->owner_name_length+1);

        memset(server_name, 0, packet->name_length+1);
        memset(description, 0, packet->description_length+1);
        memset(owner_name, 0, packet->owner_name_length+1);

        snprintf(server_name, packet->name_length+1, "%s", &((char*)data)[index+sizeof(MPPacketQueryResponseServer)]);
        snprintf(description, packet->description_length+1, "%s", &((char*)data)[index+sizeof(MPPacketQueryResponseServer)+packet->name_length]);
        snprintf(owner_name, packet->owner_name_length+1, "%s", &((char*)data)[index+sizeof(MPPacketQueryResponseServer)+packet->name_length+packet->description_length]);

        server->name = new String(server_name);
        server->description = new String(description);
        server->owner_name = new String(owner_name);

        free_memory(server_name);
        free_memory(description);
        free_memory(owner_name);

        Logger::debug("Server name: %s; Port: %d; Max players: %d, Description: %s, Owner: %s", server->name->c_str(), server->port, server->max_players, server->description->c_str(), server->owner_name->c_str());

        index += sizeof(MPPacketQueryResponseServer) + packet->name_length + packet->description_length + packet->owner_name_length;
        items += 1;

        servers->push_back(server);
    }

    if (server_query_callback_) {
        server_query_callback_((ServerListView*)Game::shared().current_view, servers);
    }

    delete servers;

    Client* client = Game::shared().client();
    client->disconnect();
    delete client;
    Game::shared().client_ = nullptr;

    return 0;
}

void Game::query_servers(int directory_id, ServerQueryCallback callback) {
    // FIXME: Unhardcode directory server.
    // - 172.104.144.15 -> boltcrate.space
    // We use IP instead of domain name because I don't trust that name resolution
    //   on PS3 will be stable forever or for everyone.
    client_ = new DirectoryClient("172.104.144.15", 2407);
    client_->_connect();

    server_query_callback_ = callback;
    ((DirectoryClient*)client_)->query_directory(directory_id, (AckCallback)&query_servers_callback);
}

void Game::connect_to(char* ip, int port) {
    client_ = new GameClient(ip, port);
    client_->_connect();

    delete current_view;
    current_view = 0;
}

void cb_dialog(int button_type, void* userdata) {
    Game::shared().dialogs_dismissed += 1;
}

void Game::alert(String& message) {
    cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL, message.c_str(), cb_dialog, nullptr, nullptr);
}

void Game::alert_blocking(String &message) {
    alert_blocking(message, 0);
}

void Game::alert_blocking(String& message, int timeout_seconds) {
    alert(message);

    int dd = dialogs_dismissed;
    int64_t start_time = get_time();
    while (dd == dialogs_dismissed) {
        cellSysutilCheckCallback();

        if (timeout_seconds > 0) {
            int64_t current_time = get_time();
            if (current_time - start_time > timeout_seconds * 1000) {
                cellMsgDialogAbort();
                break;
            }
        }
    }
}


void Game::refresh_level_flags() {
    memcpy(&level_flags1_, level_flags1 + 0x10 * current_planet, 0x10);
    memcpy(&level_flags2_, level_flags2 + 0x100 * current_planet, 0x100);
}

void Game::check_level_flags() {
    if (should_load_destination_planet) {
        reload_level_flags_ = true;
        return;
    } else if (reload_level_flags_) {
        reload_level_flags_ = false;
        refresh_level_flags();
    }

    MPPacketLevelFlag flags1[0x10];
    size_t flags1_changed = 0;
    for (int i = 0; i < 0x10; i++) {
        if (level_flags1_[i] != level_flags1[0x10 * current_planet + i]) {
            Logger::debug("Level flag %d changed from %d to %d", i, level_flags1_[i], level_flags1[0x10 * current_planet + i]);
            level_flags1_[i] = level_flags1[0x10 * current_planet + i];

            MPPacketLevelFlag flag = {1, i, level_flags1_[i]};
            flags1[flags1_changed] = flag;
            flags1_changed += 1;
        }
    }

    MPPacketLevelFlag flags2[0x100];
    size_t flags2_changed = 0;
    for (int i = 0; i < 0x100; i++) {
        if (level_flags2_[i] != level_flags2[0x100 * current_planet + i]) {
            level_flags2_[i] = level_flags2[0x100 * current_planet + i];

            MPPacketLevelFlag flag = {1, i, level_flags2_[i]};
            flags2[flags2_changed] = flag;
            flags2_changed += 1;
        }
    }

    Client* client = connected_client();
    if (client != nullptr) {
        if (flags1_changed) {
            Packet *packet = Packet::make_level_flags_changed_packet(MP_LEVEL_FLAG_TYPE_1, current_planet, flags1_changed, flags1);
            client->make_ack(packet, nullptr);
            client->send(packet);
        }

        if (flags2_changed) {
            Packet* packet = Packet::make_level_flags_changed_packet(MP_LEVEL_FLAG_TYPE_2, current_planet, flags2_changed, flags2);
            client_->make_ack(packet, nullptr);
            client_->send(packet);
        }
    }
}

void Game::on_save_operation(int action, void* savedata) {
    if (client()) {
        if (DataClient::shared() == nullptr || !DataClient::shared()->is_running()) {
            if (action == 3) {  // Don't show error if we're just trying to autosave.
                Logger::error("Couldn't autosave. DataClient not running.");
                return;
            }

            String message = String("Saving and loading is not available. The server might not support it.");
            alert_blocking(message, 10);
            return;
        }

        if (!manual_save_enabled) {
            if (action == 0 || action == 1) {
                String message = String("Saving and loading is not enabled on this server.");
                alert_blocking(message, 10);
            }
            return;
        }

        if (action == 0) {
            if (DataClient::shared()->busy()) {
                String message = String("A file operation is already in progress. Please try saving again in a moment.");
                alert_blocking(message, 10);
                return;
            }

            save(savedata);
        } else if (action == 1) {
            if (!have_save_file) {
                String message = String("There's no save file available to load. Either nothing is saved yet, or the server does not support save files.");
                alert_blocking(message, 10);
                return;
            }

            if (DataClient::shared()->busy()) {
                String message = String("A file operation is in progress, please wait a moment before loading the save again.");
                alert_blocking(message, 10);
                return;
            }

            load();
        } else if (action == 3) {
            if (DataClient::shared()->busy()) {
                Logger::error("Autosave failed! DataClient is busy");
                return;
            }

            Logger::info("Autosaving...");

            DataClient::transmit(MPFileTypeSavefile, savedata, 0xB0000);
        } else {
            Logger::error("Unknown save action!");
        }
    }
}

void Game::save(void* savedata) {
    Logger::info("Saving...");

    DataClient::transmit(MPFileTypeSavefile, savedata, 0xB0000);
}

void Game::load() {
    void* data = (void*)0x1000000;

    Logger::info("Loading savefile...");
    load_savefile(0, data);

    destination_planet = *(int*)((u32)data + 0x18);
    should_load_destination_planet = true;

    // Initialize Proxy Array based on unlocked weapons/gadgets/levels

    u8* unlock_array = (u8*)(0x96c140);
    u8* level_array = (u8*)(0x96ca0c);

    memcpy(proxy_item_array, unlock_array, 36);
    memcpy(proxy_level_array, level_array, proxy_level_array_size);

    // custom items also belong in proxy_item_array
    proxy_item_array[48] = *((u8*)0x96bff0); // Zoomerator
    proxy_item_array[49] = *((u8*)0x96bff1); // Raritanium
    proxy_item_array[50] = *((u8*)0x96bff2); // Codebot
    proxy_item_array[52] = *((u8*)0x96bff4); // Premium Nanotech
    proxy_item_array[53] = *((u8*)0x96bff5); // Ultra Nanotech
}


extern "C" void _c_game_tick() {
    Game::shared().on_tick();
}

extern "C" void _c_game_render() {
    Game::shared().on_render();
}

extern "C" void _c_game_start() {
    Game::shared().start();
}

extern "C" void _c_game_quit() {
    if (Game::shared().client()) {
        Game::shared().client()->disconnect();
    }
}

extern "C" void _c_on_respawn() {
    Game::shared().before_player_spawn();
}

extern "C" void _c_bink_do_frame() {
    Game::shared().on_bink_do_frame();
}

extern "C" void _c_on_save_operation(int action, void* savedata) {
    Game::shared().on_save_operation(action, savedata);
}

extern "C" void _c_game_reset() {
    Game::shared().reset();
}