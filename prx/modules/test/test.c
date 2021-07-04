// Make sure to preface your C files with a header guard for which game they're intended to be used for.
#ifdef GAME_TEST

// Include associated header file
#include "test.h"

// Game functions need to be defined like this before you can use them.
// These functions' prototypes are in the header file, and are thus publicly accessible
// to anyone who includes your header.

s32 testPlaySfx( s32 a0, s32 a1, s32 cueId, s32 a3, s32 a4, s32 a5 ) 
{ 
    // 6 indicating the number of parameters
    // If you use vscode youll get descriptive tooltips when you hover these macros.
#ifdef GAME_P5
    SHK_FUNCTION_CALL_6( 0x10F0D4, s32, s32, a0, s32, a1, s32, cueId, s32, a3, s32, a4, s32, a5 ); 
#endif
}

s32 testSetSeq( s32 seqId, void* params, s32 paramsSize, s32 r6 ) {
#ifdef GAME_P5
    SHK_FUNCTION_CALL_4( 0x10DB4, s32, s32, seqId, void*, params, s32, paramsSize, s32, r6 );
#endif
}

// The benefit of this approach is that you can add doxygen comments to these prototypes, and they'll
// be available through intellisense. It also gives you more control over how the function is called.
// The shorthand alternative would be
// SHK_FUNCTION_DEFINE_STATIC( 0x10F0D4, testPlaySfx, s32, s32, a0, s32, a1, s32, a2, s32, a3, s32, a4, s32, a5) OR
// SHK_FUNCTION_DEFINE( ... )

// This variable was declared in the shared game header, but now we have to define it.
// Example definition string array
const char* testPartyMemberNames[] =
{
    "None",
    "Joker",
    "Ryuji",
    "Morgana",
    "Ann",
    "Yusuke",
    "Makoto",
    "Haru",
    "Futaba",
    "Akechi"
};

// You'd use it like so:
const char* testGetPartyMemberName( s32 id )
{
    return testPartyMemberNames[ id ];
}

#endif

