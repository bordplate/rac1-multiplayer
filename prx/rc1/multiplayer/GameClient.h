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
    GameClient(char* ip, int port);

    void update_moby(MPPacketMobyUpdate* packet);
    void moby_delete(MPPacketMobyCreate* packet);
    void moby_clear_all();
    void moby_delete_all();
    void update_set_state(MPPacketSetState* packet);
    void update_set_text(MPPacketSetHUDText* packet);
    void toast_message(MPPacketToastMessage* packet);
    bool update(MPPacketHeader* header, void* packet_data);
    void on_tick();

    static int connect_callback(void* packetData, unsigned int size, void* userdata);
private:
    char* ip_;
    Vector<Moby*> mobys_;
    long ticks_;

    bool connection_complete_;

    RemoteView* remote_view_;

    void disconnect();
    void reset(); // override
    bool waiting_for_connect_;
};


#endif //RAC1_MULTIPLAYER_GAMECLIENT_H
