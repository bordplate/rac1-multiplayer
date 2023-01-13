//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_TEXTELEMENT_H
#define PS3_CKIT_TEXTELEMENT_H

#include <rc1/common.h>
#include <rc1/rc1.h>

#include "View.h"
#include "ViewElement.h"

#include <lib/string.h>

typedef enum {
    HUDTextDropShadow = 1,
    HUDElementDelete = 2
} HUDElementTextFlags;

struct TextElement : public ViewElement {
    TextElement(int x, int y, const char* text);
    ~TextElement();

    Color color;
    String* text;
    bool has_shadow;

    void render();
};


#endif //PS3_CKIT_TEXTELEMENT_H
