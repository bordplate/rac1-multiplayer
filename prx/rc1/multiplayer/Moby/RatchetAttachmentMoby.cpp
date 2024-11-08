//
// Created by bordplate on 11/2/2024.
//

#include "RatchetAttachmentMoby.h"
#include "rc1/multiplayer/Client.h"
#include "rc1/Game.h"

#include <rc1/multiplayer/Packet.h>
#include <rc1/Player.h>

SyncedMoby* RatchetAttachmentMoby::make_synced_moby(Moby* moby, u8 position_bone, u8 transform_bone) {
    Logger::info("Making backpack moby");

    SyncedMoby* synced_moby = SyncedMoby::make_synced_moby(
        moby,
        MP_MOBY_FLAG_ATTACHED_TO,
        0,
        0,
        position_bone,
        transform_bone
    );

    synced_moby->p_update = nullptr;

    return synced_moby;
}