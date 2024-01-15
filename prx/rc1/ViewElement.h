//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_VIEWELEMENT_H
#define PS3_CKIT_VIEWELEMENT_H
#include "rc1.h"

struct ViewElement {
    int x;
    int y;

    GameState state;

    virtual void render() = 0;
};


#endif //PS3_CKIT_VIEWELEMENT_H
