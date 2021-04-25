#ifndef RANDOM_H
#define RANDOM_H

#include "lib/common.h"

// random prime
#define RANDOM_INITIAL_SEED 0xBBD583E3
#define RANDOM_MAX          0x7FFFFFFF

// Sets the random seed.
void randomSetSeed( u32 seed );

// Generates a random signed integer.
s32 randomInt();

// Generates a random normalized float (0-1)
f32 randomFloat();

// Generates a random signed integer between min (inclusive) and max (inclusive).
s32 randomIntBetween( s32 min, s32 max );

// Generates a random float between min (inclusive) and max (inclusive).
f32 randomFloatBetween( f32 min, f32 max );

#endif