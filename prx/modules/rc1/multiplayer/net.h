#ifndef NET_H
#define NET_H

#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_DGRAM_P2P 6
#define SOCK_STREAM_P2P 10

#define AF_INET 2

#define IPPROTO_IP 0
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

#define INADDR_ANY 0
#define MSG_DONTWAIT 0x80

#define IP(a, b, c, d) (d | c << 8 | b << 16 | a << 24)

struct sockaddr {
    unsigned char sa_len;
    unsigned char sa_family;
    char sa_data[14];
};

struct sockaddr_in {
    unsigned char sin_len;
    unsigned char sin_family;
    unsigned short sin_port;
    unsigned int sin_addr;
    unsigned long long sin_zero;
};

#endif // NET_H