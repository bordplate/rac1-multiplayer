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
void testModuleInit( void );
void testModuleShutdown( void );

// You define your structs, functions, and other declarations here.
// Note: do not put any definitions here. Meaning, no code, no assigned variables, only declarations.

// This is how you define structures.
typedef struct
{
    char* stringValue; // C doesn't have strings, instead you use char*
} exStruct2;

typedef struct
{
    u32 intValue;   // this is how you define integer (4 byte) fields
    f32 floatValue; // this is how you define float fields
    exStruct2* structPointerValue; // this is how you define structure pointer
} exStruct;

#endif // TESTMODULE_H
#endif // GAME_TEST