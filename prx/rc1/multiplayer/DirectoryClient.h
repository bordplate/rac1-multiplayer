//
// Created by bordplate on 12/27/2022.
//

#ifndef RAC1_MULTIPLAYER_DIRECTORYCLIENT_H
#define RAC1_MULTIPLAYER_DIRECTORYCLIENT_H

#include "Client.h"
#include "byteswap.h"

class DirectoryClient : public Client {
public:
    DirectoryClient(char* ip, int port) : Client(ip, port) {};

    void query_directory(int directory_id, AckCallback callback);

    bool update(MPPacketHeader* header, void* packet_data);
    void on_tick();
};


#endif //RAC1_MULTIPLAYER_DIRECTORYCLIENT_H
