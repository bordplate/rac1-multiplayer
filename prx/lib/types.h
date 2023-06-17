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

// ghidra types
typedef u8 undefined;
typedef u16 undefined2;
typedef u32 undefined4;
typedef u64 undefined8;

typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

typedef struct Color {
    unsigned char a;
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Color;

#ifndef NULL
#define NULL 0
#endif


#endif