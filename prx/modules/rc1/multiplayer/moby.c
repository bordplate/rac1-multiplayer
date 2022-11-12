#include "moby.h"

void set_moby_animation(Moby *moby, u8 param_2, u8 param_3, float param_4) {
	SHK_FUNCTION_CALL_4(0xfddc0, void, Moby *, moby, u8, param_2, u8, param_3, float, param_4);
}

Moby *spawn_moby(int o_class) {
	SHK_FUNCTION_CALL_1( 0x0efa28, Moby*, int, o_class);
}

int idk( void* p1)
{
    SHK_FUNCTION_CALL_1( 0xf31a8, int, void*,  p1);
}

int idk2(void* p1, s64 p2, u64 p3, s64 p4, s64 p5) {
	SHK_FUNCTION_CALL_5(0xf1ea0, int, void*, p1, s64, p2, u64, p3, s64, p4, s64, p5);
}

void moby_update(Moby *self) {
	MPMobyVars *vars = (MPMobyVars*)(self->pVars);
	
	if (self->animationID != vars->next_animation_id && vars->next_animation_id < 127) {
		//MULTI_TRACE("Animating to ID: %d, from ID: %d\n", vars->next_animation_id, self->animationID);
		set_moby_animation(self, vars->next_animation_id, 0, 10.5);
		MULTI_TRACE("Done animating\n");
	} else {
		//MULTI_TRACE("Updating moby (uid:%d) %d != %d\n", vars->uuid, self->animationID, vars->next_animation_id);
	}
}