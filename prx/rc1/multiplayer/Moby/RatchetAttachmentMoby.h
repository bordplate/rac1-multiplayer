//
// Created by bordplate on 11/2/2024.
//

#ifndef RAC1_MULTIPLAYER_RATCHETATTACHMENTMOBY_H
#define RAC1_MULTIPLAYER_RATCHETATTACHMENTMOBY_H

#include <rc1/multiplayer/SyncedMoby.h>

struct RatchetAttachmentMoby : public SyncedMoby {
public:
    static SyncedMoby* make_synced_moby(Moby* moby, u8 position_bone, u8 transform_bone);
};


#endif //RAC1_MULTIPLAYER_RATCHETATTACHMENTMOBY_H
