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
SHK_HOOK( void, FUN_0062b08c, int id );
SHK_HOOK( void, FUN_0062b080, int id );
SHK_HOOK( void, FUN_00af4434, int a1, int a2 );
SHK_HOOK( void, FUN_0063acc8, int a1, int a2 );
SHK_HOOK( int, FUN_0072360c, int id );
SHK_HOOK( int, FUN_001cf704, u64 unk, int a1, int a2, int a3 );
SHK_HOOK( void*, LoadEPL, char* EPL, u8 a2 );
SHK_HOOK( int, criFs_Initialize );
SHK_HOOK( u16, LoadMeleeWeaponModelTable, int a1 );
SHK_HOOK( u16, LoadGunWeaponModelTable, int a1 );
SHK_HOOK( u16, FUN_0003a658, int a1 );
SHK_HOOK( u16, FUN_0003a698, int a1 );
SHK_HOOK( int, FUN_000bee20, int a1, int a2, int a3 );
SHK_HOOK( int, criFsBinder_BindCpk, char* arg );
SHK_HOOK( u64, criFsBinder_SetPriority, u32 a1, u32 a2 );
SHK_HOOK( int, crifsloader_load_registered_file, fileAccessStruct* a1, int a2, int a3, int a4, int a5 );
SHK_HOOK( int, GenericCharacterModelLoader, char* result, u64 modelType, u64 characterID, u64 modelID, u64 modelSubID );

static void setBgmHook( int id )
{
  if ( CONFIG_ENABLED( enableExpandedBGM ) )
  {
    u32 btlEquipBgmTableEntryCount = sizeof( btlEquipBgmTable ) / sizeof( btlEquipBgmTableEntry );
    u32 playerOutfitModel = PlayerUnitGetModelMinorID( 1, 50, 0 );
    if ( id == 340 && wasBGMReplaced ) // fix replaced music
    {
      encounterIDTBL* result = GetEncounterEntryFromTBL( EncounterIDBGM );
      result->BGMID = 0;
      wasBGMReplaced = false;
    }
    if ( id == 340 && CONFIG_ENABLED( randomDLCBGM )) // Victory theme
    { 
      isAmbush = false;
      isAmbushed = false;
      btlEquipBgmTableEntry* pEntry = &btlEquipBgmTable[rngBGM];
      id = pEntry->bgmId + 1;
      wasBGMRandom = false;
    }
    else if ( id == 340 && !CONFIG_ENABLED( randomDLCBGM ) ) // Victory theme
    { 
      isAmbush = false;
      isAmbushed = false;
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
    /*if ( id == 471 ) // Life Will Change
    {
      if ( GetTotalDays() == 267 ) // ingame date for Qlipoth, 12/24
      {
        id = 904;
      }
    }*/
    if ( id == 101 && sequenceIDGlobal == 1 && CONFIG_ENABLED( enableExpandedBGM ) && CONFIG_ENABLED( P5RTitleBGM ) )
    {
      if ( titleScreenBGM == 0 ) // P5
      {
        RandomizeTitleScreenBGM();
      }
      else if ( titleScreenBGM == 1 ) // P5R
      {
        id = 901;
        RandomizeTitleScreenBGM();
      }
      else if ( titleScreenBGM == 2 ) // P5S
      {
        id = 996;
        RandomizeTitleScreenBGM();
      }
    }
  }
  else wasBGMReplaced = false;

  printf("SetBGM Called with BGM ID -> %d\n", id);
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
  //printf( "Model Type %d loading for character ID %d\n", modelType, characterID );
  if ( modelType == 2 || modelType == 5 ) // human character models
  {
    //printf("Character ID %d loading model ID %d\n", characterID, modelID);
    if ( characterID == 9 && modelID == 51 && GetEquippedPersonaFunction(9) != Persona_RobinHood ) // Darkechi model
    {
      modelID = 52;
    }
    if ( characterID >= 1 && characterID <= 10 ) // check PT models in cutscenes
    {
      CharModelReplacementTable* pEntry = &charModelReplacementTableEntry[characterID];
      for ( int j = 0; j <= 4; j++ )
      {
        u32 playerOutfitModel = PlayerUnitGetModelMinorID( characterID, 50, 0 );
        if ( modelID == pEntry->modelId[j] && playerOutfitModel != 51 && CONFIG_ENABLED( enableCutsceneOutfits ) )
        {
          modelID = playerOutfitModel;
        }
      }
      if ( modelID == 49 && CONFIG_ENABLED( enableExpandedBGM ) && CONFIG_ENABLED( P5RTitleBGM ) )
      {
        modelSubID = titleScreenBGM;
      }
    }
    if ( characterID == 1 )
    {
      JokerModel = modelID;
    }
    if ( characterID == 10 && modelID == 50 ) //for now copy Joker outfit to Kasumi
    {
      modelID = JokerModel;
      if ( CONFIG_ENABLED( enableSumire ) )
      {
        modelSubID = 1;
      }
    }
    if ( characterID >= 1 && characterID <= 10 || characterID == 1011 || characterID == 1003 )
    {
      if ( modelID == 2 && isMidWinterValid() )
      {
        modelID = 5;
      }
      else if ( modelID == 4 && isMidWinterValid() )
      {
        modelID = 6;
      }
    }
    else if ( characterID == 2104 && modelID == 0 && isMidWinterValid() ) //Wakaba loads model 0 for some reason
    {
      modelID = 6;
    }
    //printf("Character ID %d loading model ID %d\n", characterID, modelID);
  }
  else if ( modelType == 4 && characterID > 200 ) // force load regular Persona models instead of using PSZ/Enemy model
  {
    if ( characterID >= 221 && characterID <= 0251 ) // party member reserve personas
    {
      modelType = 3;
    }
    else if ( characterID >= 0322 ) // only force this on reserve personas
    {
      modelType = 3;
    }
  }
  return SHK_CALL_HOOK( GenericCharacterModelLoader, result, modelType, characterID, modelID, modelSubID );
}

static void* LoadEPLHook( char* EPL, u8 a2 )
{
  if ( strcmp( EPL, "battle/gui/bes_ui_p_advantage.EPL" ) == 0 ||
   strcmp( EPL, "battle/event/BCD/chance/bes_chance_solo.EPL" ) == 0 ||
   strcmp( EPL, "battle/event/BCD/chance/bes_ui_randam.EPL" ) == 0 )
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

static int criFsBinder_BindCpkHook( char* arg )
{
  int result = SHK_CALL_HOOK( criFsBinder_BindCpk, arg );
  DEBUG_LOG("criFsBinder_BindCpk result %d\n", result );
  return result;
}

static int criFs_InitializeHook( void )
{
  if ( !CONFIG_ENABLED( enableModCPK ) )
  {
    return SHK_CALL_HOOK( criFs_Initialize );
  }
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
    criFsBinder_BindCpkHook(acStack288);
  }

  pcVar2 = FUN_00968be8();
  pcVar3 = FUN_00968bf4();
  
  if ( CONFIG_ENABLED( enableModCPK ) )
  {
    iVar1 = sprintf(acStack288,"%s%s/%s.cpk",pcVar2,pcVar3, CONFIG_STRING(modCPKName));
    iVar1 = criFsBinder_BindCpkHook(acStack288);
    if (iVar1 > 0) criFsBinder_SetPriority(iVar1, 20 + CONFIG_INT( extraModCPK ) + 1);
  }

  u32 extraCPK = CONFIG_INT( extraModCPK );
  if ( CONFIG_ENABLED( enableModCPK ) && extraCPK > 0 )
  {
    for (int i = 0; i < extraCPK; i++)
    {
      iVar1 = sprintf(acStack288,"%s%s/%s_%02d.cpk", pcVar2, pcVar3, CONFIG_STRING_ARRAY(extraModCPKName)[i], i + 1);
      iVar1 = criFsBinder_BindCpkHook(acStack288);
      if (iVar1 > 0) criFsBinder_SetPriority(iVar1, 20 + i);
    }
  }

  iVar1 = sprintf(acStack288,"%s%s/ps3.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpkHook(acStack288);
  
  iVar1 = sprintf(acStack288,"%s%s/data.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpkHook(acStack288);

  return iVar1;
}

static u64 criFsBinder_SetPriorityHook( u32 a1, u32 a2 )
{
  DEBUG_LOG("criFsBinder_SetPriority called; a1 -> 0x%x ;  a2 -> 0x%x\n", a1, a2);
  return SHK_CALL_HOOK( criFsBinder_SetPriority, a1, a2 );
}

static int crifsloader_load_registered_fileHook( fileAccessStruct* a1, int a2, int a3, int a4, int a5 )
{
  if ( CONFIG_ENABLED( enableFileAccessLogging ) ) printf("%s\n", a1->fileNamePtr);
  return SHK_CALL_HOOK( crifsloader_load_registered_file, a1, a2, a3, a4, a5 );
}

static u16 LoadMeleeWeaponModelTableHook( int a1 )
{
  a1 = FUN_2604C4(a1); // weird thing they do to flip negative values
  u16 result = GetMeleeWeaponTBLEntry(a1)->field_0xe;

  /*printf("Melee weapon model load; a1 -> %d\n", a1);
  printf("Weapon Model ID result -> %d\n", result);*/
  return result;
}

static u16 LoadGunWeaponModelTableHook( int a1 )
{
  a1 = FUN_2604C4(a1); // weird thing they do to flip negative values
  u16 result = GetRangedWeaponTBLEntry(a1)->RESERVE;
  /*printf("Gun model load; a1 -> %d\n", a1);
  printf("Gun Model ID result -> %d\n", result);*/
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

static int FUN_001cf704Hook( u64 unk, int charID, int expressionID, int outfitID ) // Bustups function
{
  //printf("Bustup file loaded\ncharacter ID -> %d\nExpression ID -> %d\nOutfit ID -> %d\n", charID, expressionID, outfitID);
  if ( charID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood && expressionID >= 100 && expressionID < 120 )
  {
    expressionID += 700;
  }

  if ( charID > 0 && charID <= 11 || charID == 111 || charID == 208 || charID == 103 )
  {
    if ( outfitID == 2 && isMidWinterValid() )
    {
      outfitID = 5;
    }
    else if ( outfitID == 4 && isMidWinterValid() )
    {
      outfitID = 6;
    }
  }
  return SHK_CALL_HOOK( FUN_001cf704, unk, charID, expressionID, outfitID );
}

static int FUN_000bee20Hook( int a1, int a2, int a3 )
{
  //printf("Field Pac Loaded -> %03d_%03d\n", a2 % 1000, a3 % 1000 );
  lastUsedFieldMajorID = a2 % 1000;
  lastUsedFieldMinorID = a3 % 1000;
  return SHK_CALL_HOOK( FUN_000bee20, a1, a2, a3 );
}

static void FUN_0062b08c_Hook( int a1 )
{
  //a1 = 0;
  return SHK_CALL_HOOK( FUN_0062b08c, a1 );
}

static void FUN_0062b080_Hook( int a1 )
{
  //a1 = 1;
  return SHK_CALL_HOOK( FUN_0062b080, a1 );
}

static void FUN_00af4434Hook( int a1, int a2 )
{
  hexDump( "FUN_00af4434", a1, 0x5E0 );
  return SHK_CALL_HOOK( FUN_00af4434, a1, a2 );
}

static void BtlPlayBGM( int a1, int a2 )
{
  /*char hexdumpString[64];
  sprintf( hexdumpString, "BtlPlayBGM; struct at 0x%x", a1);
  hexDump( hexdumpString, a1, 0x5E0 );*/
  printf("BtlPlayBGM called with BGM ID %d\n", a2);
  return SHK_CALL_HOOK( FUN_0063acc8, a1, a2 );
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void p5eInit( void )
{
  randomSetSeed( getTicks() );
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( FUN_0072360c, forceSingleGAP );
  SHK_BIND_HOOK( FUN_0062b08c, FUN_0062b08c_Hook );
  SHK_BIND_HOOK( FUN_0062b080, FUN_0062b080_Hook );
  SHK_BIND_HOOK( setBgm, setBgmHook );
  SHK_BIND_HOOK( LoadEPL, LoadEPLHook );
  SHK_BIND_HOOK( GenericCharacterModelLoader, GenericCharacterModelLoaderHook );
  SHK_BIND_HOOK( criFsBinder_BindCpk, criFsBinder_BindCpkHook );
  SHK_BIND_HOOK( criFs_Initialize, criFs_InitializeHook );
  SHK_BIND_HOOK( criFsBinder_SetPriority, criFsBinder_SetPriorityHook );
  SHK_BIND_HOOK( crifsloader_load_registered_file, crifsloader_load_registered_fileHook );
  SHK_BIND_HOOK( LoadMeleeWeaponModelTable, LoadMeleeWeaponModelTableHook );
  SHK_BIND_HOOK( LoadGunWeaponModelTable, LoadGunWeaponModelTableHook );
  SHK_BIND_HOOK( FUN_0003a658, FUN_0003a658Hook );
  SHK_BIND_HOOK( FUN_0003a698, FUN_0003a698Hook );
  SHK_BIND_HOOK( FUN_001cf704, FUN_001cf704Hook );
  SHK_BIND_HOOK( FUN_000bee20, FUN_000bee20Hook );
  SHK_BIND_HOOK( FUN_00af4434, FUN_00af4434Hook );
  SHK_BIND_HOOK( FUN_0063acc8, BtlPlayBGM );
  titleScreenBGM = 99;
  RandomizeTitleScreenBGM();
}

void p5eShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}

void RandomizeTitleScreenBGM()
{
  int lastBGM = titleScreenBGM;
  while ( lastBGM == titleScreenBGM )
  {
    titleScreenBGM = randomIntBetween( 0, 2 );
  }
}
#endif