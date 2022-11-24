#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <rc1/rc1.h>
#include <rc1/moby.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "packet.h"

#define MP_MAX_MOBYS 1024

int mp_sock;
struct sockaddr_in mp_sockaddr;

// Networking metadata
typedef struct {
    void* data;
    int (*ack_cb)(void* data, size_t len);
    size_t len;
    unsigned char next_unacked;
} mp_unacked_metadata;

unsigned char mp_acked[256];
mp_unacked_metadata mp_unacked[256];
unsigned char mp_ack_id;
unsigned char mp_ack_cycle;


Moby* mp_mobys[MP_MAX_MOBYS];
u32 mp_current_weapon_uuid;

void mp_start();
void mp_tick();
void mp_send(void* buffer, size_t len);

#endif