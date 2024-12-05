//
// Created by Vetle Hjelle on 30/12/2022.
//

#ifndef RAC1_MULTIPLAYER_GAMECLIENT_H
#define RAC1_MULTIPLAYER_GAMECLIENT_H

#include "Client.h"

#include <rc1/rc1.h>
#include <rc1/game/Moby.h>

#include <lib/vector.h>

#include <rc1/views/RemoteView.h>
#include <rc1/multiplayer/HybridMoby.h>
#include <rc1/multiplayer/MonitoredValue.h>

// Maximum amount of moby's we can deal with
#define MAX_MP_MOBYS 1024

struct MobyInfo {
    Moby* moby;
    u16 uuid;
    u16 parent_uuid;
    u16 o_class;
    u16 flags;
    u16 mode_bits;
    u16 position_bone;
    u16 transform_bone;
};

struct GameClient : public Client {
    GameClient(char* ip, int port);

    void create_moby(MobyInfo* moby_info);
    void create_moby(MPPacketMobyCreate* packet);
    void update_moby(MPPacketMobyUpdate* packet);
    void update_moby_ex(MPPacketMobyExtended* packet);
    void change_moby_value(MPPacketChangeMobyValue* packet);
    void moby_delete(MPPacketMobyDelete* packet);
    void moby_clear_all();
    void moby_delete_all();
    void update_set_state(MPPacketSetState* packet);
    void update_set_text(MPPacketSetHUDText* packet);
    void toast_message(MPPacketToastMessage* packet);
    void register_hybrid_moby(MPPacketRegisterHybridMoby* packet);
    void refresh_hybrid_mobys();
    void clear_hybrid_mobys();
    void monitor_address(MPPacketMonitorAddress* packet);
    bool update(MPPacketHeader* header, void* packet_data);
    void on_tick();

    static int connect_callback(void* packetData, unsigned int size, void* userdata);
private:
    char* ip_;
    Vector<MobyInfo> mobys_;
    Vector<HybridMoby*> hybrid_mobys_;
    Vector<MonitoredValue*> monitored_addresses_;
    long ticks_;

    bool connection_complete_;

    RemoteView* remote_view_;

    void disconnect();
    void reset(); // override
    bool waiting_for_connect_;
};


#endif //RAC1_MULTIPLAYER_GAMECLIENT_H
