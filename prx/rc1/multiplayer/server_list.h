//
// Created by Vetle Hjelle on 04/12/2022.
//

#ifndef PS3_CKIT_SERVER_LIST_H
#define PS3_CKIT_SERVER_LIST_H

#include <rc1/common.h>
#include <rc1/rc1.h>
#include <rc1/hud.h>

typedef struct {
    u32 ip;
    u16 port;
    char name[50];
    int num_players;
    int max_players;
} GameServer;

// List of available game servers
GameServer game_servers[20];

// Number of game servers in the list
int num_game_servers;

int game_server_last_refresh;

void render_server_list();
void handle_input();

#endif //PS3_CKIT_SERVER_LIST_H
