//
// Created by bordplate on 12/7/2024.
//

#ifndef RAC1_MULTIPLAYER_INPUTELEMENT_H
#define RAC1_MULTIPLAYER_INPUTELEMENT_H


#include "ViewElement.h"
#include "Input.h"

struct InputElement;

struct InputDelegate {
    virtual void on_input_callback(InputElement* input_element) = 0;
};

struct InputElement : public ViewElement {
public:
    InputElement() : ViewElement(0, 0, 0, 0) {
        input_.input_text = "";
        input_.set_callback(input_callback, this);
        delegate_ = nullptr;
    }

    String text() const { return input_.input_text; }
    void activate() { input_.activate(); }
    void set_prompt(const char* prompt) { input_.set_prompt(prompt); }

    void set_delegate(InputDelegate* delegate) { delegate_ = delegate; }

    static int input_callback(Input *input, void *userdata, int status) {
        InputElement* self = (InputElement*)userdata;
        if (self->delegate_) {
            self->delegate_->on_input_callback(self);
        }
        return 0;
    }

    void render() { ViewElement::render(); input_.check_callback(); }
private:
    Input input_;
    InputDelegate* delegate_;
};


#endif //RAC1_MULTIPLAYER_INPUTELEMENT_H
