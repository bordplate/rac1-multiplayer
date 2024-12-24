//
// Created by bordplate on 11/1/2024.
//

#include "SyncedMoby.h"
#include "Player.h"
#include <rc1/rc1.h>
#include <rc1/Game.h>

static SyncedMobyVars synced_moby_vars[4096];

void SyncedMoby::reset() {
    SyncedMobyVars* vars = get_vars();
    Client* client = Game::shared().connected_client();

    vars->uuid = 0;
    vars->status = SyncedMobyStatusReady;

    // If we're not attached to anything, or we're attached to the main moby (0), activate
    if ((vars->flags & MP_MOBY_FLAG_ATTACHED_TO) == 0 && vars->parent_uuid != 0) {
        activate();
        return;
    }

    vars->status = SyncedMobyStatusWaiting;
}

void SyncedMoby::activate() {
    u32 id = ((u32)this - (u32)moby_ptr) / 0x100;
    SyncedMobyVars* vars = &synced_moby_vars[id];

    if (vars->status == SyncedMobyStatusActive) {
        return;
    }

    if (game_state != PlayerControl) {
        vars->status = SyncedMobyStatusWaiting;
        return;
    }

    Client* client = Game::shared().connected_client();
    if (client != nullptr) {
        client->send_rpc(Packet::make_moby_create_packet(
                0,
                vars->parent_uuid,
                Player::shared().spawn_id,
                vars->flags,
                o_class,
                0,
                vars->position_bone,
                vars->transform_bone
        ), (AckCallback)&moby_create_callback, (void*)id);

        vars->connection_start_time = client->connection_start_time_;
        vars->uuid = 0;
        vars->status = SyncedMobyStatusConnecting;

        return;
    }

    vars->status = SyncedMobyStatusReady;
}

void SyncedMoby::update() {
    if (game_state == Menu) {
        return;
    }

    SyncedMobyVars* vars = get_vars();

    if (vars->p_update != nullptr) {
        vars->p_update((Moby*)this);
    }

    Client* client = Game::shared().connected_client();

    if (vars->status == SyncedMobyStatusWaiting && game_state == PlayerControl && vars->parent_uuid == 0 && client) {
        activate();
        return;
    }

    if ((vars->flags & MP_MOBY_FLAG_ATTACHED_TO) && vars->status == SyncedMobyStatusWaiting && vars->parent_uuid != 0) {
        activate();
        return;
    }

    if (vars->status != SyncedMobyStatusActive) {
        return;
    }

    if (client != nullptr && client->connection_start_time_ != vars->connection_start_time) {
        reset();
        return;
    }

    if (this->state < 0) {
        if (client != nullptr) {
            Logger::debug("Sending delete packet for moby %d", vars->uuid);
            client->send_ack(Packet::make_moby_delete_packet(vars->uuid));
            memset(vars, 0, sizeof(SyncedMobyVars));

            vars->status = SyncedMobyStatusReady;
        }

        return;
    }

    if (client != nullptr && vars->uuid != 0) {
        if (state >= 0) {
            Packet *packet = new Packet(sizeof(MPPacketMobyUpdate));
            packet->header->type = MP_PACKET_MOBY_UPDATE;

            MPPacketMobyUpdate* payload = (MPPacketMobyUpdate*)packet->body;
            payload->uuid = vars->uuid;
            payload->o_class = o_class;
            payload->animation_id = animation_id;
            payload->animation_duration = 10;
            payload->x = position.x;
            payload->y = position.y;
            payload->z = position.z;
            payload->rotX = rotation.x;
            payload->rotY = rotation.y;
            payload->rotZ = rotation.z;
            payload->scale = scale;

            client->send(packet);
        }
    }
}

SyncedMobyVars* SyncedMoby::get_vars() {
    u32 id = ((u32)this - (u32)moby_ptr) / 0x100;
    return &synced_moby_vars[id];
}

SyncedMoby* SyncedMoby::make_synced_moby(Moby* moby, u16 flags, u16 mode_bits, u16 uuid, u8 position_bone, u8 transform_bone) {
    u32 id = ((u32)moby - (u32)moby_ptr) / 0x100;
    SyncedMobyVars* vars = &synced_moby_vars[id];
    memset(vars, 0, sizeof(SyncedMobyVars));

    // Store the original update function and replace it with our own
    vars->p_update = (void* (*)(Moby*))(moby->p_update);
    moby->p_update = (void*)SyncedUpdate;

    vars->status = SyncedMobyStatusReady;
    vars->parent_uuid = uuid;
    vars->flags = flags;
    vars->position_bone = position_bone;
    vars->transform_bone = transform_bone;
    vars->moby = moby;

    return (SyncedMoby*)moby;
}

int SyncedMoby::moby_create_callback(void* data, size_t len, void* extra) {
    u32 id = (u32)extra;
    SyncedMobyVars* vars = &synced_moby_vars[id];

    MPPacketMobyCreateResponse* response = (MPPacketMobyCreateResponse*)data;
    vars->uuid = response->uuid;
    vars->status = SyncedMobyStatusActive;

    Logger::info("Server acknowledged moby creation and gave us UUID: %d", vars->uuid);

    return 1;
}

void SyncedMoby::synced_update(SyncedMoby *moby) {
    moby->update();
}
