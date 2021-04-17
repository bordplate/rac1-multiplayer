#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "printf/printf.h"

#ifndef __cplusplus
    typedef int bool;
    typedef int8_t b8;
    typedef int32_t b32;
    #define true 1
    #define false 0
#endif

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

#define XSTRINGIZE(s) STRINGIZE(s)
#define STRINGIZE(s) #s

// memory
int strlen( const char* p );
void memset( unsigned char* p, size_t size, unsigned char value );

// stdio
void _putchar( char c );

// random
void srand( u32 seed );
s32 rand();

// assert
void _Assert( const char* pCond, const char* pFile, int line );
#define ASSERT( cond ) \
    do { if ( !(cond) ) { _Assert( XSTRINGIZE(cond), __FILE__, __LINE__ ); }} while ( false )

#ifdef __cplusplus
    #define HOOK_API extern "C"
#else
    #define HOOK_API 
#endif

#endif

