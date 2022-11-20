#ifndef PACKET_H
#define PACKET_H

#include <lib/types.h>
#include "multiplayer.h"

#define MP_PACKET_CONNECT 		1
#define MP_PACKET_SYN 			2
#define MP_PACKET_ACK 			3
#define MP_PACKET_MOBY_UPDATE 	4
#define MP_PACKET_IDKU			5
#define MP_PACKET_MOBY_CREATE   6

#define MP_PACKET_FLAG_RPC      0x1

typedef struct {
	u16 type;
    u16 flags;
	u32 size;
    unsigned char requires_ack;
    unsigned char ack_cycle;
} MPPacketHeader;

typedef struct {
	u16 uuid;
    u16 parent;
	u32 enabled;
	u16 o_class;
    u16 level;
	u32 animation_id;
	float x;
	float y;
	float z;
	float rotation;
} MPPacketMobyUpdate;

typedef struct {
    u32 uuid;
} MPPacketMobyCreate;

MPPacketHeader mp_make_syn_packet();
void mp_send_ack(char id, char cycle);
void mp_send_handshake();
void mp_rpc_spawn_moby(int (*ack_cb)(void* data, size_t len));

#endif