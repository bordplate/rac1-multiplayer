#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <rc1/rc1.h>
#include <rc1/moby.h>

#include "packet.h"

int mp_sock;
struct sockaddr_in mp_sockaddr;

Moby* mp_mobys[1024];

void mp_start();
void mp_tick();

#endif