#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/**
 * @brief Copies a range of memory from source to destination.
 * 
 * @param destination 
 * @param destinationLength 
 * @param source 
 * @param count 
 * @return void* 
 */
void* memoryCopy( void* destination, size_t destinationLength, const void* source, size_t count );

/**
 * @brief Clears a range of memory with 0 bytes.
 * 
 * @param destination 
 * @param destinationLength 
 * @return void* 
 */
void* memoryClear( void* destination, size_t destinationLength );

/**
 * @brief Parses a signed decimal or hexadecimal integer value from the given string.
 * 
 * @param s The input string.
 * @return s32 The parsed integer value.
 */
s32 intParse( const char* s );

/**
 * @brief Parses an unsigned decimal or hexadecimal integer value from the given string.
 * 
 * @param s The input string.
 * @return s32 The parsed integer value.
 */
u32 uintParse( const char* s );

/**
 * @brief Parses a single-precision floating point value from the given string.
 * 
 * @param s The input string.
 * @return s32 The parsed float value.
 */
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