//
// Created by bordplate on 12/27/2022.
//

#include "DirectoryClient.h"

#include <lib/logger.h>


bool DirectoryClient::update(MPPacketHeader* header, void* packet_data) {
    // If Client::update returns false, we don't continue processing here.
    // It could return false if e.g. the received packet is a packet we've previously
    //  processed.
    if (!Client::update(header, packet_data)) {
        return false;
    }

    return true;
}

void DirectoryClient::query_directory(int directory_id, AckCallback callback) {
    Logger::trace("Making and sending directory query packet for directory ID %d", directory_id);

    Packet* query_packet = Packet::make_query_directory_packet(directory_id);
    this->make_rpc(query_packet, callback);

    this->send(query_packet);
}

void DirectoryClient::on_tick() {
    Client::on_tick();
}