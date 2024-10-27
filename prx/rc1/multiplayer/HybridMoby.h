//
// Created by bordplate on 10/22/2024.
//

#ifndef RAC1_MULTIPLAYER_HYBRIDMOBY_H
#define RAC1_MULTIPLAYER_HYBRIDMOBY_H

#include <rc1/Moby.h>
#include "MonitoredValue.h"

struct HybridMoby {
public:
    HybridMoby(Moby* moby, u16 uid);
    ~HybridMoby();

    u16 uid;

    void monitor_attribute(u32 offset, u32 size);
    void monitor_pvar(u32 offset, u32 size);

    void set_moby(Moby* moby) { moby_ = moby; }

    void on_tick();
    void refresh_old_values_without_notifying_server();
private:
    Moby* moby_;

    Vector<MonitoredValue*> monitored_attributes_;
    Vector<MonitoredValue*> monitored_pvars_;
};

#endif //RAC1_MULTIPLAYER_HYBRIDMOBY_H
