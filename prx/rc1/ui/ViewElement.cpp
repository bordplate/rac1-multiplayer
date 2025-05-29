//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "ViewElement.h"
#include "lib/math.h"

void ViewElement::render() {
    if (!visible) {
        return;
    }

    int draw_x = x;
    int draw_y = y;

    if (alignment == MPUIElementAlignmentCentered) {
        draw_x -= size.width / 2;
    } else if (alignment == MPUIElementAlignmentRight) {
        draw_x -= size.width;
    }

    if (draws_background) {
        draw_rounded_box(draw_y, draw_y+size.height, draw_x, draw_x+size.width, 100);
    }
}

void ViewElement::calculate_distance_from_camera() {
    distance_from_camera = distance(camera_pos, world_position);
}