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
  u16 Field05; // 5
  u8 SkillCost; // 7
  u8 Field08; // 8
  u8 MagicOrPhys; // 9
  u8 TargetNum; // A
  u8 UsableOnEnemyOrAllies; // B
  u8 AdditionalTargetRestrictions; // C
  u8 Field0D; // D
  u8 Field0E; // E
  u8 Field0F; // F
  u8 Field10; // 10
  u8 Field11; // 11
  u8 Field12; // 12
  u8 Field13; // 13
  u8 Accuracy; // 14
  u8 MinHits; // 15
  u8 MaxHits; // 16
  u8 DamageHealingType; // 17
  u16 BaseDamage; // 18
  u8 SPEffect; // 1A
  u16 Field1B; // 1B
  u8 SPAmount; // 1D
  u8 SkillEffect; // 1E
  u8 EffectChance; // 1F
  u8 Field20; // 20
  u8 EffectList1; // 21
  u8 EffectList2; // 22
  u8 EffectList3; // 23
  u8 EffectList4; // 24
  u8 EffectList5; // 25
  u8 EffectList6; // 26
  u8 BuffDebuff; // 27
  u8 ExtraSkillEffect; // 28
  u8 CritChance; // 29
  u8 Field2A; // 2A
  u8 Field2B; // 2B
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
  ActiveSkillData entry[2000];
}SKILLTBLSegment1;

SKILLTBLSegment0 NewSKILLTBLSegment0;
SKILLTBLSegment1 NewSKILLTBLSegment1;

#endif