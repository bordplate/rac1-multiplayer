//
// Created by Vetle Hjelle on 20/12/2022.
//

//#include "memory.h"

#ifdef __cplusplus

#include "common.h"
#include "logger.h"

#endif

// NOTE: Most of the memory allocation code was generated by ChatGPT. It seems to work, but I never really properly battle tested it.

char* memory_area[1000000];
struct memory_block* head;

void init_memory_allocator(void* start, size_t size) {
    // Create the head of the linked list
    head = (struct memory_block*)start;
    head->size = size - sizeof(struct memory_block);
    head->is_allocated = 0;
    head->next = NULL;
}

void *allocate_memory(size_t size) {
#ifdef __cplusplus
    Logger::trace("Allocating memory of size %d", size);
#endif

    // Round up the size to the next multiple of the word size
    size_t word_size = sizeof(void *);
    size = (size + word_size - 1) & ~(word_size - 1);

    // Iterate through the blocks and find the best fit
    struct memory_block *current = head;
    struct memory_block *best_fit = NULL;
    while (current != NULL) {
        if (!current->is_allocated && current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
        }

        if ((unsigned int)current->next < 0x01000000 && (unsigned int)current->next > 0) {
#ifdef __cplusplus
            Logger::error("next memory block at potentially invalid address: 0x%08x (current: 0x%08x:%d)", current->next, current, current->size);
#endif
        }

        current = current->next;
    }

    // If a suitable block was found, split it if necessary and mark it as allocated
    if (best_fit != NULL) {
        // Calculate the size of the remaining free space
        size_t remaining_size = best_fit->size - size;

        // If the best fit block is larger than the allocation request, split it into two blocks. Don't split blocks smaller than 8 bytes.
        if (remaining_size > 0) {
#ifdef __cplusplus
            Logger::trace("Making a new block for size %d", remaining_size);
#endif

            struct memory_block *new_block = (struct memory_block *)((char *)best_fit + sizeof(struct memory_block) + size);
            new_block->size = remaining_size;
            new_block->is_allocated = 0;
            new_block->next = best_fit->next;
            best_fit->next = new_block;
            best_fit->size = size;

            if (new_block->size <= 0 || best_fit->size <= 0) {
#ifdef __cplusplus
                Logger::error("Whoops. Just made a block with size 0. best_fit->size: %d; new_block->size: %d", best_fit->size, new_block->size);
#endif
            }
        }

        best_fit->is_allocated = 1;
        return (void *)((unsigned long)best_fit + sizeof(struct memory_block));
    }

#ifdef __cplusplus
    Logger::critical("Couldn't allocate memory for size %d", size);
#endif

    // If no suitable block was found, return NULL
    // FIXME: This potentially means we've split all the chunks into too small parts. We should probably join some chunks. (Maybe more relevant in `free_memory`)
    return NULL;
}

void free_memory(void *ptr) {
    struct memory_block *current = (struct memory_block*)((char*)ptr - sizeof(struct memory_block));
    current->is_allocated = 0;

#ifdef __cplusplus
    Logger::trace("Freed memory at 0x%08x:%d; next: 0x%p", current, current->size, current->next);
#endif
}

#ifdef __cplusplus

void* operator new(size_t size) {
    void* pointer = allocate_memory(size);

    return pointer;
}

void operator delete(void* pointer) {
    free_memory(pointer);

    return;
}
#endif
