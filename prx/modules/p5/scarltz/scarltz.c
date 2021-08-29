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
SHK_HOOK( u64, SkillHealDamageModifier, u32 SkillID, btlUnit_Unit *btlUnit_1, btlUnit_Unit *btlUnit_2, u32 param_4, short ElementalAffinity, u8 param_6, u8 param_7, u8 Bit01 );
SHK_HOOK( void, ParseSKILLTBL, u64 param_1);
SHK_HOOK( u64, ReturnAddressOfSKILLTBL_Segment0, u32 param_1);
SHK_HOOK( u64, ReturnAddressOfSKILLTBL_Segment1, u32 param_1);

static u64 ResistancePassiveCheckHook ( btlUnit_Unit* btlUnit, ElementalType ElementID ){
    u64 Resist = 0x10000032;
    u64 Block = 0x1000064;
    u64 Repel = 0x2000064;
    u64 Drain =  0x4000064;
    u64 Weak = 0x800007d;
    u64 Neutral = 0x140032; //Made-up tail value

    u64 Resistance = SHK_CALL_HOOK( ResistancePassiveCheck, btlUnit, ElementID );

    bool pass = false;
    
    if (ElementID >= Physical) {
        if ( CheckHasSkill ( btlUnit, 1000 ) ) {
            Resistance = Resistance | Repel;
            pass = true;
        }
    }

    if (ElementID == GunSkill) {
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

static u64 SkillHealDamageModifierHook ( u32 SkillID, btlUnit_Unit* btlUnit_1, btlUnit_Unit* btlUnit_2, u32 param_4, short ElementalAffinity, u8 param_6, u8 param_7, u8 Bit01 )
{
    u64 Result = SHK_CALL_HOOK( SkillHealDamageModifier, SkillID, btlUnit_1, btlUnit_2, param_4, ElementalAffinity, param_6, param_7, Bit01 );

    return Result;

/*
    ActiveSkillData *SkillData;
    int i;

    u32 CurrentHPSP_sharedvar;
    u32 HPSPBonus;
    int ElementId_sharedvar;
    undefined uVar1;
    bool bVar2;
    short AccessoryId;
    u64 uVar3;
    undefined uVar4;
    u64 HPSPCopy;
    u64 TotalHPIncrease;
    u64 TotalHPReduce;
    u64 Result;
    double extraout_f1;
    double extraout_f1_00;
    double uVar11;
    double dVar5;
    double dVar6;
    double dVar7;
    double dVar8;
    double EffectToHP;
    double HPDamage;
    u16 DamageHealingTypeOrUnknown;
    byte BaseDamageOrMaxHit;
    byte DamageStat;
    u32 SkillIDMasked;
    int TotalHealing;

    SkillData = GetCopySegment1SKILL(SkillID);
    EffectToHP, dVar7, dVar8, HPDamage, v20 = 1.0;
    TotalHPReduce, TotalHPIncrease = 0;

    if (Bit01 < 2) {
        if (Bit01 == 0) goto FinalDamageCalc;
        BaseDamageOrMaxHit = SkillData->MaxHit;
        DamageHealingTypeOrUnknown = &SkillData->DamageHealingType;
        CurrentHPSP_sharedvar = GetCurrentHP(btlUnit_2);
        HPSPBonus = GetCountEquipHPBonus(btlUnit_2);
    }
    else {
        if (2 < Bit01) goto FinalDamageCalc;
        BaseDamageOrMaxHit = &SkillData->BaseDamage + 1;
        DamageHealingTypeOrUnknown = SkillData->Unknown9;
        CurrentHPSP_sharedvar = GetCurrentSP(btlUnit_2);
        HPSPBonus = GetCountEquipSPBonus(btlUnit_2);
    }
    uVar3 = DamageHealingTypeOrUnknown;
    HPSPCopy = CurrentHPSP_sharedvar;

    ElementId_sharedvar = Function_GetSkillData(btlUnit_1,SkillID);
    DamageStat = SkillData->DamageStat;
    if (DamageStat < 2) {
      if (DamageStat != 0) {
        if (BaseDamageOrMaxHit != 0x10) {
          FUN_00250a10(btlUnit_1,0,SkillID);
          FUN_002595c4(btlUnit_1,0x16);
        }
        FUN_00250a10(btlUnit_2,2,SkillID);
        if (BaseDamageOrMaxHit != 0x10) {
          FUN_0025a328(btlUnit_1,0x16);
        }
      }
    }
    else {
      if (DamageStat < 3) {
        if (BaseDamageOrMaxHit != 0x10) {
          FUN_00250a10(btlUnit_1,0,SkillID);
          FUN_002595c4(btlUnit_1,0x17);
        }
        FUN_00250a10(btlUnit_2,2,SkillID);
        if (BaseDamageOrMaxHit != 0x10) {
          FUN_0025a328(btlUnit_1,0x17);
        }
      }
    }

    if ( ElementID <= 9 )
    {
        CurseDmgMul = 1.0;
        CurseAccID = GetEquipActual(btlUnit_Unit_1, 2);
        if ( CurseAccID != 0x2000 )
        {
            i = 0;
            do
            {
                if ( GetEquipmentEffect(CurseAccID, i) == 151 )
                    CurseDmgMul = CurseDmgMul * (CalculateResistEffect(CurseAccID) / 100.0) + 1.0;
                i++;
            }
            while ( i < 3 );
        }

        if ( CheckHasSkill(btlUnit_Unit_1, 982) )
            CurseDmgMul = CurseDmgMul * 1.5);
        if ( CheckHasSkill(btlUnit_Unit_1, 984) )
            CurseDmgMul = CurseDmgMul * 1.25);
        if ( CurseDmgMul > 2.0 )
            CurseDmgMul = 2.0;

        EffectToHP = 1.0 * CurseDmgMul;
    }

    v70 = 0.5;
    if ( CheckHasSkill(btlUnit_Unit_2, 858) )
        EffectToHP = EffectToHP * 0.5);
    if ( !sub_2588B4(btlUnit_Unit_2) && CheckAccessoryEffect(btlUnit_Unit_2, 186) )
        EffectToHP = EffectToHP * 0.5);
    if ( &btlUnit_Unit_1->Flags & 0x20000000) != 0 )
        EffectToHPCalc = EffectToHPCalc * 0.1);
        v71 = 1.0;
        if ( GetBitflagState(0x2160) )
        {
          if ( !sub_2588B4(btlUnit_Unit_1) )
            v70 = 2.0;
          v71 = v70;
        }
        else if ( GetBitflagState(0x2161) )
        {
          if ( !sub_2588B4(btlUnit_Unit_1) )
            v70 = 1.25;
          v71 = v70;
        }
        else if ( GetBitflagState(0x2162) )
        {
          v71 = 1.0;
          if ( !sub_2588B4(btlUnit_Unit_1) )
            v71 = 1.25;
        }
        else
        {
          v72 = 1.6;
          if ( GetBitflagState(0x2163) )
          {
            if ( GetBitflagState(0x10CE) )
            {
              if ( !sub_2588B4(btlUnit_Unit_1) )
                v72 = 0.80000001;
            }
            else
            {
              v72 = 1.3;
              if ( !sub_2588B4(btlUnit_Unit_1) )
                v72 = 0.80000001;
            }
            v71 = v72;
          }
          else if ( GetBitflagState(0x2164) )
          {
            if ( !sub_2588B4(btlUnit_Unit_1) )
              v72 = 0.80000001;
            v71 = v72;
          }
        }
        v73 = EffectToHPCalc * v71);
        if ( &btlUnit_Unit_1->Flags & 0x800000) != 0 )
          sub_2588B4(btlUnit_Unit_1);
        if ( &btlUnit_Unit_2->Flags & 0x800000) != 0 )
          sub_2588B4(btlUnit_Unit_2);
        if ( !Function_AilmentCheckProb(btlUnit_Unit_2, 0x100u) )
        {
          if ( Function_AilmentCheckProb(btlUnit_Unit_2, 0x200u) )
          {
            ExtraEffectToHP = 1.0 * 2.0);
          }
          else if ( Function_AilmentCheckProb(btlUnit_Unit_2, 0x1000u) )
          {
            ExtraEffectToHP = (1.0 * 1.5);
          }
          else if ( Function_AilmentCheckProb(btlUnit_Unit_2, 0x10000u) )
          {
            ExtraEffectToHP = (1.0 * 1.75);
          }
          else if ( Function_AilmentCheckProb(btlUnit_Unit_2, 0x400000u) )
          {
            ExtraEffectToHP = (1.0 * 2.0);
          }
        }
        if ( Function_AilmentCheckProb(btlUnit_Unit_1, 0x80u) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 0.34999999);
        }
        else if ( Function_AilmentCheckProb(btlUnit_Unit_1, 0x200u) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 2.0);
          if ( Function_CheckAccessoryEffect(btlUnit_Unit_1, 156) )
            ExtraEffectToHP = (ExtraEffectToHP * 1.5);
        }
        else if ( Function_AilmentCheckProb(btlUnit_Unit_1, 0x1000u) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 1.75);
        }
        else if ( Function_AilmentCheckProb(btlUnit_Unit_1, 0x10000u) )
        {
          ExtraEffectToHP = (ExtraEffectToHP * 1.5);
        }
        if ( v10 == 2 )
        {
          sub_2588B4(btlUnit_Unit_1);
          v74 = Function_GetBitflagState(8548);
          v75 = (EffectToHPCalc * v71);
          v76 = 1.5;
          if ( v74 )
          {
            sub_2588B4(btlUnit_Unit_1);
            v75 = ((EffectToHPCalc * v71) * 1.5);
            v76 = 3.0;
          }
        }
        else if ( v10 == 8 )
        {
          v77 = 1.5;
          if ( !sub_2588B4(btlUnit_Unit_1) )
            v77 = 1.75;
          v78 = v77;
          v79 = (v73 * v77);
          v80 = Function_GetBitflagState(8548);
          v75 = v73;
          v76 = v78;
          if ( v80 )
          {
            sub_2588B4(btlUnit_Unit_1);
            v75 = v79;
            v76 = 3.0;
          }
        }
        else
        {
          v75 = EffectToHPCalc;
          v76 = v71;
          if ( v10 == 4 )
          {
            v81 = Function_GetBitflagState(8548);
            v75 = EffectToHPCalc;
            v76 = v71;
            if ( v81 )
            {
              sub_2588B4(btlUnit_Unit_1);
              v75 = (EffectToHPCalc * v71);
              v76 = 3.0;
            }
          }
        }
        v82 = (v75 * v76);
        if ( sub_25963C(btlUnit_Unit_2) )
          v20 = (v20 * 0.40000001);
        if ( Function_AilmentCheckProb(btlUnit_Unit_2, 0x100000u) )
          ExtraEffectToHP = (ExtraEffectToHP * 1.25);
        if ( SkillData->unsigned28 > 15u )
        {
          if ( SkillData->unsigned28 <= 16u )
          {
            EffectToHPCalc = v82;
            if ( (a7 & 0x40) != 0 )
            {
              v83 = sub_25893C(btlUnit_Unit_1);
              EffectToHPCalc = v82;
              if ( v83 != sub_25893C(btlUnit_Unit_2) )
                EffectToHPCalc = (v82 * 1.5);
            }
          }
          else
          {
            EffectToHPCalc = v82;
            if ( SkillData->unsigned28 <= 0x11u )
            {
              EffectToHPCalc = v82;
              if ( (a7 & 0x400) != 0 )
              {
                v84 = sub_25893C(btlUnit_Unit_1);
                EffectToHPCalc = v82;
                if ( v84 != sub_25893C(btlUnit_Unit_2) )
                  EffectToHPCalc = (v82 * 1.5);
              }
            }
          }
        }
        else
        {
          EffectToHPCalc = v82;
          if ( SkillData->unsigned28 == 3 )
          {
            EffectToHPCalc = v82;
            if ( Function_AilmentCheckProb(btlUnit_Unit_2, 0x100000u) )
              EffectToHPCalc = (v82 * 2.0);
          }
        }
        if ( Function_GetBitflagState(8303) )
        {
          v85 = 1.0;
          if ( (sub_24BF5C(205) * 0.25) < 1.0 )
            v85 = (sub_24BF5C(205) * 0.25);
          v86 = v85;
          v87 = (1.0 - v85);
          v88 = (v86 + 1.0);
          if ( Function_GetBitflagState(8298) || Function_GetBitflagState(8301) )
          {
            if ( sub_2588B4(btlUnit_Unit_2) )
            {
              v91 = Function_GetBitflagState(8301);
              v90 = v88;
              if ( v91 )
                v90 = v87;
            }
            else
            {
              v89 = Function_GetBitflagState(8298);
              v90 = v88;
              if ( v89 )
                v90 = v87;
            }
            v92 = (v20 * v90);
            v20 = 0.0;
            if ( v92 > 0.0 )
              v20 = v92;
          }
          if ( Function_GetBitflagState(8297) || Function_GetBitflagState(8300) )
          {
            if ( sub_2588B4(btlUnit_Unit_2) )
            {
              if ( Function_GetBitflagState(8300) )
                v87 = v88;
            }
            else if ( Function_GetBitflagState(8297) )
            {
              v87 = v88;
            }
            v93 = (EffectToHPCalc * v87);
            EffectToHPCalc = 0.0;
            if ( v93 > 0.0 )
              EffectToHPCalc = v93;
          }
        }
        v94 = sub_24BF5C(220);
        if ( v94 > 0 )
        {
          v95 = 2;
          if ( v94 - 1 < 2 )
            v95 = v94 - 1;
          EffectToHPCalc = (EffectToHPCalc * ((v95 * 0.25) + 1.5));
        }
        if ( sub_2588B4(btlUnit_Unit_2) )
        {

          TotalDmg = ((((TotalHPReduce * EffectToHPCalc) * v20) * ExtraEffectToHP) * EffectToHP);
          if ( v10 == 16 )
          {
            TotalHPReduce = 99999LL;
          }
          else
          {
            TotalDmgCopy = 99999;
            if ( TotalDmg <= 99999 )
            {
              if ( TotalDmg < 1 )
                TotalDmg = 1;
              TotalDmgCopy = TotalDmg;
            }
            TotalHPReduce = (TotalDmgCopy * sub_26AE14(0xBu) + 95 * TotalDmgCopy) / 100;
            if ( TotalHPReduce <= 0 )
              TotalHPReduce = 1LL;
          }
          goto LABEL_506;
        }
        if ( !ElementID )
        {
          if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 79) )
            EffectToHPCalc = (EffectToHPCalc * 0.89999998);
          if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 80) )
            EffectToHPCalc = (EffectToHPCalc * 0.80000001);
          if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 81) )
            EffectToHPCalc = (EffectToHPCalc * 0.69999999);
        }
        if ( ElementID > 5 )
        {
          if ( ElementID <= 7 || ElementID <= 9 )
            goto LABEL_388;
        }
        else if ( ElementID > 1 )
        {
LABEL_388:
          if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 106) )
            EffectToHPCalc = (EffectToHPCalc * 0.89999998);
          if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 107) )
            EffectToHPCalc = (EffectToHPCalc * 0.80000001);
          if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 108) )
            EffectToHPCalc = (EffectToHPCalc * 0.69999999);
          goto LABEL_394;
        }
LABEL_394:
        switch ( ElementID )
        {
          case 9:
            v117 = Function_GetEquipActual(btlUnit_Unit_2, 2);
            v118 = v117;
            if ( (_DWORD)v117 && (_DWORD)v117 != 0x2000 )
            {
              LOWORD(v119) = 0;
              do
              {
                if ( Function_GetEquipmentEffect(v118, v119) == 155 )
                  EffectToHPCalc = (EffectToHPCalc * (1.0 - (Function_CalculateResistEffect(v118) / 100.0)));
                v119 = v119 + 1;
              }
              while ( v119 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 103) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 104) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 105) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
          case 8:
            v114 = Function_GetEquipActual(btlUnit_Unit_2, 2);
            v115 = v114;
            if ( (_DWORD)v114 && (_DWORD)v114 != 0x2000 )
            {
              LOWORD(v116) = 0;
              do
              {
                if ( Function_GetEquipmentEffect(v115, v116) == 154 )
                  EffectToHPCalc = (EffectToHPCalc * (1.0 - (Function_CalculateResistEffect(v115) / 100.0)));
                v116 = v116 + 1;
              }
              while ( v116 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 100) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 101) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 102) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
          case 7:
            v108 = Function_GetEquipActual(btlUnit_Unit_2, 2);
            v109 = v108;
            if ( (_DWORD)v108 && (_DWORD)v108 != 0x2000 )
            {
              LOWORD(v110) = 0;
              do
              {
                if ( Function_GetEquipmentEffect(v109, v110) == 152 )
                  EffectToHPCalc = (EffectToHPCalc * (1.0 - (Function_CalculateResistEffect(v109) / 100.0)));
                v110 = v110 + 1;
              }
              while ( v110 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 94) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 95) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 96) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
          case 6:
            v111 = Function_GetEquipActual(btlUnit_Unit_2, 2);
            v112 = v111;
            if ( (_DWORD)v111 && (_DWORD)v111 != 0x2000 )
            {
              LOWORD(v113) = 0;
              do
              {
                if ( Function_GetEquipmentEffect(v112, v113) == 153 )
                  EffectToHPCalc = (EffectToHPCalc * (1.0 - (Function_CalculateResistEffect(v112) / 100.0)));
                v113 = v113 + 1;
              }
              while ( v113 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 97) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 98) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 99) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
          case 5:
            v105 = Function_GetEquipActual(btlUnit_Unit_2, 2);
            v106 = v105;
            if ( v105 && v105 != 0x2000 )
            {
              v107 = 0;
              do
              {
                if ( Function_GetEquipmentEffect(v106, v107) == 146 )
                  EffectToHPCalc = (EffectToHPCalc * (1.0 - (Function_CalculateResistEffect(v106) / 100.0)));
                v107 = v107 + 1;
              }
              while ( v107 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 91) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 92) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 93) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
          case 4:
            v102 = Function_GetEquipActual(btlUnit_Unit_2, 2);
            v103 = v102;
            if ( (_DWORD)v102 && (_DWORD)v102 != 0x2000 )
            {
              LOWORD(v104) = 0;
              do
              {
                if ( Function_GetEquipmentEffect(v103, v104) == 147 )
                  EffectToHPCalc = (EffectToHPCalc * (1.0 - (Function_CalculateResistEffect(v103) / 100.0)));
                v104 = v104 + 1;
              }
              while ( v104 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 88) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 89) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 90) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
          case 3:
            v99 = Function_GetEquipActual(btlUnit_Unit_2, 2);
            v100 = v99;
            if ( (_DWORD)v99 && (_DWORD)v99 != 0x2000 )
            {
              LOWORD(v101) = 0;
              do
              {
                if ( Function_GetEquipmentEffect(v100, v101) == 145 )
                  EffectToHPCalc = (EffectToHPCalc
                                         * (1.0
                                                 - (Function_CalculateResistEffect(v100)
                                                         / 100.0)));
                v101 = (unsigned __int16)(v101 + 1);
              }
              while ( v101 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 85) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 86) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 87) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
          case 2:
            FireRelatedAccID = Function_GetEquipActual(btlUnit_Unit_2, 2);
            FireRelatedAccCopy = FireRelatedAccID;
            if ( (_DWORD)FireRelatedAccID && (_DWORD)FireRelatedAccID != 0x2000 )
            {
              LOWORD(v98) = 0;
              do
              {
                if ( Function_GetEquipmentEffect(FireRelatedAccCopy, v98) == 144 )
                  EffectToHPCalc = (EffectToHPCalc
                                         * (1.0
                                                 - (Function_CalculateResistEffect(FireRelatedAccCopy)
                                                         / 100.0)));
                v98 = (unsigned __int16)(v98 + 1);
              }
              while ( v98 < 3 );
            }
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 82) )
              EffectToHPCalc = (EffectToHPCalc * 0.89999998);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 83) )
              EffectToHPCalc = (EffectToHPCalc * 0.80000001);
            if ( Function_CheckAccessoryEffect(btlUnit_Unit_2, 84) )
              EffectToHPCalc = (EffectToHPCalc * 0.69999999);
            break;
        }
        goto LABEL_498;
      }
      if ( BaseDamageOrMaxHit <= 13 )
      {
LABEL_50:
        TotalHPReduce = HPReduceCopy;
        if ( BaseDamageOrMaxHit == 13 )
        {
          if ( HPReduceCopy > CurrentHPSP )
            HPReduceCopy = CurrentHPSP;
          TotalHPReduce = HPReduceCopy;
        }
        goto FinalDamageCalc;
      }
    }
    else
    {
      if ( BaseDamageOrMaxHit <= 9 )
      {
        if ( BaseDamageOrMaxHit <= 8 )
        {
          TotalHPReduce = CurrentHPSP * HPReduceCopy / 100;
          if ( TotalHPReduce <= 0 && HPReduceCopy > 0 )
            TotalHPReduce = 1LL;
        }
        else
        {
          TotalHPIncrease = (CurrentHPSP * (((0x51EB851FLL * CurrentHPSP * HPReduceCopy) >> 32) >> 31)) / 0x64;
          if ( TotalHPIncrease < 1 )
            TotalHPIncrease = 1LL;
        }
        goto FinalDamageCalc;
      }
      if ( BaseDamageOrMaxHit <= 10 )
      {
        v31 = ((0x51EB851FLL * HPSPBonus * HPReduceCopy) >> 32) >> 5;
        TotalHPReduce = (v31 + (v31 >> 31));
        if ( TotalHPReduce <= 0 && HPReduceCopy > 0 )
          TotalHPReduce = 1LL;
        goto FinalDamageCalc;
      }
      if ( BaseDamageOrMaxHit <= 11 )
      {
        v29 = ((0x51EB851FLL * HPSPBonus * HPReduceCopy) >> 32) >> 5;
        TotalHPIncrease = (v29 + (v29 >> 31));
        if ( TotalHPIncrease < 1 )
          TotalHPIncrease = 1LL;
        goto FinalDamageCalc;
      }
    }
LABEL_60:
    if ( SkillData->DamageStat > 1 )
    {
      if ( SkillData->DamageStat <= 2 )
        TotalHPReduce = sub_25E5D8( 2, btlUnit_Unit_1, btlUnit_Unit_2, SkillId_v8, Bit01Copy);
    }
    else if ( SkillData->DamageStat )
    {
      if ( (a7 & 0x80) != 0 )
      {
        v32 = sub_25E5D8(12LL, btlUnit_Unit_1, btlUnit_Unit_2, SkillId_v8, Bit01Copy);
      }
      else if ( &SkillData->CasterEffect1 & 0x200) != 0 )
      {
        v32 = sub_25E5D8(11LL, btlUnit_Unit_1, btlUnit_Unit_2, SkillId_v8, Bit01Copy);
      }
      else
      {
        v32 = sub_25E5D8(1LL, btlUnit_Unit_1, btlUnit_Unit_2, SkillId_v8, Bit01Copy);
      }
      TotalHPReduce = v32;
    }
    if ( !((BaseDamageOrMaxHit > 13) | ((BaseDamageOrMaxHit > 11))
      && TotalHPReduce > CurrentHPSP )
    {
      TotalHPReduce = CurrentHPSP;
    }
    if ( BaseDamageOrMaxHit == 14 && v14 > 1 )
      TotalHPReduce = TotalHPReduce / v14;
    if ( TotalHPReduce <= 0 )
      TotalHPReduce = 1;
    goto LABEL_85;
  }
  switch ( BaseDamageOrMaxHit )
  {
    case 7:
      TotalHPIncrease = Function_Return50();
      if ( TotalHPIncrease <= 0 )
        TotalHPIncrease = 1;
      goto LABEL_85;
    case 6:
      TotalHPReduce = 50;
      goto LABEL_85;
    case 5:
      TotalHPIncrease = HPReduceCopy;
      goto FinalDamageCalc;
    case 4:
      goto LABEL_50;
  }
  if ( BaseDamageOrMaxHit != 3 )
  {
    if ( BaseDamageOrMaxHit != 2 )
    {
      if ( BaseDamageOrMaxHit != 1 )
        goto FinalDamageCalc;
      goto LABEL_60;
    }
    if ( SkillData->DamageStat > 1 )
    {
      if ( SkillData->DamageStat <= 2 )
      {
        TotalHPIncrease = sub_25E5D8(
                            3,
                            btlUnit_Unit_1,
                            btlUnit_Unit_2,
                            SkillId_v8,
                            Bit01Copy);
      }
    }
    else if ( SkillData->DamageStat )
    {
      if ( &SkillData->CasterEffect1 & 0x200) != 0 )
        v30 = sub_252980(SkillId_v8, btlUnit_Unit_1, btlUnit_Unit_2, Bit01Copy);
      else
        v30 = sub_25294C(SkillId_v8, btlUnit_Unit_1, btlUnit_Unit_2, Bit01Copy);
      TotalHPIncrease = v30;
      if ( TotalHPIncrease > 0 )
        goto LABEL_85;
      goto LABEL_47;
    }
    TotalHPIncrease = 1LL;
    goto LABEL_85;
  }
  if ( CurrentHPSP > HPReduceCopy )
    TotalHPReduce = CurrentHPSP - HPReduceCopy;

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
  if ( Bit01Copy == 1 && SkillData->ExtraSkillEffect == 21 && AilmentCheckProb(btlUnit_Unit_2, 0x80u) )
  {
    TotalHPIncrease = GetCountEquipHPBonus(btlUnit_Unit_2);
    TotalHPReduce = 0;
  }
  result = TotalHPIncrease;
  if ( TotalHPReduce > 0 )
    result = -TotalHPReduce;
  return result;
*/
}


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
  //SHK_BIND_HOOK( FUN_00044400, FUN_00044400Hook );
}

void scarltzShutdown( void ){
  // Executed when the PRX module is unloaded.    
}
#endif