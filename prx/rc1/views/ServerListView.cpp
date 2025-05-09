//
// Created by bordplate on 12/27/2022.
//

#include "ServerListView.h"

#include "../Game.h"
#include "rc1/ui/TextElement.h"
#include "rc1/ui/Input.h"

#include "rc1/utils/PersistentStorage.h"
#include "rc1/multiplayer/Player.h"


#include <lib/logger.h>

const char* connecting_str = "Loading servers...";

ServerListView::ServerListView() {
    connecting_label_ = new TextElement(150, 200, "", ViewMovie);
}

ServerListView::~ServerListView() {

}

void ServerListView::set_servers(Vector<GameServer*>* servers) {
    servers_ = *servers;

    // Make the server list
    if (!servers_.empty()) {
        connecting_label_->text->set("");

        for (int i = 0; i < servers_.size(); i++) {
            GameServer* server = servers_[i];

            Logger::trace("Adding server '%s'[%d]", server->name->c_str(), i);

            if (server) {
                ListMenuItem item = ListMenuItem();

                item.title = *server->name;
                item.details = String::format("%d/%d players", server->num_players, server->max_players);

                servers_list_->add_item(item);
            }
        }

        servers_list_->visible = true;
        servers_list_->focus();
        this->on_item_selected(0, servers_list_);

        Logger::debug("Made server list");
    } else {
        connecting_label_->text->set("No servers available");
    }
}

void ServerListView::render() {
    frames_ += 1;
    if (frames_ > 8) {
        game_state = Movie;
    }

    View::render();

    username_input_.check_callback();
    direct_ip_input_.check_callback();
}

void ServerListView::refresh() {
    if (servers_list_ != nullptr) {
        servers_list_->clear_items();
        servers_list_->visible = false;
    }

    Logger::info("Querying game servers from directory");
    Game::shared().query_servers(0, (ServerQueryCallback)&server_query_callback);

    connecting_label_->text->set(connecting_str);
}

void ServerListView::on_load() {
    View::on_load();

    username_input_.set_callback(username_input_callback, this);
    username_input_.set_prompt("Enter username");

    direct_ip_input_.set_callback(direct_ip_input_callback, this);
    direct_ip_input_.set_prompt("Enter IP address");

    PersistentStorage storage = PersistentStorage("settings.conf");

    String username = storage.get_string("username");
    String last_direct_ip = storage.get_string("last_direct_ip");

    if (username.length() > 0) {
        username_input_.input_text = username;
    }

    if (last_direct_ip.length() > 0) {
        direct_ip_input_.input_text = last_direct_ip;
    }

    String label = username.length() > 0 ? String::format("\x13 %s", username.c_str()) : "\x13 Change nickname";

    if (username.length() <= 0) {
        username_input_.activate();
    } else {
        Player::shared().username = username;
    }

    username_label_ = new TextElement(120, 390, label.c_str(), ViewMovie);

    add_element(username_label_);
    add_element(new TextElement(380, 390, "\x12 Back", ViewMovie));
    add_element(new TextElement(380, 370, "\x11 Direct IP", ViewMovie));
    add_element(connecting_label_);

    add_element(new TextElement(250, 10, "Select server", ViewMovie));

    servers_list_ = new ListMenuElement(0, 40, 300, 320);
    servers_list_->visible = false;
    servers_list_->margins.width = 5;
    servers_list_->margins.height = 5;
    servers_list_->draws_background = true;
    servers_list_->set_delegate(this);

    add_element(servers_list_);

    description_area_ = new TextAreaElement(310, 40, 190, 160);
    description_area_->draws_background = true;
    description_area_->margins.width = 5;
    description_area_->margins.height = 5;

    info_area_ = new TextAreaElement(310, 205, 190, 155);
    info_area_->draws_background = true;
    info_area_->margins.width = 5;
    info_area_->margins.height = 5;

    add_element(description_area_);
    add_element(info_area_);

    refresh();
}

void ServerListView::on_unload() {
    View::on_unload();

    delete servers_list_;
    delete connecting_label_;
    delete username_label_;

    game_state = PlayerControl;
}

void ServerListView::on_pressed_buttons(CONTROLLER_INPUT input) {
    View::on_pressed_buttons(input);

    if (input & Square) {
        username_input_.activate();
    }

    if (input & Circle) {
        direct_ip_input_.activate();
    }

    if (input & Triangle) {
        game_state = PlayerControl;
    }
}

int ServerListView::username_input_callback(Input *input, void *userdata, int status) {
    ServerListView *self = (ServerListView *)userdata;

    if (status == 0) {
        self->username_label_->text->setf("\x13 %s", input->input_text.c_str());

        PersistentStorage storage = PersistentStorage("settings.conf");
        storage.set("username", input->input_text);
        Player::shared().username = input->input_text;
    } else {
        if (Player::shared().username.length() <= 0) {
            self->username_input_.activate();
        }
    }

    return 0;
}

int ServerListView::direct_ip_input_callback(Input *input, void *userdata, int status) {
    ServerListView *self = (ServerListView *)userdata;

    if (status == 0) {
        PersistentStorage storage = PersistentStorage("settings.conf");
        storage.set("last_direct_ip", input->input_text);

        int port = 2407;

        // Get port from input if it's there
        if (input->input_text.find(":") != -1) {
            String port_str = input->input_text.substr(input->input_text.find(":") + 1);
            port = atoi(port_str.c_str());
        }

        Game::shared().connect_to((char*)input->input_text.c_str(), port);
    }

    return 0;
}

void ServerListView::server_query_callback(ServerListView* self, Vector<GameServer*>* servers) {
    Logger::debug("Got servers from directory");

    self->set_servers(servers);
}

void ServerListView::on_item_activated(int index, ListMenuElement* list_menu_element) {
    GameServer* server = servers_[index];

    // This is kind of dumb, but it was easier than making constructors that takes int as IP address
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server->ip, ip, INET_ADDRSTRLEN);

    Logger::info("Selected server '%s'", server->name->c_str());

    Game::shared().connect_to(ip, server->port);
}

void ServerListView::on_item_selected(int index, ListMenuElement *list_menu_element) {
    description_area_->text.setf("%s", servers_[index]->description->c_str());

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &servers_[index]->ip, ip, INET_ADDRSTRLEN);

    info_area_->text.setf("Players: %d/%d\1Owner: %s\1\1IP: %s\1Port: %d\1",
                          servers_[index]->num_players,
                          servers_[index]->max_players,
                          servers_[index]->owner_name->c_str(),
                          ip,
                          servers_[index]->port
    );
}
