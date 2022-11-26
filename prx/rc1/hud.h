#ifndef HUD_H
#define HUD_H

#include <lib/common.h>
#include <rc1/common.h>

typedef struct {
    short y_min;
    short y_max;
    short x_min;
    short x_max;
    short x;
    short y;
    u16 unk_0c; /* probably an output */
    u16 unk_0e; /* probably an output */
    short line_spacing;
    u16 flags;
} TextOpt;

SHK_FUNCTION_DEFINE_STATIC_5(0x7100c, void, draw_text, TextOpt*, opt, u32, color, char*, text, u32, char_max, double, size);
SHK_FUNCTION_DEFINE_STATIC_5(0x70514, void, draw_center_medium_text, int, p1, int, p2, unsigned int, p3, char*, p4, int, p5);

#endif
