//
// Created by Vetle Hjelle on 19/12/2022.
//

#include "Client.h"

//#include <netex/net.h>

#include <lib/common.h>
#include <lib/clib.h>
#include <lib/logger.h>
#include <lib/memory.h>
#include <sys/sys_time.h>

int64_t get_time() {
    return sys_time_get_system_time() / 1000;
}

Client::Client(char* ip, int port) {
    Logger::debug("New client for %s:%d", ip, port);

    memset(&sockaddr_, 0, sizeof(sockaddr_));
    memset(&unacked_, 0, sizeof(unacked_));

    sockaddr_.sin_addr.s_addr = inet_addr(ip);
    sockaddr_.sin_port = htons(port);
    sockaddr_.sin_family = AF_INET;

    estimated_server_time_ = 0;
    last_sync_time = 0;
    has_first_time_sync = false;
    send_buffer_len = 0;
}

void Client::_connect() {
    Logger::debug("Starting client");
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd_ < 0) {
        Logger::critical("Couldn't open socket: %d", sockfd_);
        return;
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sockaddr_.sin_addr), ip, INET_ADDRSTRLEN);

    Logger::info("Connecting to %s:%d...", ip, sockaddr_.sin_port);

    connected_ = true;
}

void Client::disconnect() {
    send(Packet::make_disconnect_packet());

    socketclose(sockfd_);

    connected_ = false;
    handshake_complete_ = false;
    sockfd_ = 0;
}

void Client::reset() {
    handshake_complete_ = false;
    connected_ = false;
    sockfd_ = 0;

    _connect();
}

void Client::send(Packet* packet) {
    packet->header->timeSent = get_time();

    // Calculate the total size of the packet.
    size_t packet_size = packet->len;

    // Check if we have enough space in the buffer.
    if (send_buffer_len + packet_size > sizeof(send_buffer)) {
        // Not enough space, send the buffer now.
        flush();
    }

    // Copy the packet data into the buffer.
    memcpy((char*)&send_buffer + send_buffer_len, packet->header, sizeof(MPPacketHeader));
    send_buffer_len += sizeof(MPPacketHeader);
    memcpy((char*)&send_buffer + send_buffer_len, packet->body, packet->header->size);
    send_buffer_len += packet->header->size;

    if (!packet->retain_after_send) {
        delete packet;
    }
}

void Client::flush() {
    if (sockfd_ && send_buffer_len > 0) {
        sendto(sockfd_, &send_buffer, send_buffer_len, 0, (struct sockaddr*)&sockaddr_, sizeof(sockaddr_));
        send_buffer_len = 0;
    }
}

/**
 * Makes the provided packet require acknowledgement from the server. If a callback is provided, that callback
 *  will be called when the packet has been acknowledged.
 *
 * @param packet
 */
void Client::make_ack(Packet* packet, AckCallback callback, void* extra) {
    // Handle overflows and first run
    do {
        if (ack_id_++ == 0) {
            ack_cycle_ += 1;

            Logger::debug("new ack cycle %d", ack_cycle_);
        }
    } while (!ack_id_);

    packet->header->requires_ack = ack_id_;
    packet->header->ack_cycle = ack_cycle_;

    MPUnacked* unacked = &(unacked_[ack_id_]);

    if (unacked->data != nullptr) {
        delete unacked->data;
    }

    memset(unacked, 0, sizeof(MPUnacked));

    unacked->acked = false;
    unacked->send_time = get_time();
    unacked->data = packet;

    packet->retain_after_send = true;

    if (packet->header->flags & MP_PACKET_FLAG_RPC) {
        unacked->ack_cb = callback;
        unacked->len = packet->len;
        unacked->extra = extra;
    }
}

void Client::make_rpc(Packet *packet, AckCallback callback) {
    packet->header->flags = MP_PACKET_FLAG_RPC;
    make_ack(packet, callback);
}

void Client::make_self_referencing_rpc(Packet* packet, AckCallback callback) {
    packet->header->flags = MP_PACKET_FLAG_RPC;
    make_ack(packet, callback, this);
}

void Client::ack(char* packet, size_t len) {
    MPPacketHeader* header = (MPPacketHeader*)packet;

    unsigned char ack_id = header->requires_ack;

    MPUnacked* unacked = &(unacked_[ack_id]);

    if (unacked->acked) {
        Logger::debug("Discarding already acked packet");
        return;
    }

    unacked->acked = true;

    if (unacked->data != nullptr) {
        delete unacked->data;
        unacked->data = nullptr;
    }

    // If the ack packet has data, call the event handler for this packet
    if (header->size > 0 && unacked->ack_cb) {
        unacked->ack_cb(&packet[sizeof(MPPacketHeader)], header->size, unacked->extra);

        Logger::trace("Acked message %d with size %d", header->type, header->size);
    } else {
        Logger::trace("Acked packet type %d:%d without data", header->type, header->size);
    }
}

void Client::send_handshake() {
    send(Packet::make_handshake_packet());
}

void Client::send_ack(unsigned char id, unsigned char cycle) {
    Packet* ack_packet = Packet::make_ack_packet(id, cycle);

    send(ack_packet);
}

bool Client::update(MPPacketHeader* header, void* packet_data) {
    //Logger::trace("Processing a received packet with type %d", header->type);

//    if (last_packet_time_get_ != 0 && header->timeSent < last_packet_time_get_) {
//        // Idk why packets would come out of sync.
//        return false;
//    }

    last_packet_time_get_ = header->timeSent;

    // If the packet was sent more than 5 seconds ago, we discard this packet and start to process next packet to
    //   increase performance if the operating system receive buffer is full.
//    int64_t difference = server_time_difference(header->timeSent);
//    if (difference > latency_ * latency_ && header->type != MP_PACKET_ACK) {
//        Logger::debug("Discarding stale packet sent %ld ms ago, server_time: %ld", difference, get_estimated_server_time());
//        return false;
//    }

    // If a packet requires acknowledgement, it has a value in the requires_ack field.
    // We register that we got the packet and only process the packet if it hasn't been
    // registered already.
    if (header->requires_ack > 0 && header->type != MP_PACKET_ACK) {
        // Always acknowledge first.
        send_ack(header->requires_ack, header->ack_cycle);

        // Check the table to see if we've processed this ack ID in this ack cycle.
        if (acked_[header->requires_ack] != header->ack_cycle) {
            Logger::trace("Acked %d, cycle: %d", header->requires_ack, header->ack_cycle);

            // We haven't seen this packet before, so we add the cycle value to the acked table.
            acked_[header->requires_ack] = header->ack_cycle;
        } else {
            return false;
        }
    }

    switch(header->type) {
        case MP_PACKET_ACK: {
            if (!header->requires_ack) {
                handshake_complete_ = true;
                Logger::info("Server handshake complete");
            } else {
                // Register packet as acknowledged to stop repeating the packet
                ack((char*)header, sizeof(MPPacketHeader) + header->size);
            }

            return false;
        }
        case MP_PACKET_IDKU: {
            reset();

            return false;
        }
    }

    return true;
}

void Client::receive() {
    received_ = 0;
    do {
        memset(&recv_buffer, 0, sizeof(recv_buffer));
        socklen_t paddrlen = sizeof(struct sockaddr_in);

        sockaddr _sockaddr;
        _sockaddr.sa_family = AF_INET;

        received_ = recv(sockfd_, &recv_buffer, sizeof(recv_buffer), MSG_DONTWAIT);

        // Anything above this value means an error message
        if (received_ > 0x80000000) {
            if (received_ != 0x80010223) {  // Just means there was no data available.
                Logger::error("recvfrom encountered error: 0x%08x", received_);
            }
            continue;
        }

        int index = 0;
        while (index < received_ && received_ - index >= sizeof(MPPacketHeader)) {
            MPPacketHeader *packet_header = (MPPacketHeader *) &recv_buffer[index];

            this->update(packet_header, &(recv_buffer[index + sizeof(MPPacketHeader)]));

            index += sizeof(MPPacketHeader) + packet_header->size;
        }
    } while (received_ > 0);
}

void Client::drop_receive() {
    do {
        memset(&recv_buffer, 0, sizeof(recv_buffer));
        socklen_t paddrlen = sizeof(struct sockaddr_in);

        sockaddr _sockaddr;
        _sockaddr.sa_family = AF_INET;

        received_ = recv(sockfd_, &recv_buffer, sizeof(recv_buffer), MSG_DONTWAIT);

        // Anything above this value means an error message
        if (received_ > 0x80000000) {
            if (received_ != 0x80010223) {  // Just means there was no data available.
                Logger::error("recvfrom encountered error: 0x%08x", received_);
            }
            continue;
        }
    } while (received_ > 0);
}

void Client::request_server_time() {
    Logger::debug("Requesting server time");

    Packet* time_request_packet = Packet::make_time_request_packet();
    make_self_referencing_rpc(time_request_packet, (AckCallback)&server_time_response_callback);
    send(time_request_packet);
}

int Client::server_time_response_callback(void* data, size_t len, void* extra) {
    MPPacketTimeResponse* timeResponsePacket = (MPPacketTimeResponse*)data;
    Client* self = (Client*)extra;

    self->calculate_offset(timeResponsePacket->client_send_time, timeResponsePacket->server_receive_time);

    return 0;
}

void Client::calculate_offset(int64_t client_send_time, int64_t server_receive_time) {
    int64_t now = get_time();
    latency_ = (now - client_send_time) / 2;
    estimated_server_time_ = (server_receive_time + latency_);

    has_first_time_sync = true;

    Logger::debug("Server time: %ld, latency_: %ld", estimated_server_time_, latency_);
}

int64_t Client::get_estimated_server_time() {
    return estimated_server_time_ + (get_time() - last_sync_time);
}

int64_t Client::server_time_difference(int64_t time) {
    if (!has_first_time_sync) {
        return 0;
    }

    return get_estimated_server_time() - time;
}

void Client::on_tick() {
    int64_t current_time = get_time();
    for(int i = 0; i < sizeof(unacked_)/sizeof(unacked_[0]); ++i) {
        MPUnacked* unacked = &(unacked_[i]);
        if (unacked->data != nullptr && !unacked->acked && current_time - unacked->send_time > 1000) {
            Logger::trace("Resending packet");
            // Resend packet
            send(unacked->data);
            unacked->send_time = current_time;
        }
    }

    if (sockfd_ > 0) {
        this->receive();

        // Sync time with the server every sync_interval milliseconds
        if (get_time() - last_sync_time > sync_interval) {
            Logger::trace("Time to get time from server");
            request_server_time();
            last_sync_time = get_time();
            Logger::trace("The time to get time from server is over.");
        }
    }
}