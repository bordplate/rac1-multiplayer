//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_MPMOBY_H
#define PS3_CKIT_MPMOBY_H

#include "rc1/game/Moby.h"

#pragma pack(push, 1)

typedef struct {
    u32 unk1;
    u32 unk2;
    u8 unk3_f1;
    u8 unk3_f2;
    u8 unk3_f3;
    u8 unk3_f4;
} MovablePlatformConfiguration;

typedef struct {
    float unk1;
    float unk2;
    float unk3;
    u32 pad;
    Vec4 rotation;
} MobyOrientationInfo;

typedef struct {
    void* config_pointer1;
    void* config_pointer2;
    void* config_pointer3;
    u16 uuid;
    u16 flags;
    bool attached_to_parent;
    Moby* parent;
    u16 parent_o_class;
    u8 parent_position_bone;
    u8 parent_transform_bone;
    char next_animation_id;
    int animation_duration;
    Vec4 last_position;
    MovablePlatformConfiguration platform_configuration;
    MobyOrientationInfo platform_orientation_info;
} MPMobyVars;

#pragma pack(pop)

struct MPMoby : public Moby {
public:
    void update();

    static MPMoby* spawn(uint16_t uuid, unsigned short o_class, unsigned short flags, uint16_t modeBits);
};


#endif //PS3_CKIT_MPMOBY_H
