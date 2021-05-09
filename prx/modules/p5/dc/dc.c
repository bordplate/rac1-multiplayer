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
SHK_HOOK( u64, BtlUnitGetUnitID, btlUnit_Unit* btlUnit );
SHK_HOOK( int, LoadShdPersonaEnemy, char* result );
SHK_HOOK( int, ParseUNITTBL, u64 a1 );
SHK_HOOK( u64*, ReturnAddressOfUNITTBL_EnemyStats, s64 a1 );
SHK_HOOK( u16, ReturnAddressOfUNITTBL_EnemyAffinities, u32 a1, u16 a2 );
SHK_HOOK( u16, ReturnAddressOfUNITTBL_PersonaAffinities, u32 a1, u16 a2 );
SHK_HOOK( u64*, ReturnAddressOfUNITTBL_Segment3, s64 a1 );
SHK_HOOK( u64*, ReturnAddressOfELSAITBL_Segment1, u32 a1 );
SHK_HOOK( u64, CalculateShdPersonaEnemyEntry, shdHelper* a1, u32 a2 );

u64 BtlUnitGetUnitIDHook( btlUnit_Unit* btlUnit  )
{
  u64 unitType = btlUnit->unitType;
  u64 unitID = btlUnit->unitID;
  if ( unitType != 1 )
  {
    //DEBUG_LOG( "BtlUnitGetUnitID called; - unit type %d - unit ID %d\n", unitType, unitID );
    if ( unitType == 2 )
    {
      GlobalEnemyID = unitID;
      GlobalBtlUnitEnemyAddress = btlUnit;
      enemyBtlUnit = btlUnit;
    }
    else GlobalEnemyID = 0;
  }
  return unitID;
}

int LoadShdPersonaEnemyHook( char* result )
{
  int size;
  if ( GlobalEnemyID >= 351 )
  {
    size = sprintf(result, "init/shdCustomEnemy.pdd");
  }
  else size = sprintf(result, "init/shdPersonaEnemy.pdd");

  return size;
}

void* BlackMaskBossVoiceHook( structA* a1 )
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
  return ppuVar3;
}

void* LoadPlayerCombatVoicePackHook( structA* a1 )
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

void* LoadPartyPanelSPDHook(char* a1)
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/gui/p5_battle_partypanel.spd" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/gui/p5r_battle_partypanel.spd";
  }
  return SHK_CALL_HOOK( LoadPartyPanelSPD, a1 );
}

void* LoadBEDFileHook(char* a1, char a2)
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

void* LoadCombatCutinHook(char* a1 , char a2)
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

void CombatPersonaCueIDHook( CueIDThingy* param_1, int param_2, short param_3, char param_4, char param_5 )
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
      if ( CONFIG_ENABLED( enableAkechiMod ) && ( param_3 == 360 || param_3 == 361 ) ) // Charge and Concentrate
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

void Load2DUIDDSHook(u32* a1, char* a2)
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

void* LoadBCDFunctionHook( char* a1 , u32 a2, u32 a3, int* a4 )
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/event/BCD/goodbye/bksk_aketi.BCD" ) == 0 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/event/BCD/goodbye/bksk_aketi_b.BCD";
  }
  else if ( CONFIG_ENABLED( enablePersonaEnemies ) && strcmp( a1, "battle/event/BCD/0316/000/bes_0316_000.BCD" ) == 0 )
  {
    char introBCD[128];
    sprintf(introBCD, "battle/event/BCD/%04x/000/bes_%04x_000.BCD", EncounterIDGlobal, EncounterIDGlobal);
    return SHK_CALL_HOOK( LoadBCDFunction, introBCD, a2, a3, a4 );
  }
  return SHK_CALL_HOOK( LoadBCDFunction, a1, a2, a3, a4 );
}

void* LoadPartyPanelPLGHook(char* a1, u32 a2, u32 a3, char a4)
{
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/gui/battle_active_pp.plg" ) == 0 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
      a1 = "battle/gui/battle_active_p5.plg";
  }
  return SHK_CALL_HOOK( LoadPartyPanelPLG, a1, a2, a3, a4 );
}

void setBlackMaskCueIDHook( CueIDThingy* param_1, u32 param_2, u16 skill_ID )
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

int LoadAnimationPackHook( u64 param_1, int animationID, char* result, int animationType )
{
  u64 animType = (u32)(param_1 >> 0x3a);
  u64 unitID = (u32)(param_1 >> 0x14) & 0xffff;
  DEBUG_LOG("Anim type %d loaded\n", animType);
  if ( CONFIG_ENABLED( enableAkechiMod ) && animType == 2 )
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

int ParseUNITTBLHook( u64 param_1 )
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

u64* ReturnAddressOfUNITTBL_EnemyStatsHook( s64 a1 )
{
  return &NewEnemyStatsTBL.EnemyStats[a1];
}

u16 ReturnAddressOfUNITTBL_EnemyAffinitiesHook( u32 a1, u16 a2 )
{
  return NewEnemyAffinityTBL.EnemyAffinities[a1].affinity[a2];
}

u16 ReturnAddressOfUNITTBL_PersonaAffinitiesHook( u32 a1, u16 a2 )
{
  return NewPersonaAffinityTBL.PersonaAffinities[a1].affinity[a2];
}

u64* ReturnAddressOfUNITTBL_Segment3Hook( s64 a1 )
{
  return &NewSegment3TBL.unit[a1];
}

u64* ReturnAddressOfELSAITBL_Segment1Hook( u32 a1 )
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

u64 CalculateShdPersonaEnemyEntryHook( shdHelper* param_1, u32 param_2 )
{
  if ( param_2 >= 351 )
  {
    param_2 = param_2 - 109;
  }
  u64 returnVal = SHK_CALL_HOOK( CalculateShdPersonaEnemyEntry, param_1, param_2);
  DEBUG_LOG("CalculateShdPersonaEnemyEntry returned address -> 0x%08x\n", returnVal);
  return returnVal;
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void dcInit( void )
{
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
  SHK_BIND_HOOK( BtlUnitGetUnitID, BtlUnitGetUnitIDHook );
  SHK_BIND_HOOK( LoadShdPersonaEnemy, LoadShdPersonaEnemyHook );
  SHK_BIND_HOOK( ParseUNITTBL, ParseUNITTBLHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_EnemyStats, ReturnAddressOfUNITTBL_EnemyStatsHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_EnemyAffinities, ReturnAddressOfUNITTBL_EnemyAffinitiesHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_PersonaAffinities, ReturnAddressOfUNITTBL_PersonaAffinitiesHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_Segment3, ReturnAddressOfUNITTBL_Segment3Hook );
  SHK_BIND_HOOK( ReturnAddressOfELSAITBL_Segment1, ReturnAddressOfELSAITBL_Segment1Hook );
  SHK_BIND_HOOK( CalculateShdPersonaEnemyEntry, CalculateShdPersonaEnemyEntryHook );
}

void dcShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif