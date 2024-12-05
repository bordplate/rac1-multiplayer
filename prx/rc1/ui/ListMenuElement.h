//
// Created by bordplate on 12/5/2024.
//

#ifndef RAC1_MULTIPLAYER_LISTMENUELEMENT_H
#define RAC1_MULTIPLAYER_LISTMENUELEMENT_H

#include "ViewElement.h"
#include "ListMenuItem.h"

struct ListMenuElement;

struct ListMenuDelegate {
    virtual void on_item_activated(int index, ListMenuElement* list_menu_element) = 0;
    virtual void on_item_selected(int index, ListMenuElement *list_menu_element) {};
};

struct ListMenuElement : public ViewElement {
public:
    ListMenuElement(int x, int y, int width, int height);

    TextOpt title_options;
    float title_size;

    TextOpt details_options;
    float details_size;

    Color default_color;
    Color selected_color;

    int selected_item;

    int element_spacing;

    void add_item(ListMenuItem& item) { items_.push_back(item); }
    void render();
    void set_delegate(ListMenuDelegate* delegate) { delegate_ = delegate; }

    void on_pressed_buttons(CONTROLLER_INPUT input);

    void clear_items() { }
private:
    Vector<ListMenuItem> items_;
    ListMenuDelegate* delegate_;
};


#endif //RAC1_MULTIPLAYER_LISTMENUELEMENT_H
