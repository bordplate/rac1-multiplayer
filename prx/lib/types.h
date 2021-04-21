#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

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

#endif