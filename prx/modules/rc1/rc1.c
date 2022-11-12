#ifdef GAME_RC1

#include "rc1.h"

#include "lib/shk.h"
#include "lib/config.h"
#include "multiplayer/moby.h"

#include "multiplayer/multiplayer.h"

//SHK_HOOK(Moby *, moby_spawn, s32 o_class);
//Moby* moby_spawn_hook(s32 o_class) {
	//Moby* moby = SHK_CALL_HOOK(moby_spawn, o_class);
	
	//moby->scale = moby->scale * 2;
	
	//return moby;
//}

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
void STUB_0006544c_hook(Moby *moby) {
	moby_update(moby);
	
	//SHK_CALL_HOOK(STUB_0006544c);
}

void rc1_init() {
	MULTI_LOG("Congrats\n");
	
	mp_start();
	
	//SHK_BIND_HOOK(moby_spawn, moby_spawn_hook);
	SHK_BIND_HOOK(ratchet_dying, ratchet_dying_hook);
	SHK_BIND_HOOK(STUB_0006544c, STUB_0006544c_hook);
	SHK_BIND_HOOK(game_loop_start, game_loop_start_hook);
}

void rc1_shutdown() {
	
}

#endif