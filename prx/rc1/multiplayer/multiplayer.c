#include "multiplayer.h"

#include <netex/net.h>

int handshake_complete = 0;

void mp_start() {
	// Need to initialize networking before we're allowed to open sockets and stuff.
	// You can technically bypass it by syscalling directly, but that brings other 
	// 		bugs, so this is easier. 
	sys_net_initialize_network();
	
	memset(&mp_mobys, 0, sizeof(mp_mobys));
}


// Send first handshake packet to the server to identify and connect to server.
// Since this is UDP and transport isn't guaranteed, handshaking just ensures that
//		there's actually a server talking our language on the other end before we
//		start sending and receiving random junk. 
void mp_send_handshake() {
	MPPacketHeader syn_packet = { 0, 0 };
	syn_packet.type = MP_PACKET_SYN;
	syn_packet.size = 0;
	
	MULTI_TRACE("Sent SYN packet with type: %d. Size: %d\n", syn_packet.type, sizeof(syn_packet));
	sendto(mp_sock, &syn_packet, sizeof(syn_packet), MSG_DONTWAIT, &mp_sockaddr, sizeof(struct sockaddr_in));
}

// Open socket and connect to the server. 
void mp_connect() {
	MULTI_LOG("Starting multiplayer...\n");
	MULTI_LOG("Opening socket...\n");
	mp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (mp_sock <= 0) {
		MULTI_LOG("Couldn't open socket: %d\n", mp_sock);
	}
	
	mp_sockaddr.sin_addr.s_addr = inet_addr("10.9.0.212");
	mp_sockaddr.sin_port = htons(2407);
	mp_sockaddr.sin_len = sizeof(mp_sockaddr);
	mp_sockaddr.sin_family = AF_INET;
	
	MULTI_LOG("Socket opened, and ready to blast data...\n");
}

Moby* mp_spawn_moby(u32 uuid, u32 o_class) {
	// Check that we're not trying to update a moby beyond our predefined moby space. 
	if (uuid > sizeof(mp_mobys)/sizeof(mp_mobys[0])) {
		MULTI_LOG("Tried to spawn illegal Moby UUID: %d\n", uuid);
		return 0;
	}
	
	MULTI_TRACE("Spawning moby oClass: %d\n", o_class);
	
	Moby* moby = spawn_moby(o_class);
	
	MULTI_TRACE("Moby address: %d\n", moby);
	
	moby->pUpdate = (void*)0x704720;  

	MPMobyVars *vars = (MPMobyVars*)(moby->pVars);
	vars->uuid = uuid;

	moby->enabled = 1;
	moby->draw_distance = 0xff;
	moby->update_distance = 0xff;
	moby->state = 0;
	moby->alpha = 0xff;
	
	// idk what these mode bits do, I just copied them from an enemy moby
	moby->mode_bits = 0x10 | 0x20 | 0x400 | 0x1000 | 0x4000;

	idk(moby);
	
	mp_mobys[uuid] = moby;
	
	MULTI_LOG("Spawned Moby (oClass: %d, uuid: %d)\n", o_class, uuid);
	
	return moby;
}


void mp_update_moby(MPPacketMobyUpdate* update_packet) {
	// Check that we're not trying to update a moby beyond our predefined moby space. 
	if (update_packet->uuid > sizeof(mp_mobys)/sizeof(mp_mobys[0])) {
		MULTI_LOG("Tried to update illegal Moby UUID: %d\n", update_packet->uuid);
		return;
	}
	
	//MULTI_TRACE("Player state: %d\n", player_state);
	
	if (game_state != PlayerControl) {
		MULTI_TRACE("Moby not updating because game state %d\n", game_state);
	}

	Moby* moby = mp_mobys[update_packet->uuid];
	
	if (!moby || moby->state < 0) {
		// Spawn moby
		MULTI_TRACE("Spawning Moby at (%f, %f, %f)\n", update_packet->x, update_packet->y, update_packet->z);
		moby = mp_spawn_moby(update_packet->uuid, update_packet->o_class);
		
		if (!moby) return;
	}
	
	MPMobyVars *vars = (MPMobyVars*)(moby->pVars);
	
	if (moby->position.x != update_packet->x)
		moby->position.x = update_packet->x;
	
	if (moby->position.y != update_packet->y)
		moby->position.y = update_packet->y;

	if (moby->position.z != update_packet->z)
		moby->position.z = update_packet->z;
	
	if (moby->rotation.z != update_packet->rotation)
		moby->rotation.z = update_packet->rotation;

	vars->next_animation_id = (char)update_packet->animation_id;
	
	//MULTI_TRACE("Moby update success with animID: %d | %d\n", update_packet->animation_id, vars->next_animation_id);
}

void mp_reset_environment() {
	memset(&mp_mobys, 0, sizeof(mp_mobys));
}

void mp_server_reset() {
	MULTI_LOG("Server reset\n");
	mp_reset_environment();
}

char recv_buffer[1024];
// Receive and process updates and events from the server. Typically called once per tick.
void mp_receive_update() {
	int recv = 0;
	do {
		MPPacketHeader packet_header = { 0, 0 };
		
		memset(&recv_buffer, 0, sizeof(recv_buffer));
		int paddrlen = sizeof(struct sockaddr_in);
		recv = recvfrom(mp_sock, &recv_buffer, sizeof(recv_buffer), MSG_DONTWAIT, &mp_sockaddr, &paddrlen);
		
		if (recv > 0) {
			memcpy(&packet_header, &recv_buffer, sizeof(packet_header));
		
			MULTI_TRACE("Received %d bytes from server\n", recv);
			
			switch(packet_header.type) {
				case MP_PACKET_ACK:
					handshake_complete = 1;
					MULTI_LOG("Handshake complete!\n");
					break;
				case MP_PACKET_IDKU:
					handshake_complete = 0;
					mp_server_reset();
					break;
				case MP_PACKET_MOBY_UPDATE:
					MULTI_TRACE("Updating moby\n");
					mp_update_moby(((MPPacketMobyUpdate*)&recv_buffer[sizeof(packet_header)]));
					MULTI_TRACE("Done updating moby\n");
					break;
				default:
					MULTI_LOG("Unknown packet from server\n");
			}
		}
	} while (recv > 0);
}

void mp_send_update() {
	MPPacketHeader header = { 0, 0 };
	header.type = MP_PACKET_MOBY_UPDATE;
	header.size = sizeof(MPPacketMobyUpdate);
	
	// Send packet body
	MPPacketMobyUpdate update_packet; 
	memset(&update_packet, 0, sizeof(update_packet));
	update_packet.uuid = 0;
	update_packet.enabled = 1;
	update_packet.o_class = 0;
	update_packet.animation_id = ratchet_moby->animationID;
	update_packet.x = player_pos.x;
	update_packet.y = player_pos.y;
	update_packet.z = player_pos.z;
	update_packet.rotation = player_rot.z;
	
	char buffer[sizeof(header) + sizeof(update_packet) + 16];
	memset(&buffer, 0, sizeof(buffer));
	memcpy(&buffer, &header, sizeof(header));
	memcpy(&buffer[sizeof(header)], &update_packet, sizeof(update_packet));
	
	MULTI_TRACE("Sent update packet with advertised size: %d. Actual size: %d\n", header.size, sizeof(buffer));
	sendto(mp_sock, &buffer, sizeof(buffer), MSG_DONTWAIT, &mp_sockaddr, sizeof(struct sockaddr_in));
}

int last_frame_count = 0;

void mp_tick() {
	MULTI_TRACE("New tick, new life\n");
	
	if (frame_count < last_frame_count) {
		MULTI_LOG("Environment reset\n");
		mp_reset_environment();
	}
	
	last_frame_count = frame_count;
	
	if (mp_sock <= 0) {
		mp_connect();
		mp_send_handshake();
		
		// Wait until next game tick to receive handshake response and start doing actual stuff 
		return;
	}
	
	// Receive state from server
	mp_receive_update();
	
	MULTI_TRACE("Server sync complete\n");
	
	if (!handshake_complete) {
		mp_send_handshake();
		
		// Wait until next game tick to receive handshake response and start doing actual stuff 
		return; 
	}
	
	// Send current state to server
	// Start by sending packet header.
	MULTI_TRACE("Sending current state to server\n");

	mp_send_update();
	
	MULTI_TRACE("Done sending state to server\n");
	
	return;
}