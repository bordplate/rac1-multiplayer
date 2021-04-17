// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>

// Pre-prepared libraries exist in lib
// Common includes things like printf for printf, strlen, etc.
// PRX dont have access to the usual C libraries, so any functionality that you need from it
// will have to be reimplemented in there.
#include "lib/common.h"

// SHK (Static Hook library)
#include "lib/shk.h"

// include PRX runtime header
#include "runtime/runtime.h"

// Include the header file in which type are defined
#include "mod.h"

// PRX initialization boilerplate
s32 _start( void );
SYS_MODULE_INFO( modPrx, 0, 1, 1 );
SYS_MODULE_START( _start );
SYS_MODULE_STOP( _stop );

// typedef s32 (*SoundManager_PlaySfx_t)(s32, s32, s32, s32, s32, s32);

// Game functions are declared like this
// R, meaning result, and 6, the number of parameters
// A void function does not return anything, and requires you to use SHK_FUNCTION_V<N> instead.
// If you use vscode youll get descriptive tooltips when you hover these macros.
SHK_FUNCTION_R6( 0x10F0D4, s32, sndManPlaySfx, s32, a0, s32, a1, s32, a2, s32, a3, s32, a4, s32, a5 );

// Copied & adapted from IDA decompiler output
s32 sndManPlayBgm(s32 a1)
{
    s32 v1; // r31
    s32 result; // r3

    v1 = a1;
    sndManPlaySfx( 0, 0, a1, 0, -1, -1 );

    // This is how you write to memory addresses in C
    *(s32*)0xCFF4C0 = result;
    *(s32*)0xCFF4C4 = v1;

    return result;
}

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, setBgm, s32 id );

// This is the handler function for the setBgm hook.
// It's linked to the hook in _start, through SHK_BIND_HOOK
void setBgmHook( s32 id )
{
    printf( "set bgm: %d\n", id );
    sndManPlayBgm( id );
    
    // SHK_CALL_ORIGINAL( setBgm, id + 1 );
}

SHK_HOOK( s32, setSeq, s32 seqId, void* params, s32 paramsSize, s32 r6 );
s32 setSeqHook( s32 seqId, void* params, s32 paramsSize, s32 r6 )
{
    printf( "set seq: %d\n", seqId );

    // Calling the original unhooked function is done like this.
    return SHK_CALL_HOOK( setSeq, seqId, params, paramsSize, r6 );
}

long factorial( s32  n)
{
    if ( n == 0 )
        return 1;
    else
        return ( n * factorial( n - 1 )) ;
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
s32 _start( void )
{
    // These prs32s show up in the TTY log if everything is working as it should.
    printf( "hello world from prx\n" );

    // Make sure to initialize the runtime
    runtimeInit();

    // Example of prs32ing values with printf.
    // See printf format reference on google for more info.

    // Print s32eger with %d
    s32 foo = 420;
    printf( "foo = %d\n", foo );
    
    // Print f32 with %f
    f32 bar = 69.0f;
    printf( "bar = %f\n", bar );

    // Print hex s32eger with 0x%08X
    s32 qux = 0xDEADBABE;
    printf( "qux = 0x%08X\n", qux );

    // Now all together
    printf( "foo=%d bar=%f qux=0x%08X\n", foo, bar, qux );

    // Call your own functions
    printf( "factorial of 5 is %d\n", factorial( 5 ) );

    // Hooks must be 'bound' to a handler like this in the start function.
    // If you don't do this, the game will crash.
    SHK_BIND_HOOK( setBgm, setBgmHook );
    SHK_BIND_HOOK( setSeq, setSeqHook );

    // Here you could potentially start a thread that runs in the background, if you want to react to button inputs etc.
    // I'll figure this out later.

    // Our job is done. 
    printf( "goodbye world from prx\n" );
    return SYS_PRX_START_OK;
}

s32 _stop( void )
{
    // Executed when the PRX module is unloaded.
    return SYS_PRX_STOP_OK;
}
