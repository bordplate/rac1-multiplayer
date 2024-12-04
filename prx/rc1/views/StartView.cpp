////
//// Created by Vetle Hjelle on 20/12/2022.
////
//
//#include "StartView.h"
//
//#include <lib/logger.h>
//
//#include "ServerListView.h"
//
//
//#include "../Game.h"
//#include "rc1/ui/TextElement.h"
//
//
//const char* start_multiplayer_str = "\x13 Start multiplayer";
//const char* connecting_str = "Loading servers...";
//
//StartView::~StartView() {
//
//}
//
//void StartView::on_load() {
//    label_ = new TextElement(250, 310, start_multiplayer_str, ViewMenu);
//
//    this->add_element(label_);
//
//    test_opt.line_spacing = 1;
//    test_opt.x = 40;
//    test_opt.y = 350;
//    test_opt.x_min = 0;
//    test_opt.x_max = 500;
//    test_opt.y_min = 0;
//    test_opt.y_max = 500;
//    test_opt.flags = 0;
//}
//
//void StartView::render() {
//    char* blah = "Hello there!\0";
//
//    draw_text_opt(&test_opt, 0x80ffa888, blah, -1, 1.0f);
//
//    View::render();
//}
//
//void StartView::on_pressed_buttons(CONTROLLER_INPUT input) {
//    if (input & Square) {
//        Logger::info("Querying game servers from directory");
//        Game::shared().query_servers(0, (ServerQueryCallback)&server_query_callback);
//
//        label_->text->set(connecting_str);
//    }
//}
