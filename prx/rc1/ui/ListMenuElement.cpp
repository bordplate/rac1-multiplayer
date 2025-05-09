//
// Created by bordplate on 12/5/2024.
//

#include "ListMenuElement.h"
#include "../rc1.h"
#include "../../lib/types.h"
#include "../../lib/logger.h"

ListMenuElement::ListMenuElement(int x, int y, int width, int height) : ViewElement(x, y, width, height) {
    title_size = 1.0f;
    details_size = 0.7f;
    accessory_size = 1.0f;

    title_options.x_min = x;
    title_options.y_min = y;
    title_options.x_max = x + width;
    title_options.y_max = y + height;

    details_options.x_min = x;
    details_options.y_min = y;
    details_options.x_max = x + width;
    details_options.y_max = y + height;

    accessory_options.flags = 0;

    selected_item = 0;

    element_spacing = 20;

    default_color = RGBA(0x88, 0xa8, 0xff, 0xc0);
    selected_color = RGBA(0x88, 0xa8, 0x0, 0xc0);
}

void ListMenuElement::render() {
    if (!visible) {
        return;
    }

    title_options.x_min = x;
    title_options.y_min = y;
    title_options.x_max = x + size.width;
    title_options.y_max = y + size.height;

    details_options.x_min = x;
    details_options.y_min = y;
    details_options.x_max = x + size.width;
    details_options.y_max = y + size.height;

    accessory_options.x_min = x;
    accessory_options.y_min = y;
    accessory_options.x_max = x + size.width;
    accessory_options.y_max = y + size.height;

    ViewElement::render();

    int x = this->x + margins.width * 2;
    int y = this->y + margins.height * 2;

    for (int i = 0; i < items_.size(); i++) {
        ListMenuItem item = items_[i];

        title_options.x = x;
        title_options.y = y;

        if (item.accessory.length() > 0) {
            // Calculate size of the text so we can right-align it
            u8* glyph_sizes = (u8*)&latin_glyph_sizes;
            int text_size = 5;

            for (int j = 0; j < item.accessory.length(); j++) {
                text_size += *(u8*)(glyph_sizes + item.accessory[j] * 4 + 3);
            }

            accessory_options.x = x + size.width - (margins.width*2) - text_size;
            accessory_options.y = y;
        }

        if (item.details.length() != 0) {
            y += element_spacing * details_size;

            details_options.x = x;
            details_options.y = y;
        }

        y += element_spacing * title_size;

        Color color = i == selected_item ? selected_color : default_color;
        if (!is_focused()) {
            color = default_color;
        }

        draw_text_opt(&title_options, color, (char*)item.title.c_str(), -1, title_size);
        draw_text_opt(&accessory_options, color, (char*)item.accessory.c_str(), -1, accessory_size);
        draw_text_opt(&details_options, color, (char*)item.details.c_str(), -1, details_size);
    }
}

void ListMenuElement::on_pressed_buttons(CONTROLLER_INPUT input) {
    if (input & Cross) {
        if (delegate_ != nullptr) {
            delegate_->on_item_activated(selected_item, this);
        }
    }

    if (input & Up) {
        selected_item--;
        if (selected_item < 0) {
            selected_item = items_.size() - 1;
        }

        if (delegate_ != nullptr) {
            delegate_->on_item_selected(selected_item, this);
        }
    }

    if (input & Down) {
        selected_item++;
        if (selected_item >= items_.size()) {
            selected_item = 0;
        }

        if (delegate_ != nullptr) {
            delegate_->on_item_selected(selected_item, this);
        }
    }
}