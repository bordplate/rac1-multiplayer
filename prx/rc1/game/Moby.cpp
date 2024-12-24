#include "Moby.h"

#include <rc1/rc1.h>
#include <rc1/Game.h>
#include <rc1/multiplayer/network/Packet.h>

#include <lib/logger.h>
#include <rc1/multiplayer/MPMoby.h>

void Moby::set_animation(char animation_id, char animation_frame, u32 duration)
{
    return set_moby_animation(this, animation_id, animation_frame, duration);
}

void Moby::check_collision() {
    // Not implemented
}

void Moby::change_values(MPPacketChangeMobyValuePayload* changes, size_t num, u16 value_type) {
    char* base_address = value_type & MP_MOBY_FLAG_CHANGE_ATTR ? (char*)this : (char*)this->vars;

    for(int i = 0; i < num; i++) {
        MPPacketChangeMobyValuePayload* payload = &changes[i];

        // Handle edge case with hero moby coloring
        if (this == ratchet_moby && payload->offset == 0x38 && value_type & MP_MOBY_FLAG_CHANGE_ATTR) {
            use_custom_player_color = true;
            custom_player_color = payload->value;

            continue;
        }

        if (payload->size == 1) {
            u8 temp_value = (u8)payload->value;

            Logger::debug("Changing byte at offset %d to %d", payload->offset, temp_value);

            memcpy(base_address + payload->offset, &temp_value, payload->size);
        } else if (payload->size == 2) {
            u16 temp_value = (u16)payload->value;

            Logger::debug("Changing short at offset %d to %d", payload->offset, temp_value);

            memcpy(base_address + payload->offset, &temp_value, payload->size);
        } else {
            Logger::debug("Changing int at offset %d to %d", payload->offset, payload->value);
            memcpy(base_address + payload->offset, &payload->value, payload->size);
        }
    }
}

Damage* Moby::get_damage(u32 flags, u32 unk) {
    return moby_get_damage(this, flags, unk);
}

Moby* Moby::spawn(unsigned short o_class, unsigned short flags, uint16_t modeBits) {
    // If the main hero moby isn't spawned in, we shouldn't try to spawn anything else either.
    if (!ratchet_moby) {
        return nullptr;
    }

    Moby* moby = spawn_moby(o_class);

    if ((int)moby->vars == nullptr) {
        Logger::error("Moby spawned with invalid pVars: %d. Allocating pVars from custom allocator", (int) moby->vars);
        moby->vars = allocate_memory(0x80);
    }

    moby->enabled = 1;
    moby->draw_distance = 0xff;
    moby->update_distance = 0xff;
    moby->alpha = 0xff;

    // Used to set flag 0x1000 here as well, but setting flag 0x1000 makes it so that
    // the first 4 bytes of pVars are checked when using weapons and wrench and that crashes
    // on console when the address doesn't make sense. Does not crash in RPCS3 for whatever reason.
    moby->mode_bits = modeBits;

    Logger::info("Spawned Moby (oClass: %d)", o_class);

    return moby;
}

Moby* Moby::find_by_uid(u16 uid) {
    Logger::trace("Finding moby by UID: %d", uid);

    for (Moby* moby = moby_ptr; moby <= moby_ptr_end; moby++) {
        if (moby->state >= 0 && moby->uid == uid) {
            return moby;
        }
    }

    return nullptr;
}

Moby* Moby::find_first_oclass(u16 o_class) {
    Logger::trace("Finding first moby by oClass: %d", o_class);

    for (Moby* moby = moby_ptr; moby <= moby_ptr_end; moby++) {
        if (moby->state >= 0 && moby->o_class == o_class) {
            return moby;
        }
    }

    return nullptr;
}