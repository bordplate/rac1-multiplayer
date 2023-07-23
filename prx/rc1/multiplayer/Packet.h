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
#define MP_PACKET_IDKU               5
#define MP_PACKET_MOBY_CREATE        6
#define MP_PACKET_DISCONNECT         7
#define MP_PACKET_MOBY_DELETE        8
#define MP_PACKET_MOBY_COLLISION     9
#define MP_PACKET_SET_STATE          10
#define MP_PACKET_SET_HUD_TEXT       11
#define MP_PACKET_QUERY_GAME_SERVERS 12
#define MP_PACKET_CONTROLLER_INPUT   13
#define MP_PACKET_TIME_SYNC          14
#define MP_PACKET_PLAYER_RESPAWNED   15
#define MP_PACKET_REGISTER_SERVER    16
#define MP_PACKET_TOAST_MESSAGE      17

#define MP_PACKET_FLAG_RPC           0x1

#define MP_COLLISION_FLAG_AGGRESSIVE 1

#define MP_MOBY_FLAG_ACTIVE           0x1
#define MP_MOBY_FLAG_NO_COLLISION     0x2
#define MP_MOBY_FLAG_ORIG_UDPATE_FUNC 0x4

#define MP_CONTROLLER_FLAGS_PRESSED 1
#define MP_CONTROLLER_FLAGS_HELD 2

#pragma pack(push, 1)

struct MPPacketHeader {
    u16 type;
    u16 flags;
    u32 size;
    int64_t timeSent;
    unsigned char requires_ack;
    unsigned char ack_cycle;
};

struct MPPacketConnect {
    int32_t userid;
    char passcode[8];
    u16 nick_length;
};

#define MP_CONNECT_ERROR_UNKNOWN 0
#define MP_CONNECT_SUCCESS 1
#define MP_CONNECT_ERROR_USER_ALREADY_CONNECTED 2
#define MP_CONNECT_ERROR_NOT_ALLOWED 3

struct MPPacketConnectCallback {
    int32_t status;
};

struct MPPacketMobyUpdate {
    u16 uuid;
    u16 parent;
    u16 flags;
    u16 o_class;
    u16 level;
    u32 animation_id;
    u32 animation_duration;
    float x;
    float y;
    float z;
    float rotX;
    float rotY;
    float rotZ;
    float scale;
    char alpha;
    u8 padding;
    u16 modeBits;
    u16 state;
};

typedef struct {
    u16 uuid;
} MPPacketMobyUpdateExtended;

typedef struct {
    u32 uuid;
} MPPacketMobyCreate;

#define MP_MOBY_DELETE_FLAG_UUID   1
#define MP_MOBY_DELETE_FLAG_OCLASS 2
#define MP_MOBY_DELETE_FLAG_ALL    4

typedef struct {
    u32 value;
    u32 flags;
} MPPacketMobyDelete;

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
#define MP_STATE_TYPE_SET_RESPAWN 7
#define MP_STATE_TYPE_COLLECTED_GOLD_BOLT 8
#define MP_STATE_TYPE_BLOCK_BOLT 9

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

typedef struct {
    uint64_t client_send_time;
    uint64_t server_receive_time;
} MPPacketTimeResponse;

typedef struct {
    uint32_t type;
    uint32_t duration;
    char message[0x50];
} MPPacketToastMessage;

#pragma pack(pop)

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
    static Packet* make_collision(u16 uuid, u16 collided_with, Vec4* position, bool aggressive);
    static Packet* make_connect_packet(const String& nickname, int32_t userid);
    static Packet* make_disconnect_packet();
    static Packet* make_time_request_packet();
    static Packet* make_player_respawned_packet();
    static Packet* make_game_state_changed_packet(GameState state);
    static Packet* make_collected_gold_bolt_packet(int bolt_number);
};

#endif