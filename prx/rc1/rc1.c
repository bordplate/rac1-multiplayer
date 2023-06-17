#include "rc1.h"
#include "common.h"

#include <lib/memory.h>
#include <netex/net.h>

#include <rc1/Game.h>
#include "multiplayer/packet.h"

#include "bridging.h"

extern "C" {

//char *start_multiplayer_text = "\x13 MULTIPLAYER";
//char *connecting_text = "Connecting to multiplayer...";

void game_tick() {
    _c_game_tick();
}

SHK_HOOK(void, game_loop_start);
void game_loop_start_hook() {
    if (current_planet != 0 && ratchet_moby != 0) {
        game_tick();
    }

    SHK_CALL_HOOK(game_loop_start);
}

SHK_HOOK(void, game_loop_intro_start);
void game_loop_intro_start_hook() {
    game_tick();

    SHK_CALL_HOOK(game_loop_intro_start);
}

SHK_HOOK(void, on_respawn);
void on_respawn_hook() {
    SHK_CALL_HOOK(on_respawn);
    _c_on_respawn();
}

//SHK_HOOK(void, ratchet_dying);
//void ratchet_dying_hook() {
//    SHK_CALL_HOOK(ratchet_dying);
//
//    //MULTI_LOG("Dying...\n");
//}

SHK_HOOK(void, STUB_0006544c);
void STUB_0006544c_hook(Moby *moby) {
    _c_moby_update(moby);
}

// Hook to avoid some consoles getting a "game is corrupted, restart the game" on game start
// I think maybe it makes trophies not work?
SHK_HOOK(void, authenticate_game);
void authenticate_game_hook() {
    MULTI_LOG("Game totally authenticated\n");
}

SHK_HOOK(void, FUN_000784e8);
void FUN_000784e8_hook() {
    _c_game_render();

    SHK_CALL_HOOK(FUN_000784e8);
}

SHK_HOOK(void, wrench_update_func, Moby *);
void wrench_update_func_hook(Moby *moby) {
    // Clear the collision out ptr before calling original wrench function
    coll_moby_out = 0;

    SHK_CALL_HOOK(wrench_update_func, moby);

    // If coll_moby_out has a value, the wrench has "attacked" something
    if (!coll_moby_out) {
        return;
    }

    // Figure out what moby we hit and if we need to tell the server about it
    Moby* hit = coll_moby_out;
    if (!hit->pVars) {
        // If we don't have pVars, this isn't something the server needs to know about
        return;
    }

    MPMobyVars* vars = (MPMobyVars*)hit->pVars;

    // If this moby has UUID vars
    if (vars->uuid && vars->sig == 0x4542) {
        if (Game::shared().client()) {
            Game::shared().client()->send(Packet::make_collision(0, vars->uuid, &hit->position, true));
        }
    }
}

void rc1_init() {
    MULTI_LOG("Multiplayer initializing.\n");

    init_memory_allocator(memory_area, sizeof(memory_area));

    //SHK_BIND_HOOK(ratchet_dying, ratchet_dying_hook);
    SHK_BIND_HOOK(STUB_0006544c, STUB_0006544c_hook);  // Used as a "trampoline" to our custom Moby update func
    SHK_BIND_HOOK(game_loop_start, game_loop_start_hook);
    SHK_BIND_HOOK(game_loop_intro_start, game_loop_intro_start_hook);
    SHK_BIND_HOOK(wrench_update_func, wrench_update_func_hook);
    SHK_BIND_HOOK(authenticate_game, authenticate_game_hook);
    SHK_BIND_HOOK(FUN_000784e8, FUN_000784e8_hook);
    SHK_BIND_HOOK(on_respawn, on_respawn_hook);
    //SHK_BIND_HOOK(FUN_000850f8, FUN_000850f8_hook);

    sys_net_initialize_network();
}

void rc1_shutdown() {

}

};