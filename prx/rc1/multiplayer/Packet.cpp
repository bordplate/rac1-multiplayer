//
// Created by Vetle Hjelle on 29/12/2022.
//

#include "Packet.h"

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

Packet* Packet::make_collision(u16 uuid, u16 collided_with, Vec4* position, bool aggressive) {
    Packet* packet = new Packet(sizeof(MPPacketMobyCollision));
    MPPacketHeader* header = (MPPacketHeader*)packet->header;
    header->size = sizeof(MPPacketMobyCollision);
    header->type = MP_PACKET_MOBY_COLLISION;

    MPPacketMobyCollision* body = (MPPacketMobyCollision*)packet->body;
    body->flags = aggressive == true ? MP_COLLISION_FLAG_AGGRESSIVE : 0;
    body->uuid = uuid;
    body->collided_with = collided_with;
    body->x = position->x;
    body->y = position->y;
    body->z = position->z;

    // Aggressive packets (weapon attacks, etc.) must be acked be the server.
    // We want to be sure we don't drop any attacks.
    // Other collision events we just blindly spam and should be fine
    if (aggressive) {
        Client* client = Game::shared().client();
        if (client) {
            client->make_ack(packet, nullptr);
        }
    }

    return packet;
}

Packet* Packet::make_connect_packet(const String& nickname, int32_t userid) {
    Packet* packet = new Packet(sizeof(MPPacketConnect) + nickname.length());
    MPPacketHeader* header = (MPPacketHeader*)packet->header;
    header->size = sizeof(MPPacketConnect) + nickname.length();
    header->type = MP_PACKET_CONNECT;

    MPPacketConnect *body = (MPPacketConnect*)packet->body;
    body->userid = userid;
    body->version = 1;
    body->nick_length = nickname.length();

    memcpy((char*)packet->body + sizeof(MPPacketConnect), nickname.c_str(), nickname.length());
    Logger::debug("Connecting with username %s with length %d", &packet->body + sizeof(MPPacketConnect), nickname.length());

    return packet;
}

Packet* Packet::make_disconnect_packet() {
    Packet* packet = new Packet(0);
    packet->header->type = MP_PACKET_DISCONNECT;

    return packet;
}

Packet* Packet::make_time_request_packet() {
    Packet* packet = new Packet(0);
    packet->header->type = MP_PACKET_TIME_SYNC;

    return packet;
}

Packet* Packet::make_player_respawned_packet() {
    Packet* packet = new Packet(0);
    packet->header->type = MP_PACKET_PLAYER_RESPAWNED;

    return packet;
}

Packet* Packet::make_game_state_changed_packet(GameState state) {
    Packet* packet = new Packet(sizeof(MPPacketSetState));
    packet->header->type = MP_PACKET_SET_STATE;
    packet->header->size = sizeof(MPPacketSetState);

    MPPacketSetState* body = (MPPacketSetState*)packet->body;
    body->state_type = MP_STATE_TYPE_GAME;
    body->value = state;
    body->offset = 0;

    return packet;
}

Packet* Packet::make_collected_gold_bolt_packet(int bolt_number) {
    Packet* packet = new Packet(sizeof(MPPacketSetState));
    packet->header->type = MP_PACKET_SET_STATE;
    packet->header->size = sizeof(MPPacketSetState);

    MPPacketSetState* body = (MPPacketSetState*)packet->body;
    body->state_type = MP_STATE_TYPE_COLLECTED_GOLD_BOLT;
    body->value = bolt_number;
    body->offset = current_planet;

    return packet;
}

Packet* Packet::make_unlock_item_packet(int* item_id) {
    Packet* packet = new Packet(sizeof(MPPacketSetState));
    packet->header->type = MP_PACKET_SET_STATE;
    packet->header->size = sizeof(MPPacketSetState);

    MPPacketSetState* body = (MPPacketSetState*)packet->body;
    body->state_type = MP_STATE_TYPE_UNLOCK_ITEM;
    body->value = item_id;
    body->offset = 0;

    return packet;
}