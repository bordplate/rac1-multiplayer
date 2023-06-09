#include "Moby.h"

#include "Game.h"

#include "rc1.h"
#include "multiplayer/packet.h"

#include <lib/logger.h>

void Moby::set_animation(char animation_id, char animation_frame, u32 duration)
{
    int seq_idx;
    u8 seq_frame_count;
    char frame_count;
    u32 uVar6;
    double dVar7;
    float frame_rate;

    seq_idx = animation_id << 2;
    seq_frame_count = this->pClass->seqs[seq_idx].frame_count;
    frame_count = seq_frame_count - 1;

    // Handle a few special cases
    if (this->updateID != 0xff || this->field36_0x60 != 0 || this->field40_0x64 != NULL) {
        uVar6 = FUN_000f0f9c(this);
        if (uVar6 != -1) {
            // Do some stuff with the moby
            this->field26_0x50 = (u8)uVar6;
        }
    }

    // Set the moby's animation
    this->animationFrame = frame_count + (animation_frame - frame_count);
    this->animationID = animation_id;

    // Do something with the moby's frame data
    moby_frame_data_something(this);

    // Calculate the animation frame rate
    frame_rate = 1e+07;
    if (duration != 0) {
        dVar7 = int_to_double(duration);
        frame_rate = (float)(1.0 / dVar7);
    }
    this->field35_0x5c = frame_rate;

    // Set some other moby fields
    this->field34_0x58 = 1.0;
    this->field30_0x54 = 0.0;
    this->field43_0x70 = this->field43_0x70 & 0xfd;
    this->field49_0x7c = this->pClass->seqs[seq_idx].next_seq;
}

void Moby::check_collision() {
    if (this->state == 0) {
        this->state = 1;
    }

    MPMobyVars* vars = (MPMobyVars*)(this->pVars);

    if (!vars) {
        return;
    }

    // I'm not sure about these arguments, this kinda works, so I stuck with it.
    int success = coll_sphere(&this->position, 0, 1, this);

    if (success > 0 && coll_moby_out != 0) {
        if (coll_moby_out->oClass == 0) {
            if (Game::shared().client()) {
                Game::shared().client()->send(Packet::make_collision(0, vars->uuid, &this->position, false));
            }
        }
    }
}

Moby* Moby::spawn(unsigned short o_class, unsigned short flags, uint16_t modeBits) {
    // If the main hero moby isn't spawned in, we shouldn't try to spawn anything else either.
    if (!ratchet_moby) {
        return nullptr;
    }

    Moby* moby = spawn_moby(o_class);

    if ((int)moby->pVars == nullptr) {
        Logger::error("Moby spawned with invalid pVars: %d. Allocating pVars from custom allocator", (int) moby->pVars);
        moby->pVars = allocate_memory(0x80);
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