//
// Created by Vetle Hjelle on 29/12/2022.
//

#include "packet.h"

#include <lib/clib.h>
#include <lib/memory.h>
#include <lib/logger.h>

Packet::Packet(size_t body_len) {
    this->header = (MPPacketHeader*)allocate_memory(sizeof(MPPacketHeader) + body_len);
    memset(this->header, 0, sizeof(MPPacketHeader) + body_len);

    this->header->size = body_len;

    this->len = sizeof(MPPacketHeader) + body_len;
    this->retain_after_send = false;

    this->body = &((char*)this->header)[sizeof(MPPacketHeader)];
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

//Packet* Packet::make_moby_update_packet(unsigned short uuid) {
//    Packet* packet = new Packet(sizeof(MPPacketMobyUpdate));
//
//    packet->header->type = MP_PACKET_MOBY_UPDATE;
//
//    MPPacketMobyUpdate* payload = (MPPacketMobyUpdate*)packet->body;
//    payload->uuid = 0;  // Player moby is always uuid 0
//    payload->flags |= ratchet_moby != 0 ? 1 : 0;
//    payload->o_class = 0;
//    payload->level = (u16)current_planet;
//    payload->animation_id = ratchet_moby != 0 ? ratchet_moby->animationID : 0;
//    payload->animation_duration = player_animation_duration;
//    payload->x = player_pos.x;
//    payload->y = player_pos.y;
//    payload->z = player_pos.z;
//    payload->rotation = player_rot.z;
//
//    return packet;
//}