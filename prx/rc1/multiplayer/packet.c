#include "packet.h"

MPPacketHeader mp_make_syn_packet(MPPacketHeader* packet_header) {
	packet_header->type = MP_PACKET_SYN;
	packet_header->size = 0;
}

/**
 * Makes and registers a packet that requires acknowledgement from the server.
 *
 * @param packet pointer to the packet to be sent
 * @param ack_cb callback function to be called when acknowledgement is received from the server
 */
void mp_make_ack(void* packet, int (*ack_cb)(void* data, size_t len)) {
    // Handle overflows and first run
    do {
        if (mp_ack_id++ == 0) {
            mp_ack_cycle += 1;

            MULTI_LOG("new ack cycle %d\n", mp_ack_cycle);
        }
    } while (!mp_ack_id);

    MPPacketHeader* header = (MPPacketHeader*)packet;
    header->requires_ack = mp_ack_id;
    header->ack_cycle = mp_ack_cycle;

    mp_unacked_metadata* unacked = &mp_unacked[mp_ack_id];

    if (ack_cb) {
        unacked->ack_cb = ack_cb;
        unacked->len = sizeof(MPPacketHeader) + header->size;

        MULTI_TRACE("Before malloc\n");

        // FIXME: Hacky way to allocate memory dynamically. This is allocating in the vram space now.
        unacked->data = kalloc((void *) 0x90d7c8, sizeof(MPPacketHeader) + header->size);
        MULTI_TRACE("After malloc 0x%08x\n", unacked->data);

        memcpy(unacked->data, header, unacked->len);
    }

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

void mp_send_ack(unsigned char id, unsigned char cycle) {
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

/**
 * Send an RPC request to create a new game object (moby) in the multiplayer game.
 * In practice you request the server register that you have a moby that you want to update.
 * So when you call this function, you should get a response from the server with a UUID, and then you can just
 *  start giving the server updates about that moby and it should sync to the other games as well.
 *
 * @param ack_cb Callback function to be called when an ACK packet is received in response to this RPC request. The
 *  callback should accept a void* for the data and a size_t for the length of the data.
 */
void mp_rpc_spawn_moby(int (*ack_cb)(void* data, size_t len)) {
    // Create a buffer to hold the packet header
    char buffer[sizeof(MPPacketHeader)];

    // Clear the buffer
    memset(&buffer, 0, sizeof(buffer));

    // Cast the buffer to a pointer to an MPPacketHeader struct
    MPPacketHeader* header = (MPPacketHeader*)&buffer;

    // Set the packet type and size
    header->type = MP_PACKET_MOBY_CREATE;
    header->size = 0;

    // Set the RPC flag and register the ACK callback
    mp_make_rpc(&buffer, ack_cb);
    header->flags = MP_PACKET_FLAG_RPC;

    MULTI_TRACE("Sending RPC packet %d, size: %d, payload size: %d\n", header->type, sizeof(MPPacketHeader), header->size);

    // Send the packet over the network
    mp_send(header, sizeof(MPPacketHeader));
}

/**
 * Sends a collision packet to the server, indicating that the game object with the given UUID
 * has collided with another game object.
 *
 * @param uuid The UUID of the game object that has collided.
 * @param collided_with The UUID of the game object that was collided with.
 * @param position The position of the game object at the time of the collision.
 * @param aggressive Whether the collision was aggressive (e.g. a weapon attack).
 */
void mp_send_collision(u16 uuid, u16 collided_with, Vec4* position, bool aggressive) {
    char packet[sizeof(MPPacketHeader) + sizeof(MPPacketMobyCollision)];
    memset(&packet, 0, sizeof(packet));
    MPPacketHeader* header = (MPPacketHeader*)&packet;
    header->size = sizeof(MPPacketMobyCollision);
    header->type = MP_PACKET_MOBY_COLLISION;

    MPPacketMobyCollision* body = (MPPacketMobyCollision*)&packet[sizeof(MPPacketHeader)];
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
        mp_make_ack(&packet, 0);
    }

    mp_send(&packet, sizeof(packet));
}



/**
 * Sends a packet containing controller input data to a multiplayer game.
 *
 * @param inputs The controller input data to be sent.
 * @param flags The flags for the input data.
 */
void mp_send_controller_input(CONTROLLER_INPUT inputs, u16 flags) {
    // Create a buffer for the packet
    char packet[sizeof(MPPacketHeader) + sizeof(MPPacketControllerInput)];
    // Clear the buffer
    memset(&packet, 0, sizeof(packet));

    // Get a pointer to the header part of the packet
    MPPacketHeader* header = (MPPacketHeader*)&packet;
    // Set the size of the packet
    header->size = sizeof(MPPacketControllerInput);
    // Set the type of the packet
    header->type = MP_PACKET_CONTROLLER_INPUT;

    // Get a pointer to the body of the packet
    MPPacketControllerInput* body = (MPPacketControllerInput*)&packet[sizeof(MPPacketHeader)];
    // Set the controller input data in the packet
    body->input = inputs;
    // Set the flags in the packet
    body->flags = flags;

    // If the input is a press (not a hold), set the packet to require an acknowledgement
    if (flags & MP_CONTROLLER_FLAGS_PRESSED) {
        mp_make_ack(&packet, 0);
    }

    // Send the packet
    mp_send(&packet, sizeof(packet));
}

/**
 * mp_send_state - send a packet to set a state variable in a multiplayer game
 *
 * @param type the type of the state variable to set
 * @param offset the offset of the state variable in the game state structure
 * @param value the value to set the state variable to
 */
void mp_send_state(u32 type, u32 offset, u32 value) {
    // Allocate memory for the packet and zero it out
    char packet[sizeof(MPPacketHeader) + sizeof(MPPacketMobyCollision)];
    memset(&packet, 0, sizeof(packet));

    // Set the size and type fields in the packet header
    MPPacketHeader* header = (MPPacketHeader*)&packet;
    header->size = sizeof(MPPacketMobyCollision);
    header->type = MP_PACKET_SET_STATE;

    // Set the state type, offset, and value in the packet body
    MPPacketSetState* body = (MPPacketSetState*)&packet[sizeof(MPPacketHeader)];
    body->state_type = type;
    body->offset = offset;
    body->value = value;

    // Set the packet header fields for reliable packet transfer (ACKs)
    mp_make_ack(&packet, 0);

    // Send the packet
    mp_send(&packet, sizeof(packet));
}