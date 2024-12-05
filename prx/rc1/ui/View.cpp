//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "View.h"
#include "ViewElement.h"

View::View() {

}

void View::render() {
    for (int i = 0; i < this->elements_.size(); i++) {
        ViewElement* element = this->elements_[i];
        if (element && element->active_for_state(game_state)) {
            element->render();
        }
    }
}

void View::on_pressed_buttons(CONTROLLER_INPUT input) {
    if (focused_element != nullptr) {
        focused_element->on_pressed_buttons(input);
    }
}

void View::add_element(ViewElement* element) {
    element->view = this;
    this->elements_.push_back(element);
}

void View::remove_element(ViewElement* element) {
    for (int i = 0; i < this->elements_.size(); i++) {
        if (element == this->elements_[i]) {
            this->elements_[i] = nullptr;
            delete element;
        }
    }
}

// Free any elements on screen
View::~View() {
    for (int i = 0; i < this->elements_.size(); i++) {
        ViewElement* element = this->elements_[i];
        if (element) {
            delete element;
        }
    }
}
