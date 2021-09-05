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
#include "dc.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, setBlackMaskCueID );
SHK_HOOK( void, IsEncounterEventSoundBankExist, EncounterFuncStruct* a1 );
SHK_HOOK( void*, BlackMaskBossVoice, structA* a1 );
SHK_HOOK( void*, LoadPlayerCombatVoicePack, structA* a1 );
SHK_HOOK( void*, LoadPartyPanelSPD, char* a1 );
SHK_HOOK( void*, LoadPartyPanelPLG, char* a1 , u32 a2, u32 a3, char a4);
SHK_HOOK( void*, LoadBCDFunction, char* a1 , u32 a2, u32 a3, int* a4);
SHK_HOOK( void, Load2DUIDDS, u32* a1 , char* a2);
SHK_HOOK( void*, LoadBEDFile, char* a1 , char a2);
SHK_HOOK( void*, LoadCombatCutin, char* a1 , char a2);
SHK_HOOK( int, LoadAnimationPack, u64 param_1, int animationID, char* result, int animationType );
SHK_HOOK( void, CombatPersonaCueID, CueIDThingy* param_1, int param_2, short param_3, char param_4, char param_5);
SHK_HOOK( void, JokerPersonaNameCueID, CueIDThingy* param_1, int param_2 );
SHK_HOOK( u64, BtlUnitGetUnitID, btlUnit_Unit* btlUnit );
SHK_HOOK( encounterIDTBL*, FUN_00263b94, int a1 );
SHK_HOOK( resrcNPCTblEntry*, FUN_00043fac, int a1 );
SHK_HOOK( int, LoadShdPersonaEnemy, char* result );
SHK_HOOK( int, SetEnemyAkechiPersona, u64 a1, u64 a2, EnemyPersonaFunctionStruct1* a3 );
SHK_HOOK( bool, EnemyHasCombatCutin, int a1, int a2 );
SHK_HOOK( int, SetUpEncounterFlags, EncounterFuncStruct* a1, EncounterStructShort* a2);
SHK_HOOK( int, BlackMaskEncounterIntroBCD, int a1, int a2, EncounterFuncStruct* a3);
// EXIST stuff
SHK_HOOK( int, FUN_0026b390, u16 a1 );
SHK_HOOK( int, FUN_0026b360, u16 a1 );
SHK_HOOK( int, FUN_0026b2b0, u16 a1 );
SHK_HOOK( int, FUN_0026b2e0, u16 a1 );
SHK_HOOK( int, FUN_0026b320, u16 a1 );
SHK_HOOK( int, FUN_0026aff0, u16 a1 );
SHK_HOOK( int, FUN_0026b094, u16 a1 );
SHK_HOOK( int, FUN_0026b148, u16 a1 );
SHK_HOOK( int, FUN_0026b1fc, u16 a1 );
// EXIST end
SHK_HOOK( int, FUN_00262258, u16 a1 );
SHK_HOOK( int, ParseUNITTBL, u64 a1 );
SHK_HOOK( int, FUN_00af3cb0, int a1 );
SHK_HOOK( GFDModelMaterial_Processed*, FUN_0094c048, int* a1 );
SHK_HOOK( int, FUN_00829ce8, ActiveCombatUnitStruct* a1 );
SHK_HOOK( u64*, ReturnAddressOfUNITTBL_EnemyStats, s64 a1 );
SHK_HOOK( u16, ReturnAddressOfUNITTBL_EnemyAffinities, u32 a1, u16 a2 );
SHK_HOOK( u16, ReturnAddressOfUNITTBL_PersonaAffinities, u32 a1, u16 a2 );
SHK_HOOK( u64*, ReturnAddressOfUNITTBL_Segment3, s64 a1 );
SHK_HOOK( u64*, ReturnAddressOfELSAITBL_Segment1, u32 a1 );
SHK_HOOK( u64, CalculateShdPersonaEnemyEntry, shdHelper* a1, u32 a2 );
SHK_HOOK( void, FUN_003735d8, fechance* a1, u64 a2, u64 a3, u64 a4, u64 a5 );

static u64 BtlUnitGetUnitIDHook( btlUnit_Unit* btlUnit  )
{
  u64 unitType = btlUnit->unitType;
  u64 unitID = btlUnit->unitID;
  if ( unitType != 1 )
  {
    //printf( "BtlUnitGetUnitID called; - unit type %d - unit ID %d\n", unitType, unitID );
    lastAccessedUnitType = unitType;
    if ( unitType == 2 )
    {
      GlobalEnemyID = unitID;
      enemyBtlUnit = btlUnit;
    }
    else
    {
      GlobalEnemyID = 0;
      enemyBtlUnit = 0;
    }
  }
  return unitID;
}

static int LoadShdPersonaEnemyHook( char* result )
{
  int size;
  if ( GlobalEnemyID >= 451 )
  {
    size = sprintf(result, "init/shdCustomEnemy3.pdd");
  }
  else if ( GlobalEnemyID >= 401 )
  {
    size = sprintf(result, "init/shdCustomEnemy2.pdd");
  }
  else if ( GlobalEnemyID >= 351 )
  {
    size = sprintf(result, "init/shdCustomEnemy1.pdd");
  }
  else size = sprintf(result, "init/shdPersonaEnemy.pdd");

  return size;
}

static void* BlackMaskBossVoiceHook( structA* a1 )
{
  idkman* someAddressPointer;
  u64 uVar1;
  undefined8 in_r7;
  undefined8 in_r8;
  u32 uVar2;
  idkman* ppuVar3;
  char acStack2080 [128];
  char acStack1056 [128];

  someAddressPointer = MallocAndReturn(0x20);
  if (someAddressPointer != (void*)0x0) {
  SomethingAboutLoadingVoices(someAddressPointer);
  someAddressPointer->ptr1 = 0x00ba90e4;
  ppuVar3 = someAddressPointer;
  }
  CalculateAddressWithPointer(&ppuVar3->ptr2,a1);
  uVar1 = BtlUnitGetUnitIDHook(a1->field0C->btlUnitPointer);
  uVar2 = uVar1;
  uVar1 = GetSavedataBitflagAlt(0x218f);
  if (uVar1 == 0) {
  sprintf(acStack2080,"sound/battle/be_boss_%04d.acb",uVar2);
  sprintf(acStack1056,"sound/battle/be_boss_%04d.awb",uVar2);
  }
  else {
  sprintf(acStack2080,"sound_JP/battle/be_boss_%04d.acb",uVar2);
  sprintf(acStack1056,"sound_JP/battle/be_boss_%04d.awb",uVar2);
  }
  FUN_0074ae50(ppuVar3, acStack2080, acStack1056, 0x100e8);

  // enable Fast Persona Summon Animation from config
  // This is done here because this needs to happen before
  // the first AI action
  for ( int i = 0; i <= sizeof(CONFIG_INT_ARRAY( fastSummonEnemy )); i++ )
  {
    if ( (u16)CONFIG_INT_ARRAY( fastSummonEnemy )[i] == uVar2)
    {
      a1->field0C->btlUnitPointer->context.player.TacticsState = 4;
    }
  }
    
  return ppuVar3;
}

static void* LoadPlayerCombatVoicePackHook( structA* a1 )
{
  idkman* someAddressPointer;
  u64 uVar1;
  u32 playerID;
  idkman* pmVar2;
  char acStack2080 [128];
  char acStack1056 [128];

  someAddressPointer = MallocAndReturn(0x20);
  pmVar2 = (void *)0x0;
  if (someAddressPointer != (void *)0x0) 
  {
    SomethingAboutLoadingVoices(someAddressPointer);
    someAddressPointer->ptr1 = 0xba7d30;
    pmVar2 = someAddressPointer;
  }
  CalculateAddressWithPointer(&pmVar2->ptr2,a1);
  uVar1 = BtlUnitGetUnitIDHook(a1->field0C->btlUnitPointer);
  playerID = uVar1;
  uVar1 = GetSavedataBitflagAlt(0x218f);

  DEBUG_LOG("Player Voicepack being loaded\nPlayer ID -> %02d\nPersona ID -> %02d\n", playerID, GetEquippedPersonaFunction(playerID));
  if ( playerID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood && CONFIG_ENABLED( enableAkechiMod ) )
  {
    if ((int)uVar1 == 0) 
    {
      FUN_0074ae50(pmVar2, "sound/battle/bp09_01.acb", "sound/battle/bp09_01.awb", playerID);
    }
    else 
    {
      FUN_0074ae50(pmVar2, "sound_JP/battle/bp09_01.acb", "sound_JP/battle/bp09_01.awb", playerID);
    }
  }
  else
  {
    if ((int)uVar1 == 0) 
    {
      sprintf(acStack2080,"sound/battle/bp%02d.acb",playerID);
      sprintf(acStack1056,"sound/battle/bp%02d.awb",playerID);
    }
    else 
    {
      sprintf(acStack2080,"sound_JP/battle/bp%02d.acb",playerID);
      sprintf(acStack1056,"sound_JP/battle/bp%02d.awb",playerID);
    }
    FUN_0074ae50(pmVar2, acStack2080, acStack1056, playerID);
  }
  return pmVar2;
}

static void* LoadPartyPanelSPDHook(char* a1)
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/gui/p5_battle_partypanel.spd" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/gui/p5r_battle_partypanel.spd";
  }
  return SHK_CALL_HOOK( LoadPartyPanelSPD, a1 );
}

static void* LoadBEDFileHook(char* a1, char a2)
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/effect/bes_gun9.BED" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/effect/bes_gun9_01.BED";
  }
  else if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/skill/0416.BED" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/skill/0416b.BED";
  }
  return SHK_CALL_HOOK( LoadBEDFile, a1, a2 );
}

static void* LoadCombatCutinHook(char* a1 , char a2)
{
  // Combat Cutin
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/cutin/bct_p_bc0009.dds" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/cutin/bct_p_bc0009_01.dds";
  }
  // AoA Start/Jump image
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/gui/bksk_p_bc0009.dds" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/gui/bksk_p_bc0009_01.dds";
  }
  return SHK_CALL_HOOK( LoadCombatCutin, a1, a2 );
}

static void CombatPersonaCueIDHook( CueIDThingy* param_1, int param_2, short param_3, char param_4, char param_5 )
{
  u32 uVar1;
  u64 uVar2;
  u64 uVar3;
  u16* puVar5;
  sample_int_struct* puVar4;
  
  uVar1 = (int)param_3 & 0xffff;
  uVar2 = BtlUnitGetUnitIDHook(param_1->Field10->btlUnitPointer);
  u32 unitID = uVar2; // for debugging purposes
  uVar2 = GetEquippedPersonaFunction(uVar2);
  u16 personaID = uVar2;
  DEBUG_LOG("CombatPersonaCueIDHook called, character -> %d, persona -> %d, skill -> %d\n", unitID, personaID, param_3);
  if ( personaID == 200 || (personaID & 0x1000) != 0 )
  {
    uVar3 = 0;
  }
  else uVar3 = 1;

  if (((int)uVar3 == 0) || (param_4 != 0)) 
  {
    uVar3 = FUN_002584cc(uVar1,1);
    if (((int)uVar3 == 0) && (uVar3 = FUN_0025867c(uVar1), (int)uVar3 == 0)) 
    {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 0x65, 0);
    }
    else 
    {
      if ( personaID >= 201 && personaID <= 210 ) // tier 1 personas
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x4f, 0);
      }
      else if ( personaID >= 211 && personaID <= 220 || personaID == 240 ) // tier 2, 240 -> Vanadis 
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x50, 0);
      }
      else if ( CONFIG_ENABLED( enableAkechiMod ) && personaID >= 241 && personaID <= 250 ) // tier 3 personas
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x4D, 0);
      }
      else
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x20, 0); // no valid Persona found, use generic attack line
        DEBUG_LOG("Failed to find valid Party Member persona ID. Result %d\n", personaID);
      }
    }
  }
  else 
  {
    uVar3 = FUN_002584cc(uVar1,1);
    if (((int)uVar3 == 0) && (uVar3 = FUN_0025867c(uVar1), (int)uVar3 == 0)) 
    {
      if ( CONFIG_ENABLED( enableAkechiMod ) && ( param_3 == 360 || param_3 == 361 || param_3 == 1511 || param_3 == 1505 ) ) // Charge and Concentrate type skills
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 78, 0);
      }
      else LoadSoundByCueIDCombatVoice(param_1, param_2, 0x65, 0);
    }
    else 
    {
      uVar3 = 0x20;
      if (param_5 != 0) 
      {
        uVar3 = 0x68;
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, uVar3, 0);
    }
  }
  return;
}

static void JokerPersonaNameCueIDHook( CueIDThingy* param_1, int param_2 )
{
  if (GetEquippedPersonaFunction(1) >= 309 )
  {
    LoadSoundByCueIDCombatVoice(param_1, param_2, 213, 0);
  }
  else return SHK_CALL_HOOK( JokerPersonaNameCueID, param_1, param_2 );
}

static void Load2DUIDDSHook(u32* a1, char* a2)
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a2, "camp/wanted/wanted_09.dds") == 0 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a2 = "camp/wanted/wanted_09_01.dds";
  }
  else if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a2, "camp/charaTex/c_chara_09.dds") == 0 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a2 = "camp/charaTex/c_chara_09b.dds";
  }
  SHK_CALL_HOOK( Load2DUIDDS, a1, a2 );
}

static void* LoadBCDFunctionHook( char* a1 , u32 a2, u32 a3, int* a4 )
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/event/BCD/goodbye/bksk_aketi.BCD" ) == 0 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/event/BCD/goodbye/bksk_aketi_b.BCD";
  }
  else if ( CONFIG_ENABLED( enablePersonaEnemies ) && strcmp( a1, "battle/event/BCD/030c/000/bes_030c_000.BCD" ) == 0 )
  {
    char introBCD[128];
    sprintf(introBCD, "battle/event/BCD/%04x/000/bes_%04x_000.BCD", EncounterIDGlobal, EncounterIDGlobal);
    return SHK_CALL_HOOK( LoadBCDFunction, introBCD, a2, a3, a4 );
  }
  if ( strcmp( a1, "battle/event/BCD/chance/bes_randomfire_in.BCD" ) == 0 ||
   strcmp( a1, "battle/event/BCD/chance/bes_randomfire_solo.BCD" ) == 0 ||
   strcmp( a1, "battle/event/BCD/chance/bes_randomfire.BCD" ) == 0 )
  {
    isAmbush = true;
  }
  return SHK_CALL_HOOK( LoadBCDFunction, a1, a2, a3, a4 );
}

static void* LoadPartyPanelPLGHook(char* a1, u32 a2, u32 a3, char a4)
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/gui/battle_active_pp.plg" ) == 0 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
      a1 = "battle/gui/battle_active_p5.plg";
  }
  return SHK_CALL_HOOK( LoadPartyPanelPLG, a1, a2, a3, a4 );
}

static void setBlackMaskCueIDHook( CueIDThingy* param_1, u32 param_2, u16 skill_ID )
{
  u16 uVar1;
  u64 uVar2;
  u32 Custom;

  Custom = ActualGetCount(420);
  if (Custom > 0)
  {
    LoadSoundByCueIDCombatVoice(param_1, param_2, Custom, 0);
    return;
  }
  
  if (skill_ID < 0x48) {
    if (skill_ID < 0x18) {
      if (skill_ID < 0xe) {
        if (skill_ID < 10) {
          return;
        }
      }
      else {
        if (0x13 < skill_ID) 
        {
            LoadSoundByCueIDCombatVoice(param_1, param_2, 2, 0);
            return;
        }
        if (0xf < skill_ID) {
          return;
        }
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, 3, 0);
      return;
    }
    if (skill_ID < 0x40) {
      if (0x3b < skill_ID) {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 7, 0);
        return;
      }
      if (0x19 < skill_ID) {
        return;
      }
    LoadSoundByCueIDCombatVoice(param_1, param_2, 2, 0);
      return;
    }
    if (skill_ID < 0x43) {
      if (skill_ID < 0x42) {
        return;
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, 7, 0);
      return;
    }
    if (skill_ID < 0x46) {
      return;
    }
  }
  else {
    if (0x10f < skill_ID) {
      if (skill_ID == 0x2a2) {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 10, 0);
        return;
      }
      if (skill_ID == 0x2a1) {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0xb, 0);
        return;
      }
      if (skill_ID == 0x173) {
        uVar2 = 0xb;
        uVar1 = param_1->Field1C;
        param_1->Field1C = uVar1 + 1;
        if ((uVar1 & 1) != 0) {
          uVar2 = 10;
        }
        LoadSoundByCueIDCombatVoice(param_1, param_2, uVar2, 0);
        return;
      }
      if (skill_ID != 0x172) {
        if (skill_ID == 0x15c) {
          LoadSoundByCueIDCombatVoice(param_1, param_2, 8, 0);
          return;
        }
        if (skill_ID != 0x110) {
          return;
        }
        LoadSoundByCueIDCombatVoice(param_1, param_2, 6, 0);
        return;
      }
      uVar2 = 0xb;
      uVar1 = param_1->Field1C;
      param_1->Field1C = uVar1 + 1;
      if ((uVar1 & 1) != 0) {
        uVar2 = 10;
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, uVar2, 0);
      return;
    }
    if (skill_ID == 0x106) {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 6, 0);
      return;
    }
    if (skill_ID == 0xf3) {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 5, 0);
      return;
    }
    if (skill_ID == 0xd4) {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 4, 0);
      return;
    }
    if (skill_ID == 0xca) {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 4, 0);
      return;
    }
    if (skill_ID != 0x48) {
      return;
    }
  }
  LoadSoundByCueIDCombatVoice(param_1, param_2, 9, 0);
  return;
}

static int LoadAnimationPackHook( u64 param_1, int animationID, char* result, int animationType )
{
  u64 animType = (u32)(param_1 >> 0x3a);
  u64 unitID = (u32)(param_1 >> 0x14) & 0xffff;
  DEBUG_LOG("Anim type %d loaded\n", animType);
  if ( CONFIG_ENABLED( enableAkechiMod ) && animType == 2 && animationID == 51 )
  {
    if( unitID == 6 && GetEquippedPersonaFunction(6) != Persona_Anat )
    {
        animationID = 52;
    }
    else if( unitID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
    {
        animationID = 52;
    }
  }
  return SHK_CALL_HOOK( LoadAnimationPack, param_1, animationID, result, animationType);
}

static int ParseUNITTBLHook( u64 param_1 )
{
  memset( &NewEnemyStatsTBL, 0x69, sizeof( NewEnemyStatsTBL ) );
  memset( &NewEnemyAffinityTBL, 0x69, sizeof( NewEnemyAffinityTBL ) );
  memset( &NewPersonaAffinityTBL, 0x69, sizeof( NewPersonaAffinityTBL ) );
  memset( &NewSegment3TBL, 0x69, sizeof( NewSegment3TBL ) );
  memset( &NewVisualIndexTBL, 0x69, sizeof( NewVisualIndexTBL ) );
  
  u16 uVar1;
  u32 uVar2;
  u64 uVar3;
  undefined8 uVar4;
  u16 *puVar5;
  undefined *puVar6;
  int iVar7;
  u64 uVar8;
  s64 lVar9;
  u16 *puVar10;
  u16 *puVar11;
  u16 *puVar12;
  undefined *puVar13;
  u16 *puVar14;
  u16 *puVar15;
  u16 *puVar16;
  u8 bVar17;
  int iVar18;
  s64 lVar19;
  u32 local_40 [2];
  
  FUN_0090053c((u32)local_40,param_1,4);
  iVar18 = 0;
  uVar4 = __builtin_clz(0);
  bVar17 = (u8)((u32)uVar4 >> 5) & 1 ^ 1;
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = param_1 + 4 & 0xffffffff;
  puVar16 = &NewEnemyStatsTBL;
  FUN_0090053c((u32)&NewEnemyStatsTBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = uVar3 + 4 & 0xffffffff;
  puVar15 = &NewEnemyAffinityTBL;
  FUN_0090053c((u32)&NewEnemyAffinityTBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = uVar3 + 4 & 0xffffffff;
  puVar14 = &NewPersonaAffinityTBL;
  FUN_0090053c((u32)&NewPersonaAffinityTBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = uVar3 + 4 & 0xffffffff;
  puVar13 = &NewSegment3TBL;
  FUN_0090053c((u32)&NewSegment3TBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  puVar5 = (u16 *)FUN_0090053c((u32)0x00d79a2c,uVar3 + 4 & 0xffffffff,(u64)local_40[0]);
  if (bVar17 != 0) {
    iVar7 = 0;
    do {
      uVar2 = *(u32 *)(puVar16 + 4);
      *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
      uVar1 = puVar16[2];
      *(u32 *)(puVar16 + 4) =
           uVar2 << 0x18 | (uVar2 & 0xff00) << 8 | uVar2 >> 0x18 | uVar2 >> 8 & 0xff00;
      uVar2 = *(u32 *)(puVar16 + 6);
      puVar16[2] = uVar1 >> 8 | uVar1 << 8;
      lVar19 = 8;
      *(u32 *)(puVar16 + 6) =
           uVar2 << 0x18 | (uVar2 & 0xff00) << 8 | uVar2 >> 0x18 | uVar2 >> 8 & 0xff00;
      puVar5 = puVar16;
      do {
        puVar10 = puVar5 + 0xb;
        puVar5 = puVar5 + 1;
        uVar1 = *puVar10;
        *puVar10 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      uVar1 = puVar16[0x13];
      lVar19 = 4;
      puVar16[0x13] = uVar1 >> 8 | uVar1 << 8;
      uVar1 = puVar16[0x14];
      puVar16[0x14] = uVar1 >> 8 | uVar1 << 8;
      puVar5 = puVar16;
      do {
        puVar10 = puVar5 + 0x15;
        puVar11 = puVar5 + 0x16;
        puVar5 = puVar5 + 2;
        uVar1 = *puVar10;
        *puVar10 = uVar1 >> 8 | uVar1 << 8;
        uVar1 = *puVar11;
        *puVar11 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      puVar10 = puVar16 + 0x1d;
      puVar11 = puVar16 + 0x1e;
      puVar12 = puVar16 + 0x1f;
      puVar5 = puVar16 + 0x21;
      uVar1 = *puVar10;
      iVar7 = iVar7 + 1;
      puVar16 = puVar16 + 0x22;
      *puVar10 = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *puVar11;
      *puVar11 = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *puVar12;
      *puVar12 = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *puVar5;
      *puVar5 = uVar1 >> 8 | uVar1 << 8;
    } while (iVar7 < 999);
    iVar7 = 0;
    do {
      lVar19 = 0x14;
      puVar16 = puVar15;
      do {
        *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
        lVar19 = lVar19 + -1;
        puVar16 = puVar16 + 1;
      } while (lVar19 != 0);
      iVar7 = iVar7 + 1;
      puVar15 = puVar15 + 0x14;
    } while (iVar7 < 999);
    iVar7 = 0;
    do {
      lVar19 = 0x14;
      puVar16 = puVar14;
      do {
        *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
        lVar19 = lVar19 + -1;
        puVar16 = puVar16 + 1;
      } while (lVar19 != 0);
      iVar7 = iVar7 + 1;
      puVar14 = puVar14 + 0x14;
    } while (iVar7 < 999);
    do {
      lVar19 = 4;
      uVar1 = *(u16 *)(puVar13 + 4);
      *(u16 *)(puVar13 + 4) = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *(u16 *)(puVar13 + 6);
      *(u16 *)(puVar13 + 6) = uVar1 >> 8 | uVar1 << 8;
      puVar6 = puVar13;
      do {
        puVar16 = (u16 *)(puVar6 + 8);
        puVar6 = puVar6 + 2;
        uVar1 = *puVar16;
        *puVar16 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      lVar19 = 4;
      puVar6 = puVar13;
      do {
        puVar16 = (u16 *)(puVar6 + 0x10);
        puVar6 = puVar6 + 2;
        uVar1 = *puVar16;
        *puVar16 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      iVar18 = iVar18 + 1;
      puVar13 = puVar13 + 0x18;
    } while (iVar18 < 999);
    lVar19 = 999;
    puVar16 = 0x00d79a2c;
    puVar15 = 0x00d79a2e;
    do {
      puVar5 = puVar15 + 2;
      *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
      *puVar15 = *puVar15 >> 8 | *puVar15 << 8;
      lVar19 = lVar19 + -1;
      puVar16 = puVar16 + 2;
      puVar15 = puVar5;
    } while (lVar19 != 0);
  }
  return puVar5;
}

static u64* ReturnAddressOfUNITTBL_EnemyStatsHook( s64 a1 )
{
  return &NewEnemyStatsTBL.EnemyStats[a1];
}

static u16 ReturnAddressOfUNITTBL_EnemyAffinitiesHook( u32 a1, u16 a2 )
{
  return NewEnemyAffinityTBL.EnemyAffinities[a1].affinity[a2];
}

static u16 ReturnAddressOfUNITTBL_PersonaAffinitiesHook( u32 a1, u16 a2 )
{
  return NewPersonaAffinityTBL.PersonaAffinities[a1].affinity[a2];
}

static u64* ReturnAddressOfUNITTBL_Segment3Hook( s64 a1 )
{
  return &NewSegment3TBL.unit[a1];
}

static u64* ReturnAddressOfELSAITBL_Segment1Hook( u32 a1 )
{
  if ( a1 >= 350 )
  {
    NewSegment2TBL.entry[a1-350].ScriptID = a1;
    return &NewSegment2TBL.entry[a1-350];
  }
  else
  {
    u64* returnVal = SHK_CALL_HOOK( ReturnAddressOfELSAITBL_Segment1, a1);
    DEBUG_LOG("ReturnAddressOfELSAITBL_Segment1Hook return val from %d (a1)\nExpected -> 0x%08x\n", a1, returnVal);
    return returnVal;
  }
}

static u64 CalculateShdPersonaEnemyEntryHook( shdHelper* param_1, u32 param_2 )
{
  bool wasEnemy = false;
  if ( lastAccessedUnitType == 2 && param_2 == GlobalEnemyID )
  {
    if ( param_2 >= 451 )
    {
      param_2 = param_2 - 250;
      wasEnemy = true;
    }
    else if ( param_2 >= 401 )
    {
      param_2 = param_2 - 200;
      wasEnemy = true;
    }
    else if ( param_2 >= 351 )
    {
      param_2 = param_2 - 150;
      wasEnemy = true;
    }
  }
  u64 returnVal = SHK_CALL_HOOK( CalculateShdPersonaEnemyEntry, param_1, param_2);
  if ( lastAccessedUnitType == 2 && wasEnemy )
  {
    printf("shdPersonaEnemy Entry %03d\n", param_2-141);
    DEBUG_LOG("CalculateShdPersonaEnemyEntry returned address -> 0x%08x\n", returnVal);
  }
  return returnVal;
}

static int isPersonaExist(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026b390, 201 );
}

static int isPersonaExist2(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026b360, 201 );
}

static int isEnemyExist(u16 a1)
{
  if ( a1 >= 350 )
  {
    return SHK_CALL_HOOK( FUN_0026b2b0, 69 );
  }
  else if ( a1 >= 114 && a1 <= 120 ) // original enemies not in exist
  {
    return SHK_CALL_HOOK( FUN_0026b2b0, 69 );
  }
  else if ( a1 >= 145 && a1 <= 169 && a1 != 157 ) // big range, exclude reaper
  {
    return SHK_CALL_HOOK( FUN_0026b2b0, 69 );
  }
  else return SHK_CALL_HOOK( FUN_0026b2b0, a1 );
}

static int isEnemyExist2(u16 a1)
{
  if ( a1 >= 350 )
  {
    return SHK_CALL_HOOK( FUN_0026b2e0, 69 );
  }
  else if ( a1 >= 114 && a1 <= 120 ) // original enemies not in exist
  {
    return SHK_CALL_HOOK( FUN_0026b2e0, 69 );
  }
  else if ( a1 >= 145 && a1 <= 169 && a1 != 157 ) // big range, exclude reaper
  {
    return SHK_CALL_HOOK( FUN_0026b2e0, 69 );
  }
  else return SHK_CALL_HOOK( FUN_0026b2e0, a1 );
}

static int isEnemyExist3(u16 a1)
{
  if ( a1 >= 350 )
  {
    return SHK_CALL_HOOK( FUN_0026b320, 69 );
  }
  else if ( a1 >= 114 && a1 <= 120 ) // original enemies not in exist
  {
    return SHK_CALL_HOOK( FUN_0026b320, 69 );
  }
  else if ( a1 >= 145 && a1 <= 169 && a1 != 157 ) // big range, exclude reaper
  {
    return SHK_CALL_HOOK( FUN_0026b320, 69 );
  }
  else return SHK_CALL_HOOK( FUN_0026b320, a1 );
}

static int isPartyMemberExist(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026aff0, 2 );
}

static int isPartyMemberExist2(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026b094, 2 );
}

static int isPartyMemberExist3(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026b148, 2 );
}

static int isPartyMemberExist4(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026b1fc, 2 );
}

static int SetEnemyAkechiPersonaHook( u64 a1, u64 a2, EnemyPersonaFunctionStruct1* a3 )
{
  // Field04 is pointer to battle struct of the battle unit currently acting
  if ( a3->field0c->field18->field04->unitType == 2) // only execute on enemy units
  {
    int PersonaEnemyID = a3->field0c->field18->field04->unitID;
    
    int result = SHK_CALL_HOOK( SetEnemyAkechiPersona, a1, a2, a3 ); //call original function and store result

    if ( PersonaEnemyID >= 350 )
    {
      DEBUG_LOG("Enemy %d summoning Persona %d\n", PersonaEnemyID, EnemyPersona);
      return EnemyPersona;
    }

    // black mask conditional to avoid using same Loki ID as player Akechi
    else if ( PersonaEnemyID == 232 && CONFIG_ENABLED( enableAkechiMod ) ) 
    {
      DEBUG_LOG("Black Mask enemy ID %d summoning Persona %d\n", PersonaEnemyID, 239);
      return 239;
    }

    else if ( PersonaEnemyID == 217 && result == 210 ) 
    {
      DEBUG_LOG("Akechi %d summoning Persona %d\n", PersonaEnemyID, 240);
      return 240;
    }

    if ( result > 0 ) DEBUG_LOG("Enemy ID %d summoning Persona %d\n", PersonaEnemyID, result);

    return result;
  }
  return SHK_CALL_HOOK( SetEnemyAkechiPersona, a1, a2, a3 ); // Player unit is executing this function, use original function
}

static bool EnemyHasCombatCutinHook( int a1, int a2 )
{
  bool result = SHK_CALL_HOOK( EnemyHasCombatCutin, a1, a2 );
  if ( GlobalEnemyID >= 350 )
  {
    if ( EnemyPersona == 0 )
    {
      return false;
    }
    else return true;
  }
  else return result;
}

static int SetUpEncounterFlagsHook( EncounterFuncStruct* a1, EncounterStructShort* a2)
{
  int encounterIDReal = a2->encounterIDLocal;
  EncounterIDBGM = encounterIDReal;
  printf("Starting Encounter %d\n", encounterIDReal);
  int previousBGM = GetCurrentBGMCueID();
  bool isLifeWillChange = false;
  if ( previousBGM == 471 )
  {
    isLifeWillChange = true;
  }
  if ( previousBGM == 904 )
  {
    isLifeWillChange = true;
  }

  if ( CONFIG_ENABLED( enableExpandedBGM ) && !isLifeWillChange )
  {
    encounterIDTBL* CurrentEncounter = GetEncounterEntryFromTBL(EncounterIDBGM);
    int CurrentEncounterBGMID = CurrentEncounter->BGMID;
    // DLC BGM Stuff
    u32 btlEquipBgmTableEntryCount = sizeof( btlEquipBgmTable ) / sizeof( btlEquipBgmTableEntry );
    u32 playerOutfitModel = PlayerUnitGetModelMinorID( 1, 50, 0 );
    if ( CurrentEncounterBGMID == 0 && !CONFIG_ENABLED( randomDLCBGM ) && isAmbush )
    {
      int AmbushRNG = GetRandom( 2 );
      //printf("Ambush RNG -> %d\n", AmbushRNG);
      if ( AmbushRNG == 0 )
      {
        CurrentEncounter->BGMID = 907; // Take Over
      }
      else if ( AmbushRNG == 1 )
      {
        CurrentEncounter->BGMID = 999; // What you wish for
      }
      else if ( AmbushRNG == 2 )
      {
        CurrentEncounter->BGMID = 1000; // Axe to grind
      }
      wasBGMReplaced = true;
    }
    if ( CurrentEncounterBGMID == 0 && !CONFIG_ENABLED( randomDLCBGM ) && isAmbushed )
    {
      CurrentEncounter->BGMID = 1004; // Last Surprise (Strikers)
      wasBGMReplaced = true;
    }
    // Apply DLC BGM directly to tbl
    if ( CurrentEncounterBGMID == 0 && !CONFIG_ENABLED( randomDLCBGM ) ) // Last Surprise
    {
      for ( u32 i = 0; i < btlEquipBgmTableEntryCount; ++i )
      {
        btlEquipBgmTableEntry* pEntry = &btlEquipBgmTable[i];
        if ( pEntry->modelID == playerOutfitModel )
        {
          CurrentEncounter->BGMID = pEntry->bgmId;
          wasBGMReplaced = true;
          break;
        }
      }
    }
    else if ( CurrentEncounterBGMID == 0 && CONFIG_ENABLED( randomDLCBGM ) ) // Last Surprise
    {
      btlEquipBgmTableEntry* pEntry = &btlEquipBgmTable[rngBGM];
      CurrentEncounter->BGMID = pEntry->bgmId;
      wasBGMReplaced = true;
    }
    // Allow Ambush Themes to override DLC
    if ( CurrentEncounterBGMID == 0 && CONFIG_ENABLED( DLCAmbush ) && isAmbush )
    {
      int AmbushRNG = GetRandom( 2 );
      //printf("Ambush RNG -> %d\n", AmbushRNG);
      if ( AmbushRNG == 0 )
      {
        CurrentEncounter->BGMID = 907; // Take Over
      }
      else if ( AmbushRNG == 1 )
      {
        CurrentEncounter->BGMID = 999; // What you wish for
      }
      else if ( AmbushRNG == 2 )
      {
        CurrentEncounter->BGMID = 1000; // Axe to grind
      }
      wasBGMReplaced = true;
    }
    if ( CurrentEncounterBGMID == 0 && CONFIG_ENABLED( DLCAmbush ) && isAmbushed )
    {
      CurrentEncounter->BGMID = 1004; // Last Surprise (Strikers)
      wasBGMReplaced = true;
    }
  }

  if ( encounterIDReal >= 830 && encounterIDReal < 990 )
  {
    a2->encounterIDLocal = 780;
  }
  int result = SHK_CALL_HOOK( SetUpEncounterFlags, a1, a2 );
  a1->encounterIDLocal = encounterIDReal;
  a2->encounterIDLocal = (u16)encounterIDReal;

  for ( int i = 0; i <= 11; i++) // navi stuff
  {
    hasUzukiAilmentAnnounce[i] = false;
  }
  UzukiLowHPWarn = false;
  UzukiDebuffAttackWarn = false;
  UzukiDebuffDeffenseWarn = false;
  UzukiDebuffSpeedWarn = false;

  return result;
}

static encounterIDTBL* FUN_00263b94Hook( int a1 )
{
  encounterIDTBL* result;
  if ( a1 == 780 && EncounterIDGlobal != 780 ) 
  {
    result = SHK_CALL_HOOK( FUN_00263b94, EncounterIDGlobal );
    LastUsedEncounterID = EncounterIDGlobal;
  }
  else 
  {
    result = SHK_CALL_HOOK( FUN_00263b94, a1 );
  }

  if ( LastUsedEncounterID != a1 && a1 != 780 && a1 < 1000) // prevent spam
  {
    printf("Encounter Block %03d loaded\n", a1);
    hexDump("TBL Data", result, 24);
    LastUsedEncounterID = a1;
  }
  u32 btlEquipBgmTableEntryCount = sizeof( btlEquipBgmTable ) / sizeof( btlEquipBgmTableEntry );
  if (!wasBGMRandom)
  {
    rngBGM = randomIntBetween(0, btlEquipBgmTableEntryCount-1);
    wasBGMRandom = true;
  }
  return result;
}

static int BlackMaskEncounterIntroBCDHook( int a1, int a2, EncounterFuncStruct* a3)
{
  int encounterIDReal = a3->encounterIDLocal;
  if ( encounterIDReal >= 830 && 990 > encounterIDReal )
  {
    a3->encounterIDLocal = 780;
  }
  int result = SHK_CALL_HOOK( BlackMaskEncounterIntroBCD, a1, a2, a3 );
  a3->encounterIDLocal = encounterIDReal;
  return result;
}

static void IsEncounterEventSoundBankExistHook( EncounterFuncStruct* a1 )
{
  if ( EncounterIDGlobal >= 830 ) return LoadEncounterEventSoundbank( EncounterIDGlobal );
  else return SHK_CALL_HOOK( IsEncounterEventSoundBankExist, a1 );
}

static int CheckIsEncounterAmbush( int a1 )
{
  int result = SHK_CALL_HOOK( FUN_00af3cb0, a1 );
  if ( result == 2 )
  {
    isAmbush = true;
  }
  return result;
}

static void FUN_003735d8Hook( fechance* a1, u64 a2, u64 a3, u64 a4, u64 a5 )
{
  //printf("FUN_003735d8 a1->field182 -> %d\n", a1->field182 );
  if ( a1->field182 == 2 || a1->field182 == 1 )
  {
    isAmbush = true;
    isAmbushed = false;
  }
  else if ( a1->field182 == 6 )
  {
    isAmbush = false;
    isAmbushed = true;
  }
  return SHK_CALL_HOOK( FUN_003735d8, a1, a2, a3, a4, a5 );
}

static int FUN_00829ce8Hook( ActiveCombatUnitStruct* a1 )
{ 
  int currentUnitID = a1->field30->field18->field04->unitID;
  //printf( "FUN_00829ce8 called; a1 -> 0x%x; UnitID -> %d\n", a1, currentUnitID );
  int accessoryID = a1->field30->field18->field04->context.player.accessoryID;
  accessoryID = accessoryID - 0x2000;
  //printf("Unit ID %d has accessory %d\n", currentUnitID, accessoryID);
  int skillID = GetAccessoryTBLEntry( accessoryID )->RESERVE;
  //printf("Skill ID %d obtained from accessory\n", skillID);

  int numOfSkills = 0;
  int result = SHK_CALL_HOOK( FUN_00829ce8, a1 );
  if ( skillID != 0 && accessoryID >= 255 && skillID < 2000 )
  {
    //printf("Unit holding accessory ID %d granting skill ID %d\n", accessoryID, skillID );
    for ( int i = 0; i <= 8; i++ )
    {
      if ( a1->SkillID[i] == 0 )
      {
        a1->SkillID[i] = skillID;
        numOfSkills = a1->amountOfSkills + 1;
        a1->amountOfSkills += 1;
        //printf( "Skill ID %d granted on Skill slot %d\n", skillID, i );
        break;
      }
    }
  }
  //printf("Previous Active Skill Slot -> %d\n", ActiveGlobalSkillSlot );
  //printf("Current Skill slot -> %d\n", a1->ActiveSkillID );
  //printf("Amount of Skills in List -> %d\n", a1->amountOfSkills);
  if ( ActiveGlobalSkillSlot == numOfSkills - 2 && a1->ActiveSkillID == 0 )
  {
    //printf("Skill slot looped from %d to %d while skipping new skill\n", ActiveGlobalSkillSlot, a1->ActiveSkillID );
    a1->ActiveSkillID = numOfSkills - 1;
    ActiveGlobalSkillSlot = numOfSkills - 1;
  }
  else if ( ActiveGlobalSkillSlot == 0 && a1->ActiveSkillID == numOfSkills - 2 )
  {
    //printf("Skill slot looped from %d to %d while skipping new skill\n", ActiveGlobalSkillSlot, a1->ActiveSkillID );
    a1->ActiveSkillID = numOfSkills - 1;
    ActiveGlobalSkillSlot = numOfSkills - 1;
  }
  else ActiveGlobalSkillSlot = a1->ActiveSkillID;
  //hexDump( "Active Unit Combat Struct", a1, 0x250 );

  return result;
}

static resrcNPCTblEntry* GetNPCTBLEntry( int a1 )
{
  printf("NPC TBL NPC ID %d loaded\n", a1 );
  //printf("NPC TBL Entry %d loaded\n",  ( (int)SHK_CALL_HOOK( FUN_00043fac, a1 ) - (int)SHK_CALL_HOOK( FUN_00043fac, 0 ) ) / 0x1C );
  return SHK_CALL_HOOK( FUN_00043fac, a1 );
}

static int GetItemTBLMeleeWeaponField0E( u16 a1 )
{
  return 0;
}

static GFDModelMaterial_Processed* ReadMaterial( int* a1 )
{
  GFDModelMaterial_Processed* result = SHK_CALL_HOOK( FUN_0094c048, a1 );
  if ( CONFIG_ENABLED( enableModelDebuggingHelp ) ) printf("GFD Material %s at address 0x%x\n", result->MaterialName, (int)result);
  return result;
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void dcInit( void )
{
  randomSetSeed( getTicks() );
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( setBlackMaskCueID, setBlackMaskCueIDHook );
  SHK_BIND_HOOK( BlackMaskBossVoice, BlackMaskBossVoiceHook );
  SHK_BIND_HOOK( LoadPlayerCombatVoicePack, LoadPlayerCombatVoicePackHook );
  SHK_BIND_HOOK( LoadPartyPanelSPD, LoadPartyPanelSPDHook );
  SHK_BIND_HOOK( LoadPartyPanelPLG, LoadPartyPanelPLGHook );
  SHK_BIND_HOOK( Load2DUIDDS, Load2DUIDDSHook );
  SHK_BIND_HOOK( LoadBEDFile, LoadBEDFileHook );
  SHK_BIND_HOOK( LoadCombatCutin, LoadCombatCutinHook );
  SHK_BIND_HOOK( LoadAnimationPack, LoadAnimationPackHook );
  SHK_BIND_HOOK( LoadBCDFunction, LoadBCDFunctionHook );
  SHK_BIND_HOOK( CombatPersonaCueID, CombatPersonaCueIDHook );
  SHK_BIND_HOOK( JokerPersonaNameCueID, JokerPersonaNameCueIDHook );
  SHK_BIND_HOOK( BtlUnitGetUnitID, BtlUnitGetUnitIDHook );
  SHK_BIND_HOOK( LoadShdPersonaEnemy, LoadShdPersonaEnemyHook );
  SHK_BIND_HOOK( ParseUNITTBL, ParseUNITTBLHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_EnemyStats, ReturnAddressOfUNITTBL_EnemyStatsHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_EnemyAffinities, ReturnAddressOfUNITTBL_EnemyAffinitiesHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_PersonaAffinities, ReturnAddressOfUNITTBL_PersonaAffinitiesHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_Segment3, ReturnAddressOfUNITTBL_Segment3Hook );
  SHK_BIND_HOOK( ReturnAddressOfELSAITBL_Segment1, ReturnAddressOfELSAITBL_Segment1Hook );
  SHK_BIND_HOOK( CalculateShdPersonaEnemyEntry, CalculateShdPersonaEnemyEntryHook );
  SHK_BIND_HOOK( SetUpEncounterFlags, SetUpEncounterFlagsHook );
  SHK_BIND_HOOK( FUN_0026b390, isPersonaExist );
  SHK_BIND_HOOK( FUN_0026b360, isPersonaExist2 );
  SHK_BIND_HOOK( FUN_0026b2b0, isEnemyExist );
  SHK_BIND_HOOK( FUN_0026b2e0, isEnemyExist2 );
  SHK_BIND_HOOK( FUN_0026b320, isEnemyExist3 );
  SHK_BIND_HOOK( FUN_0026aff0, isPartyMemberExist );
  SHK_BIND_HOOK( FUN_0026b094, isPartyMemberExist2 );
  SHK_BIND_HOOK( FUN_0026b148, isPartyMemberExist3 );
  SHK_BIND_HOOK( FUN_0026b1fc, isPartyMemberExist4 );
  SHK_BIND_HOOK( SetEnemyAkechiPersona, SetEnemyAkechiPersonaHook );
  SHK_BIND_HOOK( EnemyHasCombatCutin, EnemyHasCombatCutinHook );
  SHK_BIND_HOOK( FUN_00263b94, FUN_00263b94Hook );
  SHK_BIND_HOOK( BlackMaskEncounterIntroBCD, BlackMaskEncounterIntroBCDHook );
  SHK_BIND_HOOK( IsEncounterEventSoundBankExist, IsEncounterEventSoundBankExistHook );
  SHK_BIND_HOOK( FUN_00af3cb0, CheckIsEncounterAmbush );
  SHK_BIND_HOOK( FUN_003735d8, FUN_003735d8Hook );
  SHK_BIND_HOOK( FUN_00829ce8, FUN_00829ce8Hook );
  SHK_BIND_HOOK( FUN_00043fac, GetNPCTBLEntry );
  SHK_BIND_HOOK( FUN_00262258, GetItemTBLMeleeWeaponField0E );
  SHK_BIND_HOOK( FUN_0094c048, ReadMaterial );
  wasBGMRandom = false;
}

void dcShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif