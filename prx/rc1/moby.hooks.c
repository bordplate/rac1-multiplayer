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
