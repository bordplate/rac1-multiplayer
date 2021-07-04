// Make sure to preface your C files with a guard for which game they're intended to be used for.
#ifdef GAME_TEST

// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>

// Pre-prepared libraries exist in lib
// Common includes things like printf for printf, strlen, etc.
// PRX dont have access to the usual C libraries, so any functionality that you need from it
// will have to be reimplemented in there.
// Make sure you include headers in the right order!
// You can only use things that have been declared before their usage.
#include "lib/common.h"

// Include the header file in which type are defined
// See the header file for more information
#include "testmodule.h"

// Include our shared "game" header. 
#include "test.h"

// Define logging functions for this module
// Automatically add prefix to every logged messag 
#define TEST_DEBUG_LOG 0
#define TEST_LOG( msg, ... ) printf( "testmodule: " msg, ##__VA_ARGS__ )
#if TEST_TRACE_LOG
#define TEST_DEBUG( msg, ... ) printf( "testmodule: " msg, ##__VA_ARGS__ )
#else
#define TEST_DEBUG( msg, ... ) do {} while ( false )
#endif

// IMPORTANT: declare anything you don't want publicly accessible as 'static'
// Failure to do so will cause redefinition errors whenever you or someone else happens
// to use the same name for a variable or function.

//
// Variable examples
//
// Example string
static const char* helloWorld = "Hello World";

// Example global variables
// Note: to make these accessible in other C files, you'll have to add their 
// declaration (that is everything without the value) to the header file.
static u32 exampleIntGlobal = 1;
static f32 exampleFloatGlobal = 1.0f;

// you can initialize structs like this
// each value corresponds to a field
// in this case: intValue, floatValue, structPointerValue
static exStruct exampleStructGlobal = { 420, 69.420f, NULL }; 
static exStruct2 exampleStructGlobal2 = { "TEST" };

// You can make an array of structs too
// Useful for tables of related data, like skill ids.
static exStruct exampleStructArray[] = 
{
    // In this example instead of null, the address of the exStruct2 is used to fill in the value
    { 1, 2.0f, &exampleStructGlobal2 },
    { 2, 4.0f, &exampleStructGlobal2 },
};

#ifdef GAME_P5

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, setBgm, s32 id );

// This is the handler function for the setBgm hook.
// It's linked to the hook in _start, through SHK_BIND_HOOK
static void setBgmHook( s32 id )
{
    // prints current bgm id
    TEST_LOG( "set bgm: %d\n", id );

    // Copied & adapted from IDA decompiler output
    // Simple functions decompile pretty accurately
    s32 v1; // r31
    s32 result; // r3

    v1 = id;

    // Call our function from the shared game header
    testPlaySfx( 0, 0, id, 0, -1, -1 );

    // This is how you write to memory addresses in C
    *(s32*)0xCFF4C0 = result;
    *(s32*)0xCFF4C4 = v1;
    
    // Normally you'd call the original hook, but in this example
    // I adapted the pseudocode output of IDA to make changes
    // SHK_CALL_ORIGINAL( setBgm, id + 1 );
}

SHK_HOOK( s32, setSeq, s32 seqId, void* params, s32 paramsSize, s32 r6 );
static s32 setSeqHook( s32 seqId, void* params, s32 paramsSize, s32 r6 )
{
    // Prints the current sequence id
    TEST_LOG( "set seq: id=%d paramsSize=%d r6=%d\n", seqId, paramsSize, r6 );

    // Use hex dump to dump the param data
    if ( params != NULL && paramsSize > 0 )
        hexDump( "seq params", params, paramsSize );

    // Calling the original unhooked function is done like this.
    return SHK_CALL_HOOK( setSeq, seqId, params, paramsSize, r6 );
}

#endif

// Calculates the factorial of N
// Used to illustrate that you can define functions as normal in the PRX code as well, aside from hooks.
static long factorial( s32 n )
{
    if ( n == 0 )
        return 1;
    else
        return ( n * factorial( n - 1 )) ;
}

static TtyCmdStatus ttyAddCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
    f32 a = floatParse( args[0] );
    f32 b = floatParse( args[1] );
    printf( "%f\n", a + b );
    return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyEchoCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
    for ( u32 i = 0; i < argc; ++i )
    {
        printf( "%s ", args[i] );
    }

    printf( "\n" );
    return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetBgmCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
    s32 id = intParse( args[0] );

#ifdef GAME_P5
    // only want to call this when compiling for p5
    testPlaySfx( 0, 0, id, 0, -1, -1 );
#endif

    return TTY_CMD_STATUS_OK;
}

typedef struct
{
    s16 fieldMajorId;
    s16 fieldMinorId;
    s16 envMajorId;
    s16 envMinorId;
    s16 field08;
    s16 field0a;
    s16 field0c;
    s16 field0e;
    s16 field10;
    s16 field12;
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1a;
    s16 field1c;
    s16 field1e;
} seqFieldParams;

typedef struct
{
    s16 field00;        // 16
    s16 field04;        // 0
    s16 encounterId;    // 100
    s16 fieldMajorId;   // 252
    s16 fieldMinorId;   // 2
    s16 envMajorId;     // 252
    s16 envMinorId;     // 2
    s16 field10;        // 0
    s16 field12;        // 0
    s16 field14;        // 0
    s16 field16;        // 0
    s16 field18;        // 0
    s16 field1a;        // 0
    s16 field1c;        // 0
    s16 field1e;        // 0
    s16 field20;        // -1
    s16 field22;        // -1
    s16 field24;        // -1
    s16 field26;        // -1
    s16 field28;        // -1
} seqBtlParams;

static TtyCmdStatus ttySetSeqCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
    s32 id = intParse( args[0] );

#ifdef GAME_P5
    if ( id == 3 )
    {
        seqFieldParams params = {};
        params.fieldMajorId = intParse( args[1] );
        params.fieldMinorId = intParse( args[2] );
        params.envMajorId = params.fieldMajorId;
        params.envMinorId = params.fieldMinorId;
        params.field08 = 0xb;
        params.field0c = -1;
        testSetSeq( id, &params, sizeof(seqFieldParams), 0 );
    }
    else if ( id == 4 )
    {
        seqBtlParams params = {};
        params.field00 = 16;
        params.encounterId = intParse( args[1] );
        params.fieldMajorId = 252;
        params.fieldMinorId = 2;
        params.envMajorId = 252;
        params.envMinorId = 2;
        params.field20 = -1;
        params.field22 = -1;
        params.field24 = -1;
        params.field26 = -1;
        params.field28 = -1;
        testSetSeq( id, &params, sizeof(seqBtlParams), 4 );
    }
    else
    {
        testSetSeq( id, NULL, 0, 0 );
    }
#endif

    return TTY_CMD_STATUS_OK;
}

// List of commands that can be handled by the command listener
TtyCmd ttyCommands[] =
{
    TTY_CMD( ttyAddCmd, "add", "Adds 2 numbers and prints the result", TTY_CMD_FLAG_NONE,
        TTY_CMD_PARAM( "a", "The first number to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_FLOAT ), 
        TTY_CMD_PARAM( "b", "The second number to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_FLOAT )),

    TTY_CMD( ttyEchoCmd, "echo", "Prints the given input back to you", TTY_CMD_FLAG_VARARGS ),

    TTY_CMD( ttySetBgmCmd, "setbgm", "Sets the current BGM using the Cue ID", TTY_CMD_FLAG_NONE,
        TTY_CMD_PARAM( "cueId", "The Cue ID of the BGM to play", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),
        
    TTY_CMD( ttySetSeqCmd, "setseq", "Sets the current sequence", TTY_CMD_FLAG_VARARGS,
        TTY_CMD_PARAM( "seqId", "The sequence ID", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT ) ),

    TTY_CMD_END(), 
};

#ifdef GAME_P5

SHK_HOOK( u64, mainUpdate, f32 deltaTime );
static u64 mainUpdateHook( f32 deltaTime )
{
    // Process TTY commands
    ttyCmdProcess( ttyCommands );
    return SHK_CALL_HOOK( mainUpdate, deltaTime );
}

#endif

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void testModuleInit( void )
{
    // These prints show up in the TTY log if everything is working as it should.
    TEST_LOG( "hello world\n" );

    if ( CONFIG_ENABLED( debug ) )
    {
        // Example 
        TEST_LOG( "debug enabled via config\n" );
    }

    TEST_LOG( "Message of the day: %s\n", CONFIG_STRING( testMessage ) );

    u32 maxRandomMessageIndex = CONFIG_ARRAY_COUNT( testRmotd ) - 1;
    u32 randomMessageIndex = ( randomIntBetween(0, maxRandomMessageIndex) + 1 ) % maxRandomMessageIndex;
    TEST_LOG( "Random message of the day: %s\n", CONFIG_STRING_ARRAY( testRmotd )[randomMessageIndex] );

    // Example of printing values with printf.
    // See printf format reference on google for more info.

    // Print integer with %d
    s32 foo = 420;
    TEST_LOG( "foo = %d\n", foo );
    
    // Print float with %f
    f32 bar = 69.0f;
    TEST_LOG( "bar = %f\n", bar );

    // Print hex integer with 0x%08X
    s32 qux = 0xDEADBABE;
    TEST_LOG( "qux = 0x%08X\n", qux );

    // Now all together
    TEST_LOG( "foo=%d bar=%f qux=0x%08X\n", foo, bar, qux );

    // Call your own functions
    TEST_LOG( "factorial of 5 is %d\n", factorial( 5 ) );

    // Hooks must be 'bound' to a handler like this in the start function.
    // If you don't do this, the game will crash with a 0xDEADBABE access error

#ifdef GAME_P5
    // Don't bind those hook unless the game we're building for is actually P5
    // as to not crash.
    SHK_BIND_HOOK( setBgm, setBgmHook );
    SHK_BIND_HOOK( setSeq, setSeqHook );

    // Handle command handling in main update function
    // to prevent crashes
    SHK_BIND_HOOK( mainUpdate, mainUpdateHook );
#endif

    // Here you could potentially start a thread that runs in the background, if you want to react to button inputs etc.

    // Start TTY command listener
#ifndef GAME_P5
    ttyCmdStartListenerThread( ttyCommands );
#endif

    // Our job is done. 
    TEST_LOG( "goodbye world\n" );
}

void testModuleShutdown( void )
{
    // Executed when the PRX module is unloaded.    
}

#endif
