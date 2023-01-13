//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_MEMORY_H
#define PS3_CKIT_MEMORY_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct memory_block {
    size_t size;
    int is_allocated;
    struct memory_block *next;
};

extern char *memory_area[1000000];

void init_memory_allocator(void *start, size_t size);
void *allocate_memory(size_t size);
void free_memory(void *ptr);

#ifdef __cplusplus
};
#endif


#endif //PS3_CKIT_MEMORY_H
