#ifndef RC1_COMMON_H
#define RC1_COMMON_H

#include <lib/common.h>

#define MULTI_TRACE_LOG 0
#define MULTI_LOG(msg, ...) printf("rac1multi: " msg, ##__VA_ARGS__)
//#define MULTI_LOG(msg, ...) do {} while (false)
#if MULTI_TRACE_LOG
#define MULTI_TRACE(msg, ...) printf("*rac1multi: " msg, ##__VA_ARGS__)
#else
#define MULTI_TRACE(msg, ...) do {} while (false)
#endif

struct Size {
    u32 width;
    u32 height;
};

typedef enum ITEM {
    HeliPack=2,
    ThrusterPack=3,
    HydroPack=4,
    SonicSummoner=5,
    O2Mask=6,
    PilotsHelmet=7,
    Wrench=8,
    SuckCannon=9,
    BombGlove=10,
    Devastator=11,
    Swingshot=12,
    VisibombGun=13,
    Taunter=14,
    Blaster=15,
    Pyrocitor=16,
    MineGlove=17,
    Walloper=18,
    TeslaClaw=19,
    GloveOfDoom=20,
    MorphORay=21,
    Hydrodisplacer=22,
    RYNO=23,
    DroneDevice=24,
    DecoyGlove=25,
    Trespasser=26,
    MetalDetector=27,
    Magneboots=28,
    Grindboots=29,
    Hoverboard=30,
    Hologuise=31,
    PDA=32,
    MapOMatic=33,
    BoltGrabber=34,
    Persuader=35
} ITEM;

typedef enum EnableCommunicationsFlags {
    ENABLE_ON_UNLOCK_ITEM     =0x00000001,
    ENABLE_ON_UNLOCK_LEVEL    =0x00000002,
    ENABLE_ON_PICKUP_GOLD_BOLT=0x00000004,
    ENABLE_ON_GET_BOLTS       =0x00000008,

    ENABLE_ALL=                0xffffffff
} EnableCommunicationsFlags;

extern EnableCommunicationsFlags enable_communication_bitmap;

#endif
