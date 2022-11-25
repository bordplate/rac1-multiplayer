#include "moby.h"

#include "rc1.h"
#include "multiplayer/packet.h"

SHK_FUNCTION_DEFINE_STATIC_1(0xf0f9c, u32, FUN_000f0f9c, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0xef74c, int, moby_frame_data_something, void*, p1);
SHK_FUNCTION_DEFINE_STATIC_1(0x502730, double, long_long_to_double, u64, p1);
SHK_FUNCTION_DEFINE_STATIC_2(0xfde80, int, FUN_000fa940, void*, p1, u32, p2);

// This function is decompiled set_ratchet_animation function, sprinkled with set_moby_animation stuff.
// It fixes a crash that occurs on console, but not emulator.
void set_moby_animation_custom(Moby* moby, char animationID, char animationFrame) {
    int uVar1;
    int iVar2;
    u64 lVar3;

    lVar3 = (u64)animationID;

    uVar1 = FUN_000f0f9c(moby);
    if (uVar1 > -1) {
        // NOTE: Originally in set_moby_animation and set_ratchet_animation, but crashes on console.
        //FUN_000fa940(moby, uVar1 | 0x300);
        iVar2 = uVar1 * 0x10;

        if (moby->updateID != -1) {
            moby->field69_0xa5 = moby->updateID;
        }

        *(u32*)(iVar2 + 0xa34600) = moby->field127_0xf0;
        *(u32*)(iVar2 + 0xa34604) = moby->field128_0xf4;
        *(u32*)(iVar2 + 0xa34608) = moby->field132_0xf8;
        *(u32*)(iVar2 + 0xa3460c) = moby->field136_0xfc;

        moby->updateID = 0xff;
        moby->field26_0x50 = (u8)uVar1;
    }

    moby->animationFrame = animationFrame;
    moby->animationID = (u8)lVar3;
    moby_frame_data_something(moby);

    moby->field34_0x58 = 1.0;
    if (animationFrame != 0) {

    }

    moby->field35_0x5c = 1e+07;
    moby->field30_0x54 = 0.0;
    moby->field43_0x70 = moby->field43_0x70 & 0xfd;

    // NOTE: Originally in set_moby_animation and set_ratchet_animation, but crashes on console.
    //moby->field49_0x7c = *(u8 *)(*(int *)((int)&moby->pClass->seqs + (animationID << 2)) + 0x11);
}

void __attribute__ ((noinline)) moby_check_collision(Moby* self) {
    MPMobyVars* vars = (MPMobyVars*)(self->pVars);

    if (!vars) {
        return;
    }

    // I'm not sure about these arguments, this kinda works, so I stuck with it.
    int success = coll_sphere(&self->position, 0, 1, self);

    if (success > 0 && coll_moby_out != 0) {
        if (coll_moby_out->oClass == 0) {
            mp_send_collision(vars->uuid, 0, &self->position, false);
            MULTI_TRACE("Collision with ratchet! 0x%08x : 0x%08x : 0x%08x\n", coll_moby_out, ratchet_moby, self);
        }
    }
}

void moby_update(Moby* self) {
    // Don't check collision for Ratchet moby, because that apparently crashes the game on console only.
    if (self->oClass != 0) {
        moby_check_collision(self);
    }

    MPMobyVars* vars = (MPMobyVars*)(self->pVars);

    if (self->animationID != vars->next_animation_id && vars->next_animation_id < 127) {
        // FIXME: This commented MULTI_TRACE call makes the game crash later in the rendering sequence for unknown reasons.
        MULTI_TRACE("(uid: %d) Animating to ID: %hhd, from ID: %hhu\n", vars->uuid, vars->next_animation_id, self->animationID);
        set_moby_animation_custom(self, vars->next_animation_id, 0);
    }
}

int item_to_oclass(ITEM item) {
    switch(item) {
        case Wrench:
            return 0x47;
        case Devastator:
            return 0x9d;
        default: return 0x47;
    }

    return 0x47;
}
