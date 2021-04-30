#ifdef GAME_TEST
#ifndef TEST_H
#define TEST_H

// This is the shared "game" header.
// It's good practice to put all the stuff that can be shared between modules in a shared header file.

#include "lib/common.h"

// Example of declaring shared game variables/functions that can be used in multiple modules

/**
 * @brief Array of party member names.
 * 
 */
const char* testPartyMemberNames[10];

/**
 * @brief Gets the party member name associated with the given id.
 * 
 * @param id The id of the party member.
 * @return const char* A string containing the name of the party member. 
 */
const char* testGetPartyMemberName( s32 id );

// This is an example of how you declare prototypes of game functions in the header file.
// In a prototype, you specify the signature of the function and you add documentation for other users
// on how to use it as intended.

/**
 * @brief Plays an SFX using a specified cue ID.
 * 
 * @param a0 
 * @param a1 
 * @param cueId The Cue ID of the sound to play.
 * @param a3 
 * @param a4 
 * @param a5 
 * @return s32 
 */
s32 testPlaySfx( s32 a0, s32 a1, s32 cueId, s32 a3, s32 a4, s32 a5 );

/**
 * @brief Sets the next game sequence (AKA game mode/state)
 * 
 * @param seqId The ID of the sequence.
 * @param params Pointer to the param data.
 * @param paramsSize Size of the param data.
 * @param r6 Unknown, specify 0.
 * @return s32 
 */
s32 testSetSeq( s32 seqId, void* params, s32 paramsSize, s32 r6 );

#endif
#endif