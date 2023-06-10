//
// Created by Vetle Hjelle on 31/12/2022.
//

#include "RemoteView.h"
#include <rc1/Game.h>

void RemoteView::on_load() {
    memory_info_text_ = new TextElement(80, 0, "<memory>");
    ping_text_ = new TextElement(0, 20, "<ping>");

    add_element(memory_info_text_);
    add_element(ping_text_);
}

void RemoteView::render() {
    memory_info_text_->text->setf("mem: %d/%d (%d)", used_memory, sizeof(memory_area), num_allocated);
    ping_text_->text->setf("ping: %lu", Game::shared().client()->latency*2);

    View::render();
}

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

        text_elements_[id] = nullptr;
    }
}