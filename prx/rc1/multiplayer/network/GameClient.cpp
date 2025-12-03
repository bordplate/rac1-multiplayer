//
// Created by Vetle Hjelle on 30/12/2022.
//

#include "GameClient.h"

#include <rc1/Game.h>

#include "../Player.h"
#include "../MPMoby.h"

#include <rc1/game/GoldBolt.h>

GameClient::GameClient(char *ip, int port) : Client(ip, port) {
    mobys_.resize(MAX_MP_MOBYS);
    ip_ = ip;
    port_ = port;
    data_thread_id_ = -1;

    for(int i = 0; i < mobys_.capacity(); i++) {
        mobys_[i].moby = nullptr;
        mobys_[i].uuid = 0;
    }

    connection_complete_ = false;
}

void GameClient::disconnect() {
    Client::disconnect();

    if (ratchet_moby == nullptr && current_planet == 0) {
        game_state = Movie;
    }

    close_data_client();

    ServerListView* view = new ServerListView();
    Game::shared().transition_to(view);
}

void GameClient::reset() {
    waiting_for_connect_ = false;
    connection_complete_ = false;

    if (remote_view_ != nullptr) {
        remote_view_->clear_all();
    }

    clear_hybrid_mobys();
    moby_delete_all();

    for (size_t i = 0; i < monitored_addresses_.size(); i++) {
        delete monitored_addresses_[i];
    }
    monitored_addresses_.resize(0);

    Player::shared().spawn_id = 0;

    close_data_client();

    Client::reset();
}

void GameClient::close_data_client() {
    if (data_thread_id_ != -1) {
        DataClient::stop = true;

        sys_ppu_thread_join(data_thread_id_, nullptr);

        data_thread_id_ = -1;
    }
}

void GameClient::create_moby(MobyInfo* moby_info) {
    Moby* moby = moby_info->moby;

    if (moby && moby->state >= 0) {
        Logger::error("Replaced moby with existing UUID: %d", moby_info->uuid);
        delete_moby(moby);
    }

    Logger::info("[%d] Spawning Moby (oClass: %d)", moby_info->uuid, moby_info->o_class);

    moby = (Moby*)MPMoby::spawn(moby_info->uuid, moby_info->o_class, moby_info->flags, moby_info->mode_bits);

    if (!moby) {
        Logger::error("Failed to spawn moby %d", moby_info->uuid);
        moby_info->moby = nullptr;
        send_ack(Packet::make_moby_create_failure_packet(moby_info->uuid, MP_MOBY_CREATE_FAILURE_REASON_UNKNOWN));
        return;
    }

    if (moby_info->flags & MP_MOBY_FLAG_NO_COLLISION) {
        moby->collision = nullptr;
    }

    MPMobyVars* vars = (MPMobyVars*)(moby->vars);

    if (moby_info->flags & MP_MOBY_FLAG_ATTACHED_TO) {
        if (moby_info->parent_uuid == 0) {
            Logger::error("Server tried to attach moby %d to parent 0", moby_info->o_class);
            send_ack(Packet::make_moby_create_failure_packet(moby_info->uuid, MP_MOBY_CREATE_FAILURE_REASON_UNKNOWN));
            return;
        }

        MobyInfo* parent_moby_info = &mobys_[moby_info->parent_uuid];
        if (parent_moby_info->uuid == 0 || parent_moby_info->moby == nullptr || parent_moby_info->moby->state < 0) {
            Logger::trace("Server tried to attach moby %d to an invalid parent %d, but we are not ready", moby_info->o_class, moby_info->parent_uuid);
            send_ack(Packet::make_moby_create_failure_packet(moby_info->uuid, MP_MOBY_CREATE_FAILURE_REASON_NOT_READY));
            return;
        }

        Logger::info("Created attached moby %d to parent %d", moby_info->uuid, moby_info->parent_uuid);
        vars->attached_to_parent = true;
        vars->parent = mobys_[moby_info->parent_uuid].moby;
        moby->mode_bits = moby->mode_bits | 0x100;
    }

    vars->parent_position_bone = moby_info->position_bone;
    vars->parent_transform_bone = moby_info->transform_bone;

    moby_info->moby = moby;

    send_ack(Packet::make_moby_create_failure_packet(moby_info->uuid, MP_MOBY_CREATE_FAILURE_REASON_SUCCESS));
}

void GameClient::create_moby(MPPacketMobyCreate* packet) {
// Check that we're not trying to create a moby beyond our predefined moby space.
    if (packet->uuid > mobys_.capacity()) {
        Logger::error("Tried to create illegal Moby UUID: %d", packet->uuid);
        send_ack(Packet::make_moby_create_failure_packet(packet->uuid, MP_MOBY_CREATE_FAILURE_REASON_MAX_MOBYS));

        return;
    }

    if (game_state != PlayerControl) {
        Logger::trace("Moby not creating because game state %d", game_state);
        send_ack(Packet::make_moby_create_failure_packet(packet->uuid, MP_MOBY_CREATE_FAILURE_REASON_NOT_READY));

        return;
    }

    MobyInfo* moby_info = &mobys_[packet->uuid];

    moby_info->uuid = packet->uuid;
    moby_info->parent_uuid = packet->parent_uuid;
    moby_info->flags = packet->flags;
    moby_info->o_class = packet->o_class;
    moby_info->mode_bits = packet->mode_bits;
    moby_info->position_bone = packet->position_bone;
    moby_info->transform_bone = packet->transform_bone;

    create_moby(moby_info);
}

void GameClient::update_moby(MPPacketMobyUpdate* packet) {
    // Check that we're not trying to update a moby beyond our predefined moby space.
    if (packet->uuid > mobys_.capacity()) {
        Logger::error("Tried to update illegal Moby UUID: %d", packet->uuid);
        return;
    }

    if (packet->uuid == 0) {
        Logger::error("Tried to update Ratchet Moby with MPPacketMobyUpdate");
        return;
    }

    if (game_state != PlayerControl) {
        Logger::trace("Moby not updating because game state %d", game_state);
        return;
    }

    MobyInfo* moby_info = &mobys_[packet->uuid];
    Moby* moby = moby_info->moby;

    if (moby_info->uuid != 0 && moby_info->uuid != packet->uuid) {
        Logger::error("Mismatch in moby info table, expected %d but got %d", packet->uuid, moby_info->uuid);
        return;
    }

    if (moby && moby->state >= 0 && moby->o_class != packet->o_class) {
        Logger::debug("[%d] Incoming o_class %d changed o_class %d for existing moby", packet->uuid, packet->o_class, moby->o_class);

        delete_moby(moby);
        moby_info->o_class = packet->o_class;
        moby_info->moby = nullptr;

        create_moby(moby_info);

        if (moby_info->moby == nullptr) {
            return;
        }

        moby = moby_info->moby;
    }

    if (!moby && moby_info->uuid == 0) {
        Logger::error("Tried to update non-existent Moby UUID: %d", packet->uuid);

        send_ack(Packet::make_moby_create_failure_packet(packet->uuid, MP_MOBY_CREATE_FAILURE_REASON_UPDATE_NON_EXISTENT));

        return;
    } else if (!moby) {
        Logger::error("Respawning Moby UUID: %d", packet->uuid);
        create_moby(moby_info);

        if (moby_info->moby == nullptr) {
            Logger::error("Failed to respawn it!");
            return;
        }

        moby = moby_info->moby;
    }

    MPMobyVars* vars = (MPMobyVars*)(moby->vars);

    if (!vars->attached_to_parent) {
        moby->position.x = packet->x;
        moby->position.y = packet->y;
        moby->position.z = packet->z;
        moby->rotation.x = packet->rotX;
        moby->rotation.y = packet->rotY;
        moby->rotation.z = packet->rotZ;
        moby->scale = packet->scale;
    }

    vars->next_animation_id = (char)packet->animation_id;
    vars->animation_duration = (int)packet->animation_duration;
}

void GameClient::update_moby_ex(MPPacketMobyExtended *packet) {
    // Check that we're not trying to update a moby beyond our predefined moby space.
    if (packet->uuid > mobys_.capacity()) {
        Logger::error("Tried to update illegal Moby UUID: %d", packet->uuid);
        return;
    }

    Moby* moby;

    if (packet->uuid == 0) {
        moby = ratchet_moby;
    } else {
        moby = mobys_[packet->uuid].moby;
    }

    if (moby) {
        // Get the payload
        for(int i = 0; i < packet->num_values; i++) {
            MPPacketMobyExtendedPayload* payload = (MPPacketMobyExtendedPayload*)((char*)packet + sizeof(MPPacketMobyExtended) + (i * sizeof(MPPacketMobyExtendedPayload)));

            if (payload->offset > 0x100) {
                Logger::error("Tried to update illegal Moby offset: %d", payload->offset);
                continue;
            }

            // Handle edge case with hero moby coloring
            if (moby == ratchet_moby && payload->offset == 0x38) {
//                use_custom_player_color = true;
//                custom_player_color = payload->value;

                continue;
            }

            *((unsigned int*)((char*)moby + payload->offset)) = payload->value;
        }
    }
}

void GameClient::change_moby_value(MPPacketChangeMobyValue* packet) {
    Moby* moby = nullptr;

    if (packet->flags & MP_MOBY_FLAG_FIND_BY_UUID) {
        Logger::debug("Changing moby value by UUID %d", packet->id);
        moby = mobys_[packet->id].moby;
    } else if (packet->flags & MP_MOBY_FLAG_FIND_BY_UID) {
        Logger::debug("Changing moby value by UID %d", packet->id);
        moby = Moby::find_by_uid(packet->id);
    }

    if (moby == nullptr) {
        return;
    }

    moby->change_values((MPPacketChangeMobyValuePayload*)((char*)packet + sizeof(MPPacketChangeMobyValue)), packet->num_values, packet->flags);

    // If we have this moby as a hybrid moby, we should update the old values without notifying the server
    for (size_t i = 0; i < hybrid_mobys_.size(); i++) {
        if (hybrid_mobys_[i]->uid == moby->uid) {
            hybrid_mobys_[i]->refresh_old_values_without_notifying_server();
        }
    }
}

void GameClient::moby_delete(MPPacketMobyDelete* packet) {
    Logger::trace("Deleting moby %d", packet->value);

    if (packet->flags & MP_MOBY_DELETE_FLAG_UUID) {
        MobyInfo* moby_info = &mobys_[packet->value];
        Moby* moby = moby_info->moby;

        moby_info->uuid = 0;
        moby_info->moby = nullptr;

        if (!moby || moby->state < 0) {
            Logger::debug("Already deleted moby %d @ 0x%08x", packet->value, moby);
            return;
        }

        Logger::debug("Deleting moby %d", packet->value);

        delete_moby(moby);
    } else if (packet->flags & MP_MOBY_DELETE_FLAG_OCLASS) {
        for (Moby* moby = moby_ptr; moby <= moby_ptr_end; moby++) {
            if (moby->state >= 0 && moby->o_class == packet->value) {
                moby->state = -1;
                delete_moby(moby);
            }
        }
    } else if (packet->flags & MP_MOBY_DELETE_FLAG_UID) {
        for (Moby* moby = moby_ptr; moby <= moby_ptr_end; moby++) {
            if (moby->state >= 0 && moby->uid == packet->value) {
                moby->state = -1;
                delete_moby(moby);
            }
        }
    }
}

void GameClient::moby_clear_all() {
    for(int i = 0; i < mobys_.size(); i++) {
        mobys_[i].moby = nullptr;
    }
}

void GameClient::moby_delete_all() {
    for(int i = 0; i < mobys_.size(); i++) {
        MobyInfo* moby_info = &mobys_[i];
        Moby* moby = moby_info->moby;
        if (moby != nullptr && moby->state >= 0) {
            delete_moby(moby);
        }

        memset(moby_info, 0, sizeof(MobyInfo));
    }
}

void GameClient::update_set_state(MPPacketSetState* packet) {
    switch(packet->state_type) {
        case MP_STATE_TYPE_DAMAGE: {
            player_health -= (int)packet->value;
            break;
        }
        case MP_STATE_TYPE_PLAYER: {
            transition_to_movement_state(packet->value, 1);
            break;
        }
        case MP_STATE_TYPE_PLAYER_INPUT: {
            player_state_input = (int)packet->value;
            break;
        }
        case MP_STATE_TYPE_POSITION: {
            if (packet->offset > 8) {
                Logger::error("Server tried to set position at invalid offset %d", packet->offset);
                break;
            }

            ((float*)&player_pos)[packet->offset] = ((MPPacketSetStateFloat*)packet)->value;
            break;
        }
        case MP_STATE_TYPE_PLANET: {
            if ((current_planet != (int)packet->value && destination_planet != (int)packet->value) ||
                ratchet_moby == nullptr) {
                Logger::info("Going to planet %d", (int)packet->value);
                seen_planets[0] = 1;
                seen_planets[packet->value] = 1;
                *(int*)0xa10700 = 1;
                *(int*)0xa10704 = (int)packet->value;
                *(int*)0x969c70 = (int)packet->value;
            }

            break;
        }
        case MP_STATE_TYPE_ITEM: {
            u8 flags = (u8)(packet->value  >> 16);
            u16 item = (u16)(packet->value & 0xFFFF);

            u8 give = flags & 1;
            u8 equip = flags & 2;

            if (give) {
                unlock_item(item, equip);
            } else {
                // TODO: Take item away from player
                // Since the game never really takes an item away from you
                // there's no simple way to "unequip" an item as far as we know from reversing.
                Logger::error("Can't take away items yet.");
            }

            break;
        }
        case MP_STATE_TYPE_SET_RESPAWN:  {
            Logger::debug("Setting respawn %d to %f", packet->offset, ((MPPacketSetStateFloat*)packet)->value);
            if (packet->offset < 3) {
                ((float*)&Player::shared().respawn_position)[packet->offset] = ((MPPacketSetStateFloat*)packet)->value;
            } else if (packet->offset < 6) {
                ((float*)&Player::shared().respawn_rotation)[packet->offset-3] = ((MPPacketSetStateFloat*)packet)->value;
            }

            if (current_planet > 0 && ratchet_moby != nullptr) {
                set_spawn_point(&Player::shared().respawn_position, &Player::shared().respawn_rotation);
            }
            break;
        }
        case MP_STATE_TYPE_BLOCK_BOLT: {
            blocked_bolts[(packet->offset * 4) + packet->value] = 1;

            break;
        }
        case MP_STATE_TYPE_ARBITRARY: {
            u16 size = packet->flags;

            u32 value = packet->value << 8 * (4 - size);

            memcpy((unsigned char*)packet->offset, &value, size);

            // If we're monitoring this address, we must change its old_value
            for (size_t i = 0; i < monitored_addresses_.size(); i++) {
                if (monitored_addresses_[i]->offset == packet->offset) {
                    memcpy(&monitored_addresses_[i]->old_value, &value, size);
                    break;
                }
            }

            break;
        }
        case MP_STATE_TYPE_GIVE_BOLTS: {
            #define SET_BOLTS 1
            if (packet->offset == SET_BOLTS) {
                player_bolts = (s32)packet->value;
            } else {
                player_bolts += (s32)packet->value;
            }
            Player::shared().previous_bolt_count = player_bolts;
            break;
        }
        case MP_STATE_TYPE_UNLOCK_LEVEL: {
            int level = (int)(packet->value);
            unlock_level(level);
            break;
        }
        case MP_STATE_TYPE_UNLOCK_SKILLPOINT: {
            unlock_skillpoint((u8)packet->value);
            break;
        }
        case MP_STATE_TYPE_START_IN_LEVEL_MOVIE: {
            start_in_level_movie((u32)packet->value);
            break;
        }
        case MP_STATE_TYPE_LEVEL_FLAG: {
            int level = (int)(packet->offset >> 24) & 0xFF;
            int type = (int)(packet->offset >> 16) & 0xFF;
            int offset = (int)(packet->offset) & 0xFFFF;

            Logger::debug("Setting level flag for %d; type: %d; %d->%d", level, type, offset, packet->value);

            if (type == MP_LEVEL_FLAG_TYPE_1) {
                level_flags1[level * 0x10 + offset] = packet->value;

                if (current_planet == 1 && offset == 0) {  // Special hack to enable ship on Novalis when we get this flag
                    ratchets_ship->mode_bits &= 0xfffc;
                    ratchets_ship->collision = ratchets_ship->p_class->collision;
                }
            }
            if (type == MP_LEVEL_FLAG_TYPE_2) {
                level_flags2[level * 0x100 + offset] = packet->value;
            }

            Game::shared().refresh_level_flags();
            break;
        }
        case MP_STATE_TYPE_COMMUNICATION_FLAGS: {
            enable_communication_bitmap = (EnableCommunicationsFlags)packet->value;
            break;
        }
        case MP_STATE_TYPE_SAVE_FILE_OPERATION: {
            switch (packet->value) {
                case 0:
                    Logger::debug("Server told us to save.");
                    perform_save_operation(3);
                    break;
                case 1:
                    Logger::debug("Server told us to load stored savefile.");
                    Game::shared().force_load_save_file = true;
                    break;
                case 2:
                    Game::shared().manual_save_enabled = true;
                    break;
                default:
                    Logger::error("Server asked us to perform uknown save file operation %d", packet->value);
            }
            break;
        }
        default: {
            Logger::error("Server asked us to set unknown state type %d", packet->state_type);
        }
    }
}

void GameClient::update_set_text(MPPacketSetHUDText* packet) {
    // If we don't have a view in time for this packet, just return
    if (remote_view_ == nullptr) {
        return;
    }

    if (packet->flags & HUDElementDelete) {
        Logger::trace("Deleting HUD element");
        remote_view_->delete_text_element(packet->id);

        return;
    }

    TextElement* element = remote_view_->get_text_element(packet->id);

    if (!element) {
        Logger::critical("Unable to create text element for remote view: %d", packet->id);
        return;
    }

    Logger::trace("Updating text element at %p", element);

    element->x = packet->x;
    element->y = packet->y;

    #define game_state_mask 0xFF
    #define game_state_offset 0x3

    #define flags_set_mask 0x1
    #define flags_set_offset 0x2

    if (packet->flags >> flags_set_offset & flags_set_mask) { // flag idicating that the server has configured the state flags
        element->states = (packet->flags >> game_state_offset) & game_state_mask;
    } else { // if the flag is not set, default to original behavior for new client -> old server backward compatibility
        element->states = ViewPlayerControl;
    }

    element->color = packet->color;

    Logger::trace("Before setting text");

    element->text->set(packet->text);
}

void GameClient::toast_message(MPPacketToastMessage* packet) {
    String message = String(packet->message);
    strcpy((char*)(0xa15c8c), message.c_str());
    u32 duration = packet->duration;

    if (duration < 20) {
        duration = 20;
    }

    (*((u32*)0xa15c80)) = duration;
}

void GameClient::register_hybrid_moby(MPPacketRegisterHybridMoby* packet) {
    Logger::debug("Registering hybrid moby %d", packet->moby_uid);

    Moby* native_moby = Moby::find_by_uid(packet->moby_uid);

    if (native_moby == nullptr) {
        Logger::error("Server tried to register hybrid moby %d, but it doesn't exist. Frame count: %d", packet->moby_uid, frame_count);
        return;
    }

    HybridMoby* hybrid_moby;

    for (size_t i = 0; i < hybrid_mobys_.size(); i++) {
        if (hybrid_mobys_[i]->uid == packet->moby_uid) {
            hybrid_moby = hybrid_mobys_[i];
            break;
        }
    }

    if (hybrid_moby == nullptr) {
        hybrid_moby = new HybridMoby(native_moby, packet->moby_uid);
    }

    hybrid_mobys_.push_back(hybrid_moby);

    for (size_t i = 0; i < packet->n_monitored_attributes + packet->n_monitored_pvars; i++) {
        MPPacketMonitorValue* monitor_value = (MPPacketMonitorValue*)((char*)packet + sizeof(MPPacketRegisterHybridMoby)
                + (i * sizeof(MPPacketMonitorValue)));

        if (i < packet->n_monitored_attributes) {
            hybrid_moby->monitor_attribute(monitor_value->offset, monitor_value->size);
        } else {
            hybrid_moby->monitor_pvar(monitor_value->offset, monitor_value->size);
        }
    }
}

void GameClient::refresh_hybrid_mobys() {
    for (size_t i = 0; i < hybrid_mobys_.size(); i++) {
        hybrid_mobys_[i]->set_moby(Moby::find_by_uid(hybrid_mobys_[i]->uid));
    }
}

void GameClient::clear_hybrid_mobys() {
    for (size_t i = 0; i < hybrid_mobys_.size(); i++) {
        delete hybrid_mobys_[i];
    }

    hybrid_mobys_.resize(0);
}

void GameClient::monitor_address(MPPacketMonitorAddress* packet) {
    if (packet->size > 4) {
        Logger::error("Server tried to monitor address with size %d. Ignoring.", packet->size);
        return;
    }

    Logger::debug("Monitoring address %p with size %d", packet->address, packet->size);

    MonitoredValue* value = new MonitoredValue();
    value->offset = packet->address;
    value->size = packet->size;

    memcpy(&value->old_value, (unsigned char*)packet->address, packet->size);

    // Send initial value back to server
    Packet* addrChangedPacket = Packet::make_address_changed_packet(packet->address, packet->size, value->old_value, value->old_value);
    make_ack(addrChangedPacket, nullptr);
    send(addrChangedPacket);

    monitored_addresses_.push_back(value);
}

static Profiler client_update_timer_("client update");
static Profiler game_update_timer_("game update");
bool GameClient::update(MPPacketHeader *header, void *packet_data) {
    {
        Profiler::Scope scope(&client_update_timer_);

        if (!Client::update(header, packet_data)) {
            return false;
        }
    }

    {
        Profiler::Scope scope(&game_update_timer_);

        // Don't update if handshake isn't complete
        // We also wait a couple of frames after spawning to mitigate an unknown crash that occurs sometimes when we spawn
        //  mobys too quickly after spawn
        if (!handshake_complete()) {
            return false;
        }

        // Process packet
        switch (header->type) {
            case MP_PACKET_MOBY_CREATE:
                create_moby((MPPacketMobyCreate*)packet_data);
                break;
            case MP_PACKET_MOBY_UPDATE:
                update_moby((MPPacketMobyUpdate*)packet_data);
                break;
            case MP_PACKET_MOBY_EX:
                update_moby_ex((MPPacketMobyExtended*)packet_data);
                break;
            case MP_PACKET_CHANGE_MOBY_VALUE:
                change_moby_value((MPPacketChangeMobyValue*)packet_data);
                break;
            case MP_PACKET_MOBY_DELETE:
                moby_delete((MPPacketMobyDelete*)packet_data);
                break;
            case MP_PACKET_SET_STATE: {
                // Server can send multiple of these messages in 1 packet to ensure the actions are performed in the right sequence.
                int recvd = 0;
                while (recvd < header->size) {
                    MPPacketSetState* state_data = (MPPacketSetState*)((char*)packet_data + recvd);
                    update_set_state(state_data);

                    recvd += sizeof(MPPacketSetState);
                }

                break;
            }
            case MP_PACKET_SET_HUD_TEXT: {
                update_set_text((MPPacketSetHUDText*)packet_data);
                break;
            }
            case MP_PACKET_TOAST_MESSAGE: {
                toast_message((MPPacketToastMessage*)packet_data);
                break;
            }
            case MP_PACKET_ERROR_MESSAGE: {
                MPPacketErrorMessage* error = (MPPacketErrorMessage*)packet_data;

                String message = String((const char*)((char*)packet_data + sizeof(MPPacketErrorMessage)));
                message = message.slice(0, error->message_length);

                Game::shared().alert(message);

                break;
            }
            case MP_PACKET_REGISTER_HYBRID_MOBY: {
                register_hybrid_moby((MPPacketRegisterHybridMoby*)packet_data);
                break;
            }
            case MP_PACKET_MONITOR_ADDRESS: {
                monitor_address((MPPacketMonitorAddress*)packet_data);
                break;
            }
            case MP_PACKET_UI: {
                if (remote_view_ == nullptr) {
                    Logger::error("No remote view to handle UI updates.");
                    break;
                }

                remote_view_->handle_packet((MPPacketUI*)packet_data);

                break;
            }
            case MP_PACKET_UI_EVENT: {
                if (remote_view_ == nullptr) {
                    Logger::error("No remote view to handle UI events.");
                    break;
                }

                remote_view_->handle_event((MPPacketUIEvent*)packet_data);

                break;
            }
            case MP_PACKET_OPEN_DATA_STREAM: {
                if (data_thread_id_ != -1) {
                    Logger::warning("Server tried to open data stream, but it's already open");
                    break;
                }

                MPPacketOpenDataStream* open_data_stream = (MPPacketOpenDataStream*)packet_data;

                DataClientConnectionInfo connection_info = {
                    &sockaddr_, open_data_stream->key
                };

                sys_ppu_thread_create(
                    &data_thread_id_,
                    (void(*)(uint64_t))&DataClient::start_data_client,
                    (uint64_t)&connection_info,
                    1000,
                    0x1000,
                    SYS_PPU_THREAD_CREATE_JOINABLE,
                    "DataClient"
                );
                break;
            }
            default:
                Logger::error("Received %ld bytes of unknown packet %d:", received(), header->type);
                Logger::error("> Advertised size: %d", sizeof(MPPacketHeader) + header->size);
                Logger::error("> Ack ID: %d. Cycle: %d", header->requires_ack, header->ack_cycle);
                Logger::error("> Flags: %d", header->flags);
                if (header->size > 0 && header->size <= received() - sizeof(MPPacketHeader)) {
                    hexDump("> Packet body data", packet_data, header->size);
                } else if (header->size <= 0 && received() - sizeof(MPPacketHeader) <= 0) {
                    Logger::error(
                            ">!Advertised packet body size is 0, but we received %d more bytes than size of header.",
                            received() - sizeof(MPPacketHeader));
                } else {
                    Logger::error(">!Received less bytes than advertised size, not printing data");
                }
        }
    }

    return true;
}

int GameClient::connect_callback(void* packetData, size_t size, void* userdata) {
    GameClient* self = (GameClient*)userdata;
    MPPacketConnectCallback* response = (MPPacketConnectCallback*)packetData;

    if (response->status == 1) {
        Logger::info("Connection successfully established!");
        self->connection_complete_ = true;

        return 0;
    }

    String message;

    switch (response->status) {
        case 0:
            message = String("An unknown error occurred.");
            break;
        case 2:
            message = String("A user with that username is already connected to this server. Please change to a different username.");
            break;
        case 3:
            message = String("You must've been banned from this server or something. Good job.");
            break;
        case 4:
            message = String("Your multiplayer mod is outdated. Please update your multiplayer mod to connect to this game server.");
            break;
        case 5:
            message = String("Wrong passcode.");
            break;
        default:
            message = String("A super unknown error occurred, something's fucky.");
            break;
    }

    Game::shared().alert(message);
    self->disconnect();

    return 0;
}

static Profiler game_client_timer_("game client");
static Profiler hybrid_mobys_timer_("hybrid mobys");
static Profiler monitored_addresses_timer_("monitored addresses");
static Profiler player_update_timer_("player update");
void GameClient::on_tick() {
    {
        Profiler::Scope scope(&game_client_timer_);

        Client::on_tick();
    }

    if (!remote_view_) {
        remote_view_ = new RemoteView();
        Game::shared().transition_to(remote_view_);
    }

    {
        Profiler::Scope scope(&hybrid_mobys_timer_);

        for (size_t i = 0; i < hybrid_mobys_.size(); i++) {
            hybrid_mobys_[i]->on_tick();
        }
    }

    {
        Profiler::Scope scope(&monitored_addresses_timer_);

        for (size_t i = 0; i < monitored_addresses_.size(); i++) {
            MonitoredValue* address_value = monitored_addresses_[i];

            u32 value;
            memcpy(((char*)&value) + 4 - address_value->size, (char*)address_value->offset, address_value->size);

            if (value != address_value->old_value) {
                Packet* packet = Packet::make_address_changed_packet(address_value->offset, address_value->size,
                                                                     address_value->old_value, value);
                make_ack(packet, nullptr);
                send(packet);
            }

            address_value->old_value = value;
        }
    }

    ticks_ += 1;

    if (connected() && !handshake_complete()) {
        send_handshake();

        // Wait until next game tick to receive handshake response and start doing actual stuff
        return;
    }

    if (!connection_complete_) {
        if (!waiting_for_connect_) {
            Packet *packet = Packet::make_connect_packet(Player::shared().username, Game::shared().userid);
            make_self_referencing_rpc(packet, connect_callback);
            send(packet);
            waiting_for_connect_ = true;
        }

        return;
    }

    {
        Profiler::Scope scope(&player_update_timer_);
        Player::shared().on_tick();
    }
}