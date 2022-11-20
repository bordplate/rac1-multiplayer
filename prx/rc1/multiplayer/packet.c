#include "packet.h"

MPPacketHeader mp_make_syn_packet(MPPacketHeader* packet_header) {
	packet_header->type = MP_PACKET_SYN;
	packet_header->size = 0;
}

// Makes and registers a packet that requires acknowledgement from the server.
void mp_make_ack(void* packet, int (*ack_cb)(void* data, size_t len)) {
    // Handle overflows and first run
    do {
        if (!mp_ack_id++) {
            mp_ack_cycle += 1;

            MULTI_LOG("new ack cycle %d\n", mp_ack_cycle);
        }
    } while (!mp_ack_id);

    MPPacketHeader* header = (MPPacketHeader*)packet;
    header->requires_ack = mp_ack_id;
    header->ack_cycle = mp_ack_cycle;

    mp_unacked_metadata* unacked = &mp_unacked[mp_ack_id];
    unacked->ack_cb = ack_cb;
    unacked->len = sizeof(MPPacketHeader) + header->size;

    MULTI_LOG("Before malloc\n");

    // FIXME: Hacky way to allocate memory dynamically. This is allocating in the vram space now.
    unacked->data = kalloc((void*)0x90d7c8, sizeof(MPPacketHeader)+header->size);
    memcpy(unacked->data, header, unacked->len);

    MULTI_LOG("After malloc 0x%08x\n", unacked->data);

    // Register the packet as unacknowledged
    unsigned char prev_unacked_id = mp_ack_id-1;
    mp_unacked_metadata* prev_unacked = &mp_unacked[prev_unacked_id];

    while (true) {
        if (!prev_unacked->data && prev_unacked->next_unacked) {
            prev_unacked_id = prev_unacked->next_unacked;
            prev_unacked = &mp_unacked[prev_unacked_id];
        } else if (!prev_unacked->data && !prev_unacked->next_unacked) {
            prev_unacked_id = 0;
            break;
        } else {
            break;
        }
    }

    unacked->next_unacked = prev_unacked_id;
}

void mp_make_rpc(void* packet, int (*ack_cb)(void* data, size_t len)) {
    ((MPPacketHeader*)packet)->flags = MP_PACKET_FLAG_RPC;
    mp_make_ack(packet, ack_cb);
}

void mp_send_ack(char id, char cycle) {
    MPPacketHeader ack_packet = {MP_PACKET_ACK, 0};
    ack_packet.size = 0;
    ack_packet.requires_ack = id;
    ack_packet.ack_cycle = cycle;
    mp_send(&ack_packet, sizeof(ack_packet));
}

// Send first handshake packet to the server to identify and connect to server.
// Since this is UDP and transport isn't guaranteed, handshaking just ensures that
//		there's actually a server talking our language on the other end before we
//		start sending and receiving random junk.
void mp_send_handshake() {
    MPPacketHeader syn_packet = { 0, 0, 0, 0, 0 };
    syn_packet.type = MP_PACKET_SYN;
    syn_packet.size = 0;

    MULTI_TRACE("Sent SYN packet with type: %d. Size: %d\n", syn_packet.type, sizeof(syn_packet));
    mp_send(&syn_packet, sizeof(syn_packet));
}

void mp_rpc_spawn_moby(int (*ack_cb)(void* data, size_t len)) {
    char buffer[sizeof(MPPacketHeader)];
    memset(&buffer, 0, sizeof(buffer));
    MPPacketHeader* header = (MPPacketHeader*)&buffer;

    header->type = MP_PACKET_MOBY_CREATE;
    header->size = 0;

    mp_make_rpc(&buffer, ack_cb);
    header->flags = MP_PACKET_FLAG_RPC;

    MULTI_LOG("Sending RPC packet %d, size: %d, payload size: %d\n", header->type, sizeof(MPPacketHeader), header->size);

    mp_send(header, sizeof(MPPacketHeader));
}