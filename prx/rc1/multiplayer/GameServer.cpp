//
// Created by bordplate on 12/27/2022.
//

#include "GameServer.h"

#include <lib/memory.h>

GameServer::~GameServer() {
    if (this->name) {
        delete this->name;
    }
}