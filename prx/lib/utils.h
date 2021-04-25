#ifndef UTILS_H
#define UTILS_H

#include "common.h"

// MEMORY
void* memoryCopy( void* destination, size_t destinationLength, const void* source, size_t count );
void* memoryClear( void* destination, size_t destinationLength );

// PARSE
s32 intParse( const char* s );
u32 uintParse( const char* s );
f32 floatParse( const char* s );

/**
 * @brief Prints a hex dump of the specified memory range to the console.
 * 
 * @param description The description of the hex dump
 * @param address The starting address of the memory to dump
 * @param length The length/size of the memory range, starting from the start address, to dump.
 */
void hexDump( char *description, void *address, u32 length );

#endif