//
// Created by Vetle Hjelle on 29/12/2022.
//

#include "Packet.h"

#include "rc1/rc1.h"
#include "rc1/Game.h"

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
Packet* Packet::make_query_directory_packet(int page) {
    Logger::debug("Making directory query packet");

    Packet* packet = new Packet(sizeof(MPPacketQueryGameServers));

    packet->header->type = MP_PACKET_QUERY_GAME_SERVERS;

    MPPacketQueryGameServers* payload = (MPPacketQueryGameServers*)packet->body;
    payload->version = MP_API_VERSION;
    payload->page = page;

    return packet;
}

Packet* Packet::make_moby_create_packet(u16 uuid, u16 parent_uuid, u8 spawn_id, u16 flags, u16 o_class, u16 mode_bits, u8 position_bone, u8 transform_bone) {
    Packet* packet = new Packet(sizeof(MPPacketMobyCreate));
    packet->header->type = MP_PACKET_MOBY_CREATE;

    MPPacketMobyCreate* body = (MPPacketMobyCreate*)packet->body;
    body->uuid = uuid;
    body->parent_uuid = parent_uuid;
    body->spawn_id = spawn_id;
    body->flags = flags;
    body->o_class = o_class;
    body->mode_bits = mode_bits;
    body->position_bone = position_bone;
    body->transform_bone = transform_bone;

    return packet;
}

Packet* Packet::make_moby_delete_packet(u16 uuid) {
    Packet* packet = new Packet(sizeof(MPPacketMobyDelete));
    packet->header->type = MP_PACKET_MOBY_DELETE;

    MPPacketMobyDelete* body = (MPPacketMobyDelete*)packet->body;
    body->value = uuid;

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

Packet* Packet::make_damage_packet(u16 uuid, u16 collided_with_uuid, u32 flags, u16 damaged_o_class, u16 source_o_class, Vec4* position, float damage) {
    Packet* packet = new Packet(sizeof(MPPacketDamage));
    packet->header->type = MP_PACKET_MOBY_DAMAGE;

    MPPacketDamage* body = (MPPacketDamage*)packet->body;
    body->uuid = uuid;
    body->collided_with_uuid = collided_with_uuid;
    body->flags = flags;
    body->damaged_o_class = damaged_o_class;
    body->source_o_class = source_o_class;
    body->x = position->x;
    body->y = position->y;
    body->z = position->z;
    body->damage = damage;

    return packet;
}

Packet* Packet::make_connect_packet(const String& nickname, int32_t userid) {
    Packet* packet = new Packet(sizeof(MPPacketConnect) + nickname.length());
    MPPacketHeader* header = (MPPacketHeader*)packet->header;
    header->size = sizeof(MPPacketConnect) + nickname.length();
    header->type = MP_PACKET_CONNECT;

    MPPacketConnect *body = (MPPacketConnect*)packet->body;
    body->userid = userid;
    body->version = MP_API_VERSION;
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

Packet* Packet::make_player_respawned_packet(u8 spawn_id) {
    Packet* packet = new Packet(sizeof(MPPacketSpawned));
    packet->header->type = MP_PACKET_PLAYER_RESPAWNED;

    MPPacketSpawned* body = (MPPacketSpawned*)packet->body;
    body->spawn_id = spawn_id;

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

Packet* Packet::make_unlock_item_packet(int item_id, bool equip) {
    Packet* packet = new Packet(sizeof(MPPacketSetState));
    packet->header->type = MP_PACKET_SET_STATE;
    packet->header->size = sizeof(MPPacketSetState);

    uint16_t flags = equip ? 1 : 0;

    MPPacketSetState* body = (MPPacketSetState*)packet->body;
    body->state_type = MP_STATE_TYPE_UNLOCK_ITEM;
    body->value = (flags << 16) | item_id;
    body->offset = 0;

    return packet;
}

Packet* Packet::make_unlock_level_packet(int level) {
    Packet* packet = new Packet(sizeof(MPPacketSetState));
    packet->header->type = MP_PACKET_SET_STATE;
    packet->header->size = sizeof(MPPacketSetState);

    MPPacketSetState* body = (MPPacketSetState*)packet->body;
    body->state_type = MP_STATE_TYPE_UNLOCK_LEVEL;
    body->value = level;
    body->offset = 0;

    return packet;
}

Packet* Packet::make_unlock_skillpoint_packet(u8 skillpoint) {
    Packet* packet = new Packet(sizeof(MPPacketSetState));
    packet->header->type = MP_PACKET_SET_STATE;
    packet->header->size = sizeof(MPPacketSetState);

    MPPacketSetState* body = (MPPacketSetState*)packet->body;
    body->state_type = MP_STATE_TYPE_UNLOCK_SKILLPOINT;
    body->value = skillpoint;
    body->offset = 0;

    return packet;
}

Packet* Packet::make_monitored_value_changed_packet(u16 uid, u32 offset, u32 size, u8 flags, u32 old_value, u32 new_value) {
    Packet* packet = new Packet(sizeof(MPPacketMonitoredValueChanged));
    packet->header->type = MP_PACKET_MONITORED_VALUE_CHANGED;
    packet->header->size = sizeof(MPPacketMonitoredValueChanged);

    MPPacketMonitoredValueChanged* body = (MPPacketMonitoredValueChanged*)packet->body;
    body->uid = uid;
    body->offset = offset;
    body->size = size;
    body->flags = flags;
    body->old_value = old_value;
    body->new_value = new_value;

    return packet;
}


Packet* Packet::make_level_flag_changed_packet(u16 type, u8 level, u8 size, u16 index, u32 value) {
    Packet* packet = new Packet(sizeof(MPPacketLevelFlagChanged));
    packet->header->type = MP_PACKET_LEVEL_FLAG_CHANGED;
    packet->header->size = sizeof(MPPacketLevelFlagChanged);

    MPPacketLevelFlagChanged* body = (MPPacketLevelFlagChanged*)packet->body;
    body->type = type;
    body->level = level;
    body->size = size;
    body->index = index;
    body->value = value;

    return packet;
}

Packet* Packet::make_address_changed_packet(u32 address, u16 size, u32 old_value, u32 new_value) {
    Packet* packet = new Packet(sizeof(MPPacketAddressChanged));
    packet->header->type = MP_PACKET_ADDRESS_CHANGED;
    packet->header->size = sizeof(MPPacketAddressChanged);

    MPPacketAddressChanged* body = (MPPacketAddressChanged*)packet->body;
    body->address = address;
    body->size = size;
    body->old_value = old_value;
    body->new_value = new_value;

    return packet;
}

Packet* Packet::make_bolt_count_changed_packet(s32 bolt_diff, u32 current_bolts) {
    Packet* packet = new Packet(sizeof(MPPacketSetState));
    packet->header->type = MP_PACKET_SET_STATE;
    packet->header->size = sizeof(MPPacketSetState);

    MPPacketSetState* body = (MPPacketSetState*)packet->body;
    body->state_type = MP_STATE_TYPE_GIVE_BOLTS;
    body->value = bolt_diff;
    body->offset = current_bolts;

    return packet;
}

Packet* Packet::make_moby_create_failure_packet(u16 uuid, u8 reason) {
    Packet* packet = new Packet(sizeof(MPPacketMobyCreateFailure));
    packet->header->type = MP_PACKET_MOBY_CREATE_FAILURE;
    packet->header->size = sizeof(MPPacketMobyCreateFailure);

    MPPacketMobyCreateFailure* body = (MPPacketMobyCreateFailure*)packet->body;
    body->uuid = uuid;
    body->reason = reason;

    return packet;
}