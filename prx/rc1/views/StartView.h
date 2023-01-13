//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_STARTVIEW_H
#define PS3_CKIT_STARTVIEW_H

#include <rc1/View.h>

#include "../TextElement.h"
#include "../multiplayer/GameServer.h"

struct StartView : public View {
public:
    ~StartView();

    void render();

    void on_load();

    void on_pressed_buttons(CONTROLLER_INPUT input);
private:
    bool loading_server_list_;
    TextElement* label_;

    static void server_query_callback(Vector<GameServer*>* servers);
};


#endif //PS3_CKIT_STARTVIEW_H
