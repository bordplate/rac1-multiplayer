#ifdef GAME_RC1

#ifndef RC1_H
#define RC1_H

#define RGBA(r, g, b, a) \
    (((unsigned int)(a) << 24) | \
     ((unsigned int)(b) << 16) | \
     ((unsigned int)(g) << 8)  | \
     ((unsigned int)(r)))

#include <lib/shk.h>
#include <lib/types.h>
#include "Moby.h"

#ifdef __cplusplus
#include "Moby.h"

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

// Our global variables
extern int game_ticks;

extern int current_weapon;

extern int planetUnlockedByServer;

SHK_FUNCTION_DEFINE_STATIC_2(0x4fe52c, void*, kalloc, void*, ptr, size_t, len);
SHK_FUNCTION_DEFINE_STATIC_3(0x650764, int, sys_mmapper_allocate_memory, size_t, size, u64, flags, void*, addr);
SHK_FUNCTION_DEFINE_STATIC_3(0x4e8470, void, new_game, int, p1, void*, p2, u64, p3);
SHK_FUNCTION_DEFINE_STATIC_2(0x0e0854, void*, load_gadget_weapon_model, void*, ptr, int, unk2);
SHK_FUNCTION_DEFINE_STATIC_2(0xb72b0, u64, transition_to_movement_state, u32, state, u32, unk2);
SHK_FUNCTION_DEFINE_STATIC_0(0x164c58, void, load_destination_planet);
SHK_FUNCTION_DEFINE_STATIC_1(0xccda0, void, toast_message, char*, message);
SHK_FUNCTION_DEFINE_STATIC_1(0x112c20, void, unlock_planet, int, planet);
SHK_FUNCTION_DEFINE_STATIC_4(0xdbc38, void, load_moby_model, void**, addr, int, unk0, int, unk1, short, oClass);

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
    Moby* pMoby;
    int poly;
    Vec4 ip;
    Vec4 push;
    Vec4 normal;
    Vec4 v0;
    Vec4 v1;
    Vec4 v2;
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

extern Vec3 camera_pos;
extern Vec3 camera_rot;

extern int player_animation_duration;

extern enum CONTROLLER_INPUT pressed_buttons;
extern enum CONTROLLER_INPUT held_buttons;

extern int in_cutscene;

extern float animation_speed;

extern int user_option_camera_left_right_movement;
extern int user_option_camera_up_down_movement;
extern int user_option_camera_rotation_speed;

extern PlayerType player_type;

void unlock_item(int item_id, uint8_t equip);
void unlock_level(int level);

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