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

int ticker_tacker = 0;
//SHK_HOOK(void, coll_mobys_sphere);
void coll_mobys_sphere_hook(float param_1, vec4* param_2, vec4* param_3, u32 flags, Moby* param_5) {
    ticker_tacker++;

    if ((ticker_tacker % 10000) == 0) {
        //printf("coll_mobys_sphere(%f, *0x%08x, %ld, 0x%08x, *0x%08x, %ld);", param_1, param_2, param_3, flags, param_5, param_6);
    }

    //SHK_CALL_HOOK(coll_mobys_sphere, param_1, param_2, param_3, flags, param_5);
}

void rc1_init() {
	MULTI_LOG("Multiplayer initializing.\n");
	
	SHK_BIND_HOOK(ratchet_dying, ratchet_dying_hook);
	SHK_BIND_HOOK(STUB_0006544c, STUB_0006544c_hook);
	SHK_BIND_HOOK(game_loop_start, game_loop_start_hook);
    //SHK_BIND_HOOK(coll_mobys_sphere, coll_mobys_sphere_hook);
	
	// Ininitalize and start multiplayer
	mp_start();
}

void rc1_shutdown() {
	
}

#endif