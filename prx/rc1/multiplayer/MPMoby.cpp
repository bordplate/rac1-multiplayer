//
// Created by Vetle Hjelle on 20/12/2022.
//

#include "MPMoby.h"
#include "rc1/multiplayer/network/Packet.h"

#include <rc1/rc1.h>
#include <rc1/Game.h>

void MPMoby::update() {
    // Don't check collision for Ratchet moby, because that apparently crashes the game on console only.
    if (this->o_class != 0) {
        this->check_collision();
    }

    MPMobyVars* vars = (MPMobyVars*)(this->vars);

    Damage* damage = this->get_damage(0x330000, 0);

    if (damage != nullptr) {
        Logger::info("Moby %d took %f damage from o_class %d", this->uid, damage->damage_dealt, damage->source_o_class);

        if (Game::shared().client() != nullptr) {
            Game::shared().client()->send_ack(Packet::make_damage_packet(
                0,
                vars->uuid,
                0,
                this->o_class,
                damage->source_o_class,
                &damage->position,
                damage->damage_dealt
            ));
        }
    }

    if (
        vars->attached_to_parent && vars->parent && vars->parent->state >= 0 &&
        vars->parent->p_class->joint_cnt > vars->parent_position_bone &&
        vars->parent->p_class->joint_cnt > vars->parent_transform_bone
    ) {
        Vec4 bone_position;
        get_bone_position(vars->parent, vars->parent_position_bone, &bone_position);

        this->position.x = bone_position.x;
        this->position.y = bone_position.y;
        this->position.z = bone_position.z;

        Vec4 transformations[4];
        get_bone_transformations(vars->parent, vars->parent_transform_bone, transformations);

        apply_transformations(this->transform, transformations);

//        if (o_class == 173 || o_class == 195) { // Special fix for boots
//            Vec4* up = &transform[2];
//            Vec4* forward = &transform[0];
//
//            position.x += up->x * 0.2f + forward->x * 0.05f;
//            position.y += up->y * 0.2f + forward->y * 0.05f;
//            position.z += up->z * 0.2f + forward->z * 0.05f;
//        }

//        idk(this);
    }

    this->damage = -1;

    if (this->animation_id != vars->next_animation_id) {
        this->set_animation(vars->next_animation_id, 0, vars->animation_duration);
    }
}

MPMoby* MPMoby::spawn(unsigned short uuid, unsigned short o_class, unsigned short flags, uint16_t modeBits) {
    if (flags & MP_MOBY_FLAG_ORIG_UDPATE_FUNC) {
        Logger::error("MPMoby::spawn called with MP_MOBY_FLAG_ORIG_UDPATE_FUNC flag, but this is not supported.");
        return nullptr;
    }

    MPMoby* moby = (MPMoby*)Moby::spawn(o_class, flags, modeBits);

    if (!moby) {
        return moby;
    }

    moby->enabled = 1;

    // Set update function to MPMoby::update()
    moby->p_update = (void *)0x710ed8;

    // Set Moby variables
    MPMobyVars* vars = (MPMobyVars*)(moby->vars);
    memset(vars, 0, sizeof(MPMobyVars));

    if (vars) {
        vars->uuid = uuid;
    }

    idk(moby);

    return moby;
}

extern "C" void _c_moby_update(Moby* moby) {
    ((MPMoby*)moby)->update();
}