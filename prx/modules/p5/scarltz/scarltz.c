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

SHK_HOOK( u64, ResistancePassiveCheck, btlUnit_Unit* btlUnit, ElementalType ElementID);
SHK_HOOK( u64, SkillHealDamageModifier, u16 SkillID, btlUnit_Unit* btlUnit_1, btlUnit_Unit* btlUnit_2, u16 param_4, u16 ElementalAffinity, u16 param_6, u16 param_7, u8 SkillMode_1_2 );
SHK_HOOK( void, ParseSKILLTBL, u64 param_1);
SHK_HOOK( u64, ReturnAddressOfSKILLTBL_Segment0, u32 param_1);
SHK_HOOK( u64, ReturnAddressOfSKILLTBL_Segment1, u32 param_1);

static void ParseSKILLTBLHook ( u64 param_1 ){
  memset( &NewSKILLTBL_Segment0, 0xFF, sizeof( NewSKILLTBL_Segment0 ) );
  memset( &NewSKILLTBL_Segment1, 0xFF, sizeof( NewSKILLTBL_Segment1 ) );

  u16 uVar1;
  u32 uVar2;
  u64 uVar3;
  undefined8 uVar4;
  u64 uVar5;
  int iVar7;
  u64 lVar6;
  u16 *puVar8;
  u8 bVar9;
  u64 lVar10;
  u32 local_30 [4];
  
  FUN_0090053c((u32)local_30,param_1,4);
  uVar4 = __builtin_clz(0);
  bVar9 = (u8)((u32)uVar4 >> 5) & 1 ^ 1;
  if (bVar9 != 0) {
    local_30[0] = local_30[0] << 0x18 | (local_30[0] & 0xff00) << 8 | local_30[0] >> 0x18 | local_30[0] >> 8 & 0xff00;
  }
  uVar3 = param_1 + 4 & 0xffffffff;
  FUN_0090053c(&NewSKILLTBL_Segment0,uVar3,(u64)local_30[0]);
  uVar5 = (u64)local_30[0] + 4;
  iVar7 = (int)uVar5;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar5 & 0xf) != 0)) * -0x10);
  lVar6 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar5;
  lVar10 = 0;
  if (0 < (int)lVar6) {
    lVar10 = lVar6;
  }
  uVar3 = uVar3 + local_30[0] + lVar10 & 0xffffffff;
  FUN_0090053c((u32)local_30,uVar3,4);
  if (bVar9 != 0) {
    local_30[0] = local_30[0] << 0x18 |
                  (local_30[0] & 0xff00) << 8 | local_30[0] >> 0x18 | local_30[0] >> 8 & 0xff00;
  }
  FUN_0090053c(&NewSKILLTBL_Segment1,uVar3 + 4 & 0xffffffff,(u64)local_30[0]);
  if (bVar9 != 0) {
    lVar10 = 2000; // Change
    puVar8 = &NewSKILLTBL_Segment1;
    do {
      uVar2 = *(puVar8 + 0x10);
      *puVar8 = *puVar8 >> 8 | *puVar8 << 8;
      uVar1 = puVar8[3];
      *(puVar8 + 0x10) =
           uVar2 << 0x18 | (uVar2 & 0xff00) << 8 | uVar2 >> 0x18 | uVar2 >> 8 & 0xff00;
      puVar8[3] = uVar1 >> 8 | uVar1 << 8;
      uVar1 = puVar8[4];
      uVar2 = *(puVar8 + 0x12);
      puVar8[4] = uVar1 >> 8 | uVar1 << 8;
      uVar1 = puVar8[0xc];
      *(puVar8 + 0x12) =
           uVar2 << 0x18 | (uVar2 & 0xff00) << 8 | uVar2 >> 0x18 | uVar2 >> 8 & 0xff00;
      puVar8[0xc] = uVar1 >> 8 | uVar1 << 8;
      uVar1 = puVar8[0xe];
      uVar2 = *(puVar8 + 8);
      puVar8[0xe] = uVar1 >> 8 | uVar1 << 8;
      *(puVar8 + 8) =
           uVar2 << 0x18 | (uVar2 & 0xff00) << 8 | uVar2 >> 0x18 | uVar2 >> 8 & 0xff00;
      lVar10 = lVar10 + -1;
      puVar8 = puVar8 + 0x16;
    } while (lVar10 != 0);
  }
  return;
}

static u64* ReturnAddressOfSKILLTBL_Segment0Hook( u32 param_1 )
{
    return &NewSKILLTBL_Segment0.entry[param_1];
}

static u64* ReturnAddressOfSKILLTBL_Segment1Hook( u32 param_1 )
{
    
    if (param_1 >= 1500){ // New active skills starts at ID 1500, change if necessary
        return &NewSKILLTBL_Segment1.entry[param_1-700];
    }
    else {
        return &NewSKILLTBL_Segment1.entry[param_1];
    }
    
}

static u64 ResistancePassiveCheckHook ( btlUnit_Unit* btlUnit, ElementalType ElementID ){
    u64 Resist = 0x10000032;
    u64 Block = 0x1000064;
    u64 Repel = 0x2000064;
    u64 Drain =  0x4000064;
    u64 Weak = 0x800007d;
    u64 Neutral = 0x140032; //Made-up tail value

    u64 Resistance = SHK_CALL_HOOK( ResistancePassiveCheck, btlUnit, ElementID ); // Have some initialization I don't really understand

    bool pass = false;
    
    if (ElementID >= ET_Physical) {
        if ( CheckHasSkill ( btlUnit, 1000 ) ) {
            Resistance = Resistance | Repel;
            pass = true;
        }
    }

    if (ElementID == ET_Gun) {
        if ( CheckHasSkill ( btlUnit, 1001 ) ) {
            Resistance = Resistance | Resist;
            pass = true;
        }
        if ( CheckHasSkill ( btlUnit, 1002 ) ) {
            Resistance = Resistance | Block;
            pass = true;
        }
        if ( CheckHasSkill ( btlUnit, 1003 ) ) {
            Resistance = Resistance | Repel;
            pass = true;
        }
        if ( CheckHasSkill ( btlUnit, 1004 ) ) {
            Resistance = Resistance | Drain;
            pass = true;
        }
    }

    if ( !pass ){
        Resistance = SHK_CALL_HOOK( ResistancePassiveCheck, btlUnit, ElementID );
    }

    return Resistance;
}

static u64 SkillHealDamageModifierHook ( u16 SkillID, btlUnit_Unit* btlUnit_1, btlUnit_Unit* btlUnit_2, u16 param_4, u16 ElementalAffinity, u16 param_6, u16 param_7, u8 SkillMode_1_2 )
{
  bool Pass = false;
  u64 Result;

  double EffectToHPCalc; // fp31
  u64 TotalHPIncrease; // r25
  ActiveSkillData* SkillData; // r26
  u64 TotalHPReduce; // r24
  double SomeKindOfMultiplier; // fp30
  double ExtraEffectToHP; // fp29
  double EffectToHP; // fp28
  int SPEffectOrDamageHealingType; // r23
  u16 BaseDamage;
  u16 DamageHealingType; // r17
  u64 CurrentHPSP; // r18
  u16 HPSPBonus; // r3
  u64 HPSPReduceBase; // r4
  u16 Unknown; // r17
  int v29; // r3
  u64 v30; // r3
  int v31; // r3
  u64 v32; // r3
  ElementalType ElementID; // r18
  int v34; // r31
  int v35; // r3
  double v70; // fp21
  double v71; // fp23
  double v72; // fp21
  double v73; // fp21
  bool v74; // r3
  double v75; // fp2
  double v76; // fp3
  double v77; // fp31
  double v78; // fp23
  double v79; // fp31
  bool v80; // r3
  bool v81; // r3
  double v82; // fp23
  int v83; // r31
  int v84; // r31
  double v85; // fp27
  double v86; // fp25
  double v87; // fp27
  double v88; // fp25
  bool v89; // r3
  double v90; // fp2
  bool v91; // r3
  double v92; // fp2
  double v93; // fp2
  int v94; // r3
  int v95; // r4
  u64 v117; // r3
  int v119; // r23
  int TotalDmg; // r4
  int v122; // r3
  int v123; // r4
  int v124; // r3
  int TotalHealing; // r25
  int TotalHealingCalc; // r4

  EffectToHPCalc = 1.0;
  TotalHPIncrease = 0;
  SkillData = (ActiveSkillData*)ReturnAddressOfSKILLTBL_Segment1Hook(SkillID);
  TotalHPReduce = 0;
  SomeKindOfMultiplier = 1.0;
  ExtraEffectToHP = 1.0;
  EffectToHP = 1.0;
  if ( SkillMode_1_2 == 2 ) // SP related
  {
    SPEffectOrDamageHealingType = SkillData->SPEffect;
    Unknown = *(&SkillData->Field1B + 1);
    CurrentHPSP = btlUnit_2->currentSP;
    HPSPBonus = GetCountEquipSPBonus(btlUnit_2);
    HPSPReduceBase = Unknown;
  }
  else if ( SkillMode_1_2 == 1 ) // HP related
  {
    SPEffectOrDamageHealingType = SkillData->DamageHealingType;
    BaseDamage = SkillData->BaseDamage;
    CurrentHPSP = btlUnit_2->currentHP;
    HPSPBonus = GetCountEquipHPBonus(btlUnit_2);
    HPSPReduceBase = BaseDamage;
  }

  if ( SPEffectOrDamageHealingType > 7 )
  {
    if ( SPEffectOrDamageHealingType > 12 )
    {
      if ( SPEffectOrDamageHealingType > 14 )
      {
         if ( SPEffectOrDamageHealingType <= 15 )
        {
          TotalHPIncrease = FUN_26AE14(HPSPReduceBase);
          goto FinalDamageCalc;
        }
        if ( SPEffectOrDamageHealingType > 16 ){
          goto FinalDamageCalc;
        }
        
        TotalHPReduce = FUN_25E5D8(9, btlUnit_1, btlUnit_2, SkillID, SkillMode_1_2);
        if ( TotalHPReduce <= 0 )
          TotalHPReduce = 1;
LABEL_85:
        if ( TotalHPReduce <= 0 || (param_7 & 1) != 0 )
        {
LABEL_506:
          if ( TotalHPIncrease > 0 && (param_7 & 1) == 0 )
          {
            if ( CheckHasSkill(btlUnit_1, 829) )
              EffectToHP = (EffectToHP * 1.5);
            v122 = ActualGetCount(220);
            if ( v122 > 0 )
            {
              v123 = v122 - 1;
              v124 = 2;
              if ( v123 < 2 )
                v124 = v123;
              EffectToHPCalc = (EffectToHPCalc * ((v124 * 0.25) + 1.5));
            }
            TotalHealing = 99999;
            TotalHealingCalc = ((((TotalHPIncrease * EffectToHPCalc) * SomeKindOfMultiplier) * ExtraEffectToHP) * EffectToHP);
            if ( TotalHealingCalc <= 99999 )
            {
              if ( TotalHealingCalc < 1 )
                TotalHealingCalc = 1;
              TotalHealing = TotalHealingCalc;
            }
            TotalHPIncrease = (TotalHealing * FUN_26AE14(0xBu) + 95 * TotalHealing) / 100;
            if ( TotalHPIncrease <= 0 )
              TotalHPIncrease = 1;
          }
          goto FinalDamageCalc;
        }
        ElementID = GetSkillData(btlUnit_1, SkillID);
        if ( SkillData->DamageStat == 2 ) // Magic stat
        {
          if ( SPEffectOrDamageHealingType != 16 )
          {
            EffectToHPCalc = FUN_250A10(btlUnit_1, 0);
            if ( FUN_2595C4(btlUnit_1, 23) > 0 ) // Concentrate buff
              EffectToHPCalc = (EffectToHPCalc * 2.5);
          }
          SomeKindOfMultiplier = FUN_250A10(btlUnit_2, 2);
          if ( SPEffectOrDamageHealingType != 16 )
            FUN_25A328(btlUnit_1, 23);
        }
        else if ( SkillData->DamageStat == 1 ) // Strength stat
        {
          if ( SPEffectOrDamageHealingType != 16 )
          {
            EffectToHPCalc = FUN_250A10(btlUnit_1, 0);
            if ( FUN_2595C4(btlUnit_1, 22) > 0 ) // Charge buff
              EffectToHPCalc = (EffectToHPCalc * 2.5);
          }
          SomeKindOfMultiplier = FUN_250A10(btlUnit_2, 2);
          if ( SPEffectOrDamageHealingType != 16 )
            FUN_25A328(btlUnit_1, 22);
        }
        if ( SPEffectOrDamageHealingType == 16 )
        {
          v34 = 0;
          while ( 1 )
          {
            if ( GetUnitIDFromPartySlot(v34) )
            {
              v35 = GetBtlPlayerUnitFromID(v34);
              if ( unitGetEquipment(v35, 2) == 0x20F7 ) // Midnight Bandana?
                break;
            }
            if ( ++v34 >= 9 )
              goto ElementalDamageCalc;
          }
          EffectToHPCalc = (EffectToHPCalc * 1.15);
        }
ElementalDamageCalc:
        if ( ElementID >= ET_Physical && ElementID <= ET_Almighty)
        {
          double AmpAll = 1.0;
          if ( CheckHasSkill(btlUnit_1, 1005) ){
            AmpAll = 1.0 * 2.0;
            Pass = true;
          }
          EffectToHP = 1.0 * AmpAll;
        }

        v70 = 0.5;
        if ( CheckHasSkill(btlUnit_2, 858) )
          EffectToHP = (EffectToHP * 0.5);
        if ( !FUN_002588b4(btlUnit_2) && CheckAccessoryEffect(btlUnit_2, 186) )
          EffectToHP = (EffectToHP * 0.5);
        if ( (*&btlUnit_1->Flags & 0x20000000) != 0 )
          EffectToHPCalc = (EffectToHPCalc * 0.1);
        v71 = 1.0;
        if ( GetBitflagState(0x2160) )
        {
          if ( !FUN_002588b4(btlUnit_1) )
            v70 = 2.0;
          v71 = v70;
        }
        else if ( GetBitflagState(0x2161) )
        {
          if ( !FUN_002588b4(btlUnit_1) )
            v70 = 1.25;
          v71 = v70;
        }
        else if ( GetBitflagState(0x2162) )
        {
          v71 = 1.0;
          if ( !FUN_002588b4(btlUnit_1) )
            v71 = 1.25;
        }
        else
        {
          v72 = 1.6;
          if ( GetBitflagState(0x2163) )
          {
            if ( GetBitflagState(0x10CE) )
            {
              if ( !FUN_002588b4(btlUnit_1) )
                v72 = 0.8;
            }
            else
            {
              v72 = 1.3;
              if ( !FUN_002588b4(btlUnit_1) )
                v72 = 0.8;
            }
            v71 = v72;
          }
          else if ( GetBitflagState(0x2164) )
          {
            if ( !FUN_002588b4(btlUnit_1) )
              v72 = 0.8;
            v71 = v72;
          }
        }
        v73 = (EffectToHPCalc * v71);
        if ( (*&btlUnit_1->Flags & 0x800000) != 0 )
          FUN_002588b4(btlUnit_1);
        if ( (*&btlUnit_2->Flags & 0x800000) != 0 )
          FUN_002588b4(btlUnit_2);
        if ( !AilmentCheck(btlUnit_2, 0x100) )
        {
          if ( AilmentCheck(btlUnit_2, 0x200) )
          {
            ExtraEffectToHP = (1.0 * 2.0);
          }
          else if ( AilmentCheck(btlUnit_2, 0x1000) )
          {
            ExtraEffectToHP = (1.0 * 1.5);
          }
          else if ( AilmentCheck(btlUnit_2, 0x10000) )
          {
            ExtraEffectToHP = (1.0 * 1.75);
          }
          else if ( AilmentCheck(btlUnit_2, 0x400000) )
          {
            ExtraEffectToHP = (1.0 * 2.0);
          }
        }
        if ( AilmentCheck(btlUnit_1, 0x80u) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 0.35);
        }
        else if ( AilmentCheck(btlUnit_1, 0x200) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 2.0);
          if ( CheckAccessoryEffect(btlUnit_1, 156) )
            ExtraEffectToHP = (ExtraEffectToHP * 1.5);
        }
        else if ( AilmentCheck(btlUnit_1, 0x1000) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 1.75);
        }
        else if ( AilmentCheck(btlUnit_1, 0x10000) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 1.5);
        }
        if ( param_6 == 2 )
        {
          FUN_002588b4(btlUnit_1);
          v74 = GetBitflagState(8548);
          v75 = (EffectToHPCalc * v71);
          v76 = 1.5;
          if ( v74 )
          {
            FUN_002588b4(btlUnit_1);
            v75 = ((EffectToHPCalc * v71) * 1.5);
            v76 = 3.0;
          }
        }
        else if ( param_6 == 8 )
        {
          v77 = 1.5;
          if ( !FUN_002588b4(btlUnit_1) )
            v77 = 1.75;
          v78 = v77;
          v79 = (v73 * v77);
          v80 = GetBitflagState(8548);
          v75 = v73;
          v76 = v78;
          if ( v80 )
          {
            FUN_002588b4(btlUnit_1);
            v75 = v79;
            v76 = 3.0;
          }
        }
        else
        {
          v75 = EffectToHPCalc;
          v76 = v71;
          if ( param_6 == 4 )
          {
            v81 = GetBitflagState(8548);
            v75 = EffectToHPCalc;
            v76 = v71;
            if ( v81 )
            {
              FUN_002588b4(btlUnit_1);
              v75 = (EffectToHPCalc * v71);
              v76 = 3.0;
            }
          }
        }
        v82 = (v75 * v76);
        if ( FUN_25963C(btlUnit_2) )
          SomeKindOfMultiplier = (SomeKindOfMultiplier * 0.4);
        if ( AilmentCheck(btlUnit_2, 0x100000) )
          ExtraEffectToHP = (ExtraEffectToHP * 1.25);
        if ( SkillData->ExtraSkillEffect > 15 )
        {
          if ( SkillData->ExtraSkillEffect <= 16 )
          {
            EffectToHPCalc = v82;
            if ( (param_7 & 0x40) != 0 )
            {
              v83 = FUN_25893C(btlUnit_1);
              EffectToHPCalc = v82;
              if ( v83 != FUN_25893C(btlUnit_2) )
                EffectToHPCalc = (v82 * 1.5);
            }
          }
          else
          {
            EffectToHPCalc = v82;
            if ( SkillData->ExtraSkillEffect <= 0x11 )
            {
              EffectToHPCalc = v82;
              if ( (param_7 & 0x400) != 0 )
              {
                v84 = FUN_25893C(btlUnit_1);
                EffectToHPCalc = v82;
                if ( v84 != FUN_25893C(btlUnit_2) )
                  EffectToHPCalc = (v82 * 1.5);
              }
            }
          }
        }
        else
        {
          EffectToHPCalc = v82;
          if ( SkillData->ExtraSkillEffect == 3 )
          {
            EffectToHPCalc = v82;
            if ( AilmentCheck(btlUnit_2, 0x100000) )
              EffectToHPCalc = (v82 * 2.0);
          }
        }
        if ( GetBitflagState(8303) )
        {
          v85 = 1.0;
          if ( (ActualGetCount(205) * 0.25) < 1.0 )
            v85 = (ActualGetCount(205) * 0.25);
          v86 = v85;
          v87 = (1.0 - v85);
          v88 = (v86 + 1.0);
          if ( GetBitflagState(8298) || GetBitflagState(8301) )
          {
            if ( FUN_002588b4(btlUnit_2) )
            {
              v91 = GetBitflagState(8301);
              v90 = v88;
              if ( v91 )
                v90 = v87;
            }
            else
            {
              v89 = GetBitflagState(8298);
              v90 = v88;
              if ( v89 )
                v90 = v87;
            }
            v92 = (SomeKindOfMultiplier * v90);
            SomeKindOfMultiplier = 0.0;
            if ( v92 > 0.0 )
              SomeKindOfMultiplier = v92;
          }
          if ( GetBitflagState(8297) || GetBitflagState(8300) )
          {
            if ( FUN_002588b4(btlUnit_2) )
            {
              if ( GetBitflagState(8300) )
                v87 = v88;
            }
            else if ( GetBitflagState(8297) )
            {
              v87 = v88;
            }
            v93 = (EffectToHPCalc * v87);
            EffectToHPCalc = 0.0;
            if ( v93 > 0.0 )
              EffectToHPCalc = v93;
          }
        }
        v94 = ActualGetCount(220);
        if ( v94 > 0 )
        {
          v95 = 2;
          if ( v94 - 1 < 2 )
            v95 = v94 - 1;
          EffectToHPCalc = (EffectToHPCalc * ((v95 * 0.25) + 1.5));
        }
        if ( FUN_002588b4(btlUnit_2) )
        {
LABEL_498:
          TotalDmg = TotalHPReduce * EffectToHPCalc * SomeKindOfMultiplier * ExtraEffectToHP * EffectToHP;
          if ( param_6 == 16 )
          {
            TotalHPReduce = 99999;
          }
          else
          {
            if ( TotalDmg <= 99999 )
            {
              if ( TotalDmg < 1 )
                TotalDmg = 1;
            }
            TotalHPReduce = (TotalDmg * FUN_26AE14(0xB) + 95 * TotalDmg) / 100;
            if ( TotalHPReduce <= 0 )
              TotalHPReduce = 1;
          }
          goto LABEL_506;
        }
        if ( ElementID > 1 )
        {
LABEL_388:
          if ( CheckAccessoryEffect(btlUnit_2, 106) )
            EffectToHPCalc = (EffectToHPCalc * 0.89999998);
          if ( CheckAccessoryEffect(btlUnit_2, 107) )
            EffectToHPCalc = (EffectToHPCalc * 0.80000001);
          if ( CheckAccessoryEffect(btlUnit_2, 108) )
            EffectToHPCalc = (EffectToHPCalc * 0.69999999);
          goto LABEL_394;
        }
LABEL_394:
        switch ( ElementID )
        {
          case 9:
            v117 = unitGetEquipment(btlUnit_2, 2);
            if ( v117 && v117 != 0x2000 )
            {
              v119 = 0;
              do
              {
                if ( GetEquipmentEffect(v117, v119) == 155 )
                  EffectToHPCalc = (EffectToHPCalc * (1.0 - (CalculateResistEffect(v117) / 100.0)));
                v119 = (v119 + 1);
              }
              while ( v119 < 3 );
            }
            if ( CheckAccessoryEffect(btlUnit_2, 103) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( CheckAccessoryEffect(btlUnit_2, 104) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( CheckAccessoryEffect(btlUnit_2, 105) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
        }
        goto LABEL_498;
      }
      if ( SPEffectOrDamageHealingType <= 13 )
      {
LABEL_50:
        TotalHPReduce = HPSPReduceBase;
        if ( SPEffectOrDamageHealingType == 13 )
        {
          if ( HPSPReduceBase > CurrentHPSP )
            HPSPReduceBase = CurrentHPSP;
          TotalHPReduce = HPSPReduceBase;
        }
        goto FinalDamageCalc;
      }
    }
    else
    {
      if ( SPEffectOrDamageHealingType <= 9 )
      {
        if ( SPEffectOrDamageHealingType <= 8 )
        {
          TotalHPReduce = CurrentHPSP * HPSPReduceBase / 100;
          if ( TotalHPReduce <= 0 && HPSPReduceBase > 0 )
            TotalHPReduce = 1;
        }
        else
        {
          TotalHPIncrease = (CurrentHPSP * (((1374389535 * CurrentHPSP * HPSPReduceBase) >> 32) >> 31)) / 100;
          if ( TotalHPIncrease < 1 )
            TotalHPIncrease = 1;
        }
        goto FinalDamageCalc;
      }
      if ( SPEffectOrDamageHealingType <= 10 )
      {
        v31 = ((1374389535 * HPSPBonus * HPSPReduceBase) >> 32) >> 5;
        TotalHPReduce = (v31 + (v31 >> 31));
        if ( TotalHPReduce <= 0 && HPSPReduceBase > 0 )
          TotalHPReduce = 1;
        goto FinalDamageCalc;
      }
      if ( SPEffectOrDamageHealingType <= 11 )
      {
        v29 = ((0x51EB851F * HPSPBonus * HPSPReduceBase) >> 32) >> 5;
        TotalHPIncrease = (v29 + (v29 >> 31));
        if ( TotalHPIncrease < 1 )
          TotalHPIncrease = 1;
        goto FinalDamageCalc;
      }
    }
LABEL_60:
    if ( SkillData->DamageStat == 2 )
      TotalHPReduce = FUN_25E5D8(2, btlUnit_1, btlUnit_2, SkillID, SkillMode_1_2);
    else if ( SkillData->DamageStat == 1 )
    {
      if ( (param_7 & 0x80) != 0 )
      {
        v32 = FUN_25E5D8(12, btlUnit_1, btlUnit_2, SkillID, SkillMode_1_2);
      }
      else if ( (*&SkillData->CasterEffect1 & 0x200) != 0 )
      {
        v32 = FUN_25E5D8(11, btlUnit_1, btlUnit_2, SkillID, SkillMode_1_2);
      }
      else
      {
        v32 = FUN_25E5D8(1, btlUnit_1, btlUnit_2, SkillID, SkillMode_1_2);
      }
      TotalHPReduce = v32;
    }
    if ( !((SPEffectOrDamageHealingType > 13) | ~(SPEffectOrDamageHealingType > 11)) && TotalHPReduce > CurrentHPSP )
      TotalHPReduce = CurrentHPSP;
    if ( SPEffectOrDamageHealingType == 14 && param_4 > 1 )
      TotalHPReduce = TotalHPReduce / param_4;
    if ( TotalHPReduce <= 0 )
      TotalHPReduce = 1;
    goto LABEL_85;
  }
  switch ( SPEffectOrDamageHealingType )
  {
    case 7:
      TotalHPIncrease = 50;
      if ( TotalHPIncrease <= 0 )
        TotalHPIncrease = 1;
      goto LABEL_85;
    case 6:
      TotalHPReduce = 50;
      goto LABEL_85;
    case 5:
      TotalHPIncrease = HPSPReduceBase;
      goto FinalDamageCalc;
    case 4:
      goto LABEL_50;
  }
  if ( SPEffectOrDamageHealingType != 3 )
  {
    if ( SPEffectOrDamageHealingType != 2 )
    {
      if ( SPEffectOrDamageHealingType != 1 )
        goto FinalDamageCalc;
      goto LABEL_60;
    }
    if ( SkillData->DamageStat == 2 )
    {
      TotalHPIncrease = FUN_25E5D8(3, btlUnit_1, btlUnit_2, SkillID, SkillMode_1_2);
      goto LABEL_46;
    }
    else if ( SkillData->DamageStat == 1 )
    {
      if ( (*&SkillData->CasterEffect1 & 0x200) != 0 )
        v30 = FUN_252980(SkillID, btlUnit_1, btlUnit_2, SkillMode_1_2);
      else
        v30 = FUN_25294C(SkillID, btlUnit_1, btlUnit_2, SkillMode_1_2);
      TotalHPIncrease = v30;
LABEL_46:
      if ( TotalHPIncrease > 0 ){
        goto LABEL_85;
      }
        
      goto LABEL_47;
    }
LABEL_47:
    TotalHPIncrease = 1;
    goto LABEL_85;
  }
  if ( CurrentHPSP > HPSPReduceBase )
    TotalHPReduce = CurrentHPSP - HPSPReduceBase;
  goto FinalDamageCalc;

FinalDamageCalc:
  if (Pass){
    if ( ElementalAffinity == 0x400 && TotalHPReduce > 0 )
    {
      TotalHPIncrease = TotalHPReduce;
      TotalHPReduce = 0;
    }
    if ( ElementalAffinity == 0x100 || ElementalAffinity == 4 || ElementalAffinity == 2 )
    {
      TotalHPIncrease = 0;
      TotalHPReduce = 0;
    }
    if ( SkillMode_1_2 == 1 && SkillData->ExtraSkillEffect == 21 && AilmentCheck(btlUnit_2, 0x80u) )
    {
      TotalHPIncrease = GetCountEquipHPBonus(btlUnit_2);
      TotalHPReduce = 0;
    }
    Result = TotalHPIncrease;
    if ( TotalHPReduce > 0 )
      Result = -TotalHPReduce;
    return Result;
  }
  else {
    Result = SHK_CALL_HOOK( SkillHealDamageModifier, SkillID, btlUnit_1, btlUnit_2, param_4, ElementalAffinity, param_6, param_7, SkillMode_1_2 );
    return Result;
  }
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void scarltzInit( void ){
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( ResistancePassiveCheck, ResistancePassiveCheckHook );
  SHK_BIND_HOOK( SkillHealDamageModifier, SkillHealDamageModifierHook );
  SHK_BIND_HOOK( ParseSKILLTBL, ParseSKILLTBLHook );
  SHK_BIND_HOOK( ReturnAddressOfSKILLTBL_Segment0, ReturnAddressOfSKILLTBL_Segment0Hook );
  SHK_BIND_HOOK( ReturnAddressOfSKILLTBL_Segment1, ReturnAddressOfSKILLTBL_Segment1Hook );
  SHK_BIND_HOOK( SkillHealDamageModifier, SkillHealDamageModifierHook );
}

void scarltzShutdown( void ){
  // Executed when the PRX module is unloaded.    
}
#endif