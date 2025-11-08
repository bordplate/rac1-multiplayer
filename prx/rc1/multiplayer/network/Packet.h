#ifndef PACKET_H
#define PACKET_H

#include "rc1/rc1.h"

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

#include "lib/types.h"

#define MP_API_VERSION  9

#define MP_PACKET_CONNECT            1
#define MP_PACKET_SYN                2
#define MP_PACKET_ACK                3
#define MP_PACKET_MOBY_UPDATE        4
#define MP_PACKET_IDKU               5
#define MP_PACKET_MOBY_CREATE        6
#define MP_PACKET_DISCONNECT         7
#define MP_PACKET_MOBY_DELETE        8
#define MP_PACKET_MOBY_DAMAGE        9
#define MP_PACKET_SET_STATE          10
#define MP_PACKET_SET_HUD_TEXT       11
#define MP_PACKET_QUERY_GAME_SERVERS 12
#define MP_PACKET_CONTROLLER_INPUT   13
#define MP_PACKET_TIME_SYNC          14
#define MP_PACKET_PLAYER_RESPAWNED   15
#define MP_PACKET_REGISTER_SERVER    16
#define MP_PACKET_TOAST_MESSAGE      17
#define MP_PACKET_MOBY_EX            18
#define MP_PACKET_AUTH               19
#define MP_PACKET_CREATE_USER        20
#define MP_PACKET_ERROR_MESSAGE      21
#define MP_PACKET_REGISTER_HYBRID_MOBY 22
#define MP_PACKET_MONITORED_VALUE_CHANGED 23
#define MP_PACKET_CHANGE_MOBY_VALUE 24
#define MP_PACKET_LEVEL_FLAG_CHANGED 25
#define MP_PACKET_MONITOR_ADDRESS    26
#define MP_PACKET_ADDRESS_CHANGED    27
#define MP_PACKET_MOBY_CREATE_FAILURE 28
#define MP_PACKET_UI                  29
#define MP_PACKET_UI_EVENT            30

#define MP_PACKET_FLAG_RPC           0x1

#define MP_COLLISION_FLAG_AGGRESSIVE   1
#define MP_COLLISION_FLAG_GAME_MOBY    2

#define MP_MOBY_FLAG_ACTIVE           0x1
#define MP_MOBY_FLAG_NO_COLLISION     0x2
#define MP_MOBY_FLAG_ORIG_UDPATE_FUNC 0x4
#define MP_MOBY_FLAG_ATTACHED_TO      0x8

#define MP_CONTROLLER_FLAGS_PRESSED 1
#define MP_CONTROLLER_FLAGS_HELD 2

#pragma pack(push, 1)

struct MPPacketHeader {
    u16 type;
    u16 flags;
    u32 size;
    u64 timeSent;
    u8 requires_ack;
    u8 ack_cycle;
};

struct MPPacketConnect {
    u32 userid;
    u32 version;
    u8 passcode[8];
    u16 nick_length;
};

struct MPPacketAuth {
    u32 userid;
    u32 step;
    u64 field1;
    u64 field2;
};

struct MPPacketCreateUser {
    u32 userid;
    u16 nick_length;
    u16 email_length;
};

#define MP_CONNECT_ERROR_UNKNOWN 0
#define MP_CONNECT_SUCCESS 1
#define MP_CONNECT_ERROR_USER_ALREADY_CONNECTED 2
#define MP_CONNECT_ERROR_NOT_ALLOWED 3
#define MP_CONNECT_ERROR_OUTDATED 4
#define MP_CONNECT_ERROR_WRONG_PASSCODE 5

struct MPPacketConnectCallback {
    u32 status;
};

struct MPPacketMobyUpdate {
    u16 uuid;
    u16 o_class;
    u8 animation_id;
    u8 animation_duration;
    float x;
    float y;
    float z;
    float rotX;
    float rotY;
    float rotZ;
    float scale;
};

typedef struct {
    u16 offset;
    u32 value;
} MPPacketMobyExtendedPayload;

typedef struct {
    u16 uuid;
    u16 num_values;
} MPPacketMobyExtended;

typedef struct {
    u16 uuid;
    u16 parent_uuid;
    u8 spawn_id;
    u8 flags;
    u16 o_class;
    u16 mode_bits;
    u8 position_bone;
    u8 transform_bone;
} MPPacketMobyCreate;

typedef struct {
    u16 uuid;
} MPPacketMobyCreateResponse;

#define MP_MOBY_CREATE_FAILURE_REASON_UNKNOWN 0
#define MP_MOBY_CREATE_FAILURE_REASON_NOT_READY 1
#define MP_MOBY_CREATE_FAILURE_REASON_ALREADY_EXISTS 2
#define MP_MOBY_CREATE_FAILURE_REASON_INVALID_PARENT 3
#define MP_MOBY_CREATE_FAILURE_REASON_MAX_MOBYS 4
#define MP_MOBY_CREATE_FAILURE_REASON_UPDATE_NON_EXISTENT 5
#define MP_MOBY_CREATE_FAILURE_REASON_SUCCESS 6

typedef struct {
    u16 uuid;
    u8 reason;
} MPPacketMobyCreateFailure;

#define MP_MOBY_FLAG_FIND_BY_UUID (1 << 0)
#define MP_MOBY_FLAG_FIND_BY_UID  (1 << 1)

#define MP_MOBY_FLAG_CHANGE_ATTR (1 << 8)
#define MP_MOBY_FLAG_CHANGE_PVAR (1 << 9)

typedef struct {
    u16 id;
    u16 flags;
    u16 num_values;
} MPPacketChangeMobyValue;

// Defined in Moby.h because of annoying things with including and C/C++ interop or whatever
//typedef struct {
//    u16 offset;
//    u16 size;
//    u32 value;
//} MPPacketChangeMobyValuePayload;

#define MP_MOBY_DELETE_FLAG_UUID   1
#define MP_MOBY_DELETE_FLAG_OCLASS 2
#define MP_MOBY_DELETE_FLAG_ALL    4
#define MP_MOBY_DELETE_FLAG_UID   8

typedef struct {
    u32 value;
    u32 flags;
} MPPacketMobyDelete;

typedef struct {
    u16 uuid;
    u16 collided_with_uuid;
    u32 flags;
    u16 damaged_o_class;
    u16 source_o_class;
    float x;
    float y;
    float z;
    float damage;
} MPPacketDamage;

typedef struct {
    u16 moby_uid;
    u16 n_monitored_attributes;
    u16 n_monitored_pvars;
} MPPacketRegisterHybridMoby;

typedef struct {
    u16 offset;
    u16 size;
} MPPacketMonitorValue;

#define MP_MONITORED_VALUE_TYPE_ATTR 1
#define MP_MONITORED_VALUE_TYPE_PVAR 2

typedef struct {
    u16 uid;
    u16 offset;
    u8 flags;
    u8 size;
    u32 old_value;
    u32 new_value;
} MPPacketMonitoredValueChanged;

#define MP_STATE_TYPE_DAMAGE    1
#define MP_STATE_TYPE_PLAYER    2
#define MP_STATE_TYPE_POSITION  3
#define MP_STATE_TYPE_PLANET    4
#define MP_STATE_TYPE_GAME      5
#define MP_STATE_TYPE_ITEM      6
#define MP_STATE_TYPE_SET_RESPAWN 7
#define MP_STATE_TYPE_COLLECTED_GOLD_BOLT 8
#define MP_STATE_TYPE_BLOCK_BOLT 9
#define MP_STATE_TYPE_PLAYER_INPUT 10
#define MP_STATE_TYPE_ARBITRARY 11
#define MP_STATE_TYPE_UNLOCK_ITEM 12
#define MP_STATE_TYPE_GIVE_BOLTS 13
#define MP_STATE_TYPE_UNLOCK_LEVEL 14
#define MP_STATE_TYPE_LEVEL_FLAG 15
#define MP_STATE_TYPE_UNLOCK_SKILLPOINT 16
#define MP_STATE_TYPE_COMMUNICATION_FLAGS 17
#define MP_STATE_TYPE_START_IN_LEVEL_MOVIE 18

typedef struct {
    u16 flags;
    u16 state_type;
    u32 offset;
    u32 value;
} MPPacketSetState;

typedef struct {
    u16 flags;
    u16 state_type;
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

#define MP_UI_OPERATION_CREATE      1 << 0
#define MP_UI_OPERATION_UPDATE      1 << 1
#define MP_UI_OPERATION_DELETE      1 << 2
#define MP_UI_OPERATION_CLEAR_ALL   1 << 3

typedef struct {
    u16 id;
    u16 element_type;
    u8 operations;
    u8 items;
} MPPacketUI;

typedef struct {
    u16 id;
    u8 operations;
    u8 pad;
    u16 attribute;
    u16 data_length;
} MPPacketUIItem;

typedef struct {
    u16 event_type;
    u16 element_id;
    u32 data;
    u16 extra_length;
} MPPacketUIEvent;

// Packet used to query a directory for game servers
typedef struct {
    u8 version;
    u32 page;
} MPPacketQueryGameServers;

typedef struct {
    u32 ip;
    u16 port;
    u16 max_players;
    u16 player_count;
    u16 name_length;
    u16 description_length;
    u16 owner_name_length;
} MPPacketQueryResponseServer;

typedef struct {
    u16 input;
    u16 flags;
} MPPacketControllerInput;

typedef struct {
    u64 client_send_time;
    u64 server_receive_time;
} MPPacketTimeResponse;

typedef struct {
    u32 type;
    u32 duration;
    char message[0x50];
} MPPacketToastMessage;

struct MPPacketErrorMessage {
    u16 message_length;
};

#define MP_LEVEL_FLAG_TYPE_1  1
#define MP_LEVEL_FLAG_TYPE_2  2

typedef struct {
    u16 type;
    u8 level;
    u8 flags;
} MPPacketLevelFlagsChanged;

typedef struct {
    u8 size;
    u16 index;
    u32 value;
} MPPacketLevelFlag;

typedef struct {
    u8 flags;
    u8 size;
    u32 address;
} MPPacketMonitorAddress;

typedef struct {
    u32 address;
    u16 size;
    u32 old_value;
    u32 new_value;
} MPPacketAddressChanged;

typedef struct {
    u8 spawn_id;
    u16 level_id;
} MPPacketSpawned;

#pragma pack(pop)

struct Packet {
    MPPacketHeader* header;
    void* body;
    size_t len;
    bool retain_after_send;
    char payload_buffer[512];

    Packet(size_t body_len);
    ~Packet();

    static Packet* make_handshake_packet();
    static Packet* make_ack_packet(unsigned char id, unsigned char cycle);
    static Packet* make_query_directory_packet(int page);
    static Packet* make_moby_create_packet(u16 uuid, u16 parent_uuid, u8 spawn_id, u16 flags, u16 o_class, u16 mode_bits, u8 position_bone, u8 transform_bone);
    static Packet* make_moby_delete_packet(u16 uuid);
    static Packet* make_controller_input(CONTROLLER_INPUT inputs, u16 flags);
    static Packet* make_damage_packet(u16 uuid, u16 collided_with_uuid, u32 flags, u16 damaged_o_class, u16 source_o_class, Vec4* position, float damage);
    static Packet* make_connect_packet(const String& nickname, int32_t userid);
    static Packet* make_disconnect_packet();
    static Packet* make_time_request_packet();
    static Packet* make_player_respawned_packet(u8 spawn_id, u16 level_id);
    static Packet* make_game_state_changed_packet(GameState state);
    static Packet* make_collected_gold_bolt_packet(int bolt_number);
    static Packet* make_start_in_level_movie_packet(u32 movie);
    static Packet* make_unlock_item_packet(int item_id, bool equip);
    static Packet* make_unlock_level_packet(int level);
    static Packet* make_unlock_skillpoint_packet(u8 skillpoint);
    static Packet* make_monitored_value_changed_packet(u16 uid, u32 offset, u32 size, u8 flags, u32 old_value, u32 new_value);
    static Packet* make_level_flags_changed_packet(u16 type, u8 level, size_t num_changed_flags, MPPacketLevelFlag* level_flags);
    static Packet* make_address_changed_packet(u32 address, u16 size, u32 old_value, u32 new_value);
    static Packet* make_bolt_count_changed_packet(s32 bolt_diff, u32 current_bolts);
    static Packet* make_moby_create_failure_packet(u16 uuid, u8 reason);
    static Packet* make_ui_event_packet(u16 event_type, u16 element_id, u32 data);
    static Packet* make_ui_event_with_text_packet(u16 event_type, u16 element_id, u32 data, const char* text);
};

#endif
