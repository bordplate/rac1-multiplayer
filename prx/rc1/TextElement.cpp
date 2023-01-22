//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "TextElement.h"

TextElement::TextElement(int x, int y, const char* text) {
    this->x = x;
    this->y = y;

    this->text = new String(text);
    this->has_shadow = true;

    color.r = 0x88;
    color.g = 0xa8;
    color.b = 0xff;
    color.a = 0xc0;
}

TextElement::~TextElement() {
    if (text) {
        delete text;
    }
}

void TextElement::render() {
    // Render shadow first, if any.
    if (this->has_shadow) {
        Color shadow_color = {0xD0, 0x10, 0x10, 0x10};
        draw_center_medium_text(x+1, y+1, shadow_color, this->text->c_str(), -1);
    }

    draw_center_medium_text(x, y, color, this->text->c_str(), -1);
}