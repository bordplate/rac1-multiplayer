#ifndef PACKET_H
#define PACKET_H

#include <rc1/rc1.h>

/*
+-------------------+
|  MPPacketHeader   |
| - type            |
| - flags           |
| - size            |
| - requires_ack    |
| - ack_cycle       |
+-------------------+
|       Data        |
+-------------------+
The MPPacketHeader struct is the first part of every packet and contains metadata about the packet. It includes the type
 field, which specifies the type of packet, and the size field, which specifies the size of the data that follows the
 header. The flags field is used to set additional options for the packet, and the requires_ack and ack_cycle fields
 are used to implement reliable transfer of packets when necessary.

The data portion of the packet depends on the type of packet specified in the header. For example, if the type field in
 the header is MP_PACKET_MOBY_UPDATE, then the data portion of the packet will be an MPPacketMobyUpdate struct. If the
 type field is MP_PACKET_SET_STATE, then the data portion will be an MPPacketSetState struct.

Here is an example of how a packet with type set to MP_PACKET_MOBY_UPDATE and no flags would look:
+--------------------------------------+
|  MPPacketHeader                      |
| - type: MP_PACKET_MOBY_UPDATE        |
| - flags: 0                           |
| - size: sizeof(MPPacketMobyUpdate)   |
| - requires_ack: 1                    |
| - ack_cycle: 3                       |
+--------------------------------------+
|  MPPacketMobyUpdate                  |
| - uuid                               |
| - parent                             |
| - team                               |
| - reserved                           |
| - flags                              |
| - o_class                            |
| - level                              |
| - animation_id                       |
| - animation_duration                 |
| - x                                  |
| - y                                  |
| - z                                  |
| - rotation                           |
+--------------------------------------+
 */

#include <lib/types.h>

#define MP_PACKET_CONNECT            1
#define MP_PACKET_SYN                2
#define MP_PACKET_ACK                3
#define MP_PACKET_MOBY_UPDATE        4
#define MP_PACKET_IDKU                5
#define MP_PACKET_MOBY_CREATE       6
#define MP_PACKET_DISCONNECT        7
#define MP_PACKET_MOBY_DELETE       8
#define MP_PACKET_MOBY_COLLISION    9
#define MP_PACKET_SET_STATE         10
#define MP_PACKET_SET_HUD_TEXT      11
#define MP_PACKET_QUERY_GAME_SERVERS 12
#define MP_PACKET_CONTROLLER_INPUT 13

#define MP_PACKET_FLAG_RPC      0x1

#define MP_COLLISION_FLAG_AGGRESSIVE 1

#define MP_MOBY_FLAG_ACTIVE         0x1
#define MP_MOBY_FLAG_NO_COLLISION   0x2
#define MP_MOBY_FLAG_ORIG_UDPATE_FUNC 0x4

#define MP_CONTROLLER_FLAGS_PRESSED 1
#define MP_CONTROLLER_FLAGS_HELD 2

struct MPPacketHeader {
    u16 type;
    u16 flags;
    u32 size;
    unsigned char requires_ack;
    unsigned char ack_cycle;
};

struct Packet {
    MPPacketHeader* header;
    void* body;
    size_t len;
    bool retain_after_send;

    Packet(size_t body_len);
    ~Packet();

    static Packet* make_handshake_packet();
    static Packet* make_ack_packet(unsigned char id, unsigned char cycle);
    static Packet* make_query_directory_packet(int directory_id);
    static Packet* make_controller_input(CONTROLLER_INPUT inputs, u16 flags);
};

struct MPPacketMobyUpdate {
    u16 uuid;
    u16 parent;
    u8 team;
    u8 reserved;
    u16 flags;
    u16 o_class;
    u16 level;
    u32 animation_id;
    u32 animation_duration;
    float x;
    float y;
    float z;
    float rotation;
};

typedef struct {
    u16 uuid;

} MPPacketMobyUpdateExtended;

typedef struct {
    u32 uuid;
} MPPacketMobyCreate;

typedef struct {
    u32 flags;
    u16 uuid;
    u16 collided_with;
    float x;
    float y;
    float z;
} MPPacketMobyCollision;

#define MP_STATE_TYPE_DAMAGE    1
#define MP_STATE_TYPE_PLAYER    2
#define MP_STATE_TYPE_POSITION  3
#define MP_STATE_TYPE_PLANET    4
#define MP_STATE_TYPE_GAME      5
#define MP_STATE_TYPE_ITEM      6

typedef struct {
    u32 state_type;
    u32 offset;
    u32 value;
} MPPacketSetState;

typedef struct {
    u32 state_type;
    u32 offset;
    float value;
} MPPacketSetStateFloat;

// This whole UI rendering shit is hacky and should be improved.
typedef struct {
    u16 id;
    u16 x;
    u16 y;
    u16 flags;
    Color color;
    u16 box_height;
    u16 box_width;
    float size;
    char text[50];
} MPPacketSetHUDText;

// Packet used to query a directory for game servers
typedef struct {
    u32 directory_id;
} MPPacketQueryGameServers;

typedef struct {
    u32 ip;
    u16 port;
    u16 max_players;
    u16 player_count;
    u16 name_length;
} MPPacketQueryResponseServer;

typedef struct {
    u16 input;
    u16 flags;
} MPPacketControllerInput;

#ifndef __cplusplus

#include "multiplayer.h"

MPPacketHeader mp_make_syn_packet();
void mp_send_ack(unsigned char id, unsigned char cycle);
void mp_send_handshake();
void mp_rpc_spawn_moby(int (*ack_cb)(void* data, size_t len));
void mp_send_collision(u16 uuid, u16 collided_with, Vec4* position, bool aggressive);
void mp_rpc_query_game_servers(u32 directory_id, int (*ack_cb)(void* data, size_t len));
void mp_send_controller_input(CONTROLLER_INPUT inputs, u16 flags);
void mp_send_state(u32 type, u32 offset, u32 value);

#endif  // __cplusplus

#endif