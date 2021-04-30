// Make sure to preface your H files with a guard for which game they're intended to be used for.
#ifdef GAME_TEST

// Make sure you include header guards for all your header files like this.
// They're in the format of 'FILENAME_EXTENSION'
#ifndef TESTMODULE_H
#define TESTMODULE_H

// Make sure to include the common header
#include "lib/common.h"

// This is how you export functions for other C files to use.
// The PRX loader calls these functions for you.

/**
 * @brief Initializes the test module.
 */
void testModuleInit( void );

/**
 * @brief Shutdown the test module.
 */
void testModuleShutdown( void );

// You define your structs, functions, and other declarations here.
// Note: do not put any definitions here. Meaning, no code, no assigned variables, only declarations.

// This is how you define structures.
typedef struct exStruct2
{
    char* stringValue; // C doesn't have strings, instead you use char*
} exStruct2;

typedef struct exStruct
{
    u32 intValue;   // this is how you define integer (4 byte) fields
    f32 floatValue; // this is how you define float fields
    exStruct2* structPointerValue; // this is how you define structure pointer
} exStruct;


// This is an example of how you declare prototypes of game functions in the header file.
// In a prototype, you specify the signature of the function and you add documentation for other users
// on how to use it as intended.

/**
 * @brief Plays an SFX. aaaa
 * 
 * @param a0 a
 * @param a1 
 * @param a2 a
 * @param a3 a
 * @param a4 
 * @param a5 
 * @return s32 
 */
s32 testModulePlaySfx( s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5 );

/**
 * @brief Sets the next game sequence (AKA game mode/state)
 * 
 * @param seqId The ID of the sequence.
 * @param params Pointer to the param data.
 * @param paramsSize Size of the param data.
 * @param r6 
 * @return s32 
 */
s32 testModuleSetSeq( s32 seqId, void* params, s32 paramsSize, s32 r6 );

//SHK_FUNCTION_DEFINE_6( 0x10F0D4, s32, playSfx, s32, a0, s32, a1, s32, a2, s32, a3, s32, a4, s32, a5 );
//SHK_FUNCTION_DEFINE_4( 0x10DB4, s32, setSeq, s32, seqId, void*, params, s32, paramsSize, s32, r6 );


#endif // TESTMODULE_H
#endif // GAME_TEST