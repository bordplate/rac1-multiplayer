//
// Created by bordplate on 10/27/2024.
//

#ifndef RAC1_MULTIPLAYER_MONITOREDVALUE_H
#define RAC1_MULTIPLAYER_MONITOREDVALUE_H

#include <lib/types.h>

struct MonitoredValue {
    u32 offset;
    u32 size;
    u32 old_value;
};

#endif //RAC1_MULTIPLAYER_MONITOREDVALUE_H
