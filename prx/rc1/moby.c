#include "moby.h"

void moby_update(Moby* self) {
	MPMobyVars* vars = (MPMobyVars*)(self->pVars);
	
	if (self->animationID != vars->next_animation_id && vars->next_animation_id < 127) {
		// FIXME: This commented MULTI_TRACE call makes the game crash later in the rendering sequence for unknown reasons. 
		//MULTI_TRACE("(uid: %d) Animating to ID: %hhd, from ID: %hhu\n", vars->uuid, vars->next_animation_id, self->animationID);
		set_moby_animation(self, vars->next_animation_id, 0, 10.5);
		MULTI_TRACE("Done animating\n");
	}
}
