#ifndef MOBY_H
#define MOBY_H

#include <lib/shk.h>
#include <lib/types.h>

//#include <lib/common.h>
//#include <rc1/common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u16 uuid;
    char next_animation_id;
    char o_class;
    int animation_duration;
    u16 sig;
} MPMobyVars;

typedef struct MobySeq { /* From Deadlocked types */
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    u8 frame_count;
    u8 next_seq;
    u8 trigsCnt;
    u8 pad;
    short *trigs;
    void *animInfo;
    void *frameData;
} MobySeq;

typedef struct MobyClass { /* MobyClass from Deadlocked Types, probably very wrong */
    void *packets;
    u8 pakcet_cnt_0;
    u8 packet_cnt_1;
    u8 metal_cnt;
    u8 metal_ofs;
    u8 joint_cnt;
    u8 pad;
    u8 packet_cnt_2;
    u8 team_texs; /* Obvs wrong, ain't no teams in Rac1 */
    u8 seq_cnt;
    u8 sound_cnt;
    u8 lod_trans;
    u8 shadow;
    u16 *collision;
    void *skeleton;
    void *common_trans;
    void *anim_joints;
    void *gif_usage;
    float gScale;
    void *SoundDefs;
    char bangles;
    char mip_dist;
    short corncob;
    undefined field23_0x30;
    undefined field24_0x31;
    undefined field25_0x32;
    undefined field26_0x33;
    undefined field27_0x34;
    undefined field28_0x35;
    undefined field29_0x36;
    undefined field30_0x37;
    undefined field31_0x38;
    undefined field32_0x39;
    undefined field33_0x3a;
    undefined field34_0x3b;
    undefined field35_0x3c;
    undefined field36_0x3d;
    undefined field37_0x3e;
    undefined field38_0x3f;
    struct Moby *unk_ptr;
    u32 mode_bits;
    char type;
    char mode_bits2;
    struct MobySeq *seqs;
} MobyClass;

#ifdef __cplusplus
#pragma pack(push, 1)
#endif

struct Moby {
    Vec4 coll_pos;
    Vec4 position;
    char state;
    char group;
    char mClass;
    char alpha;
    MobyClass *pClass;
    void *pChain;
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
    Vec4 rotation;
    char field26_0x50;
    char animationFrame;
    u8 updateID;
    u8 animationID;
    float field30_0x54;
    float field34_0x58;
    float field35_0x5c;
    void *field36_0x60;
    char *field40_0x64;
    void *field41_0x68;
    void *field42_0x6c;
    char field43_0x70;
    char field44_0x71;
    char field45_0x72;
    char field46_0x73;
    void *pUpdate;
    void *pVars;
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
    struct Moby *parent;
    unsigned short *collision;
    float *collisionMesh;
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
    struct Moby *field80_0xb8;
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
    u32 field127_0xf0;
    u32 field128_0xf4;
    u32 field132_0xf8;
    unsigned int field136_0xfc;

#ifdef __cplusplus
    void set_animation(char animation_id, char animation_frame, u32 duration);
    void check_collision();

    static Moby* spawn(unsigned short o_class, unsigned short flags, uint16_t modeBits);
#endif
}
#ifdef __cplusplus
__attribute__((aligned(1)));

#pragma pack(pop)

#endif

#ifndef __cplusplus
typedef struct Moby Moby;
#endif

Moby *moby_spawn_hook(s32 o_class);

void moby_animate(Moby *self);

void moby_update(Moby *self);

//          CollMobysSphere(double param_1,vec4 *param_2,undefined8 param_3,uint flags,Moby *param_5,
//                        undefined8 param_6)

SHK_FUNCTION_DEFINE_STATIC_6(0x5e598, int, coll_mobys_sphere, float, param_1, Vec4*, param_2, int, param_3, u32, flags,
                             Moby*, param_5, int*, param_6);
SHK_FUNCTION_DEFINE_STATIC_4(0x59e20, int, coll_sphere, Vec4*, position, Vec4*, param_2, int, flags, Moby*, moby);
SHK_FUNCTION_DEFINE_STATIC_4(0xfddc0, void, set_moby_animation, Moby*, moby, u32, param_2, u32, param_3, u32, param_4);
SHK_FUNCTION_DEFINE_STATIC_1(0xefa28, Moby*, spawn_moby, int, o_class);
SHK_FUNCTION_DEFINE_STATIC_1(0xf31a8, int, idk, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_5(0xf1ea0, int, idk2, void*, p1, s64, p2, u64, p3, s64, p4, s64, p5);
SHK_FUNCTION_DEFINE_STATIC_1(0xefb38, void, delete_moby, Moby*, moby);

SHK_FUNCTION_DEFINE_STATIC_1(0xf0f9c, u32, FUN_000f0f9c, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0x502730, double, int_to_double, int, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0xef74c, int, moby_frame_data_something, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0x502730, double, long_long_to_double, u64, p1);
SHK_FUNCTION_DEFINE_STATIC_2(0xfde80, int, FUN_000fa940, void*, p1, u32, p2);

//void set_moby_animation(Moby *moby, u8 param_2, u8 param_3, double param_4);


int idk(void *);

int idk2(void *p1, s64 p2, u64 p3, s64 p4, s64 p5);

//int item_to_oclass(ITEM item);

#ifdef __cplusplus
}
#endif

#endif