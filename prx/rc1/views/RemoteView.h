//
// Created by Vetle Hjelle on 31/12/2022.
//

#ifndef RAC1_MULTIPLAYER_REMOTEVIEW_H
#define RAC1_MULTIPLAYER_REMOTEVIEW_H

#include "rc1/ui/View.h"
#include "rc1/ui/TextElement.h"
#include "rc1/ui/ListMenuElement.h"
#include "rc1/ui/Input.h"
#include "rc1/ui/InputElement.h"
#include "rc1/ui/TextAreaElement.h"

#include <lib/vector.h>
#include <rc1/multiplayer/network/Packet.h>

enum MPUIElementType {
    MPUIElementTypeNone = 0,
    MPUIElementTypeText = 1,
    MPUIElementTypeTextArea = 2,
    MPUIElementTypeListMenu = 3,
    MPUIElementTypeInput = 4,
};

enum MPUIElementAttribute {
    MPUIElementAttributeNone,
    MPUIElementAttributePosition,
    MPUIElementAttributeSize,
    MPUIElementAttributeMargins,
    MPUIElementAttributeVisible,
    MPUIElementAttributeStates,
    MPUIElementAttributeLineSpacing,
    MPUIElementAttributeElementSpacing,
    MPUIElementAttributeDrawsBackground,
    MPUIElementAttributeText,
    MPUIElementAttributeTitleText,
    MPUIElementAttributeDetailsText,
    MPUIElementAttributeTextSize,
    MPUIElementAttributeTextColor,
    MPUIElementAttributeTitleTextSize,
    MPUIElementAttributeTitleTextColor,
    MPUIElementAttributeDetailsTextSize,
    MPUIElementAttributeDetailsTextColor,
    MPUIElementAttributeMenuDefaultColor,
    MPUIElementAttributeMenuSelectedColor,
    MPUIElementAttributeMenuSelectedItem,
    MPUIElementAttributeMenuItems,
    MPUIElementAttributeShadow,
    MPUIElementAttributeInputPrompt,
    MPUIElementAttributeWorldSpacePosition,
    MPUIElementAttributeAlignment,
    MPUIElementAttributeWorldSpaceFlags,
    MPUIElementAttributeWorldSpaceMaxDistance,
};

enum MPUIElementEventType {
    MPUIElementEventTypeNone,
    MPUIElementEventTypeItemActivated,
    MPUIElementEventTypeItemSelected,
    MPUIElementEventTypeMakeFocused,
    MPUIElementEventTypeActivate,
    MPUIElementEventTypeInputText,
};

struct RemoteViewElement {
    u16 id;
    ViewElement* element;
    RemoteViewElement* next_element;
};

struct RemoteView : public View, public ListMenuDelegate, public InputDelegate {
    TextElement* get_text_element(int id);
    void delete_text_element(int id);

    void on_load();
    void render();

    int get_index_for_element(ViewElement* element);
    ViewElement* get_element(u16 id);
    void delete_element(u16 id);
    void clear_all();

    void handle_packet(MPPacketUI* packet);
    void handle_event(MPPacketUIEvent* packet);

    void on_item_activated(int index, ListMenuElement* list_menu_element);
    void on_item_selected(int index, ListMenuElement *list_menu_element);
    void on_input_callback(InputElement *input_element);
private:
    Vector<TextElement*> text_elements_;

    RemoteViewElement* first_element_;

    TextElement* ping_text_;
    TextElement* memory_info_text_;

    TextAreaElement* profiler_text_;

    ViewElement* create_element_with_type(u16 id, MPUIElementType element_type);
};


#endif //RAC1_MULTIPLAYER_REMOTEVIEW_H
