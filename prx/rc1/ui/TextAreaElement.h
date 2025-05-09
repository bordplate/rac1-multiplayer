//
// Created by bordplate on 12/5/2024.
//

#ifndef RAC1_MULTIPLAYER_TEXTAREAELEMENT_H
#define RAC1_MULTIPLAYER_TEXTAREAELEMENT_H

#include "ViewElement.h"

struct TextAreaElement: public ViewElement {
public:
    TextAreaElement(int x, int y, int width, int height) : ViewElement(x, y, width, height) {
        text_size = 1.0f;
        color = RGBA(0x88, 0xa8, 0xff, 0xc0);

        text_options.line_spacing = 15;
    }

    Color color;

    String text;
    TextOpt text_options;
    float text_size;

    void render();
};


#endif //RAC1_MULTIPLAYER_TEXTAREAELEMENT_H
