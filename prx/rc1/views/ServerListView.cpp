//
// Created by bordplate on 12/27/2022.
//

#include "ServerListView.h"

#include "../Game.h"
#include "../TextElement.h"
#include "../Input.h"

#include "../PersistentStorage.h"


#include <lib/logger.h>

ServerListView::ServerListView(Vector<GameServer*>* servers) {
    servers_ = *servers;
}

ServerListView::~ServerListView() {

}

void ServerListView::render() {
    View::render();

    username_input_.check_callback();
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

    PersistentStorage storage = PersistentStorage("settings.conf");
    String username = storage.get_string("username");

    String label = username.length() > 0 ? String::format("\x13 %s", username.c_str()) : "\x13 Change nickname";

    if (username.length() <= 0) {
        username_input_.activate();
    }

    username_label_ = new TextElement(120, 390, label.c_str());

    this->add_element(username_label_);
    this->add_element(new TextElement(380, 390, "\x12 Back"));

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
                TextElement* element = new TextElement(250, y, "");

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

int ServerListView::username_input_callback(Input *input, void *userdata) {
    ServerListView* self = (ServerListView*)userdata;

    self->username_label_->text->setf("\x13 %s", input->input_text.c_str());

    PersistentStorage storage = PersistentStorage("settings.conf");
    storage.set("username", input->input_text);

    return 0;
}