//
// Created by bordplate on 12/27/2022.
//

#include "ServerListView.h"

#include "../Game.h"
#include "../TextElement.h"
#include "../Input.h"

#include "../PersistentStorage.h"
#include <rc1/Player.h>


#include <lib/logger.h>

ServerListView::ServerListView(Vector<GameServer*>* servers) {
    servers_ = *servers;
}

ServerListView::~ServerListView() {

}

void ServerListView::render() {
    View::render();

    username_input_.check_callback();
    direct_ip_input_.check_callback();
}

void ServerListView::update_server_list() {
    for (int i = 0; i < servers_elements_.size(); i++) {
        TextElement* element = servers_elements_[i];
        if (i == selected_server_) {
            element->color.b = 0;
        } else {
            element->color.b = 255;
        }
    }
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

    this->add_element(username_label_);
    this->add_element(new TextElement(380, 390, "\x12 Back", ViewMovie));
    this->add_element(new TextElement(380, 370, "\x11 Direct IP", ViewMovie));

    // Make the server list
    if (servers_.size() > 0) {
        for (int i = 0; i < servers_.size(); i++) {
            GameServer* server = servers_[i];

            Logger::trace("Adding server '%s'[%d]", server->name->c_str(), i);

            if (server) {
                // Calculate the Y coordinate for the element
                int total_height = servers_.size() * 30;
                int y = ((500 - total_height) - total_height / 2) / 2 + i * 30;

                // Create a new text element for the game server
                TextElement* element = new TextElement(250, y, "", ViewMovie);

                // Set the element's color
                element->color.b = (i == selected_server_) ? 0 : element->color.b;

                // Set the element's text
                element->text->setf("%s (%d/%d)", server->name->c_str(), server->num_players, server->max_players);

                this->servers_elements_.push_back(element);
                this->add_element(element);
            }
        }

        Logger::debug("Made server list");
    }

    // Setting this game state just removes the "Start/load game" buttons on the main screen
    game_state = Movie;
}

void ServerListView::on_unload() {
    View::on_unload();

    game_state = Menu;
}

void ServerListView::on_pressed_buttons(CONTROLLER_INPUT input) {
    View::on_pressed_buttons(input);

    // Server list interaction
    if (input & Up) {
        selected_server_ = (selected_server_ + servers_.size() - 1) % servers_.size();
        update_server_list();
    } else if (input & Down) {
        selected_server_ = (selected_server_ + 1) % servers_.size();
        update_server_list();
    }

    if (input & Square) {
        username_input_.activate();
    }

    if (input & Circle) {
        direct_ip_input_.activate();
    }

    if (input & Triangle) {
        // Setting game_state to Menu should make the main MP game loop delete this view and present
        //  the relevant StartView.
        game_state = Menu;
    }

    if (input & Cross) {
        GameServer* server = servers_[selected_server_];

        // This is kind of dumb, but it was easier than making constructors that takes int as IP address
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &server->ip, ip, INET_ADDRSTRLEN);

        Logger::info("Selected server '%s'", server->name->c_str());

        Game::shared().connect_to(ip, server->port);
    }
}

int ServerListView::username_input_callback(Input *input, void *userdata, int status) {
    ServerListView *self = (ServerListView *) userdata;

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