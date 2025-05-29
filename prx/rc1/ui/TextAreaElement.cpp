//
// Created by bordplate on 12/5/2024.
//

#include "TextAreaElement.h"

void TextAreaElement::render() {
    if (!visible) {
        return;
    }

    if (uses_world_space && distance_from_camera > world_space_max_distance) {
        return;
    }

    float tsize = text_size;

    ViewElement::render();

    float text_length = 0;
    if (text.length() > 0) {
        u8* glyph_sizes = (u8*)&latin_glyph_sizes;

        for (int j = 0; j < text.length(); j++) {
            text_length += (float)*(u8*)(glyph_sizes + text[j] * 4 + 3);
        }
    }

    if (!uses_world_space) {
        text_options.x = x + margins.width;
        text_options.y = y + margins.height;

        text_options.x_min = x + margins.width;
        text_options.y_min = y + margins.height;
        text_options.x_max = x + size.width - margins.width * 2;
        text_options.y_max = y + size.height - margins.height * 2;
    } else {
        Vec4 screen;

        world_pos_to_screen_pos(&screen, &world_position);

        if (screen.z > 0) {
            return; // Don't render if the thing is behind the camera
        }

        x = (s16)((screen.x - (float)screen_projection_map_x) * 0.0625);
        y = (s16)((screen.y - (float)screen_projection_map_y) * 0.0625);
        text_options.x_min = 0;
        text_options.x_max = 800;
        text_options.y_min = 0;
        text_options.y_max = 800;

        text_options.x = x;
        text_options.y = y;

        if (world_space_scales_with_distance) {
            if (distance_from_camera > world_space_max_distance) {
                return;
            }

            if (
                world_space_hide_when_obstructed &&
                coll_line(&camera_pos, &world_position, 2, nullptr, nullptr)
            ) {
                return;
            }

            tsize = (text_size * 10.0f) / distance_from_camera;
        }
    }

    text_length = text_length * tsize;

    if (alignment == MPUIElementAlignmentRight) {
        text_options.x = x + size.width - (margins.width*2) - text_length;
        text_options.y = y;
    }

    if (alignment == MPUIElementAlignmentCentered) {
        text_options.x = x - text_length/2;
        text_options.y = y;
    }

    if (has_shadow) {
        text_options.x += 1;
        text_options.y += 1;

        draw_text_opt(&text_options, RGBA(0x10, 0x10, 0x10, 0xd0), (char*)text.c_str(), -1, tsize);

        text_options.x -= 1;
        text_options.y -= 1;
    }

    draw_text_opt(&text_options, color, (char*)text.c_str(), -1, tsize);
}