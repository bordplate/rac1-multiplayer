//
// Created by bordplate on 6/17/2023.
//

#ifndef RAC1_MULTIPLAYER_INPUT_H
#define RAC1_MULTIPLAYER_INPUT_H

#include <rc1/rc1.h>
#include <sysutil/sysutil_oskdialog.h>

#define SYS_MEMORY_CONTAINER_ID_INVALID         0xFFFFFFFFU

class Input;

typedef int (*InputCallback)(Input* input, void* userdata, int status);

/*E Status of the on-screen keyboard dialog */
enum {
    MODE_IDLE = 0,
    MODE_OPEN,
    MODE_RUNNING,
    MODE_ENTERED,
    MODE_EXIT,
    MODE_CLOSE,
    MODE_CANCELED
};

class Input {
public:
    int oskdialog_mode;

    String input_text;

    void activate();
    void check_callback();

    void set_callback(InputCallback callback, void* userdata);
private:
    CellOskDialogCallbackReturnParam output_info_;
    uint16_t input_buffer_[CELL_OSKDIALOG_STRING_SIZE + 1];

    InputCallback callback_;
    void* callback_userdata_;
    bool has_callback_;

    static void osk_callback(uint64_t status, uint64_t param, void *userdata);
    void open_osk();
};


#endif //RAC1_MULTIPLAYER_INPUT_H
