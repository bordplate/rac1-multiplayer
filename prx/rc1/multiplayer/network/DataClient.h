//
// Created by bordplate on 11/24/2025.
//

#ifndef RAC1_MULTIPLAYER_DATACLIENT_H
#define RAC1_MULTIPLAYER_DATACLIENT_H

#include <sys/timer.h>
#include <sys/ppu_thread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Packet.h"

struct DataClientConnectionInfo {
    sockaddr_in* sockaddr;
    u32 key;
};

class DataClient {
public:
    DataClient(sockaddr_in* sockaddr, u32 key);

    void _connect();
    void run();

    bool busy() {
        return should_transmit_ || is_receiving_;
    }

    static void start_data_client(DataClientConnectionInfo* connection_info) {
        DataClient client(connection_info->sockaddr, connection_info->key);
        client._connect();
        client.run();

        client.running_ = false;
        data_client_ = nullptr;

        sys_ppu_thread_exit(0);
    }

    static DataClient* shared() {
        // This is probably not good, considering the threading of it all.
        return data_client_;
    }

    bool is_running() const {
        return running_;
    }

    static void transmit(MPFileType file_type, void* data, size_t len);

    static bool stop;
private:
    int sockfd_;
    u32 key_;

    bool running_;

    bool is_receiving_;

    bool should_transmit_;
    void* file_to_transmit_;
    size_t file_size_;
    size_t transmitted_;
    MPFileType transmit_file_type_;

    static DataClient* data_client_;

    sockaddr_in* sockaddr_;
};


#endif //RAC1_MULTIPLAYER_DATACLIENT_H