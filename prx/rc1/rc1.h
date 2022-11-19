#ifdef GAME_RC1

#ifndef RC1_H
#define RC1_H

#include <lib/common.h>
#include <rc1/common.h>
#include <rc1/moby.h>

void rc1_init();
void rc1_shutdown();

typedef enum GameState {
    PlayerControl=0,
    Movie=1,
    CutScene=2,
    Menu=3,
    ExitRace=4,
    Gadgetron=5,
    PlanetLoading=6,
    CinematicMaybe=7,
    UnkFF=255
} GameState;

// Our global variables
extern int game_ticks;

extern ITEM current_weapon;

SHK_FUNCTION_DEFINE_STATIC_2( 0x0efa28, void*, kalloc, void*, ptr, size_t, len);
SHK_FUNCTION_DEFINE_STATIC_2( 0x0e0854, void*, load_gadget_weapon_model, void*, ptr, int, unk2);
//
// Game-global variables
//

// The currently loaded planet.
extern int current_planet;

// Game state
extern GameState game_state;

// Frames since death/reload
extern int frame_count;

// player

// The player's current bolt count.
extern int player_bolts;
// The player's current position.
extern vec4 player_pos;
// The player's current rotation in radians. Z is the most commonly used axis.
extern vec4 player_rot;
// The player's neutral momentum.
extern float player_neutral;
// The frames until "Ghost Ratchet" will run out.
extern int player_ghost_frames;
// The player's current state.
extern int player_state;
// The player's current HP.
extern int player_health;
// Pointer to Ratchet moby.
extern Moby* ratchet_moby;

extern float animation_speed;

#endif // RC1_H
#endif // GAME_RC1