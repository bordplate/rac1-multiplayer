//
// Created by bordplate on 12/27/2022.
//

#ifndef RAC1_MULTIPLAYER_SERVERLISTVIEW_H
#define RAC1_MULTIPLAYER_SERVERLISTVIEW_H

#include <lib/vector.h>

#include "../View.h"
#include "../TextElement.h"
#include "../multiplayer/GameServer.h"

#include "../Input.h"

struct ServerListView : public View {
    ServerListView(Vector<GameServer*>* servers);
    ~ServerListView();

    void render();
    void on_load();
    void on_unload();

    void on_pressed_buttons(CONTROLLER_INPUT input);
private:
    Vector<GameServer*> servers_;
    Vector<TextElement*> servers_elements_;

    TextElement* username_label_;

    int selected_server_;

    Input username_input_;

    void update_server_list();

    static int username_input_callback(Input* input, void* userdata);
};


#endif //RAC1_MULTIPLAYER_SERVERLISTVIEW_H
