//
// Created by Vetle Hjelle on 31/12/2022.
//

#include "RemoteView.h"
#include "rc1/ui/InputElement.h"
#include <rc1/Game.h>

void RemoteView::on_load() {
    memory_info_text_ = new TextElement(80, 0, "<memory>", ViewMenu);
    ping_text_ = new TextElement(0, 20, "<ping>", ViewMenu);
//    profiler_text_ = new TextAreaElement(-70, 40, 250, 300);
//    profiler_text_->draws_background = true;

    add_element(memory_info_text_);
    add_element(ping_text_);
//    add_element(profiler_text_);
}

void RemoteView::render() {
    if (game_state == Menu) {
        memory_info_text_->text->setf("mem: %d/%d (%d)", used_memory, sizeof(memory_area), num_allocated);
        ping_text_->text->setf("ping: %lu", Game::shared().client()->latency_ * 2);
    } else {
        memory_info_text_->text->set("");
        ping_text_->text->set("");
    }

    //profiler_text_->text = Profiler::get_profile_data();

    View::render();
}

/**
 * Gets an element for the given ID if it exists, otherwise creates a new element with that ID.
 *
 * @param id
 * @return
 */
TextElement* RemoteView::get_text_element(int id) {
    Logger::trace("Getting text element for remote view. ID: %d", id);
    if (id > 1024) {
        return nullptr;
    }

    if (id+1 >= text_elements_.capacity()) {
        Logger::trace("Updating text element capacity to: %d", id + 1);
        text_elements_.resize(id + 1);
    }

    if (!text_elements_[id]) {
        Logger::trace("Making new text element for remote view. ID: %d", id);
        text_elements_[id] = new TextElement(0, 0, "<?>", ViewNoView);
        this->add_element(text_elements_[id]);
        //text_elements_[id] = element;
    }

    return text_elements_[id];
}

void RemoteView::delete_text_element(int id) {
    Logger::trace("Deleting remote view element ID %d", id);

    if (id > 1024 || id > text_elements_.size()) {
        return;
    }

    TextElement* element = text_elements_[id];
    if (element != nullptr) {
        this->remove_element(element);

        text_elements_[id] = nullptr;
    }
}

int RemoteView::get_index_for_element(ViewElement *element) {
    for (RemoteViewElement* current = first_element_; current != nullptr; current = current->next_element) {
        if (current->element == element) {
            return current->id;
        }
    }

    return -1;
}

ViewElement* RemoteView::get_element(u16 id) {
    for (RemoteViewElement* current = first_element_; current != nullptr; current = current->next_element) {
        if (current->id == id) {
            return current->element;
        }
    }

    return nullptr;
}

void RemoteView::delete_element(u16 id) {
    RemoteViewElement* previous = nullptr;
    RemoteViewElement* current = first_element_;

    while (current != nullptr) {
        if (current->id == id) {
            if (previous == nullptr) {
                first_element_ = current->next_element;
            } else {
                previous->next_element = current->next_element;
            }

            delete current;
            return;
        }

        previous = current;
        current = current->next_element;
    }
}

void RemoteView::clear_all() {
    for (RemoteViewElement* current = first_element_; current != nullptr; current = current->next_element) {
        focused_element = nullptr;
        remove_element(current->element);

        delete current;
    }

    first_element_ = nullptr;
}

ViewElement* RemoteView::create_element_with_type(u16 id, MPUIElementType element_type) {
    ViewElement* element = nullptr;

    switch (element_type) {
        case MPUIElementTypeText:
            element = new TextElement(0, 0, "", 0xFF);
            break;
        case MPUIElementTypeTextArea:
            element = new TextAreaElement(0, 0, 0, 0);
            break;
        case MPUIElementTypeListMenu:
            element = new ListMenuElement(0, 0, 0, 0);
            ((ListMenuElement*)element)->set_delegate(this);
            break;
        case MPUIElementTypeInput: {
            element = new InputElement();
            ((InputElement*)element)->set_delegate(this);
            break;
        }
        default:
            element = new ViewElement(0, 0, 0, 0);
            break;
    }

    RemoteViewElement* new_element = new RemoteViewElement();
    new_element->id = id;
    new_element->element = element;
    new_element->next_element = nullptr;

    if (first_element_ == nullptr) {
        first_element_ = new_element;

        return element;
    }

    for (RemoteViewElement* current = first_element_; current != nullptr; current = current->next_element) {
        if (current->id == id) {
            Logger::error("Element with id %d already exists", id);
            return nullptr;
        }

        if (current->next_element == nullptr) {
            current->next_element = new_element;
            return element;
        }
    }

    return element;
}

void RemoteView::handle_packet(MPPacketUI* packet) {
    // Start out by clearing all if specified
    if (packet->operations & MP_UI_OPERATION_CLEAR_ALL) {
        clear_all();
    }

    ViewElement* element = nullptr;

    if (packet->operations & MP_UI_OPERATION_CREATE) {
        element = create_element_with_type(packet->id, (MPUIElementType)packet->element_type);

        add_element(element);
    }

    if (packet->operations & MP_UI_OPERATION_DELETE) {
        delete_element(packet->id);
    }

    if (packet->operations & MP_UI_OPERATION_UPDATE) {
        element = get_element(packet->id);
    }

    if (packet->operations & MP_UI_OPERATION_CREATE || packet->operations & MP_UI_OPERATION_UPDATE) {
        if (element == nullptr) {
            Logger::error("Server sent a create/update packet for id %d, but element was null", packet->id);
            return;
        }

        Logger::trace("Handling packet for UI element %d (%d)", packet->id, packet->element_type);

        size_t idx = 0;
        MPPacketUIItem* item = (MPPacketUIItem*)((char*)packet + sizeof(MPPacketUI));
        for (u16 i = 0; i < packet->items; i++) {
            size_t data_length = item->data_length;
            char* data = (char*)((char*)item + sizeof(MPPacketUIItem));

            idx += sizeof(MPPacketUIItem) + data_length;

            switch ((MPUIElementAttribute)item->attribute) {
                case MPUIElementAttributePosition: {
                    int x = (int)((u16*)data)[0];
                    int y = (int)((u16*)data)[1];

                    element->x = x;
                    element->y = y;

                    break;
                }
                case MPUIElementAttributeSize: {
                    int width = (int)((u16*)data)[0];
                    int height = (int)((u16*)data)[1];

                    element->size.width = width;
                    element->size.height = height;

                    break;
                }
                case MPUIElementAttributeMargins: {
                    int width = (int)((u16*)data)[0];
                    int height = (int)((u16*)data)[1];

                    element->margins.width = width;
                    element->margins.height = height;
                    break;
                }
                case MPUIElementAttributeVisible: {
                    element->visible = (bool)*(u32*)data;
                    break;
                }
                case MPUIElementAttributeStates: {
                    element->states = *(u32*)data;
                    break;
                }
                case MPUIElementAttributeLineSpacing: {
                    ((TextAreaElement*)element)->text_options.line_spacing = (s16)(*(u32*)data);
                    break;
                }
                case MPUIElementAttributeElementSpacing: {
                    ((ListMenuElement*)element)->element_spacing = *(s32*)data;
                    break;
                }
                case MPUIElementAttributeDrawsBackground: {
                    element->draws_background = (bool)*(u32*)data;
                    break;
                }
                case MPUIElementAttributeText: {
                    switch ((MPUIElementType)packet->element_type) {
                        case MPUIElementTypeText:
                            ((TextElement*)element)->text->setf("%s", data);
                            break;
                        case MPUIElementTypeTextArea:
                            ((TextAreaElement*)element)->text.setf("%s", data);
                            break;
                        default: break;
                    }
                    break;
                }
                case MPUIElementAttributeInputPrompt: {
                    ((InputElement*)element)->set_prompt(data);
                    break;
                }
                case MPUIElementAttributeTitleText: {
                    ((ListMenuElement*)element)->get_item(item->id)->title.set((char*)data);
                    break;
                }
                case MPUIElementAttributeDetailsText: {
                    ((ListMenuElement*)element)->get_item(item->id)->details.set((char*)data);
                    break;
                }
                case MPUIElementAttributeTextSize: {
                    float size = 0.0f;

                    memcpy(&size, data, sizeof(float));

                    ((TextAreaElement*)element)->text_size = size;
                    break;
                }
                case MPUIElementAttributeTextColor: {
                    switch ((MPUIElementType)packet->element_type) {
                        case MPUIElementTypeText:
                            ((TextElement*)element)->color = *(Color*)data;
                            break;
                        case MPUIElementTypeTextArea:
                            ((TextAreaElement*)element)->color = *(Color*)data;
                            break;
                        default: break;
                    }
                    break;
                }
                case MPUIElementAttributeTitleTextSize: {
                    float size = 0.0f;
                    memcpy(&size, data, sizeof(float));
                    ((ListMenuElement*)element)->title_size = size;
                    break;
                }
                case MPUIElementAttributeTitleTextColor: {
                    ((ListMenuElement*)element)->default_color = *(Color*)data;
                    break;
                }
                case MPUIElementAttributeDetailsTextSize: {
                    float size = 0.0f;
                    memcpy(&size, data, sizeof(float));
                    ((ListMenuElement*)element)->details_size = size;
                    break;
                }
                case MPUIElementAttributeDetailsTextColor: {
                    ((ListMenuElement*)element)->selected_color = *(Color*)data;
                    break;
                }
                case MPUIElementAttributeMenuDefaultColor: {
                    ((ListMenuElement*)element)->default_color = *(Color*)data;
                    break;
                }
                case MPUIElementAttributeMenuSelectedColor: {
                    ((ListMenuElement*)element)->selected_color = *(Color*)data;
                    break;
                }
                case MPUIElementAttributeMenuSelectedItem: {
                    ((ListMenuElement*)element)->selected_item = *(s32*)data;
                    break;
                }
                case MPUIElementAttributeShadow: {
                    switch ((MPUIElementType)item->attribute) {
                        case MPUIElementTypeText:
                            ((TextElement*)element)->has_shadow = *(bool*)data;
                            break;
                        default: break;
                    }

                    break;
                }
                case MPUIElementAttributeMenuItems: {
                    ListMenuElement* list_menu = (ListMenuElement*)element;

                    s8 item_id = -1;
                    s8 attribute_num = -1;
                    u16 attribute_length = 0;
                    s8 should_delete = -1;

                    for (size_t j = 0; j < data_length; j++) {
                        if (item_id == -1) {
                            item_id = (s8)data[j];
                            attribute_num = 0;

                            if (item_id > ((ssize_t)list_menu->item_count())-1) {
                                Logger::trace("Making new item %d", item_id);
                                ListMenuItem menu_item = ListMenuItem();
                                list_menu->add_item(menu_item);
                            }

                            continue;
                        }

                        if (should_delete == -1) {
                            should_delete = (s8)data[j];
                            continue;
                        }

                        if (attribute_num != -1) {
                            if (data[j] != 0x0) {
                                attribute_length += 1;
                                continue;
                            } else {
                                if (attribute_length > 0) {
                                    if (attribute_num == 0) {
                                        ((ListMenuElement*)element)->get_item(item_id)->title.set((char*)(data + j - attribute_length));
                                    } else if (attribute_num == 1) {
                                        ((ListMenuElement*)element)->get_item(item_id)->details.set((char*)(data + j - attribute_length));
                                    } else if (attribute_num == 2) {
                                        ((ListMenuElement*)element)->get_item(item_id)->accessory.set((char*)(data + j - attribute_length));
                                    } else {
                                        Logger::error("Unknown attribute number %d", attribute_num);
                                    }
                                }

                                if (attribute_num == 2) {
                                    if (should_delete == 1) {
                                        list_menu->remove_item(item_id);
                                    }

                                    // This is the last attribute we're getting, so we signal to start getting the next item
                                    // If you add more attributes, you need to make sure this is updated for the last attribute
                                    // in the list
                                    item_id = -1;
                                    attribute_num = -1;
                                    should_delete = -1;
                                }

                                attribute_length = 0;
                                attribute_num += 1;
                            }
                        }
                    }

                    break;
                }
                default: {
                    Logger::error("Unknown attribute %d", item->attribute);
                    break;
                }
            }

            item = (MPPacketUIItem*)((char*)item + sizeof(MPPacketUIItem) + data_length);
        }
    }
}

void RemoteView::handle_event(MPPacketUIEvent *packet) {
    ViewElement* element = get_element(packet->element_id);

    if (element == nullptr) {
        Logger::error("Element with id %d not found", packet->element_id);
        return;
    }

    switch ((MPUIElementEventType)packet->event_type) {
        case MPUIElementEventTypeMakeFocused: {
            element->focus();

            break;
        }
        case MPUIElementEventTypeActivate: {
            ((InputElement*)element)->activate();

            break;
        }
        default: {
            Logger::error("We can't handle UI event %d yet.", packet->event_type);
        }
    }
}

void RemoteView::on_item_activated(int index, ListMenuElement* list_menu_element) {
    Logger::trace("Activated item %d", index);

    int element_id = get_index_for_element(list_menu_element);
    if (element_id < 0) {
        return;
    }

    Client* client = Game::shared().connected_client();
    if (client) {
        Logger::trace("Sending item activated event for element %d", element_id);
        client->send_ack(Packet::make_ui_event_packet(MPUIElementEventTypeItemActivated, element_id, index));
    }
}

void RemoteView::on_item_selected(int index, ListMenuElement *list_menu_element) {
    int element_id = get_index_for_element(list_menu_element);
    if (element_id < 0) {
        return;
    }

    Client* client = Game::shared().connected_client();
    if (client) {
        Logger::trace("Sending item selected event for element %d", element_id);
        client->send_ack(Packet::make_ui_event_packet(MPUIElementEventTypeItemSelected, element_id, index));
    }
}

void RemoteView::on_input_callback(InputElement *input_element) {
    int element_id = get_index_for_element(input_element);
    if (element_id < 0) {
        return;
    }

    Client* client = Game::shared().connected_client();
    if (client) {
        Logger::trace("Sending item selected event for element %d", element_id);
        client->send_ack(Packet::make_ui_event_with_text_packet(MPUIElementEventTypeInputText, element_id, 0, input_element->text().c_str()));
    }
}
