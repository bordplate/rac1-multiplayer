#include "moby.h"

#include "rc1.h"
#include "multiplayer/packet.h"

void moby_update(Moby* self) {
    MPMobyVars* vars = (MPMobyVars*)(self->pVars);

    if (self->animationID != vars->next_animation_id && vars->next_animation_id < 127) {
        // FIXME: This commented MULTI_TRACE call makes the game crash later in the rendering sequence for unknown reasons.
        //MULTI_TRACE("(uid: %d) Animating to ID: %hhd, from ID: %hhu\n", vars->uuid, vars->next_animation_id, self->animationID);
        set_moby_animation(self, vars->next_animation_id, 0, 10.5);
        MULTI_TRACE("Done animating\n");
    }

    // I'm not sure about these arguments, this kinda works, so I stuck with it.
    int success = coll_sphere(&self->position, 3, 1, self);

    if (success > 0 && coll_moby_out != 0) {
        if (coll_moby_out->oClass == 0) {
            mp_send_collision(vars->uuid, 0);
            MULTI_TRACE("Collision with ratchet! 0x%08x : 0x%08x : 0x%08x\n", coll_moby_out, ratchet_moby, self);
        }
    }
}

int item_to_oclass(ITEM item) {
    switch(item) {
        case Wrench:
            return 0x47;
        case Devastator:
            return 0x9d;
        default: return 0x47;
    }

    return 0x47;
}
