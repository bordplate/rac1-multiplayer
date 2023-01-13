//
// Created by Vetle Hjelle on 19/12/2022.
//

#ifndef PS3_CKIT_BRIDGING_H
#define PS3_CKIT_BRIDGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Moby.h"

void cpp_start();

void _c_game_tick();
void _c_game_render();

void _c_moby_update(Moby* moby);

#ifdef __cplusplus
}
#endif

#endif //PS3_CKIT_BRIDGING_H
