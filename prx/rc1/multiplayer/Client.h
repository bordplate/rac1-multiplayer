//
// Created by Vetle Hjelle on 19/12/2022.
//

#ifndef PS3_CKIT_CLIENT_H
#define PS3_CKIT_CLIENT_H

#include "packet.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef int (*AckCallback)(void* data, size_t len);

struct MPUnacked {
    Packet* data;
    AckCallback ack_cb;
    size_t len;
    unsigned char next_unacked;
};

struct Client {
public:
    Client(char* ip, int port);

    virtual bool update(MPPacketHeader* header, void* packet_data);

    void make_ack(Packet* packet, AckCallback callback);
    void make_rpc(Packet* packet, AckCallback callback);

    void send(Packet* packet);
    void send_handshake();
    void send_ack(unsigned char id, unsigned char cycle);

    virtual void on_tick();
    virtual void receive();
    virtual void connect();
    virtual void reset();

    bool connected() { return connected_; }
    bool handshake_complete() { return handshake_complete_; }
    long received() { return received_; }
private:
    int sockfd_;
    struct sockaddr_in sockaddr_;
    char recv_buffer[1024];
    long received_;

    unsigned char acked_[256];
    MPUnacked unacked_[256];
    unsigned char ack_id_;
    unsigned char ack_cycle_;

    bool connected_;
    bool handshake_complete_;

    void ack(char* packet, size_t len);
};


#endif //PS3_CKIT_CLIENT_H
