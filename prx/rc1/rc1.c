#include "rc1.h"
#include "common.h"

#include <lib/memory.h>
#include <netex/net.h>
#include <sysutil/sysutil_gamecontent.h>

#include <cell/cell_fs.h>

#include <rc1/Game.h>
#include "multiplayer/Packet.h"
#include "multiplayer/SyncedMoby.h"
#include "multiplayer/Moby/RatchetAttachmentMoby.h"
#include "multiplayer/Moby/ClankAttachmentMoby.h"
#include "Player.h"

#include "bridging.h"

bool use_custom_player_color = false;
uint32_t custom_player_color = 0;

extern "C" {
void game_tick() {
    _c_game_tick();
}

SHK_HOOK(void, game_loop_start);
void game_loop_start_hook() {
    if (current_planet != 0 || ratchet_moby != 0) {
        game_tick();
    }

    SHK_CALL_HOOK(game_loop_start);
}

SHK_HOOK(void, game_loop_intro_start);
void game_loop_intro_start_hook() {
    game_tick();

    SHK_CALL_HOOK(game_loop_intro_start);
}

SHK_HOOK(void, on_respawn);
void on_respawn_hook() {
    SHK_CALL_HOOK(on_respawn);
    _c_on_respawn();
}

SHK_HOOK(void, STUB_0006544c);
void STUB_0006544c_hook(Moby *moby) {
    _c_moby_update(moby);
}

// Hook to avoid some consoles getting a "game is corrupted, restart the game" on game start
// I think maybe it makes trophies not work?
SHK_HOOK(void, authenticate_game);
void authenticate_game_hook() {
    MULTI_LOG("Game totally authenticated\n");
}

SHK_HOOK(void, FUN_000784e8);
void FUN_000784e8_hook() {
    _c_game_render();

    SHK_CALL_HOOK(FUN_000784e8);
}

SHK_HOOK(void, wrench_update_func, Moby *);
void wrench_update_func_hook(Moby *moby) {
    SHK_CALL_HOOK(wrench_update_func, moby);
}

SHK_HOOK(int, cellGameBootCheck, unsigned int*, unsigned int*, CellGameContentSize*, char*);
int cellGameBootCheckHook(unsigned int* type, unsigned int* attributes, CellGameContentSize* size, char* dirName) {
    MULTI_LOG("Type: %p, attr: %p, size: %p, dirName: %p\n", type, attributes, size, dirName);

    *type = 2;
    *attributes = 0;
    size->hddFreeSizeKB = 10000;
    size->sizeKB = -1;
    size->sysSizeKB = 4;

    int fd;
    const char* src;
    // Manually copying the string
    // Check if digital version exists and use that. Otherwise fall back to disc. If no disc then we just crash
    CellFsErrno ebootStat = cellFsOpendir("/dev_hdd0/game/NPEA00385/", &fd);
    if (ebootStat == CELL_FS_ENOENT) {
        src = "BCES01503";
    } else {
        src = "NPEA00385";
    }
    while (*src) {
        *dirName = *src;
        dirName++;
        src++;
    }
    *dirName = '\0';  // Null terminate the string

    MULTI_LOG("Done the thing\n");

    return 0;
}

SHK_HOOK(int, cellGameContentPermit, char*, char*);
int cellGameContentPermitHook(char* contentInfoPath, char* usrdirPath) {
    MULTI_LOG("contentInfoPath: %p, usrdirPath: %p\n", contentInfoPath, usrdirPath);

    int fd;
    const char *src;

    // Check if digital version exists and use that. Otherwise fall back to disc. If no disc then we just crash
    CellFsErrno ebootStat = cellFsOpendir("/dev_hdd0/game/NPEA00385/", &fd);
    if (ebootStat == CELL_FS_ENOENT) {
        src = "/dev_bdvd/PS3_GAME";
    } else {
        src = "/dev_hdd0/game/NPEA00385";
    }
    while (*src) {
        *contentInfoPath = *src;
        contentInfoPath++;
        src++;
    }
    *contentInfoPath = '\0';  // Null terminate the string

    if (ebootStat == CELL_FS_ENOENT) {
        src = "/dev_bdvd/PS3_GAME/USRDIR";
    } else {
        src = "/dev_hdd0/game/NPEA00385/USRDIR";
    }
    while (*src) {
        *usrdirPath = *src;
        usrdirPath++;
        src++;
    }
    *usrdirPath = '\0';  // Null terminate the string

    MULTI_LOG("Done the thing\n");

    return 0;
}

#include "GoldBolt.h"

SHK_HOOK(void, goldBoltUpdate, Moby* moby);
void goldBoltUpdateHook(Moby* moby) {
    ((GoldBolt*)moby)->update();
}

// Hook the item_unlock function
SHK_HOOK(void, _unlock_item, int, uint8_t);
void _unlock_item_hook(int item_id, uint8_t equip) {
    Client *client = Game::shared().client();
    if (client != nullptr) {
        Packet *packet = Packet::make_unlock_item_packet(item_id, equip);
        client->make_ack(packet, nullptr);
        client->send(packet);
    }
}

// Make original unlock_item available to our code
void unlock_item(int item_id, uint8_t equip) {
    SHK_CALL_HOOK(_unlock_item, item_id, equip);
}

SHK_HOOK(void, _unlock_level, int);
void _unlock_level_hook(int level) {
    Client *client = Game::shared().client();
    if (client != nullptr) {
        Packet *packet = Packet::make_unlock_level_packet(level);
        client->make_ack(packet, nullptr);
        client->send(packet);
    }
}

void unlock_level(int level) {
    SHK_CALL_HOOK(_unlock_level, level);
}

SHK_HOOK(void, _unlock_skillpoint, u8);
void _unlock_skillpoint_hook(u8 skillpoint) {
    Client *client = Game::shared().client();
    if (client != nullptr) {
        Packet *packet = Packet::make_unlock_skillpoint_packet(skillpoint);
        client->make_ack(packet, nullptr);
        client->send(packet);
    }
}

void unlock_skillpoint(u8 skillpoint) {
    SHK_CALL_HOOK(_unlock_skillpoint, skillpoint);
}

SHK_HOOK(void, set_ratchet_animation, u32 animation_id, char animation_frame);
void set_ratchet_animation_hook(u32 animation_id, char animation_frame) {
    // Get PowerPC f1 register using inline assembly because SHK_HOOK doesn't properly work with float parameters
    // This is the duration of the animation
    volatile double duration;
    asm volatile("stfd 1, %0" : "=m"(duration));

    _c_set_ratchet_animation_duration((int)duration);

    SHK_CALL_HOOK(set_ratchet_animation, animation_id, animation_frame);
}

SHK_HOOK(Moby*, _spawn_moby, u16 o_class);
Moby* spawn_moby_hook(u16 o_class) {
    Moby* moby = SHK_CALL_HOOK(_spawn_moby, o_class);

    if (game_state == Menu) {
        return moby;
    }

    switch(moby->o_class) {
        case 0xcb: // Decoy
        case 0xac: // Visibomb missile
        case 0x1df: // Drone
        case 0x99: // Gold devastator missile
        case 0x71a: // Sandmouse
        case 0x4a: // Mine
        case 0xba: // Doom bot
        case 457:  // RYNO missile
            SyncedMoby::make_synced_moby(moby)->activate();
            break;
        case 601: // Clank Backpack
            Player::shared().backpack_moby = RatchetAttachmentMoby::make_synced_moby(moby, 5, 5);
            Player::shared().backpack_moby->activate();
            break;
        case 607: // Heli pack
        case 608: // Thruster pack
        case 609: // Hydro pack
            Player::shared().backpack_attachment_moby = RatchetAttachmentMoby::make_synced_moby(moby, 5, 5);
            break;
        case 1289: // O2 Mask
        case 1290: // Pilots helmet
        case 433:  // Sonic summoner
            Player::shared().helmet_moby = RatchetAttachmentMoby::make_synced_moby(moby, 4, 21);
            Player::shared().helmet_moby->activate();
            break;
//        case 407:  // Persuader
//            Player::shared().persuader_moby = RatchetAttachmentMoby::make_synced_moby(moby, 5, 5);
//        case 614:  // Map-o-matic
//            Player::shared().map_o_matic_moby = RatchetAttachmentMoby::make_synced_moby(moby, 5, 5);
//            Player::shared().map_o_matic_moby->activate();
//            break;
//        case 618:  // Bolt magnetizer
//            Player::shared().bolt_magnetizer_moby = RatchetAttachmentMoby::make_synced_moby(moby, 5, 5);
//            Player::shared().bolt_magnetizer_moby->activate();
//            break;
//        case 173:  // Magneboots
//        case 195:  // Grindboots
//            if (Player::shared().left_shoe_moby == nullptr || Player::shared().left_shoe_moby->o_class != o_class) {
//                Player::shared().left_shoe_moby = RatchetAttachmentMoby::make_synced_moby(moby, 22, 22);
//                Player::shared().left_shoe_moby->activate();
//            } else if (Player::shared().right_shoe_moby == nullptr || Player::shared().right_shoe_moby->o_class != o_class) {
//                Player::shared().right_shoe_moby = RatchetAttachmentMoby::make_synced_moby(moby, 23, 23);
//                Player::shared().right_shoe_moby->activate();
//            }
//            break;
    }

    return moby;
}

Moby* spawn_moby(u16 o_class) {
    return SHK_CALL_HOOK(_spawn_moby, o_class);
}

SHK_HOOK(struct Damage*, _moby_get_damage, Moby*, u32, u32);
struct Damage* _moby_get_damage_hook(Moby* moby, u32 flags, u32 unk) {
    struct Damage* damage = SHK_CALL_HOOK(_moby_get_damage, moby, flags, unk);

    return damage;
}

struct Damage* moby_get_damage(Moby* moby, u32 flags, u32 unk) {
    return SHK_CALL_HOOK(_moby_get_damage, moby, flags, unk);
}

void rc1_init() {
    MULTI_LOG("Multiplayer initializing.\n");

    init_memory_allocator(memory_area, sizeof(memory_area));

    MULTI_LOG("Initialized memory allocator. Binding hooks\n");

    SHK_BIND_HOOK(STUB_0006544c, STUB_0006544c_hook);  // Used as a "trampoline" to our custom Moby update func
    SHK_BIND_HOOK(game_loop_start, game_loop_start_hook);
    SHK_BIND_HOOK(game_loop_intro_start, game_loop_intro_start_hook);
    SHK_BIND_HOOK(wrench_update_func, wrench_update_func_hook);
    SHK_BIND_HOOK(authenticate_game, authenticate_game_hook);
    SHK_BIND_HOOK(FUN_000784e8, FUN_000784e8_hook);
    SHK_BIND_HOOK(on_respawn, on_respawn_hook);
    SHK_BIND_HOOK(cellGameBootCheck, cellGameBootCheckHook);
    SHK_BIND_HOOK(cellGameContentPermit, cellGameContentPermitHook);
    SHK_BIND_HOOK(goldBoltUpdate, goldBoltUpdateHook);
    SHK_BIND_HOOK(_unlock_item, _unlock_item_hook);
    SHK_BIND_HOOK(_unlock_level, _unlock_level_hook);
    SHK_BIND_HOOK(_unlock_skillpoint, _unlock_skillpoint_hook);
    SHK_BIND_HOOK(set_ratchet_animation, set_ratchet_animation_hook);
    SHK_BIND_HOOK(_spawn_moby, spawn_moby_hook);
    SHK_BIND_HOOK(_moby_get_damage, _moby_get_damage_hook);

    MULTI_LOG("Bound hooks\n");
}

void rc1_shutdown() {
    sys_net_finalize_network();
}

};