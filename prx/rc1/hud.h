#ifndef HUD_H
#define HUD_H

#include <lib/common.h>
#include <rc1/common.h>
#include <rc1/rc1.h>

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

typedef struct {
    u16 id;
    u16 x;
    u16 y;
    u8 active;
} HUDElement;

typedef enum {
    HUDTextDropShadow = 1,
    HUDElementDelete = 2
} HUDElementTextFlags;

typedef struct {
    HUDElement element;
    u32 color;
    u16 box_height;
    u16 box_width;
    HUDElementTextFlags flags;
    float size;
    char text[50];
} HUDElementText;

HUDElementText text_elements[20];

SHK_FUNCTION_DEFINE_STATIC_5(0x7100c, void, draw_text, TextOpt*, opt, u32, Color, char*, text, u32, char_max, double, size);
SHK_FUNCTION_DEFINE_STATIC_5(0x70514, void, draw_center_medium_text, int, p1, int, p2, unsigned int, p3, char*, p4, int, p5);

void hud_init();
void hud_render();
void hud_clear();

void hud_delete_text_element(u16 id);
HUDElementText* hud_new_text_element(u16 id);
HUDElementText* hud_get_text_element(u16 id);



#endif
