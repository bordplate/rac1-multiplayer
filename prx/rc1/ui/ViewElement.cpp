//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "ViewElement.h"

void ViewElement::render() {
    if (!visible) {
        return;
    }

    if (draws_background) {
        draw_rounded_box(y, y+size.height, x, x+size.width, 100);
    }
}