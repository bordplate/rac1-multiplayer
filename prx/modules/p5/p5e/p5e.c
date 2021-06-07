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

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, setBgm, int id );
SHK_HOOK( int, FUN_0072360c, int id );
SHK_HOOK( void*, LoadEPL, char* EPL, u8 a2 );
SHK_HOOK( int, criFs_Initialize );
SHK_HOOK( u16, LoadMeleeWeaponModelTable, int a1 );
SHK_HOOK( u16, LoadGunWeaponModelTable, int a1 );
SHK_HOOK( u16, FUN_0003a658, int a1 );
SHK_HOOK( u16, FUN_0003a698, int a1 );
SHK_HOOK( int, crifsloader_load_registered_file, fileAccessStruct* a1, int a2, int a3, int a4, int a5 );
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

static void setBgmHook( int id )
{
  if ( id == 300 && CONFIG_ENABLED( enableExpandedBGM ) ) // Last Surprise
  {
    u32 btlEquipBgmTableEntryCount = sizeof( btlEquipBgmTable ) / sizeof( btlEquipBgmTableEntry );
    u32 playerOutfitModel = PlayerUnitGetModelMinorID( 1, 50, 0 );
    for ( u32 i = 0; i < btlEquipBgmTableEntryCount; ++i )
    {
      btlEquipBgmTableEntry* pEntry = &btlEquipBgmTable[i];
      if ( pEntry->modelID == playerOutfitModel )
      {
        id = pEntry->bgmId;
        isAmbush = false;
        break;
      }
    }
    if ( isAmbush )
    {
      id = 907; // Take Over
    }
  }
  else if ( id == 340 && CONFIG_ENABLED( enableExpandedBGM ) ) // Victory theme
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

static int GenericCharacterModelLoaderHook( char* result, u64 modelType, u64 characterID, u64 modelID, u64 modelSubID )
{
  if ( ( modelType == 2 || modelType == 5 ) && characterID <= 10 )
  {
    if ( characterID == 9 && modelID == 51 && GetEquippedPersonaFunction(9) != Persona_RobinHood ) // Darkechi model
    {
      modelID = 52;
    }
    CharModelReplacementTable* pEntry = &charModelReplacementTableEntry[characterID];
    for ( int j = 0; j <= 4; j++ )
    {
      u32 playerOutfitModel = PlayerUnitGetModelMinorID( characterID, 50, 0 );
      if ( modelID == pEntry->modelId[j] && playerOutfitModel != 51 && CONFIG_ENABLED( enableCutsceneOutfits ) )
      {
        modelID = playerOutfitModel;
      }
    }
    if ( characterID == 10 && modelID == 50 ) //for now copy Joker outfit to Kasumi
    {
      modelID = PlayerUnitGetModelMinorID( 1, 50, 0 );
      if ( CONFIG_ENABLED( enableSumire ) )
      {
        modelSubID = 1;
      }
    }
  }
  else if ( modelType == 4 && characterID > 200 ) // force load regular Persona models instead of using PSZ/Enemy model
  {
    modelType = 3;
  }
  return SHK_CALL_HOOK( GenericCharacterModelLoader, result, modelType, characterID, modelID, modelSubID );
}

static void* LoadEPLHook( char* EPL, u8 a2 )
{
  if ( strcmp( EPL, "battle/gui/bes_ui_p_advantage.EPL" ) == 0 )
  {
    isAmbush = true;
  }
  else if ( strcmp( EPL, "battle/event/BCD/j_sien/bes_j_htb.EPL" ) == 0 && CONFIG_ENABLED( enableCutsceneOutfits ) )
  {
    u32 FutabaOutfit = PlayerUnitGetModelMinorID( 8, 50, 0 );
    if ( FutabaOutfit != 51 )
    {
      char newOutfitEPL[128];
      sprintf(newOutfitEPL, "battle/event/BCD/j_sien/bes_j_htb_%03d.EPL", FutabaOutfit);
      return SHK_CALL_HOOK( LoadEPL, newOutfitEPL, a2 );
    }
  }
  return SHK_CALL_HOOK( LoadEPL, EPL, a2 );
}

static int forceSingleGAP( int playerID )
{
  if ( CONFIG_ENABLED( forceSingleCombatGAP ) )
  {
    return 0;
  }
  else return 1;
}

static int criFs_InitializeHook( void )
{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  char acStack288 [264];

  iVar1 = FUN_00ab563c((int)0x00d4bd54);
  iVar1 = FUN_001a52f8(iVar1);
  if (iVar1 != 2) 
  {
    pcVar2 = FUN_001a5834();
    sprintf(acStack288,"%s/hdd.cpk",pcVar2);
    criFsBinder_BindCpk(acStack288);
  }

  pcVar2 = FUN_00968be8();
  pcVar3 = FUN_00968bf4();
  
  if ( CONFIG_ENABLED( enableModCPK ) )
  {
    iVar1 = sprintf(acStack288,"%s%s/%s.cpk",pcVar2,pcVar3, CONFIG_STRING(modCPKName));
    iVar1 = criFsBinder_BindCpk(acStack288);
  }

  u32 extraCPK = CONFIG_INT( extraModCPK );
  if ( CONFIG_ENABLED( enableModCPK ) && extraCPK > 0 )
  {
    for (int i = 0; i < extraCPK; i++)
    {
      iVar1 = sprintf(acStack288,"%s%s/%s_%02d.cpk", pcVar2, pcVar3, CONFIG_STRING_ARRAY(extraModCPKName)[i], i + 1);
      iVar1 = criFsBinder_BindCpk(acStack288);
    }
  }

  iVar1 = sprintf(acStack288,"%s%s/ps3.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpk(acStack288);

  iVar1 = sprintf(acStack288,"%s%s/data.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpk(acStack288);

  return iVar1;
}

static int crifsloader_load_registered_fileHook( fileAccessStruct* a1, int a2, int a3, int a4, int a5 )
{
  if ( CONFIG_ENABLED( enableFileAccessLogging ) ) printf("%s\n", a1->fileNamePtr);
  return SHK_CALL_HOOK( crifsloader_load_registered_file, a1, a2, a3, a4, a5 );
}

static u16 LoadMeleeWeaponModelTableHook( int a1 )
{
  DEBUG_LOG("Gun model load; a1 -> %d, a1 result -> %d\n", a1, FUN_2604C4(a1));
  a1 = FUN_2604C4(a1);
  u16 result = 1;
  if ( a1 >= 0 && a1 <= 99 )
  {
    result = (u16)CONFIG_INT_ARRAY( MeleeWeaponModelIDTable1 )[a1];
  }
  else if ( a1 >= 100 && a1 <= 199 )
  {
    result = (u16)CONFIG_INT_ARRAY( MeleeWeaponModelIDTable2 )[a1-100];
  }
  else if ( a1 >= 200 && a1 <= 299 )
  {
    result = (u16)CONFIG_INT_ARRAY( MeleeWeaponModelIDTable3 )[a1-200];
  }
  else if ( a1 >= 300 && a1 <= 399 )
  {
    result = (u16)CONFIG_INT_ARRAY( MeleeWeaponModelIDTable4 )[a1-300];
  }
  DEBUG_LOG("Weapon Model ID result -> %d\n", result);
  return result;
}

static u16 LoadGunWeaponModelTableHook( int a1 )
{
  DEBUG_LOG("Gun model load; a1 -> %d, a1 result -> %d\n", a1, FUN_2604C4(a1));
  a1 = FUN_2604C4(a1);
  u16 result = 1;
  if ( a1 >= 0 && a1 <= 99 )
  {
    result = (u16)CONFIG_INT_ARRAY( GunWeaponModelTable1 )[a1];
  }
  else if ( a1 >= 100 && a1 <= 199 )
  {
    result = (u16)CONFIG_INT_ARRAY( GunWeaponModelTable2 )[a1-100];
  }
  else if ( a1 >= 200 && a1 <= 299 )
  {
    result = (u16)CONFIG_INT_ARRAY( GunWeaponModelTable3 )[a1-200];
  }
  else if ( a1 >= 300 && a1 <= 399 )
  {
    result = (u16)CONFIG_INT_ARRAY( GunWeaponModelTable4 )[a1-300];
  }
  DEBUG_LOG("Gun Model ID result -> %d\n", result);
  return result;
}

static u16 FUN_0003a658Hook( int a1 )
{
  a1 = FUN_2604C4(a1);
  if ( a1 >= 255 )
  {
    return 0;
  }
  else return SHK_CALL_HOOK( FUN_0003a658, a1 );
}

static u16 FUN_0003a698Hook( int a1 )
{
  a1 = FUN_2604C4(a1);
  if ( a1 >= 255 )
  {
    return 0;
  }
  else return SHK_CALL_HOOK( FUN_0003a698, a1 );
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void p5eInit( void )
{
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( FUN_0072360c, forceSingleGAP );
  SHK_BIND_HOOK( setBgm, setBgmHook );
  SHK_BIND_HOOK( LoadEPL, LoadEPLHook );
  SHK_BIND_HOOK( GenericCharacterModelLoader, GenericCharacterModelLoaderHook );
  SHK_BIND_HOOK( criFs_Initialize, criFs_InitializeHook );
  SHK_BIND_HOOK( crifsloader_load_registered_file, crifsloader_load_registered_fileHook );
  SHK_BIND_HOOK( LoadMeleeWeaponModelTable, LoadMeleeWeaponModelTableHook );
  SHK_BIND_HOOK( LoadGunWeaponModelTable, LoadGunWeaponModelTableHook );
  SHK_BIND_HOOK( FUN_0003a658, FUN_0003a658Hook );
  SHK_BIND_HOOK( FUN_0003a698, FUN_0003a698Hook );
}

void p5eShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif