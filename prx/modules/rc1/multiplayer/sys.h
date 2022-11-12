#ifndef SYS_H
#define SYS_H

#define SYS_BNET_BIND 0x2BD
#define SYS_BNET_CONNECT 0x2BE
#define SYS_BNET_RECVFROM 0x2C3
#define SYS_BNET_SENDTO 0x2C6
#define SYS_BNET_SHUTDOWN 0x2C8
#define SYS_BNET_SOCKET 0x2C9
#define SYS_BNET_CLOSE 0x2CA
#define SYS_FS_OPEN 0x321
#define SYS_FS_READ 0x322
#define SYS_FS_CLOSE 0x324
#define SYS_FS_LSEEK 0x332

// ought to be moved
#define O_RDONLY 0x00

#define SEEK_SET 0x00
#define SEEK_CUR 0x01
#define SEEK_END 0x02

//#define syscall ((int (*)(int, ...))0x4F6500)


int __attribute__((naked)) syscall(int n, ...) {
    __asm__ (
        "mr     11, 3\n\t"
        "mr     3, 4\n\t"
        "mr     4, 5\n\t"
        "mr     5, 6\n\t"
        "mr     6, 7\n\t"
        "mr     7, 8\n\t"	
        "mr     8, 9\n\t"
        "mr     9, 10\n\t"
        "sc\n\t"
        "blr\n"
    );
}

#endif // SYS_H