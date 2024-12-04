//
// Created by bordplate on 11/1/2024.
//

#ifndef RAC1_MULTIPLAYER_SYNCEDMOBY_H
#define RAC1_MULTIPLAYER_SYNCEDMOBY_H

#include <lib/types.h>
#include "rc1/game/Moby.h"

typedef enum {
    SyncedMobyStatusNone = 0,
    SyncedMobyStatusReady = 1,
    SyncedMobyStatusWaiting = 2,
    SyncedMobyStatusConnecting = 3,
    SyncedMobyStatusActive = 4,
} SyncedMobyStatus;

struct SyncedMobyVars {
    u16 uuid;
    u16 parent_uuid;
    u16 flags;
    void* (*p_update)(Moby*);
    uint64_t connection_start_time;
    SyncedMobyStatus status;
    u8 position_bone;
    u8 transform_bone;
    Moby* moby;
};

struct SyncedMoby : public Moby {
public:
    void update();
    void activate();
    SyncedMobyVars* get_vars();

    static void synced_update(SyncedMoby* moby);

    static SyncedMoby* make_synced_moby(
        Moby* moby,
        u16 flags = 0,
        u16 mode_bits = 0,
        u16 uuid = 0,
        u8 position_bone = 0,
        u8 transform_bone = 0
    );
    static int moby_create_callback(void* data, size_t len, void* extra);
private:
    void reset();
};

static void* SyncedUpdate = (void*)&SyncedMoby::synced_update;


#endif //RAC1_MULTIPLAYER_SYNCEDMOBY_H
