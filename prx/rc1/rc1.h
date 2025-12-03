#ifdef GAME_RC1

#ifndef RC1_H
#define RC1_H

#include <lib/shk.h>
#include <lib/types.h>
#include "rc1/game/Moby.h"

#ifdef __cplusplus
#include "game/Moby.h"

extern "C" {
#endif

typedef enum GameState {
    PlayerControl = 0,
    Movie = 1,
    CutScene = 2,
    Menu = 3,
    ExitRace = 4,
    Gadgetron = 5,
    PlanetLoading = 6,
    CinematicMaybe = 7,
    UnkFF = 255
} GameState;

typedef enum PlayerType {
    PlayerTypeRatchet = 0x0,
    PlayerTypeClank = 0x1,
    PlayerTypeBigClank = 0x2,
    PlayerTypeHologuise = 0x3,
} PlayerType;

typedef struct {
    s16 y_min;
    s16 y_max;
    s16 x_min;
    s16 x_max;
    s16 x;
    s16 y;
    u16 unk_0c; /* probably an output */
    u16 unk_0e; /* probably an output */
    s16 line_spacing;
    u16 flags;
} TextOpt;

// Our global variables
extern int game_ticks;

extern int current_weapon;

extern int planetUnlockedByServer;

SHK_FUNCTION_DEFINE_STATIC_1(0x0f58d8, int, compute_animation, Moby*, moby);
SHK_FUNCTION_DEFINE_STATIC_1(0xe7984, void, savedata_unk, void*, param_1);
SHK_FUNCTION_DEFINE_STATIC_1(0xe8888, void, perform_save_operation, int, operation);
SHK_FUNCTION_DEFINE_STATIC_2(0x4fe52c, void*, kalloc, void*, ptr, size_t, len);
SHK_FUNCTION_DEFINE_STATIC_2(0xe8ca0, void*, load_savefile, int, operation, void*, unk);
SHK_FUNCTION_DEFINE_STATIC_0(0x4e7404, void*, get_save_data);
SHK_FUNCTION_DEFINE_STATIC_3(0x650764, int, sys_mmapper_allocate_memory, size_t, size, u64, flags, void*, addr);
SHK_FUNCTION_DEFINE_STATIC_3(0x078178, int, display_prompt, int, p1, int, p2, int, p3);
SHK_FUNCTION_DEFINE_STATIC_3(0x4e8470, void, new_game, int, p1, void*, p2, u64, p3);
SHK_FUNCTION_DEFINE_STATIC_2(0x0e0854, void*, load_gadget_weapon_model, void*, ptr, int, unk2);
SHK_FUNCTION_DEFINE_STATIC_2(0xb72b0, u64, transition_to_movement_state, u32, state, u32, unk2);
SHK_FUNCTION_DEFINE_STATIC_0(0x164c58, void, load_destination_planet);
SHK_FUNCTION_DEFINE_STATIC_1(0xccda0, void, toast_message, char*, message);
SHK_FUNCTION_DEFINE_STATIC_1(0x112c20, void, unlock_planet, int, planet);
SHK_FUNCTION_DEFINE_STATIC_4(0xdbc38, void, load_moby_model, u32**, addr, int, unk0, int, unk1, short, oClass);
SHK_FUNCTION_DEFINE_STATIC_5(0x6a718, void, draw_rounded_box, int, y1, int, y2, int, x1, int, x2, u32, alpha);
SHK_FUNCTION_DEFINE_STATIC_4(0x7100c, void, _draw_text_opt, TextOpt*, text_opt, u32, color, char*, text, ssize_t, len);
SHK_FUNCTION_DEFINE_STATIC_2(0xef810, void, init_moby, Moby*, moby, u16, o_class);
SHK_FUNCTION_DEFINE_STATIC_2(0x65490, void, world_pos_to_screen_pos, Vec4*, out_screen_pos, Vec4*, world_pos);
SHK_FUNCTION_DEFINE_STATIC_5(0x57820, int, coll_line, Vec4*, position1, Vec4*, position2, int, flags, Moby*, moby, Vec4*, unk_vec);

void draw_text_opt(TextOpt* text_opt, Color color, char* text, ssize_t len, float text_size);

SHK_FUNCTION_DEFINE_STATIC_2(0x151e70, void, set_spawn_point, Vec4*, position, Vec4*, rotation);

//void unlock_item(ITEM item,bool equipped)
enum CONTROLLER_INPUT {
    L2 = 1,
    R2 = 2,
    L1 = 4,
    R1 = 8,
    Triangle = 16,
    Circle = 32,
    Cross = 64,
    Square = 128,
    Select = 256,
    L3 = 512,
    R3 = 1024,
    Start = 2048,
    Up = 4096,
    Right = 8192,
    Down = 16384,
    Left = 32768
};

struct CollOutput {
    void* grid;
    int pad1;
    int pad2;
    int pad3;
    int count;
    int damage_next;
    Moby* p_moby;
    int poly;
    Vec4 ip;
    Vec4 push;
    Vec4 normal;
    Vec4 v0;
    Vec4 v1;
    Vec4 v2;
};

struct MenuItem {
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
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    Moby* field20_0x14;
    undefined field21_0x18;
    undefined field22_0x19;
    undefined field23_0x1a;
    undefined field24_0x1b;
    undefined field25_0x1c;
    undefined field26_0x1d;
    undefined field27_0x1e;
    undefined field28_0x1f;
    undefined field29_0x20;
    undefined field30_0x21;
    undefined field31_0x22;
    undefined field32_0x23;
    undefined field33_0x24;
    undefined field34_0x25;
    undefined field35_0x26;
    undefined field36_0x27;
    undefined field37_0x28;
    undefined field38_0x29;
    undefined field39_0x2a;
    undefined field40_0x2b;
    undefined field41_0x2c;
    undefined field42_0x2d;
    undefined field43_0x2e;
    undefined field44_0x2f;
    unsigned int field45_0x30;
    unsigned int field46_0x34;
    undefined4 field47_0x38;
    undefined4 field48_0x3c;
    int field49_0x40;
};

struct Damage {
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
    Vec4 position;
    Moby *damaged_moby;
    undefined field18_0x24;
    undefined field19_0x25;
    undefined field20_0x26;
    undefined field21_0x27;
    u8 field22_0x28;
    u8 field23_0x29;
    u16 source_o_class;
    float damage_dealt;
};

struct MetalDetectorSpotVars {
    u8 unknown[0x18];
    int bolts;
};

//
// Various global variables
//

extern bool use_custom_player_color;
extern uint32_t custom_player_color;

//
// Game-global variables
//

// If the game should start loading the planet sepcified in `destination_planet`
extern bool should_load_destination_planet;

// The currently loaded planet.
extern int current_planet;

// Destination planet to load
extern int destination_planet;

extern u8 seen_planets[];

// Game state
extern GameState game_state;

// Frames since death/reload
extern int frame_count;

extern bool should_render_server_list;

extern Moby* moby_ptr;
extern Moby* moby_ptr_end;

extern MobyClass** mclass_table;

extern Moby* ratchets_ship;

// player

// The player's current bolt count.
extern int player_bolts;
// The player's current position.
extern Vec4 player_pos;
// The player's current rotation in radians. Z is the most commonly used axis.
extern Vec4 player_rot;
// The player's neutral momentum.
extern float player_neutral;
// The frames until "Ghost Ratchet" will run out.
extern int player_ghost_frames;
// The player's current state.
extern int player_state;
// The player's current input(?) state
extern int player_state_input;
// The player's current HP.
extern int player_health;

extern Vec4 camera_pos;
extern Vec4 camera_rot;

extern int player_animation_duration;

extern enum CONTROLLER_INPUT pressed_buttons;
extern enum CONTROLLER_INPUT held_buttons;

extern int in_cutscene;

extern int screen_projection_map_x;
extern int screen_projection_map_y;

extern float animation_speed;

extern int user_option_camera_left_right_movement;
extern int user_option_camera_up_down_movement;
extern int user_option_camera_rotation_speed;
extern bool option_helpdesk_voice;
extern bool option_helpdesk_text;

extern u8 latin_glyph_sizes;

extern PlayerType player_type;

void start_in_level_movie(u32 movie);
void unlock_item(int item_id, uint8_t equip);
void unlock_level(int level);
void unlock_skillpoint(u8 skillpoint);
Moby* spawn_moby(u16 o_class);
struct Damage* moby_get_damage(Moby* moby, u32 flags, u32 unk);

#ifdef __cplusplus
// Pointer to Ratchet moby.
extern Moby *ratchet_moby;
extern CollOutput* coll_output;
extern int n_coll_mobys;
extern Moby *coll_moby_out;
#endif

void rc1_init();
void rc1_shutdown();

#ifdef __cplusplus
}
#endif

#endif // RC1_H
#endif // GAME_RC1