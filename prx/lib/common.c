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
        p[i] = value;
}

void _Assert( const char* pCond, const char* pFile, s32 line )
{
    printf( "%s:%d assertion failed! %s\n", pFile, line, pCond );
}

// for printf implementation
void _putchar( char c )
{
    static u8 buffer[1024];
    static s32 bufferIndex = 0;
    assert( bufferIndex < sizeof( buffer ) );
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


u32 sRndSeed = RND_INITIAL_SEED;

void rndSetSeed( u32 seed )
{
    sRndSeed = seed;
}

u32 rndGetSeed()
{
    return sRndSeed;
}

s32 rndInt()
{
    s32 a = 0x41A7;
    sRndSeed = ( a * sRndSeed ) % RND_MAX;
    return sRndSeed;
}

f32 rndFloat()
{
    return (f32)rndInt() / sRndSeed;
}

s32 rndIntBetween( s32 min, s32 max )
{
    return (s32)( min + ( rndFloat() * ( 1 + max - min ) ) );
}

f32 rndFloatBetween( f32 min, f32 max )
{
    return (f32)( min + ( rndFloat() * ( max - min ) ) );
}
