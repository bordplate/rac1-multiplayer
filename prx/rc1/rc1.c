#include "rc1.h"
#include "common.h"

#include <lib/memory.h>
#include <netex/net.h>
#include <sysutil/sysutil_gamecontent.h>

#include <cell/cell_fs.h>

#include <rc1/Game.h>
#include "multiplayer/Packet.h"

#include "bridging.h"

bool use_custom_player_color = false;
uint32_t custom_player_color = 0;
EnableCommunicationsFlags enable_communication_bitmap = ENABLE_ALL;

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
    // Clear the collision out ptr before calling original wrench function
    coll_moby_out = 0;

    SHK_CALL_HOOK(wrench_update_func, moby);

    // If coll_moby_out has a value, the wrench has "attacked" something
    if (!coll_moby_out) {
        return;
    }

    // Figure out what moby we hit and if we need to tell the server about it
    Moby* hit = coll_moby_out;
    if (!hit->pVars) {
        // If we don't have pVars, this isn't something the server needs to know about
        return;
    }

    MPMobyVars* vars = (MPMobyVars*)hit->pVars;

    // If this moby has UUID vars
    if (vars->uuid && vars->sig == 0x4542) {
        if (Game::shared().client()) {
            Game::shared().client()->send(Packet::make_collision(0, vars->uuid, &hit->position, true));
        }
    }
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
    if (!(enable_communication_bitmap & ENABLE_ON_UNLOCK_ITEM)) {
        MULTI_LOG("unlock_item communication disabled. acting autonomously.\n");
        unlock_item(item_id, equip);
    }
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
    if (!(enable_communication_bitmap & ENABLE_ON_UNLOCK_LEVEL)) {
        MULTI_LOG("unlock_level communication disabled. acting autonomously.\n");
        unlock_level(level);
    }
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

    MULTI_LOG("Bound hooks\n");
}

void rc1_shutdown() {
    sys_net_finalize_network();
}

};