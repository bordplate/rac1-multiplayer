//
// Created by bordplate on 12/5/2024.
//

#include "TextAreaElement.h"

void TextAreaElement::render() {
    if (!visible) {
        return;
    }

    ViewElement::render();

    text_options.x = x + margins.width;
    text_options.y = y + margins.height;

    text_options.x_min = x + margins.width;
    text_options.y_min = y + margins.height;
    text_options.x_max = x + size.width - margins.width * 2;
    text_options.y_max = y + size.height - margins.height * 2;

    draw_text_opt(&text_options, color, (char*)text.c_str(), -1, text_size);
}