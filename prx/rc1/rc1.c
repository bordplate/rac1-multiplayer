#ifdef GAME_RC1

#include <rc1/moby.h>
#include "multiplayer/multiplayer.h"

int game_ticks = 0;

SHK_HOOK(void, game_loop_start);
void game_loop_start_hook() {
    if (current_planet > 0) {
        game_ticks += 1;

        mp_tick();

        MULTI_TRACE("Tick done\n");
    }

    return SHK_CALL_HOOK(game_loop_start);
}

SHK_HOOK(void, ratchet_dying);
void ratchet_dying_hook() {
	SHK_CALL_HOOK(ratchet_dying);
	
	MULTI_LOG("Dying...\n");
}

SHK_HOOK(void, STUB_0006544c);
void STUB_0006544c_hook(Moby* moby) {
	moby_update(moby);
}

// Hook to avoid some consoles getting a "game is corrupted, restart the game" on game start
// I think maybe it makes trophies not work?
SHK_HOOK(void, authenticate_game);
void authenticate_game_hook() {
    MULTI_LOG("Game totally authenticated\n");
}

SHK_HOOK(void, wrench_update_func);
void wrench_update_func_hook(Moby* moby) {
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
    if (vars->uuid && vars->uuid < MP_MAX_MOBYS) {
        // Verify that ptr to MP moby with this UUID matches ptr to moby we hit
        if (mp_get_moby(vars->uuid) == hit) {
            mp_send_collision(0, vars->uuid, &moby->position, true);
            MULTI_LOG("%d oClass %d at %08x got maybe hit by a wrench. Player state %d\n", vars->uuid, hit->oClass, hit, player_state);
        }
    }
}

void rc1_init() {
	MULTI_LOG("Multiplayer initializing.\n");
	
	SHK_BIND_HOOK(ratchet_dying, ratchet_dying_hook);
	SHK_BIND_HOOK(STUB_0006544c, STUB_0006544c_hook);  // Used as a "trampoline" to our custom Moby update func
	SHK_BIND_HOOK(game_loop_start, game_loop_start_hook);
    SHK_BIND_HOOK(wrench_update_func, wrench_update_func_hook);
	SHK_BIND_HOOK(authenticate_game, authenticate_game_hook);
	// Ininitalize and start multiplayer
	mp_start();
}

void rc1_shutdown() {
	
}

#endif