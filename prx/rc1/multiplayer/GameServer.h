//
// Created by bordplate on 12/27/2022.
//

#ifndef RAC1_MULTIPLAYER_GAMESERVER_H
#define RAC1_MULTIPLAYER_GAMESERVER_H

#include <lib/string.h>

struct GameServer {
    int ip;
    unsigned short port;
    String* name;
    String* description;
    String* owner_name;
    int num_players;
    int max_players;

    ~GameServer();
};


#endif //RAC1_MULTIPLAYER_GAMESERVER_H
