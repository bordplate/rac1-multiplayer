#include "lib/random.h"

u32 sRandomSeed = RANDOM_INITIAL_SEED;

void randomSetSeed( u32 seed )
{
    sRandomSeed = seed;
}

u32 randomGetSeed()
{
    return sRandomSeed;
}

s32 randomInt()
{
    s32 a = 0x41A7;
    sRandomSeed = ( a * sRandomSeed ) % RANDOM_MAX;
    return sRandomSeed;
}

f32 randomFloat()
{
    return ( f32 )randomInt() / RANDOM_MAX;
}

s32 randomIntBetween( s32 min, s32 max )
{
    return ( s32 )( min + ( randomFloat() * ( 1 + max - min ) ) );
}

f32 randomFloatBetween( f32 min, f32 max )
{
    return ( f32 )( min + ( randomFloat() * ( max - min ) ) );
}