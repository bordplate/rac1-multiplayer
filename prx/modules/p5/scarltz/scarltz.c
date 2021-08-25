#ifdef GAME_P5
// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>
// Pre-prepared libraries exist in lib
// Common includes things like printf for printing, strlen, etc.
// PRX dont have access to the usual C libraries, so any functionality that you need from it
// will have to be reimplemented in there.
#include "lib/common.h"

// SHK (Static Hook library)
#include "lib/shk.h"

// Include this to use config variables
#include "lib/config.h"

#include "modules/p5/p5.h"
#include "scarltz.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.


SHK_HOOK( u64, ResistancePassiveCheck, btlUnit_Unit* btlUnit, short ElementID);
//SHK_HOOK( bool, CheckHasSkill, btlUnit_Unit* btlUnit, int SkillID);
/*
// Resistance function specific start
SHK_HOOK( void, FUN_00263728, btlUnitContextInfo context, short ElementID);
SHK_HOOK( u64, FUN_00250000);
SHK_HOOK( u64, FUN_0025d0c4, u32 UnitId, short ElementID);
SHK_HOOK( u64, FUN_002595c4, int btlUnit, u32 Unknown);
// Resistance function specific end
*/
u32 resist = 0x10000000;
u32 null = 0x1000000;
u32 repel = 0x2000000;
u32 drain = 0x4000000;


static u64 ResistancePassiveCheckHook( btlUnit_Unit* btlUnit, short ElementID ){
    return SHK_CALL_HOOK ( ResistancePassiveCheck, btlUnit, ElementID )
}


// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void scarltzInit( void ){
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  //SHK_BIND_HOOK( ResistancePassiveCheck, ResistancePassiveCheckHook );

}

void scarltzShutdown( void ){
  // Executed when the PRX module is unloaded.    
}
#endif