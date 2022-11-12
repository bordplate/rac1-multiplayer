#include "packet.h"

MPPacketHeader mp_make_syn_packet(MPPacketHeader *packet_header) {
	packet_header->type = MP_PACKET_SYN;
	packet_header->size = 0;
}