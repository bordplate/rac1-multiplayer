//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_VIEWELEMENT_H
#define PS3_CKIT_VIEWELEMENT_H

#include <rc1/rc1.h>
#include <rc1/common.h>

#include "View.h"

struct ViewElement {
public:
    ViewElement(int x, int y, int width, int height) {
        this->x = x;
        this->y = y;
        this->size.width = width;
        this->size.height = height;
        this->margins.width = 0;
        this->margins.height = 0;
        this->draws_background = false;
        this->visible = true;
        this->states = 0xFF;
    }

    int x;
    int y;

    View* view;

    Size size;
    Size margins;

    bool draws_background;
    bool visible;

    u8 states;

    void focus() {
        if (view != nullptr) {
            view->focused_element = this;
        }
    }

    bool is_focused() {
        if (view != nullptr) {
            return view->focused_element == this;
        }
        return false;
    }

    virtual void render();
    virtual void on_pressed_buttons(CONTROLLER_INPUT input) {}

    bool active_for_state(GameState game_state) {
            switch (game_state) {
                case PlayerControl:
                    return (states >> 0) & 1;
                case Movie:
                    return (states >> 1) & 1;
                case CutScene:
                    return (states >> 2) & 1;
                case Menu:
                    return (states >> 3) & 1;
                case ExitRace:
                    return (states >> 4) & 1;
                case Gadgetron:
                    return (states >> 5) & 1;
                case PlanetLoading:
                    return (states >> 6) & 1;
                case CinematicMaybe:
                    return (states >> 7) & 1;
                default:
                    Logger::debug("ViewElement::active_for_game_state: UnkFF gamestate not handled. not rendering");
                    return false;
            }
    }
};

// Composable View states
// create a set of allowed states for a ViewElement using: ViewPlayerControl & ViewMenu & ViewGadgetron (which creates 0b00100101)
// or simply pass a single one to only allow one state

enum ViewGameStates {
    ViewPlayerControl = 0x01,    // 0b00000001
    ViewMovie = 0x02,            // 0b00000010
    ViewCutScene = 0x04,         // 0b00000100
    ViewMenu = 0x08,             // 0b00001000
    ViewExitRace = 0x10,         // 0b00010000
    ViewGadgetron = 0x20,        // 0b00100000
    ViewPlanetLoading = 0x40,    // 0b01000000
    ViewCinematicMaybe = 0x80,   // 0b10000000

    ViewNoView = 0x00            // 0b00000000
};

#endif //PS3_CKIT_VIEWELEMENT_H
