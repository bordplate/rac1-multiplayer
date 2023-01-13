#include "hud.h"
#include "multiplayer/server_list.h"

void hud_render() {
    if (!game_started) {
        return;
    }

    if (in_cutscene != 0) {
        return;
    }

    if (game_state == 6 || game_state == 7) return;

    if (should_render_server_list) {
        render_server_list();
    }

    for (int i = 0; i < 20; i++) {
        HUDElementText* element = &text_elements[i];
        if (element->element.active) {
            if (element->flags & HUDTextDropShadow) {
                draw_center_medium_text(element->element.x+1, element->element.y+1, 0xD0101010, &element->text, -1);
            }

            draw_center_medium_text(element->element.x, element->element.y, element->color, &element->text, -1);
        }
    }
}

void hud_clear() {
    memset(&text_elements, 0, sizeof(text_elements));
}

void hud_init() {
    hud_clear();
}

void hud_delete_text_element(u16 id) {
    if (id < 20) {
        memset(&text_elements[id], 0, sizeof(HUDElementText));
    }
}

HUDElementText* hud_new_text_element(u16 id) {
    if (id < 20) {
        HUDElementText* element = &text_elements[id];

        memset(element, 0, sizeof(HUDElementText));

        element->element.active = 1;

        return element;
    } else {
        return 0;
    }
}

HUDElementText* hud_get_text_element(u16 id) {
    if (id > 20) {
        return 0;
    }

    HUDElementText* element = &text_elements[id];

    if (!element->element.active) {
        return 0;
    }

    return element;
}