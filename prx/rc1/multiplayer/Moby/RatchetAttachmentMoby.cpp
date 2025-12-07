//
// Created by bordplate on 11/2/2024.
//

#include "RatchetAttachmentMoby.h"
#include "rc1/multiplayer/network/Client.h"
#include "rc1/Game.h"

#include "rc1/multiplayer/network/Packet.h"
#include "rc1/multiplayer/Player.h"

SyncedMoby* RatchetAttachmentMoby::make_synced_moby(Moby* moby, u8 position_bone, u8 transform_bone) {
    Logger::info("Making Ratchet attachment moby");

    if (moby->o_class == 1290) {
        // Pilot's Helmet has an update func that it uses only on Gaspar for its pickup, so we null it here to make
        //  so we don't run it either.
        moby->p_update = nullptr;
    }

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