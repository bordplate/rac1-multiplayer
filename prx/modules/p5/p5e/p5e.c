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
#include "p5e.h"

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, setBgm, int id );
SHK_HOOK( void, SetPlayerUnitEquipment, btlUnit_Unit* player, u32 equipType, u16 itemID );
SHK_HOOK( void*, LoadEPL, char* EPL, u8 a2 );
SHK_HOOK( int, GenericCharacterModelLoader, char* result, u64 modelType, u64 characterID, u64 modelID, u64 modelSubID );

btlEquipBgmTableEntry btlEquipBgmTable[] =
{
  { 159, 2000 }, // DLC_001
  { 158, 2002 }, // DLC_002
  { 161, 2004 }, // DLC_003
  { 160, 2006 }, // DLC_004
  { 163, 2008 }, // DLC_005
  { 166, 2102 }, // DLC_006
  { 167, 2201 }, // DLC_007
  { 168, 2203 }, // DLC_008
  { 169, 2205 }, // DLC_009
  { 162, 2207 }, // DLC_010
  { 170, 2209 }, // DLC_011
  { 171, 2211 }, // DLC_012
  { 172, 2213 }, // DLC_013
  { 173, 2215 }, // DLC_014
  { 174, 2217 }, // DLC_015
  { 175, 2310 }, // DLC_016
  { 176, 2330 }, // DLC_017
  { 177, 2332 }, // DLC_018
  { 178, 2334 }, // DLC_019
  { 179, 2336 }, // DLC_020
};

void setBgmHook( int id )
{
  printf( "bgm ID = %04d\n", id );
  if (id == 300) // Last Surprise
  {
    u32 btlEquipBgmTableEntryCount = sizeof( btlEquipBgmTable ) / sizeof( btlEquipBgmTableEntry );
    u32 playerOutfitModel = PlayerUnitGetModelMinorID( 1, 50, 0 );
    for ( u32 i = 0; i < btlEquipBgmTableEntryCount; ++i )
    {
      btlEquipBgmTableEntry* pEntry = &btlEquipBgmTable[i];
      if ( pEntry->modelID == playerOutfitModel )
      {
        id = pEntry->bgmId;
        break;
      }
    }  
    if ( isAmbush )
      {
        id = 907; // Take Over
      }
  }
  else if (id == 340) // Victory theme
  { 
    isAmbush = false;
    u32 btlEquipBgmTableEntryCount = sizeof( btlEquipBgmTable ) / sizeof( btlEquipBgmTableEntry );
    u32 playerOutfitModel = PlayerUnitGetModelMinorID( 1, 50, 0 );
    for ( u32 i = 0; i < btlEquipBgmTableEntryCount; ++i )
    {
      btlEquipBgmTableEntry* pEntry = &btlEquipBgmTable[i];
      if ( pEntry->modelID == playerOutfitModel )
      {
        id = pEntry->bgmId + 1;
        break;
      }
    }  
  }
  SHK_CALL_HOOK( setBgm, id );
}

static CharModelReplacementTable charModelReplacementTableEntry[] =
{
  { 51, 51, 51, 51, 51 }, // 0
  { 51, 52, 81, 103, 113 }, // 1
  { 51, 81, 103, 106, 51 }, // 2
  { 51, 61, 81, 101, 51 }, // 3
  { 51, 81, 106, 107, 51 }, // 4
  { 51, 81, 103, 106, 51 }, // 5
  { 51, 81, 104, 107, 51 }, // 6
  { 51, 81, 102, 10, 51 }, // 7
  { 51, 81, 102, 106, 51 }, // 8
  { 51, 52, 81, 102, 104 }, // 9
};

int GenericCharacterModelLoaderHook( char* result, u64 modelType, u64 characterID, u64 modelID, u64 modelSubID )
{
  if ( ( modelType == 2 || modelType == 5 ) && characterID <= 10 )
  {
    if ( characterID == 9 && modelID == 51 && GetEquippedPersonaFunction(9) != Persona_RobinHood ) // Darkechi model
    {
      modelID = 52;
    }
    for ( int i = 1; i <= 9; i++ )
    {
      if ( i == characterID )
      {
        CharModelReplacementTable* pEntry = &charModelReplacementTableEntry[i];
        for ( int j = 0; j <= 4; j++ )
        {
          u32 playerOutfitModel = PlayerUnitGetModelMinorID( characterID, 50, 0 );
          if ( modelID == pEntry->modelId[j] && playerOutfitModel != 51 )
          {
            modelID = playerOutfitModel;
          }
        }
      }
    }
    if ( characterID == 10 && modelID == 50 ) //for now copy Joker outfit to Kasumi
    {
      modelID = PlayerUnitGetModelMinorID( 1, 50, 0 );
    }
  }
  return SHK_CALL_HOOK( GenericCharacterModelLoader, result, modelType, characterID, modelID, modelSubID );
}

void SetPlayerUnitEquipmentHook( btlUnit_Unit* player, u32 equipType, u16 itemID )
{
  if ( player->unitID == 1 && equipType == 3 )
  {
    printf("Setting Kasumi outfit from Joker to %d\n", itemID );
    SHK_CALL_HOOK( SetPlayerUnitEquipment, GetBtlUnitFromID(10), equipType, itemID );
  }
  return SHK_CALL_HOOK( SetPlayerUnitEquipment, player, equipType, itemID );
}

void* LoadEPLHook( char* EPL, u8 a2 )
{
  if ( strcmp( EPL, "battle/gui/bes_ui_p_advantage.EPL" ) == 0 )
  {
    isAmbush = true;
  }
  return SHK_CALL_HOOK( LoadEPL, EPL, a2 );
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void p5eInit( void )
{
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( setBgm, setBgmHook );
  SHK_BIND_HOOK( LoadEPL, LoadEPLHook );
  SHK_BIND_HOOK( SetPlayerUnitEquipment, SetPlayerUnitEquipmentHook );
  SHK_BIND_HOOK( GenericCharacterModelLoader, GenericCharacterModelLoaderHook );
}

void p5eShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif