#ifndef RC1_COMMON_H
#define RC1_COMMON_H

#include <lib/common.h>

#define MULTI_TRACE_LOG 1
#define MULTI_LOG(msg, ...) printf("rac1multi: " msg, ##__VA_ARGS__)
#if MULTI_TRACE_LOG
#define MULTI_TRACE(msg, ...) printf("*rac1multi: " msg, ##__VA_ARGS__)
#else
#define MULTI_TRACE(msg, ...) do {} while (false)
#endif

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct {
    char a;
    char b;
    char g;
    char r;
} color;

#endif
