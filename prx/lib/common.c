#include "common.h"
#include <sys/tty.h>

s32 strlen( const char* pMsg )
{
    s32 len = 0;
    while ( *pMsg++ )
        len++;
    return len;
}

void memset( u8* p, size_t size, u8 value )
{
    for (size_t i = 0; i < size; i++)
    {
        p[i] = value;
    }
}

void _Assert( const char* pCond, const char* pFile, s32 line )
{
    printf( "%s:%d assertion failed! %s\n", pFile, line, pCond );
}

void _putchar( char c )
{
    static u8 buffer[1024];
    static s32 bufferIndex = 0;
    ASSERT( bufferIndex < sizeof( buffer ) );
    buffer[ bufferIndex++ ] = c;

    if ( c == '\n' )
    {
        // flush
        u32 writeLen;
        sys_tty_write( SYS_TTYP_PPU_STDOUT, buffer, strlen(buffer), &writeLen );
        memset( buffer, sizeof( buffer ), 0 );
        bufferIndex = 0;
    }
}

u32 gRndSeed = 0xBBD583E3;

void srand( u32 seed )
{
    gRndSeed = seed;
}

s32 rand()
{
    s32 a = 0x41A7;
    s32 m = 0x7FFFFFFF;
    gRndSeed = ( a * gRndSeed ) % m;
    return gRndSeed / m;
}
