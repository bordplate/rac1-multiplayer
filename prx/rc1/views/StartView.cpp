//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "StartView.h"

#include <lib/logger.h>

#include "ServerListView.h"


#include "../Game.h"
#include "../TextElement.h"


const char* start_multiplayer_str = "\x13 Start multiplayer";
const char* connecting_str = "Loading servers...";

StartView::~StartView() {

}

void StartView::on_load() {
    label_ = new TextElement(250, 310, start_multiplayer_str);

    this->add_element(label_);
}

void StartView::render() {
    View::render();
}

void StartView::on_pressed_buttons(CONTROLLER_INPUT input) {
    if (input & Square) {
        Logger::info("Querying game servers from directory");
        Game::shared().query_servers(0, (ServerQueryCallback)&server_query_callback);

        label_->text->set(connecting_str);
    }
}

void StartView::server_query_callback(Vector<GameServer*>* servers) {
    ServerListView* view = new ServerListView(servers);
    Game::shared().transition_to(view);
}