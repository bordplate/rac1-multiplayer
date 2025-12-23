#ifndef MOBY_H
#define MOBY_H

#include <lib/shk.h>
#include <lib/types.h>
#include "Moby.h"

#ifdef __cplusplus

struct Manipulator {
    char animation_bone;
    char state;
    char scaleOn;
    char absolute;
    int bone_id;
    Manipulator *pNext;
    float animation_blend_t;
    Vec4 rotation;
    Vec4 scale;
    Vec4 translation;
};

// Should be in Packet.h, but something is stopping us from including it here so we'll just define it here
typedef struct {
    u16 offset;
    u16 size;
    u32 value;
} MPPacketChangeMobyValuePayload;

extern "C" {
#endif

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
    s8 state;
    char group;
    char m_class;
    char alpha;
    MobyClass *p_class;
    void *p_chain;
    float scale;
    char update_distance;
    char enabled;
    short draw_distance;
    unsigned short mode_bits;
    unsigned short field19_0x36;
    unsigned int color;
    char field21_0x3c;
    char field22_0x3d;
    char field23_0x3e;
    char field24_0x3f;
    Vec4 rotation;
    char field26_0x50;
    char animation_frame;
    u8 update_id;
    u8 animation_id;
    float field30_0x54;
    float field34_0x58;
    float field35_0x5c;
    void *field36_0x60;
    struct Manipulator* manipulator1;
    void *field41_0x68;
    void *field42_0x6c;
    char field43_0x70;
    char field44_0x71;
    char field45_0x72;
    char field46_0x73;
    void* p_update;
    void* vars;
    char field49_0x7c;
    char field50_0x7d;
    char field51_0x7e;
    char animStateMaybe;
    struct Manipulator* manipulator2;
    int field54_0x84;
    int field55_0x88;
    char field56_0x8c;
    char field57_0x8d;
    char field58_0x8e;
    char field59_0x8f;
    unsigned int field60_0x90;
    unsigned short *collision;
    float *collision_mesh;
    unsigned int field63_0x9c;
    char field64_0xa0;
    char field65_0xa1;
    char field66_0xa2;
    char field67_0xa3;
    s8 damage;
    char field69_0xa5;
    short o_class;
    int field71_0xa8;
    unsigned int field72_0xac;
    char field73_0xb0;
    char field74_0xb1;
    unsigned short uid;
    char field76_0xb4;
    char field77_0xb5;
    char field78_0xb6;
    char field79_0xb7;
    struct Moby *field80_0xb8;
    char field81_0xbc;
    char field82_0xbd;
    char field83_0xbe;
    char field84_0xbf;
    Vec4 transform[4];

#ifdef __cplusplus
    void set_animation(char animation_id, char animation_frame, u32 duration);
    void check_collision();

    void change_values(MPPacketChangeMobyValuePayload* changes, size_t num, u16 value_type);
    struct Damage* get_damage(u32 flags, u32 unk);

    static Moby* spawn(unsigned short o_class, unsigned short flags, uint16_t modeBits);
    static Moby* find_by_uid(u16 uid);
    static Moby* find_first_oclass(u16 o_class);
#endif
}

#ifdef __cplusplus
__attribute__((aligned(1)));

#pragma pack(pop)

#endif

#ifndef __cplusplus
typedef struct Moby Moby;
#endif

SHK_FUNCTION_DEFINE_STATIC_4(0x112900, void, get_moby_orientation_info, void*, unk0, void*, unk1, Vec4*, rot1, Vec4*, rot);
SHK_FUNCTION_DEFINE_STATIC_6(0x5e598, int, coll_mobys_sphere, float, param_1, Vec4*, param_2, int, param_3, u32, flags,
                             Moby*, param_5, int*, param_6);
SHK_FUNCTION_DEFINE_STATIC_4(0x59e20, int, coll_sphere, Vec4*, position, float, param_2, int, flags, Moby*, moby);
SHK_FUNCTION_DEFINE_STATIC_4(0xfddc0, void, set_moby_animation, Moby*, moby, u32, param_2, u32, param_3, u32, param_4);
SHK_FUNCTION_DEFINE_STATIC_1(0xf31a8, int, idk, Moby*, p1);
SHK_FUNCTION_DEFINE_STATIC_5(0xf1ea0, int, idk2, void*, p1, s64, p2, u64, p3, s64, p4, s64, p5);
SHK_FUNCTION_DEFINE_STATIC_1(0xefb38, void, delete_moby, Moby*, moby);

SHK_FUNCTION_DEFINE_STATIC_1(0xf0f9c, u32, FUN_000f0f9c, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0x117c60, u32, player_is_standing_on, Moby*, moby);
SHK_FUNCTION_DEFINE_STATIC_1(0x502730, double, int_to_double, int, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0xef74c, int, moby_frame_data_something, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0x502730, double, long_long_to_double, u64, p1);
SHK_FUNCTION_DEFINE_STATIC_2(0xfde80, int, FUN_000fa940, void*, p1, u32, p2);
SHK_FUNCTION_DEFINE_STATIC_3(0x119a18, void, do_manipulator, Moby*, moby, u32, bone, Manipulator*, manipulator);
SHK_FUNCTION_DEFINE_STATIC_3(0x0f0ea8, void, set_manipulator, Moby*, moby, u32, bone, Manipulator*, manipulator);
SHK_FUNCTION_DEFINE_STATIC_3(0xf1114, void, get_bone_position, Moby*, moby, u32, bone, Vec4*, out);
SHK_FUNCTION_DEFINE_STATIC_3(0xf1038, void, get_bone_transformations, Moby*, moby, u32, bone, Vec4*, out);
SHK_FUNCTION_DEFINE_STATIC_2(0x500fa8, void, apply_transformations, Vec4*, p1, Vec4*, p2);
//SHK_FUNCTION_DEFINE_STATIC_3(0x10675c, Damage*, get_damage, Moby*, moby, u32, flags, u32, unk);

#ifdef __cplusplus
}
#endif

#endif