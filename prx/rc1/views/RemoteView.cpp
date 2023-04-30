//
// Created by Vetle Hjelle on 31/12/2022.
//

#include "RemoteView.h"

/**
 * Gets an element for the given ID if it exists, otherwise creates a new element with that ID.
 *
 * @param id
 * @return
 */
TextElement* RemoteView::get_element(int id) {
    Logger::trace("Getting text element for remote view. ID: %d", id);
    if (id > 1024) {
        return nullptr;
    }

    if (id >= text_elements_.capacity()) {
        Logger::trace("Updating text element capacity to: %d", id + 1);
        text_elements_.resize(id + 1);
    }

    if (!text_elements_[id]) {
        Logger::trace("Making new text element for remote view. ID: %d", id);
        TextElement* element = new TextElement(0, 0, "<?>");
        this->add_element(element);
        text_elements_[id] = element;
    }

    return text_elements_[id];
}

void RemoteView::delete_element(int id) {
    Logger::trace("Deleting remote view element ID %d", id);

    if (id > 1024 || id > text_elements_.size()) {
        return;
    }

    TextElement* element = text_elements_[id];
    if (element != nullptr) {
        this->remove_element(element);
        delete element;
        text_elements_[id] = nullptr;
    }
}