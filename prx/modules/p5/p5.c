#ifdef GAME_P5

// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"
#include "p5.h"

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

int RECOVERY_ALL( void )
{
    SHK_FUNCTION_CALL_0( 0x1edf9c, int );
}

int BULLET_RECOVERY( void )
{
    SHK_FUNCTION_CALL_0( 0x1f113c, int );
}

btlUnit_Unit* FLW_GetBattleUnitStructFromContext( void )
{
    SHK_FUNCTION_CALL_0( 0x1f2974, btlUnit_Unit* );
}

btlUnit_Unit* GetBtlPlayerUnitFromID( u32 id )
{
    SHK_FUNCTION_CALL_1( 0x24B200, btlUnit_Unit*, u32, id );
}

int GetUnitIDFromPartySlot( u32 id )
{
    SHK_FUNCTION_CALL_1( 0x24B21C, int, u32, id );
}

void SetPlayerBulletsToMax( btlUnit_Unit* id )
{
    SHK_FUNCTION_CALL_1( 0x25b888, void, btlUnit_Unit*, id );
}

u32 FUN_0025867c( u16 personaID )
{
    SHK_FUNCTION_CALL_1( 0x25867c, int, u16, personaID );
}

int FLW_GetIntArg( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f266c, int, u32, arg );
}

f64 FLW_GetFloatArg( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f2768, f64, u32, arg );
}

char* FLW_GetStringArg( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f2868, char*, u32, arg );
}

u64 GetBtlUnitPersona( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x25bdf8, u64, u32, arg );
}

u64 FUN_00263714( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x263714, u64, u32, arg );
}

int FLW_SetIntReturn( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x1f28d8, int, u32, arg );
}

u32 GetJokerTargetLvExp( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x2634cc, int, u32, arg );
}

u32 GetEquippedPersonaFunction( PersonaNames unitID )
{
    SHK_FUNCTION_CALL_1( 0x25be3c, u32, PersonaNames, unitID );
}

u32 ActualGetCount( u32 arg )
{
    SHK_FUNCTION_CALL_1( 0x24bf5c, u32, u32, arg );
}

u32 FUN_0025bdf8( u16 unitID )
{
    SHK_FUNCTION_CALL_1( 0x25bdf8, u32, u16, unitID );
}

void* MallocAndReturn( u32 size )
{
    SHK_FUNCTION_CALL_1( 0x625570, void*, u32, size );
}

void* malloc( u32 size )
{
    SHK_FUNCTION_CALL_1( 0x91edd8, void*, u32, size );
}

u64 GetSavedataBitflagAlt( u64 flag )
{
    SHK_FUNCTION_CALL_1( 0x116a94, u64, u64, flag );
}

void* SomethingAboutLoadingVoices( void* someAddressPointer )
{
    SHK_FUNCTION_CALL_1( 0x74a9d0, void*, void*, someAddressPointer );
}

void SetUnitLv( u32 unitID, u32 targetLv )
{
    SHK_FUNCTION_CALL_2( 0x26a678, void, u32, unitID, u32, targetLv );
}

int FUN_002584cc( u32 a1, u32 a2 )
{
    SHK_FUNCTION_CALL_2( 0x2584cc, int, u32, a1, u32, a2 );
}

int sub_24B274( u32 a1, u16 a2 )
{
    SHK_FUNCTION_CALL_2( 0x24B274, int, u32, a1, u32, a2 );
}

void FUN_0025cb8c( u32 unitID, u16 personaID )
{
    SHK_FUNCTION_CALL_2( 0x25cb8c, void, u32, unitID, u16, personaID );
}

void SetUnitEXP( u32 ID, u16 Lv )
{
    SHK_FUNCTION_CALL_2( 0x26a6f4, void, u32, ID, u16, Lv );
}

u32 unitGetEquipment( btlUnit_Unit* unitID, EquipSlot equipSlotID )
{
    SHK_FUNCTION_CALL_2( 0x258bbc, u32, btlUnit_Unit*, unitID, EquipSlot, equipSlotID );
}

u32 CalculateAddressWithPointer( void* address, structA* unk )
{
    SHK_FUNCTION_CALL_2( 0xb10eb8, u32, void*, address, structA*, unk );
}

u32 PlayerUnitGetModelMinorID( u32 playerID, u8 a2, u8 a3 )
{
    SHK_FUNCTION_CALL_3( 0x45d24, u32, u32, playerID, u8, a2, u8, a3 );
}

void LoadSoundByCueIDCombatVoice( CueIDThingy* a1, u32 a2, u32 cueID,  u8 idk )
{
    SHK_FUNCTION_CALL_4( 0x74aac0, void, CueIDThingy*, a1, u32, a2, u32, cueID, u8, idk );
}

s32 setSeq( s32 seqId, void* params, s32 paramsSize, s32 r6 ) 
{
  SHK_FUNCTION_CALL_4( 0x10DB4, s32, s32, seqId, void*, params, s32, paramsSize, s32, r6 );
}

void FUN_0074ae50( void* a1, char* acb_string, char* awb_string, u32 unitID )
{
  SHK_FUNCTION_CALL_4( 0x74ae50, void, void*, a1, char*, acb_string, char*, awb_string, u32, unitID );
}

s32 sndManPlaySfx( s32 a0, s32 a1, s32 cueId, s32 a3, s32 a4, s32 a5 ) 
{ 
    SHK_FUNCTION_CALL_6( 0x10F0D4, s32, s32, a0, s32, a1, s32, cueId, s32, a3, s32, a4, s32, a5 ); 
}

int FUN_0090053c( u32 a1, u64 a2, u64 a3 )
{
  SHK_FUNCTION_CALL_3( 0x90053c, int, u32, a1, u64, a2, u64, a3 );
}

fileHandleStruct* open_file( char* file_path, u32 a2 )
{
    SHK_FUNCTION_CALL_2( 0x1144ac, fileHandleStruct*, char*, file_path, u32, a2 );
}

u64 FUN_00118280( char *param_1, char *param_2, char *param_3, u8 param_4 )
{
    SHK_FUNCTION_CALL_4( 0x118280, u64, char*, param_1, char*, param_2, char*, param_3, u8, param_4 );
}

void FUN_00747f48( int* a1, int a2, int a3 )
{
    SHK_FUNCTION_CALL_3( 0x747f48, void, int*, a1, int, a2, int, a3 );
}

u64 fsSync( int a1 )
{
    SHK_FUNCTION_CALL_1(0x114720, u64, int, a1);
}

u64 scrRunScript( u32 a1, u32 a2, u32 a3, u32 a4 )
{
    SHK_FUNCTION_CALL_4( 0x1f1e2c, u64, u32, a1, u32, a2, u32, a3, u32, a4 );
}

int semaphore_WaitPost( int a1 )
{
    SHK_FUNCTION_CALL_1( 0x1141fc, int, int, a1 );
}

FieldGetIDStruct* FUN_00352f40( void )
{
    SHK_FUNCTION_CALL_0( 0x00352f40, FieldGetIDStruct* );
}

ResourceHandleStruct* FUN_00015c34( int a1 )
{
    SHK_FUNCTION_CALL_1( 0x15c34, ResourceHandleStruct*, int, a1 );
}

int FUN_00014a38(u32 param_1, u32 param_2, u32 param_3, float param_4, float param_5)
{
    SHK_FUNCTION_CALL_5( 0x14a38, int, u32, param_1, u32, param_2,  u32, param_3, float, param_4, float, param_5 ); 
}

void FUN_000148dc(double param_1, u64* param_2, int param_3)
{
    SHK_FUNCTION_CALL_3( 0x148dc, void, double, param_1, u64*, param_2, int, param_3 );
}

void FUN_00324b70(u64* param_1, u64* param_2, u64 param_3)
{
    SHK_FUNCTION_CALL_3( 0x324b70, void, u64*, param_1, u64*, param_2, u64, param_3 );
}

int FUN_003b9110( int a1, int a2, int a3, int a4, int a5 )
{
    SHK_FUNCTION_CALL_5( 0x3b9110, int, int, a1, int, a2, int, a3, int, a4, int, a5 );
}

u64 FUN_0032c3d4( int arg )
{
    SHK_FUNCTION_CALL_1( 0x32c3d4, u64, int, arg );
}

u64 FUN_002ab314( int arg )
{
    SHK_FUNCTION_CALL_1( 0x2ab314, u64, int, arg );
}

int FUN_00320b1c( int arg )
{
    SHK_FUNCTION_CALL_1( 0x320b1c, int, int, arg );
}

u64 FUN_002e1338(u64* param_1, u64* param_2, char param_3)
{
    SHK_FUNCTION_CALL_3( 0x2e1338, u64, u64*, param_1, u64*, param_2, char, param_3 );
}

bool FUN_002d128c(int param_1, u64 param_2)
{
    SHK_FUNCTION_CALL_2( 0x2d128c, bool, int, param_1, u64, param_2 );
}

u64 FUN_002d7b6c( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d7b6c, u64, int*, arg );
}

int FUN_003316e0( int arg )
{
    SHK_FUNCTION_CALL_1( 0x3316e0, int, int, arg );
}

int FUN_002d7b18( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d7b18, int, int*, arg );
}

undefined8 FUN_002d0870( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d0870, undefined8, int*, arg );
}

undefined8 FUN_002d08b4( int* arg )
{
    SHK_FUNCTION_CALL_1( 0x2d08b4, undefined8, int*, arg );
}

void FUN_0003b4b8(int param_1, char param_2)
{
    SHK_FUNCTION_CALL_2( 0x3b4b8, void, int, param_1, char, param_2 );
}

void FUN_0003b510(int param_1, char param_2)
{
    SHK_FUNCTION_CALL_2( 0x3b510, void, int, param_1, char, param_2 );
}

void FUN_002d9d00(undefined4* param_1)
{
    SHK_FUNCTION_CALL_1( 0x2d9d00, void, undefined4*, param_1 );
}

char* FUN_00968be8( void )
{
    SHK_FUNCTION_CALL_0( 0x968be8, char* );
}

char* FUN_00968bf4( void )
{
    SHK_FUNCTION_CALL_0( 0x968bf4, char* );
}

char* FUN_001a5834( void )
{
    SHK_FUNCTION_CALL_0( 0x1a5834, char* );
}

/*int criFsBinder_BindCpk( char* arg )
{
    SHK_FUNCTION_CALL_1( 0x114b74, int, char*, arg );
}*/

int criFsBinder_SetPriority ( int cpkBinder, int priority )
{
   SHK_FUNCTION_CALL_2( 0xAB8ED4, int, int, cpkBinder, int, priority );
}

int FUN_00ab563c( int* arg )
{
    SHK_FUNCTION_CALL_1( 0xab563c, int, int*, arg );
}

int FUN_001a52f8( int arg )
{
    SHK_FUNCTION_CALL_1( 0x1a52f8, int, int, arg );
}

int FUN_2604C4( int arg )
{
    SHK_FUNCTION_CALL_1( 0x2604C4, int, int, arg );
}

int FUN_748ff0( int arg )
{
    SHK_FUNCTION_CALL_1( 0x748ff0, int, int, arg );
}

int FUN_0024b28c( int arg )
{
    SHK_FUNCTION_CALL_1( 0x24b28c, int, int, arg );
}

int FUN_0091da04( void )
{
    SHK_FUNCTION_CALL_0( 0x91da04, int );
}

int GetCurrentBGMCueID( void )
{
    SHK_FUNCTION_CALL_0( 0x6ccb8, int );
}

int FUN_007489a8( int a1, int a2 )
{
    SHK_FUNCTION_CALL_2( 0x7489a8, int, int, a1, int, a2 );
}

bool FUN_002588b4( btlUnit_Unit* a1 )
{
    SHK_FUNCTION_CALL_1( 0x2588b4, bool, btlUnit_Unit*, a1 );
}

int FUN_0074805c( int a1, int a2 )
{
    SHK_FUNCTION_CALL_2( 0x74805c, int, int, a1, int, a2 );
}

int isCharacterAssistExpressonValid( short a1, short a2 )
{
    SHK_FUNCTION_CALL_2( 0x3b9644, int, short, a1, short, a2 );
}

bool FUN_007490a4( struct_2_pointers* a1, int a2 )
{
    SHK_FUNCTION_CALL_2( 0x7490a4, bool, struct_2_pointers*, a1, int, a2 );
}

void LoadEncounterEventSoundbank( int encounterID )
{
    SHK_FUNCTION_CALL_1( 0x745b9c, void, int, encounterID );
}

encounterIDTBL* GetEncounterEntryFromTBL( int encounterID)
{
    SHK_FUNCTION_CALL_1(0x263b94, encounterIDTBL*, int, encounterID);
}

bool GetBitflagState ( int bitFlagID )
{
    SHK_FUNCTION_CALL_1(0x263b94, bool, int, bitFlagID);
}

int GetBtlUnitMaxHP(btlUnit_Unit* param_1)
{
    SHK_FUNCTION_CALL_1(0x258d1c, int, btlUnit_Unit*, param_1);
}

void CallNaviDialogue (struct_2_pointers* param_1, int param_2, int param_3, int param_4, int param_5, int param_6, char param_7, short param_8, double param_9)
{
    SHK_FUNCTION_CALL_9(0x748d78, void, struct_2_pointers*, param_1, int, param_2, int, param_3, int, param_4, int, param_5, int, param_6, char, param_7, short, param_8, double, param_9);
}

PartyMemberLvUpThresholdExp* GetPartyMemberLvUpThreshold( int unitID )
{
    SHK_FUNCTION_CALL_1(0x263634, PartyMemberLvUpThresholdExp*, int, unitID);
}

PartyMemberPersonaBlock* GetPartyMemberPersonaBlock( int personaID )
{
    SHK_FUNCTION_CALL_1(0x263618, PartyMemberPersonaBlock*, int, personaID);
}

fieldworkdataStruct* GetFieldWorkData( void )
{
    SHK_FUNCTION_CALL_0( 0x352f40, fieldworkdataStruct* );
}

u16 GetTotalDays( void )
{
    SHK_FUNCTION_CALL_0( 0x48e80, u16 );
}

bool FUN_0031f9cc( void )
{
    SHK_FUNCTION_CALL_0( 0x31f9cc, bool );
}

itemTBLAccessoryEntry* GetAccessoryTBLEntry( u16 accessoryID )
{
    SHK_FUNCTION_CALL_1(0x263830, itemTBLAccessoryEntry*, u16, accessoryID);
}

ItemTBL_MeleeWeapon* GetMeleeWeaponTBLEntry( u16 WeaponID )
{
    SHK_FUNCTION_CALL_1(0x2637d0 , ItemTBL_MeleeWeapon*, u16, WeaponID);
}

ItemTBL_RangedWeapon* GetRangedWeaponTBLEntry( u16 WeaponID )
{
    SHK_FUNCTION_CALL_1(0x26393c , ItemTBL_RangedWeapon*, u16, WeaponID);
}

bool CheckHasSkill ( btlUnit_Unit* btlUnit, int SkillID ){
    SHK_FUNCTION_CALL_2( 0x259148, bool, btlUnit_Unit*, btlUnit, int, SkillID );
}

u32 FUN_000442cc ( int param_1, int param_2 ){
    SHK_FUNCTION_CALL_2( 0x442cc, u32, int, param_1, int, param_2 );
}

void FUN_0024c018 ( u64 param_1, char param_2 ){
    SHK_FUNCTION_CALL_2( 0x24c018, void, u64, param_1, char, param_2 );
}

double FUN_00250a10( btlUnit_Unit* btlUnit, u32 param_2, undefined8 param_3 ){
    SHK_FUNCTION_CALL_3( 0x250a10, double, btlUnit_Unit*, btlUnit, u32, param_2, undefined8, param_3 );
}

bool AilmentCheck(btlUnit_Unit* btlUnit, u32 param_2){
    SHK_FUNCTION_CALL_2( 0x2590d8, bool, btlUnit_Unit*, btlUnit, u32, param_2 );
}

int GetCountEquipHPBonus(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x258d1c, int, btlUnit_Unit*, param_1 );
}

int GetCountEquipSPBonus(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x258e48, int, btlUnit_Unit*, param_1 );
}

bool CheckAccessoryEffect(btlUnit_Unit* btlUnit, int EffectID){
    SHK_FUNCTION_CALL_2( 0x2594b4, bool, btlUnit_Unit*, btlUnit, int, EffectID );
}

int FUN_26AE14(u32 param_1){
    SHK_FUNCTION_CALL_1( 0x26ae14, int, u32, param_1 );
}

undefined4 FUN_25E5D8(int param_1, btlUnit_Unit* btlUnit_1, btlUnit_Unit* btlUnit_2, undefined2 param_4, undefined4 param_5){
    SHK_FUNCTION_CALL_5( 0x25e5d8, undefined4, int, param_1, btlUnit_Unit*, btlUnit_1, btlUnit_Unit*, btlUnit_2, undefined2, param_4, undefined4, param_5 );
}

short FUN_25893C(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25893C, short, btlUnit_Unit*, param_1 );
}

u64 GetSkillData(btlUnit_Unit* btlUnit, u32 SkillId){
    SHK_FUNCTION_CALL_2( 0x25007c, u64, btlUnit_Unit*, btlUnit, u32, SkillId );
}

u8 FUN_2588B4(btlUnit_Unit *param_1){
    SHK_FUNCTION_CALL_1( 0x2588B4, u8, btlUnit_Unit*, param_1 );
}

double FUN_250A10(btlUnit_Unit* param_1, u32 param_2){
    SHK_FUNCTION_CALL_2( 0x250A10, double, btlUnit_Unit*, param_1, u32, param_2 );
}

bool GetEquipmentEffect(u64 AccessoryID, int param_2){
    SHK_FUNCTION_CALL_2( 0x260d18, bool, u64, AccessoryID, int, param_2 );
}

double CalculateResistEffect(int AccId){
    SHK_FUNCTION_CALL_1( 0x260dc4, double, int, AccId );
}

u64 FUN_2595C4(btlUnit_Unit* btlUnit,u32 param_2){
    SHK_FUNCTION_CALL_2( 0x2595C4, u64, btlUnit_Unit*, btlUnit, u32, param_2 );
}

void FUN_25A328(btlUnit_Unit* param_1,u64 buff){
    SHK_FUNCTION_CALL_2( 0x25A328, void, btlUnit_Unit*, param_1, u64, buff );
}

bool FUN_25963C(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25963C, bool, btlUnit_Unit*, param_1 );
}

bool FUN_0025b6ac(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25b6ac, bool, btlUnit_Unit*, param_1 );
}

int GetNumberOfBullets(btlUnit_Unit* param_1){
    SHK_FUNCTION_CALL_1( 0x25b880, int, btlUnit_Unit*, param_1 );
}

bool IsConfidantBonusObtained(int param_1){
    SHK_FUNCTION_CALL_1( 0x54af94, bool, int, param_1 );
}

u64 FUN_252980(u16 SkillId, btlUnit_Unit* btlUnit_1,btlUnit_Unit *btlUnit_2,u8 mode){
    SHK_FUNCTION_CALL_4( 0x252980, u64, u16, SkillId, btlUnit_Unit*, btlUnit_1, btlUnit_Unit*, btlUnit_2, u8, mode );
}

u64 FUN_25294C(u16 SkillId,btlUnit_Unit *btlUnit_1,btlUnit_Unit *btlUnit_2,u8 mode){
    SHK_FUNCTION_CALL_4( 0x25294C, u64, u16, SkillId, btlUnit_Unit*, btlUnit_1, btlUnit_Unit*, btlUnit_2, u8, mode );
}

u32 GetCurrentHP(btlUnit_Unit *param_1){
    SHK_FUNCTION_CALL_1( 0x2589c0, u32, btlUnit_Unit*, param_1 );
}

u32 GetCurrentSP(btlUnit_Unit *param_1){
    SHK_FUNCTION_CALL_1( 0x2589cc, u32, btlUnit_Unit*, param_1 );
}

int GetRandom( int MaxValue )
{
  int v0; // r31
  u32 v1; // r3

  v0 = MaxValue;
  v1 = FUN_0091da04();
  return (v1 - (v1 / (v0 + 1) * v0 + v1 / (v0 + 1)));
}

bool isMidWinterValid( void )
{
    isMidwinter = false;

    if ( !CONFIG_ENABLED( enableMidWinterOutfits ) )
    {
        return isMidwinter;
    }
    
    if ( GetTotalDays() >= 214 )
    {
        isMidwinter = true;
        if ( sequenceIDGlobal == 6 )
        {
            if ( lastUsedFieldMajorID == 2 )
            {
                if ( lastUsedFieldMinorID == 1 || lastUsedFieldMinorID == 4 || lastUsedFieldMinorID == 5 || lastUsedFieldMinorID == 13 )
                {
                    isMidwinter = true;
                }
                else isMidwinter = false;
            }
        }
    }
    else isMidwinter = false;
    
    return isMidwinter;
}

static s32 sys_time_get_current_time( u64* secs, u64* nsecs )
{
  system_call_2( 145, (u64)secs, (u64)nsecs );
  return_to_user_prog( s32 );
} 

u64 getTicks(void)
{
    u32 ticks_micro;
    u64 secs;
    u64 nsecs;

    sys_time_get_current_time(&secs, &nsecs);
    ticks_micro =  secs * 1000000UL + (nsecs / 1000);

    return ticks_micro;                                                                         
}

#endif