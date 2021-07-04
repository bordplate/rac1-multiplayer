#ifndef RANDOM_H
#define RANDOM_H

#include "lib/common.h"

/**
 * @brief Default seed prime number used for random number generation.
 * 
 */
#define RANDOM_INITIAL_SEED 0xBBD583E3

/**
 * @brief Max value of a randomly generated int.
 * 
 */
#define RANDOM_MAX          0x7FFFFFFF

/**
 * @brief Sets the random seed.
 * 
 * @param seed The seed value.
 */
void randomSetSeed( u32 seed );

/**
 * @brief Generates a random signed integer.
 * 
 * @return s32 The randomly generated int value.
 */
s32 randomInt();

/**
 * @brief Generates a random normalized float (0-1).
 * 
 * @return f32 The randomly generated float value.
 */
f32 randomFloat();

/**
 * @brief Generates a random signed integer between min (inclusive) and max (inclusive).
 * 
 * @param min The minimum value of the random integer (inclusive).
 * @param max The maximum value of the random integer (inclusive).
 * @return s32 The randomly generated int value.
 */
s32 randomIntBetween( s32 min, s32 max );

/**
 * @brief Generates a random flaot between min (inclusive) and max (inclusive).
 * 
 * @param min The minimum value of the random float (inclusive).
 * @param max The maximum value of the random float (inclusive).
 * @return s32 The randomly generated float value.
 */
f32 randomFloatBetween( f32 min, f32 max );

#endif