//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_MPMOBY_H
#define PS3_CKIT_MPMOBY_H

#include "rc1/game/Moby.h"

typedef struct {
    u8 pad[10];
    u16 uuid;
    bool attached_to_parent;
    Moby* parent;
    u8 parent_position_bone;
    u8 parent_transform_bone;
    char next_animation_id;
    int animation_duration;
    u8 collision_debounce;
} MPMobyVars;

struct MPMoby : public Moby {
public:
    void update();

    static MPMoby* spawn(uint16_t uuid, unsigned short o_class, unsigned short flags, uint16_t modeBits);
};


#endif //PS3_CKIT_MPMOBY_H
