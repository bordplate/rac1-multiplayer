//
// Created by Vetle Hjelle on 29/12/2022.
//

#include "packet.h"

#include <rc1/rc1.h>
#include <rc1/Game.h>

#include "Client.h"

Packet::Packet(size_t body_len) {
    this->header = (MPPacketHeader*)allocate_memory(sizeof(MPPacketHeader) + body_len);
    memset(this->header, 0, sizeof(MPPacketHeader) + body_len);

    this->header->size = body_len;

    this->len = sizeof(MPPacketHeader) + body_len;
    this->retain_after_send = false;

    //this->body = &((char*)this->header)[sizeof(MPPacketHeader)];
    this->body = ((char*)this->header) + sizeof(MPPacketHeader);
}

Packet::~Packet() {
    free_memory(this->header);
}

Packet* Packet::make_handshake_packet() {
    Packet* packet = new Packet(0);
    packet->header->type = MP_PACKET_SYN;

    return packet;
}

Packet* Packet::make_ack_packet(unsigned char id, unsigned char cycle) {
    Packet* packet = new Packet(0);
    packet->header->type = MP_PACKET_ACK;
    packet->header->requires_ack = id;
    packet->header->ack_cycle = cycle;

    return packet;
}

/**
 * Sends a request to a game server directory to query for available game servers.
 *
 * @param directory_id ID of the game server directory to query.
 */
Packet* Packet::make_query_directory_packet(int directory_id) {
    Logger::debug("Making directory query packet");

    Packet* packet = new Packet(sizeof(MPPacketQueryGameServers));

    // Set the packet header type
    packet->header->type = MP_PACKET_QUERY_GAME_SERVERS;

    // Create the packet payload and set the directory ID
    MPPacketQueryGameServers* payload = (MPPacketQueryGameServers*)packet->body;
    payload->directory_id = directory_id;

    // Send the packet over the network
    return packet;
}

/**
 * Makes a packet containing controller input data.
 *
 * @param inputs The controller input data to be sent.
 * @param flags The flags for the input data.
 */
Packet* Packet::make_controller_input(CONTROLLER_INPUT inputs, u16 flags) {
    Packet* packet = new Packet(sizeof(MPPacketControllerInput));

    // Get a pointer to the header part of the packet
    MPPacketHeader* header = (MPPacketHeader*)packet->header;
    // Set the type of the packet
    header->type = MP_PACKET_CONTROLLER_INPUT;

    // Get a pointer to the body of the packet
    MPPacketControllerInput* body = (MPPacketControllerInput*)packet->body;
    // Set the controller input data in the packet
    body->input = inputs;
    // Set the flags in the packet
    body->flags = flags;

    // If the input is a press (not a hold), set the packet to require an acknowledgement
    if (flags & MP_CONTROLLER_FLAGS_PRESSED) {
        //mp_make_ack(&packet, 0);
        Client* client = Game::shared().client();
        if (client) {
            client->make_ack(packet, nullptr);
        }
    }

    return packet;
}