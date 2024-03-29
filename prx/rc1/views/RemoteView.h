//
// Created by Vetle Hjelle on 31/12/2022.
//

#ifndef RAC1_MULTIPLAYER_REMOTEVIEW_H
#define RAC1_MULTIPLAYER_REMOTEVIEW_H

#include "../View.h"
#include "../TextElement.h"

#include <lib/vector.h>

struct RemoteView : public View {
    TextElement* get_element(int id);
    void delete_element(int id);

    void on_load();
    void render();
private:
    Vector<TextElement*> text_elements_;

    TextElement* ping_text_;
    TextElement* memory_info_text_;
};


#endif //RAC1_MULTIPLAYER_REMOTEVIEW_H
