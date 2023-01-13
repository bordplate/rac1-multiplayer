//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "MPMoby.h"

void MPMoby::update() {
    // Don't check collision for Ratchet moby, because that apparently crashes the game on console only.
    if (this->oClass != 0) {
        this->check_collision();
    }

    MPMobyVars* vars = (MPMobyVars*)(this->pVars);

    if (this->animationID != vars->next_animation_id) {
        this->set_animation(vars->next_animation_id, 0, vars->animation_duration);
    }
}

extern "C" void _c_moby_update(Moby* moby) {
    ((MPMoby*)moby)->update();
}