#ifndef PACKET_H
#define PACKET_H

#include <lib/types.h>

#define MP_PACKET_CONNECT 		1
#define MP_PACKET_SYN 			2
#define MP_PACKET_ACK 			3
#define MP_PACKET_MOBY_UPDATE 	4
#define MP_PACKET_IDKU			5

typedef struct {
	u32 type;
	u32 size;
} MPPacketHeader;

typedef struct {
	u32 uuid;
	u32 enabled;
	u32 o_class;
	u32 animation_id;
	float x;
	float y;
	float z;
	float rotation;
} MPPacketMobyUpdate;

MPPacketHeader mp_make_syn_packet();

#endif