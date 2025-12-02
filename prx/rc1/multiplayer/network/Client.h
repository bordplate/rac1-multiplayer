//
// Created by Vetle Hjelle on 19/12/2022.
//

#ifndef PS3_CKIT_CLIENT_H
#define PS3_CKIT_CLIENT_H

#include "Packet.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SEND_BUFFER_SIZE 1024
#define RECV_BUFFER_SIZE (1024 * 16)

extern int64_t get_time();

typedef int (*AckCallback)(void* data, size_t len, void* extra);
typedef int (*SelfRefAckCallback)(void* self, void* data, size_t len);

struct MPUnacked {
    Packet* data;
    AckCallback ack_cb;
    size_t len;
    bool acked;
    void* extra;
    unsigned char next_unacked;
    int64_t send_time;
};

struct Client {
public:
    Client(char* ip, int port);

    virtual bool update(MPPacketHeader* header, void* packet_data);

    void make_ack(Packet* packet, AckCallback callback, void* extra);
    void make_ack(Packet* packet, AckCallback callback) { make_ack(packet, callback, nullptr); }
    void make_rpc(Packet* packet, AckCallback callback);
    void make_self_referencing_rpc(Packet* packet, AckCallback callback);

    void send_rpc(Packet* packet, AckCallback callback, void* extra);
    void send_rpc(Packet* packet, AckCallback callback);
    void send_ack(Packet* packet);
    void send(Packet* packet);
    void flush();
    void send_handshake();
    void send_ack(unsigned char id, unsigned char cycle);
    void send_ping();

    virtual void on_tick();
    virtual void receive();
    virtual void _connect();
    virtual void reset();
    virtual void disconnect();
    virtual void drop_receive();

    int64_t latency_;
    uint64_t connection_start_time_;
    bool suppress_timeout_alert_;

    void calculate_offset(int64_t client_send_time, int64_t server_receive_time);
    int64_t server_time_difference(int64_t time);

    bool connected() const { return connected_; }
    bool handshake_complete() const { return handshake_complete_; }
    long received() const { return received_; }
protected:
    struct sockaddr_in sockaddr_;
private:
    int sockfd_;

    char recv_buffer[RECV_BUFFER_SIZE];
    long received_;

    char send_buffer[SEND_BUFFER_SIZE];
    size_t send_buffer_len;

    unsigned char acked_[256];
    MPUnacked unacked_[256];
    unsigned char ack_id_;
    unsigned char ack_cycle_;

    int64_t estimated_server_time_;
    int64_t last_sync_time;
    static const int64_t sync_interval = 5000; // Sync every 5 seconds
    bool has_first_time_sync;

    int64_t last_packet_time_get_;
    int64_t last_ping_;

    bool connected_;
    bool handshake_complete_;

    void ack(char* packet, size_t len);

    void request_server_time();
    static int server_time_response_callback(void* data, size_t len, void* extra);
    int64_t get_estimated_server_time();
};


#endif //PS3_CKIT_CLIENT_H
