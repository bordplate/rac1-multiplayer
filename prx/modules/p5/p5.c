#ifdef GAME_P5

// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"
#include "p5.h"

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

#endif