//
// Created by Vetle Hjelle on 19/12/2022.
//

#include "Client.h"

//#include <netex/net.h>

#include <lib/common.h>
#include <lib/clib.h>
#include <lib/logger.h>
#include <lib/memory.h>

Client::Client(char* ip, int port) {
    Logger::debug("New client for %s:%d", ip, port);

    memset(&sockaddr_, 0, sizeof(sockaddr_));

    sockaddr_.sin_addr.s_addr = inet_addr(ip);
    sockaddr_.sin_port = htons(port);
    sockaddr_.sin_family = AF_INET;
}

void Client::connect() {
    Logger::debug("Starting client");
    sockfd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sockfd_ <= 0) {
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

    connected_ = false;
    handshake_complete_ = false;
}

void Client::reset() {
    handshake_complete_ = false;
    connected_ = false;

    connect();
}

void Client::send(Packet* packet) {
    if (sockfd_) {
        sendto(sockfd_, packet->header, packet->len, 0, (struct sockaddr*)&sockaddr_, sizeof(sockaddr_));
    }

    if (!packet->retain_after_send) {
        delete packet;
    }
}

/**
 * Makes the provided packet require acknowledgement from the server. If a callback is provided, that callback
 *  will be called when the packet has been acknowledged.
 *
 * @param packet
 */
void Client::make_ack(Packet* packet, AckCallback callback) {
    // Handle overflows and first run
    do {
        if (ack_id_++ == 0) {
            ack_cycle_ += 1;

            Logger::debug("new ack cycle %d", ack_cycle_);
        }
    } while (!ack_id_);

    packet->header->requires_ack = ack_id_;
    packet->header->ack_cycle = ack_cycle_;

    MPUnacked* unacked = &unacked_[ack_id_];

    if (callback) {
        // Retain the packet after it's been sent so we can reference and delete it when calling the ack callback.
        packet->retain_after_send = true;

        unacked->ack_cb = callback;
        unacked->len = packet->len;

        unacked->data = packet;
    }

    // Register the packet as unacknowledged
    unsigned char prev_unacked_id = ack_id_-1;
    MPUnacked* prev_unacked = &unacked_[prev_unacked_id];

    while (true) {
        if (!prev_unacked->data && prev_unacked->next_unacked) {
            prev_unacked_id = prev_unacked->next_unacked;
            prev_unacked = &unacked_[prev_unacked_id];
        } else if (!prev_unacked->data && !prev_unacked->next_unacked) {
            prev_unacked_id = 0;
            break;
        } else {
            break;
        }
    }

    unacked->next_unacked = prev_unacked_id;
}

void Client::make_rpc(Packet *packet, AckCallback callback) {
    packet->header->flags = MP_PACKET_FLAG_RPC;
    make_ack(packet, callback);
}

void Client::ack(char* packet, size_t len) {
    Logger::trace("Yeaheay");
    MPPacketHeader* header = (MPPacketHeader*)packet;

    unsigned char ack_id = header->requires_ack;

    MPUnacked* unacked = &unacked_[ack_id];

    Logger::trace("Wowowo");

    if (unacked->data) {
        // If the ack packet has data, call the event handler for this packet
        if (header->size > 0 && unacked->ack_cb) {
            unacked->ack_cb(&packet[sizeof(MPPacketHeader)], header->size);
        }

        Logger::trace("Acked message %d with size %d", header->type, header->size);

        delete unacked->data;
    } else {
        Logger::trace("Acked packet type %d:%d without data", header->type, header->size);
    }

    unacked->data = 0;

    if (unacked->next_unacked) {
        MPUnacked* next_unacked = &unacked_[unacked->next_unacked];

        if (next_unacked->data == 0 && next_unacked->next_unacked != 0) {
            unacked->next_unacked = next_unacked->next_unacked;
        } else if (next_unacked->data == 0 && next_unacked->next_unacked == 0) {
            unacked->next_unacked = 0;
        }
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
    Logger::trace("Processing a received packet with type %d", header->type);

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

        if (received_ >= sizeof(MPPacketHeader) && received_ < 60000000) {
            MPPacketHeader* packet_header = (MPPacketHeader*)&recv_buffer;

            this->update(packet_header, &(recv_buffer[sizeof(MPPacketHeader)]));
        }
    } while (received_ > 0);
}

void Client::on_tick() {
    if (sockfd_ > 0) {
        this->receive();
    }
}
