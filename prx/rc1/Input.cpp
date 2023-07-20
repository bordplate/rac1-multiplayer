#include "Input.h"

int getkbLen(char* str);
void makekbStr(char* str, char* dest, int len);

void Input::activate() {
    open_osk();
}

void Input::check_callback() {
    cellSysutilCheckCallback();

    if (oskdialog_mode == MODE_OPEN)
    {
        open_osk();
        oskdialog_mode = MODE_RUNNING;
    }

    if (oskdialog_mode == MODE_ENTERED)
    {
        cellOskDialogGetInputText(&output_info_);
        oskdialog_mode = MODE_RUNNING;
    }
    if (oskdialog_mode == MODE_CLOSE)
    {
        oskdialog_mode = MODE_EXIT;
        cellOskDialogUnloadAsync(&output_info_);

        if (cellSysutilUnregisterCallback(2) != 0) { Logger::error("Couldn't unload the keyboard!"); }

        String result = String::format("%S", (wchar_t *)output_info_.pResultString);

        if (result.length() <= 2) {
            if (has_callback_) {
                callback_(this, callback_userdata_, 1);
            }
            return;
        } else {
            input_text.set(result.c_str());
        }

        if (has_callback_) {
            callback_(this, callback_userdata_, 0);
        }
    }
}

void Input::set_callback(InputCallback callback, void* userdata) {
    callback_ = callback;
    callback_userdata_ = userdata;
    has_callback_ = true;
}

void Input::osk_callback(uint64_t status, uint64_t param, void *userdata) {
    Input* self = (Input*)userdata;

    switch (status) {
        case CELL_SYSUTIL_OSKDIALOG_LOADED:
            break;
        case CELL_SYSUTIL_OSKDIALOG_FINISHED:
            self->oskdialog_mode = MODE_CLOSE;
            break;
        case CELL_SYSUTIL_OSKDIALOG_UNLOADED:
            break;
        case CELL_SYSUTIL_REQUEST_EXITGAME:
            self->oskdialog_mode = MODE_EXIT;
            break;
        case CELL_SYSUTIL_DRAWING_BEGIN:
            break;
        case CELL_SYSUTIL_DRAWING_END:
            break;
        case CELL_SYSUTIL_OSKDIALOG_INPUT_ENTERED:
            self->oskdialog_mode = MODE_ENTERED;
            break;
        case CELL_SYSUTIL_OSKDIALOG_INPUT_CANCELED:
            self->oskdialog_mode = MODE_CANCELED;
            break;
        case CELL_SYSUTIL_OSKDIALOG_INPUT_DEVICE_CHANGED:
            break;
        case CELL_SYSUTIL_OSKDIALOG_DISPLAY_CHANGED:
            break;
        case CELL_SYSUTIL_SYSTEM_MENU_CLOSE:

            break;
        case CELL_SYSUTIL_SYSTEM_MENU_OPEN:
            break;
        default:
            break;
    }
}

void Input::open_osk()
{
    // Create the input field parameters
    CellOskDialogInputFieldInfo input_field;
    input_field.message = (uint16_t*)L"Create a username:";
    input_field.init_text = (uint16_t*)L"";
    input_field.limit_length = CELL_OSKDIALOG_STRING_SIZE;

    // Create the on-screen keyboard parameters
    CellOskDialogParam osk_param;
    osk_param.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_LATIN;
    osk_param.firstViewPanel = CELL_OSKDIALOG_PANELMODE_LATIN;
    osk_param.prohibitFlgs = CELL_OSKDIALOG_NO_SPACE | CELL_OSKDIALOG_NO_RETURN;
    CellOskDialogPoint point = {0.0, 0.0};

    osk_param.controlPoint = point;

    output_info_.result = CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK; /*E Result on-screen keyboard dialog termination */
    output_info_.numCharsResultString = 48; /*E Specify number of characters for returned text */
    output_info_.pResultString = (uint16_t*)&input_buffer_; /*E Buffer storing returned text */ ;

    cellSysutilRegisterCallback(2, osk_callback, this);

    cellOskDialogSetKeyLayoutOption (CELL_OSKDIALOG_10KEY_PANEL | CELL_OSKDIALOG_FULLKEY_PANEL);

    // Open the on-screen keyboard and pass the callback function to handle the result
    int ret = cellOskDialogLoadAsync(SYS_MEMORY_CONTAINER_ID_INVALID, &osk_param, &input_field);
    if (ret < 0)
    {
        Logger::error("Failed to open OSK: %d", ret);
    }
}