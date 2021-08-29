#ifdef GAME_P5

void scarltzInit( void );
void scarltzShutdown( void );


// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

#include "modules/p5/p5.h"

typedef struct 
{
    u32 field00;
} testStruct;

typedef struct
{
  u8 SkillElementType;
  u8 ActiveOrPassive;
  u8 Flags;
} SkillElement;

typedef struct
{
  u8 CasterEffect1; // 0
  u8 CasterEffect2; // 1
  u8 AreaType; // 2
  u8 DamageStat; // 3
  u8 CostType; // 4
  u16 Unknown0; // 5
  u8 SkillCost; // 7
  u8 Unknown1; // 8
  u8 MagicOrPhys; // 9
  u8 TargetNum; // A
  bool UsableOnEnemy; // B
  bool UsableOnAllies; // C
  u8 AdditionalTargetRestrictions; // D
  u8 Unknown2; // E
  u8 Unknown3; // F
  u8 Unknown4; // 10
  u8 Unknown5; // 11
  u8 Unknown6; // 12
  u8 Unknown7; // 13
  u8 Unknown8; // 14
  u8 Accuracy; // 15
  u8 MinHits; // 16
  u8 MaxHits; // 17
  u8 DamageHealingType; // 18
  u16 BaseDamage; // 19
  u8 SPEffect; // 1B
  u16 Unknown9; // 1C
  u8 SPAmount; // 1E
  u8 SkillEffect; // 1F
  u8 EffectChance; // 20
  u8 Unknown10; // 21
  u8 EffectList1; // 22
  u8 EffectList2; // 23
  u8 EffectList3; // 24
  u8 EffectList4; // 25
  u8 EffectList5; // 26
  u8 EffectList6; // 27
  u8 BuffDebuff; // 28
  u8 ExtraSkillEffect; // 29
  u8 CritChance; // 2A
  u8 Unknown11; // 2B
} ActiveSkillData;

typedef struct
{
  u8 idk[44];

}ActiveSkillData2;

typedef struct
{
  u8 idk[3];

} SkillElement2;

typedef struct
{
  SkillElement2 entry[2000];
}SKILLTBLSegment0;

typedef struct
{
  ActiveSkillData2 entry[2000];
}SKILLTBLSegment1;

SKILLTBLSegment0 NewSKILLTBLSegment0;
SKILLTBLSegment1 NewSKILLTBLSegment1;

#endif