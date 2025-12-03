//
// Created by Vetle Hjelle on 19/12/2022.
//

#ifndef PS3_CKIT_BRIDGING_H
#define PS3_CKIT_BRIDGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rc1/game/Moby.h"

void cpp_start();

void _c_game_tick();
void _c_game_render();
void _c_game_reset();

void _c_on_respawn();

void _c_moby_update(Moby* moby);

void _c_set_ratchet_animation_duration(u32 duration);

void _c_bink_do_frame();
void _c_on_save_operation(int action, void* savedata);

#ifdef __cplusplus
}
#endif

#endif //PS3_CKIT_BRIDGING_H
