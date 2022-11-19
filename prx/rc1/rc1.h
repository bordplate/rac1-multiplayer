#ifdef GAME_RC1

#ifndef RC1_H
#define RC1_H

#include <lib/common.h>

#define MULTI_TRACE_LOG 0
#define MULTI_LOG( msg, ... ) printf( "rac1multi: " msg, ##__VA_ARGS__ )
//#define MULTI_LOG( msg, ... ) do {} while ( false )
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



// Global variables
extern int game_ticks;

extern ITEM current_weapon;

SHK_FUNCTION_DEFINE_STATIC_2( 0x0efa28, void*, kalloc, void*, ptr, size_t, len);
SHK_FUNCTION_DEFINE_STATIC_2( 0x0e0854, void*, load_gadget_weapon_model, void*, ptr, int, unk2);

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