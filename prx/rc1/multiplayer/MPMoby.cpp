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

MPMoby* MPMoby::spawn(unsigned short uuid, unsigned short o_class, unsigned short flags, uint16_t modeBits) {
    MPMoby* moby = (MPMoby*)Moby::spawn(o_class, flags, modeBits);

    // Set update function to MPMoby::update()
    moby->pUpdate = (void *)0x710ed8;

    // Set Moby variables
    MPMobyVars* vars = (MPMobyVars*)(moby->pVars);

    if (vars) {
        vars->uuid = uuid;
        vars->o_class = o_class;

        // Add a signature to the pVars so that we can identify this as a MPMoby later.
        // Bad way to do it, but yolo
        vars->sig = 0x4542;
    }

    return moby;
}

extern "C" void _c_moby_update(Moby* moby) {
    ((MPMoby*)moby)->update();
}