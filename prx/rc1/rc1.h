#ifdef GAME_RC1

#ifndef RC1_H
#define RC1_H

#include <sys/tty.h>
#include <lib/common.h>

#define MULTI_TRACE_LOG 0
#define MULTI_LOG( msg, ... ) printf( "rac1multi: " msg, ##__VA_ARGS__ )
#if MULTI_TRACE_LOG
#define MULTI_TRACE( msg, ... ) printf( "*rac1multi: " msg, ##__VA_ARGS__ )
#else
#define MULTI_TRACE( msg, ... ) do {} while ( false )
#endif


void rc1_init();
void rc1_shutdown();

// Common rc1 structs and types

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct {
    char a;
    char b;
    char g;
    char r;
} color;

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


// Global variables
extern int game_ticks;


// The currently loaded planet.
#define current_planet (*((GameState*)0x969C70))

// Game state
#define game_state (*((int*)0xa10708))
#define frame_count (*((int*)0xa10710))

// player

// The player's current bolt count.
#define player_bolts (*((int*)0x969CA0))
// The player's current position.
#define player_pos (*((vec4*)0x969D60))
// The player's current rotation in radians. Z is the most common axis.
#define player_rot (*((vec4*)0x969D70))
// The player's neutral momentum.
#define player_neutral (*((float*)0x969E74))
// The frames until "Ghost Ratchet" will run out.
#define player_ghost_frames (*((int*)0x969EAC))
// The player's current state.
#define player_state (*((int*)0x96BD64))
// The player's current HP.
#define player_health (*((int*)0x96BF88))
// Pointer to Ratchet moby
#define ratchet_moby (*((Moby**)0x96bd60))

#define animation_speed (*((float*)0x0071f410))


#endif // RC1_H
#endif // GAME_RC1