#ifndef MOBY_H
#define MOBY_H

#include <lib/common.h>
#include <rc1/common.h>

typedef struct {
	char uuid;
	char next_animation_id;
    char o_class;
} MPMobyVars;

typedef struct {
	vec4 coll_pos;
    vec4 position;
    char state;
    char group;
    char mClass;
    char alpha;
    void* pClass;
    void* pChain;
    float scale;
    char update_distance;
    char enabled;
    short draw_distance;
    unsigned short mode_bits;
    unsigned short field19_0x36;
    unsigned int stateTimerMaybe;
    char field21_0x3c;
    char field22_0x3d;
    char field23_0x3e;
    char field24_0x3f;
    vec4 rotation;
    char field26_0x50;
    char animationFrame;
    char updateID;
    u8 animationID;
    char field30_0x54;
    char field31_0x55;
    char field32_0x56;
    char field33_0x57;
    float field34_0x58;
    float field35_0x5c;
    char field36_0x60;
    char field37_0x61;
    char field38_0x62;
    char field39_0x63;
    char* field40_0x64;
    void* field41_0x68;
    void* field42_0x6c;
    char field43_0x70;
    char field44_0x71;
    char field45_0x72;
    char field46_0x73;
    void* pUpdate;
    void* pVars;
    char field49_0x7c;
    char field50_0x7d;
    char field51_0x7e;
    char animStateMaybe;
    unsigned int field53_0x80;
    int field54_0x84;
    int field55_0x88;
    char field56_0x8c;
    char field57_0x8d;
    char field58_0x8e;
    char field59_0x8f;
    struct Moby* parent;
    unsigned short* collision;
    float* collisionMesh;
    unsigned int field63_0x9c;
    char field64_0xa0;
    char field65_0xa1;
    char field66_0xa2;
    char field67_0xa3;
    char field68_0xa4;
    char field69_0xa5;
    short oClass;
    int field71_0xa8;
    unsigned int field72_0xac;
    char field73_0xb0;
    char field74_0xb1;
    unsigned short UID;
    char field76_0xb4;
    char field77_0xb5;
    char field78_0xb6;
    char field79_0xb7;
    struct Moby* field80_0xb8;
    char field81_0xbc;
    char field82_0xbd;
    char field83_0xbe;
    char field84_0xbf;
    float field85_0xc0;
    float field86_0xc4;
    char field87_0xc8;
    char field88_0xc9;
    char field89_0xca;
    char field90_0xcb;
    char field91_0xcc;
    char field92_0xcd;
    char field93_0xce;
    char field94_0xcf;
    char field95_0xd0;
    char field96_0xd1;
    char field97_0xd2;
    char field98_0xd3;
    char field99_0xd4;
    char field100_0xd5;
    char field101_0xd6;
    char field102_0xd7;
    char field103_0xd8;
    char field104_0xd9;
    char field105_0xda;
    char field106_0xdb;
    char field107_0xdc;
    char field108_0xdd;
    char field109_0xde;
    char field110_0xdf;
    char field111_0xe0;
    char field112_0xe1;
    char field113_0xe2;
    char field114_0xe3;
    char field115_0xe4;
    char field116_0xe5;
    char field117_0xe6;
    char field118_0xe7;
    char field119_0xe8;
    char field120_0xe9;
    char field121_0xea;
    char field122_0xeb;
    char field123_0xec;
    char field124_0xed;
    char field125_0xee;
    char field126_0xef;
    void* field127_0xf0;
    char field128_0xf4;
    char field129_0xf5;
    char field130_0xf6;
    char field131_0xf7;
    char field132_0xf8;
    char field133_0xf9;
    char field134_0xfa;
    char field135_0xfb;
    unsigned int field136_0xfc;
} Moby;

Moby* moby_spawn_hook(s32 o_class);
void moby_update(Moby* self);

SHK_FUNCTION_DEFINE_STATIC_4(0xfddc0, void, set_moby_animation, Moby*, moby, u8, param_2, u8, param_3, double, param_4);
SHK_FUNCTION_DEFINE_STATIC_1(0xefa28, Moby*, spawn_moby, int, o_class);
SHK_FUNCTION_DEFINE_STATIC_1(0xf31a8, int, idk, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_5(0xf1ea0, int, idk2, void*, p1, s64, p2, u64, p3, s64, p4, s64, p5);

void set_moby_animation(Moby *moby, u8 param_2, u8 param_3, double param_4);


int idk( void* );
int idk2(void* p1, s64 p2, u64 p3, s64 p4, s64 p5);

int item_to_oclass(ITEM item);

#endif