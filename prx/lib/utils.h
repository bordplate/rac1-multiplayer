#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copies a range of memory from source to destination.
 * 
 * @param destination 
 * @param destinationLength 
 * @param source 
 * @param count 
 * @return void* 
 */
void *memoryCopy(void *destination, size_t destinationLength, const void *source, size_t count);

/**
 * @brief Clears a range of memory with 0 bytes.
 * 
 * @param destination 
 * @param destinationLength 
 * @return void* 
 */
void *memoryClear(void *destination, size_t destinationLength);

/**
 * @brief Parses a signed decimal or hexadecimal integer value from the given string.
 * 
 * @param s The input string.
 * @return s32 The parsed integer value.
 */
s32 intParse(const char *s);

/**
 * @brief Parses an unsigned decimal or hexadecimal integer value from the given string.
 * 
 * @param s The input string.
 * @return s32 The parsed integer value.
 */
u32 uintParse(const char *s);

/**
 * @brief Parses a single-precision floating point value from the given string.
 * 
 * @param s The input string.
 * @return s32 The parsed float value.
 */
f32 floatParse(const char *s);

/**
 * @brief Parses a double-precision floating point value from the given string.
 * 
 * @param s The input string.
 * @return s32 The parsed double value.
 */
f64 doubleParse(const char *s);

/**
 * @brief Prints a hex dump of the specified memory range to the console.
 * 
 * @param description The description of the hex dump
 * @param address The starting address of the memory to dump
 * @param length The length/size of the memory range, starting from the start address, to dump.
 */
void hexDump(char *description, void *address, u32 length);

/**
 * @brief Reads a line of input from the TTY terminal.
 * 
 * @return const char* 
 */
const char *ttyReadLine();


typedef void(*threadEntryFn)(u64 arg);

/**
 * @brief Creates a new thread with a specified entry (main) function, an argument, and a name, in suspended state.
 * 
 * @param entry The entrypoint function of the thread.
 * @param arg The argument passed to the entrypoint function.
 * @param name The name of the thread.
 * @return u64 
 */
u64 threadCreate(threadEntryFn entry, u64 arg, const char *name);

/**
 * @brief Runs the specified thread.
 * 
 * @param threadId 
 */
void threadRun(u64 threadId);

/**
 * @brief Creates a new thread with a specified entry (main) function, an argument, and a name, an executes it.
 * 
 * @param entry 
 * @param arg 
 * @param name 
 * @return u64 
 */
u64 threadCreateAndRun(threadEntryFn entry, u64 arg, const char *name);

/**
 * @brief Sleeps the current thread for a specified amount of milliseconds.
 * 
 * @param ms The number of milliseconds to sleep.
 */
void threadSleep(u64 ms);

#ifdef __cplusplus
}
#endif

#endif