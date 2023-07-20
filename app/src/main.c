//
// Created by bordplate on 7/17/2023.
//

#include <sys/process.h>
#include <sys/syscall.h>

#include <cell/sysmodule.h>

#include <stdio.h>



int main(void) {
    int prio = 1001;
    uint64_t flags = SYS_PROCESS_PRIMARY_STACK_SIZE_64K;

    sys_game_process_exitspawn("/dev_hdd0/game/BORD00001/USRDIR/EBOOT.mp.BIN", NULL, NULL, 0, 0, prio, flags);

    return 0;
}