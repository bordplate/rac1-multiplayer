//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_VIEW_H
#define PS3_CKIT_VIEW_H

#include <lib/shk.h>
#include <lib/types.h>
#include <lib/vector.h>
#include <rc1/rc1.h>

#include "ViewElement.h"

extern "C" {
SHK_FUNCTION_DEFINE_STATIC_5(0x70514, void, draw_center_medium_text, int, x, int, y, Color, color, const char*, p4, int, p5);
}

struct View {
public:
    View();
    ~View();

    virtual void on_load() {}
    virtual void on_unload() {}

    virtual void render();
    virtual void on_pressed_buttons(CONTROLLER_INPUT input);

    void add_element(ViewElement* element);
    void remove_element(ViewElement *element);
private:
    Vector<ViewElement*> elements_;
};


#endif //PS3_CKIT_VIEW_H
