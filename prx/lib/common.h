#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "external/printf/printf.h"

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

#ifndef __cplusplus
    typedef u32 bool;
    typedef u8 bool8;
    typedef u16 bool16; 
    typedef u32 bool32;
    #define true ((bool)1)
    #define false ((bool)0)
#endif

#define XSTRINGIZE(s) STRINGIZE(s)
#define STRINGIZE(s) #s

// memory
int strlen( const char* p );
void memset( unsigned char* p, size_t size, unsigned char value );

// stdio
void _putchar( char c );

// random

 // random prime
#define RND_INITIAL_SEED 0xBBD583E3
#define RND_MAX          0x7FFFFFFF

// Sets the random seed.
void rndSeed( u32 seed );

// Generates a random signed integer.
s32 rndInt();

// Generates a random normalized float (0-1)
f32 rndFloat();

// Generates a random signed integer between min (inclusive) and max (inclusive).
s32 rndIntBetween( s32 min, s32 max );

// Generates a random float between min (inclusive) and max (inclusive).
f32 rndFloatBetween( f32 min, f32 max );

// assert
void _Assert( const char* pCond, const char* pFile, int line );

#define assert( cond ) \
    do { if ( !(cond) ) { _Assert( XSTRINGIZE(cond), __FILE__, __LINE__ ); }} while ( false )

#endif

