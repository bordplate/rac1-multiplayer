//
// Created by Vetle Hjelle on 30/12/2022.
//

#ifndef RAC1_MULTIPLAYER_GAMECLIENT_H
#define RAC1_MULTIPLAYER_GAMECLIENT_H

#include "Client.h"

#include <rc1/rc1.h>
#include <rc1/Moby.h>

#include <lib/vector.h>

#include "../views/RemoteView.h"

// Maximum amount of moby's we can deal with
#define MAX_MP_MOBYS 1024

struct GameClient : public Client {
    GameClient(char* ip, int port) : Client(ip, port) { mobys_.reserve(MAX_MP_MOBYS); };

    void update_moby(MPPacketMobyUpdate* packet);
    void moby_delete(MPPacketMobyCreate* packet);
    void moby_delete_all();
    void update_set_state(MPPacketSetState* packet);
    void update_set_text(MPPacketSetHUDText* packet);
    bool update(MPPacketHeader* header, void* packet_data);
    void on_tick();

private:
    Vector<Moby*> mobys_;
    long ticks_;

    RemoteView* remote_view_;
};


#endif //RAC1_MULTIPLAYER_GAMECLIENT_H
