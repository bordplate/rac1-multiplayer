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
    void* self_address;
    int is_allocated;
    size_t size;
    struct memory_block *next;
};

extern char memory_area[1000000];
extern int used_memory;
extern int allocations;
extern int num_allocated;
extern int frees;

void init_memory_allocator(void *start, size_t size);
void *allocate_memory(size_t size);
void free_memory(void *ptr);

#ifdef __cplusplus
};
#endif


#endif //PS3_CKIT_MEMORY_H
