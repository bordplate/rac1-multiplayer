//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "View.h"

View::View() {

}

void View::render() {
    for (int i = 0; i < this->elements_.size(); i++) {
        ViewElement* element = this->elements_[i];
        if (element) {
            element->render();
        }
    }
}

void View::on_pressed_buttons(CONTROLLER_INPUT input) {

}

void View::add_element(ViewElement* element) {
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
