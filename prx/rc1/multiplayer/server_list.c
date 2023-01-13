//
// Created by Vetle Hjelle on 04/12/2022.
//

#include "server_list.h"
#include <sysutil/sysutil_oskdialog.h>
#include "multiplayer.h"
#include <rc1/hud.h>

#define SYS_MEMORY_CONTAINER_ID_INVALID         0xFFFFFFFFU

// Index of the currently selected game server
int selected_server = 0;

// Player's nickname
char nickname[50] = "";
HUDElementText* nickElement;

void render_server_list() {
    // Clear the HUD
    hud_clear();

// Loop through each game server in the list
    for (int i = 0; i < num_game_servers; i++) {
        GameServer server = game_servers[i];

        // Create a new text element for the game server
        HUDElementText* element = hud_new_text_element(i);

        // Calculate the Y coordinate for the element
        int total_height = num_game_servers * 30;
        int y = ((500 - total_height) - total_height / 2) / 2 + i * 30;

        // Set the element's position
        element->element.x = 250;
        element->element.y = y;

        // Set the element's color
        element->Color = (i == selected_server) ? 0xFF0000FF : 0xC0FFA888;

        // Set the element's text
        sprintf(element->text, "%s (%d/%d)", server.name, server.num_players, server.max_players);

        // Set the element's flags
        element->flags = HUDTextDropShadow;
    }

    // Create a new text element for the "back" button
    HUDElementText* backElement = hud_new_text_element(19);

    // Set the element's position
    backElement->element.x = 380;
    backElement->element.y = 390;

    // Set the element's color
    backElement->Color = 0xC0FFA888;

    // Set the element's text
    sprintf(backElement->text, "\x12 Back");

    // Set the element's flags
    backElement->flags = HUDTextDropShadow;

    // Create a new text element for the "change nickname"
    nickElement = hud_new_text_element(18);

    // Set the element's position
    nickElement->element.x = 120;
    nickElement->element.y = 390;

    // Set the element's color
    nickElement->Color = 0xC0FFA888;

    // Set the element's text
    if (strlen(nickname) > 2) {
        sprintf(nickElement->text, nickname);
    } else {
        sprintf(nickElement->text, "\x13 Change nickname");
    }

    // Set the element's flags
    nickElement->flags = HUDTextDropShadow;
}

void connect_to_server() {
    // Get the selected game server
    GameServer server = game_servers[selected_server];

    mp_sock = 0;
    mp_sockaddr.sin_addr.s_addr = server.ip;
    mp_sockaddr.sin_port = htons(server.port);

    mp_connection_directory_mode = 0;

    should_render_server_list = 0;
    mp_reset_environment();
}

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

static int oskdialog_mode = MODE_IDLE;

CellOskDialogCallbackReturnParam OutputInfo = {};

// Callback function to handle the result of the on-screen keyboard
static void osk_callback(uint64_t status, uint64_t param, void *userdata)
{
    (void)param;
    (void)userdata;

    MULTI_LOG("Hello?\n");

    switch (status) {
        case CELL_SYSUTIL_OSKDIALOG_LOADED:
            break;
        case CELL_SYSUTIL_OSKDIALOG_FINISHED:
            oskdialog_mode = MODE_CLOSE;
            break;
        case CELL_SYSUTIL_OSKDIALOG_UNLOADED:
            break;
        case CELL_SYSUTIL_REQUEST_EXITGAME:
            oskdialog_mode = MODE_EXIT;
            break;
        case CELL_SYSUTIL_DRAWING_BEGIN:
            break;
        case CELL_SYSUTIL_DRAWING_END:
            break;
        case CELL_SYSUTIL_OSKDIALOG_INPUT_ENTERED:
            oskdialog_mode = MODE_ENTERED;
            break;
        case CELL_SYSUTIL_OSKDIALOG_INPUT_CANCELED:
            oskdialog_mode = MODE_CANCELED;
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

void open_osk()
{
    // Create the input field parameters
    CellOskDialogInputFieldInfo input_field;
    input_field.message = (uint16_t*)L"Enter your name:";
    input_field.init_text = (uint16_t*)L"";
    input_field.limit_length = CELL_OSKDIALOG_STRING_SIZE;

    // Create the on-screen keyboard parameters
    CellOskDialogParam osk_param;
    osk_param.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_DEFAULT;
    osk_param.firstViewPanel = CELL_OSKDIALOG_PANELMODE_ALPHABET;
    CellOskDialogPoint point = {0.0, 0.0};

    /*E Buffer storing returned text */
    OutputInfo.result = CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK; /*E Result of on-screen keyboard dialog termination */
    OutputInfo.numCharsResultString = 48; /* E Specify number of characters for returned text */

    uint16_t Result_Text_Buffer[CELL_OSKDIALOG_STRING_SIZE + 1];
    OutputInfo.pResultString = Result_Text_Buffer; /*E Buffer storing returned text */

    osk_param.controlPoint = point;
    osk_param.prohibitFlgs = 0;

    cellSysutilRegisterCallback( 2, osk_callback, NULL );

    // Open the on-screen keyboard and pass the callback function to handle the result
    int ret = cellOskDialogLoadAsync(SYS_MEMORY_CONTAINER_ID_INVALID, &osk_param, &input_field);
    if (ret < 0)
    {
        printf("Failed to open OSK: %d\n", ret);
        return;
    }

}

int getkbLen(char* str)
{
    int nullCount = 0;
    int i = 0; //num of chars..
    for (i = 0; i < 64; i++)
    {
        if (nullCount == 2) { break; }
        if (*(str + i) == 0x00) { nullCount++; }
        else { nullCount = 0; }
    }
    return i;
}
void makekbStr(char* str, char* dest, int len)
{
    int nulls = 0;
    for (int i = 0; i < len; i++)
    {
        if (*(str + i) == 0x00) { nulls++; }
        else { *(dest + i - nulls) = *(str + i); }
    }
    *(dest + len + 1 - nulls) = 0x00;  //make sure its nulled...
}


void handle_input() {
    cellSysutilCheckCallback();

    // Check if the circle button is pressed
    if (pressed_buttons & Square) {
        // Open the nickname input dialog
        open_osk();
    }

    if (oskdialog_mode == MODE_OPEN)
    {
        open_osk();
        oskdialog_mode = MODE_RUNNING;
    }

    if (oskdialog_mode == MODE_ENTERED)
    {
        cellOskDialogGetInputText(&OutputInfo);
        oskdialog_mode = MODE_RUNNING;
    }
    if (oskdialog_mode == MODE_CLOSE)
    {
        cellOskDialogUnloadAsync(&OutputInfo);

        if (cellSysutilUnregisterCallback(2) != 0) { MULTI_LOG("Couldn't unload the keyboard !\n"); }

        memset(&nickname, 0, sizeof(nickname));

        int strLen = getkbLen((char*)(*(&OutputInfo.pResultString)));

        if (strLen <= 2) {
            sprintf(&nickname, "");
        } else {
            char dest[strLen];
            makekbStr((char *) (*(&OutputInfo.pResultString)), dest, strLen);
            snprintf(&nickname, sizeof(nickname), "\x13 %s", dest);
        }

        oskdialog_mode = MODE_EXIT;
    }

    // Move the selected game server up or down
    if (pressed_buttons & Up) {
        selected_server = (selected_server + num_game_servers - 1) % num_game_servers;
    } else if (pressed_buttons & Down) {
        selected_server = (selected_server + 1) % num_game_servers;
    }

    // Check if the cross button is pressed
    if (pressed_buttons & Cross) {
        // Connect to the selected game server
        connect_to_server();
    }
}
