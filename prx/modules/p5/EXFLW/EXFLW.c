#ifdef GAME_P5
// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>
#include "lib/file.h"
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
#include "modules/p5/printf_FLW.h"
#include "EXFLW.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( int, EX_FLW_setHumanLv );
SHK_HOOK( int, PUT_Function );
SHK_HOOK( int, PUTF_Function );
SHK_HOOK( int, PUTS_Function );
SHK_HOOK( int, FLW_AI_ACT_ATTACK );
SHK_HOOK( int, FLW_AI_ACT_SKILL );
SHK_HOOK( s32, setSeq, s32 seqId, void* params, s32 paramsSize, s32 r6 );
SHK_HOOK( void, SetCountFunction, u32 a1, u32 a2 );
SHK_HOOK( int, GetCountFunction, u32 a1 );
SHK_HOOK( bool, BIT_CHK_FUNC, u64 a1 );
SHK_HOOK( int, BIT_SET_FUNC, u64 flag, u8 targetState );
SHK_HOOK( bool, BIT_CHK_FUNC_ALT, u64 a1 );
SHK_HOOK( bool, scrGetCommandExist, u32 a1 );
SHK_HOOK( u32, scrGetCommandArgCount, u32 a1 );
SHK_HOOK( char*, scrGetCommandName, u32 a1 );
SHK_HOOK( scrCommandTableEntry*, scrGetCommandFunc, u32 id );
SHK_HOOK( undefined4*, LoadFutabaNaviBMD, void );
SHK_HOOK( undefined4*, LoadMonaNaviBMD, void );
SHK_HOOK( u64, LoadNaviSoundFile, u64 a1, u64 a2, char* acb_path, char* awb_path, u64 a5 );
SHK_HOOK( u64, FUN_00748d78, u64 param_1, u64 param_2, u64 param_3, u64 param_4, u64 param_5, u64 param_6, u64 param_7, u64 param_7_00, u64 param_9);

static bool BIT_CHK_FUNCHook( u64 a1 )
{
  bool result = SHK_CALL_HOOK( BIT_CHK_FUNC, a1 );
  for ( u32 i = 0; i < CONFIG_ARRAY_COUNT( bitChkLogIgnore ); ++i )
  {
    if ( a1 == CONFIG_INT_ARRAY( bitChkLogIgnore )[i] )
      return result;
  }
  for ( u32 i = 0; i < CONFIG_ARRAY_COUNT( bitChkLogIgnore2 ); ++i )
  {
    if ( a1 == CONFIG_INT_ARRAY( bitChkLogIgnore2 )[i] )
      return result;
  }
  DEBUG_LOG("BIT_CHK called on flag 0x%04x, result -- 0x%x\n", a1, result);
  return result;
}

static int BIT_SET_FUNCHook( u64 flag, u8 targetState )
{
  int result = SHK_CALL_HOOK( BIT_SET_FUNC, flag, targetState );
  for ( u32 i = 0; i < CONFIG_ARRAY_COUNT( bitChkLogIgnore ); ++i )
  {
    if ( flag == CONFIG_INT_ARRAY( bitSetLogIgnore )[i] )
      return result;
  }
  if ( targetState == 0 )
  {
    DEBUG_LOG("BIT_OFF called for flag 0x%04x\n", flag);
  }
  else DEBUG_LOG("BIT_ON called for flag 0x%04x\n", flag);
  return result;
}

static bool BIT_CHK_FUNC_ALTHook( u64 a1 )
{
  bool result = SHK_CALL_HOOK( BIT_CHK_FUNC, a1 );
  if ( CONFIG_ENABLED( debug ) )
  {
    for ( u32 i = 0; i < CONFIG_ARRAY_COUNT( bitChkLogIgnore ); ++i )
    {
      if ( a1 == CONFIG_INT_ARRAY( bitChkLogIgnore )[i] )
        return result;
    }
    DEBUG_LOG("BIT_CHK_ALT called on flag 0x%04x, result -- 0x%x\n", a1, result);
  }
  return result;
}

static s32 setSeqHook( s32 seqId, void* params, s32 paramsSize, s32 r6 )
{
  sequenceIDGlobal = seqId;
  if ( seqId == 4 )
  {
    if ( paramsSize == 40 )
    {
      setSeqEnc* localStruct = (setSeqEnc*)params;
      EncounterIDGlobal = localStruct->encounterID;
      if ( EncounterIDGlobal > 1000 )
      {
        EncounterIDGlobal = 0;
      }
      DEBUG_LOG("Encounter ID Obtained -> %03d\n", EncounterIDGlobal);
    }
    else DEBUG_LOG("Unknown Params size in seqID 4 -> %d\n", paramsSize);
  }
  // Calling the original unhooked function is done like this.
  return SHK_CALL_HOOK( setSeq, seqId, params, paramsSize, r6 );
}

static int EX_FLW_setHumanLvHook( void )
{
  u32 unitID = FLW_GetIntArg(0);
  u32 lv = FLW_GetIntArg(1);
  SetUnitLv(unitID, lv);
  lv = GetJokerTargetLvExp(lv);
  SetUnitEXP(unitID, lv);
  return 1;
}

static int EX_FLW_GetEquippedPersona( void )
{
  u32 unitID = FLW_GetIntArg(0);
  if (unitID <= 10) // up to Kasumi
  {
    unitID = GetEquippedPersonaFunction(unitID);
    FLW_SetIntReturn(unitID);
  }
  return 1;
}

static int EX_FLW_ReturnEncounterID ( void )
{ 
  FLW_SetIntReturn(EncounterIDGlobal);
  return 1;
}

static int EX_FLW_GET_SEQ_ID ( void )
{ 
  FLW_SetIntReturn(sequenceIDGlobal);
  return 1;
}

static int PUT_FunctionHook( void )
{
  printf ("%d\n", FLW_GetIntArg(0));
  return 1;
}

static int PUTF_FunctionHook( void )
{
  printf ("%f\n", FLW_GetFloatArg(0));
  return 1;
}

static int PUTS_FunctionHook( void )
{
  printf ("%s\n", FLW_GetStringArg(0));
  return 1;
}

static int EX_FLW_PersonaEvolution( void )
{
  u32 unitID;
  u32 uVar1;
  u32 uVar2;

  unitID = FLW_GetIntArg(0);
  uVar1 = FUN_0025bdf8(unitID);
  uVar2 = FLW_GetIntArg(1);
  FUN_0025cb8c(uVar1,uVar2);
  return 1;
}

#define ScriptInterpreter (*((ScriptInterpreter**)0xD59BFC))
static int EX_FLW_PRINTF( void ) //TGE made this into printf lol
{
  char* format = FLW_GetStringArg( 0 );
  u32 scriptArgIndex = 1;

  void* args[10];
  double arg = 0.0;
  bool argIsFloat[10] = {};
  int argCount = 0;
  bool putNewLine = true;

  if ( *format == '\\' )
  {
      char next = *( format + 1 );
      if ( next != 0 && next == 'n' && *( format + 2 ) == 0 ) // single \n
          putNewLine = false;
  }

  char* curFormat = format;
  for (; *curFormat != 0; ++curFormat )
  {
      char c = *curFormat;

      if ( c == '%' )
      {
          char next = *( ++curFormat );
          if ( next == 0 )
              break;

          switch ( next )
          {
          case 'c':
          case 's':
              args[ argCount++ ] = (void*)( FLW_GetStringArg( scriptArgIndex++ ) );
              break;

          case 'd':
          case 'i':
          case 'o':
          case 'x':
          case 'X':
          case 'u':
              args[ argCount++ ] = (void*)( FLW_GetIntArg( scriptArgIndex++ ) );
              break;

          case 'f':
          case 'F':
          case 'e':
          case 'E':
          case 'a':
          case 'A':
          case 'g':
          case 'G':
              arg = (double)( FLW_GetFloatArg( scriptArgIndex++ ) );
              args[ argCount ] = (void*)( &arg );
              argIsFloat[ argCount ] = true;
              ++scriptArgIndex;
              ++argCount;
              break;
          }
      }
  }

  switch ( argCount )
  {
  case 0: printf( format ); break;
  case 1: printf( format, args[0]); break;
  case 2: printf( format, args[0], args[1]); break;
  case 3: printf( format, args[0], args[1], args[2]); break;
  case 4: printf( format, args[0], args[1], args[2], args[3]); break;
  case 5: printf( format, args[0], args[1], args[2], args[3], args[4]); break;
  case 6: printf( format, args[0], args[1], args[2], args[3], args[4], args[5]); break;
  case 7: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6]); break;
  case 8: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]); break;
  case 9: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]); break;
  case 10: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]); break;
  }

  if ( putNewLine )
      printf( "\n" );

  ScriptInterpreter->mNumStackValues -= argCount;

  return 1;
}

static void SetCountFunctionHook( u32 COUNT, u32 VALUE )
{
  DEBUG_LOG("SET_COUNT called, COUNT %03d -- VALUE %d\n", COUNT, VALUE);
  if ( COUNT >= 256 )
  {
    GlobalCounts[COUNT-256] = VALUE;
  }
  else SHK_CALL_HOOK( SetCountFunction, COUNT, VALUE );
}

static u32 GetCountFunctionHook( u32 COUNT )
{
  if ( COUNT >= 256 )
  {
    DEBUG_LOG("GET_COUNT called, COUNT %03d -- VALUE %d\n", COUNT, GlobalCounts[COUNT-256]);
    return GlobalCounts[COUNT-256];
  }
  else
  {
    u32 returnVal = SHK_CALL_HOOK( GetCountFunction, COUNT );
    for ( u32 i = 0; i < CONFIG_ARRAY_COUNT( getCountLogIgnore ); ++i )
    {
      if ( COUNT == CONFIG_INT_ARRAY( getCountLogIgnore )[i] )
        return returnVal;
    }
    DEBUG_LOG("GET_COUNT called, COUNT %03d -- VALUE %d\n", COUNT, returnVal);
    return returnVal;
  }
}

static int EX_FLW_AI_ACT_PERSONA_SKILL( void )
{
  btlUnit_Unit* EnemyUnit = FLW_GetBattleUnitStructFromContext();
  if ( CONFIG_ENABLED( enablePersonaEnemies ) )
  {
    SetCountFunctionHook( 300, FLW_GetIntArg(0) );
    DEBUG_LOG(" Enemy Persona set to %03d with skillID %03d\n", EnemyPersona, EnemyUnit->context.enemy.ActSkillID);
  }
  EnemyUnit->context.enemy.ActSkillID = FLW_GetIntArg(1);
  EnemyUnit->context.enemy.Act_Type = 1;
  return 1;
}

static int FLW_AI_ACT_SKILLHook( void )
{
  SetCountFunctionHook( 300, 0 );
  return SHK_CALL_HOOK(FLW_AI_ACT_SKILL);
}

static int FLW_AI_ACT_ATTACKHook( void )
{
  SetCountFunctionHook( 300, 0 );
  return SHK_CALL_HOOK(FLW_AI_ACT_ATTACK);
}

inline void scrPushInt( int val ) 
{
  ScriptInterpreter->mStackValues[ ScriptInterpreter->mNumStackValues ] = val;
  ScriptInterpreter->mStackValueTypes[ ScriptInterpreter->mNumStackValues++ ] = 0;
  ScriptInterpreter->mNumStackValues += 1;
}

inline void scrPushFloat( float val )
{
  ScriptInterpreter->mStackValues[ ScriptInterpreter->mNumStackValues ] = *(u32*)&val;
  ScriptInterpreter->mStackValueTypes[ ScriptInterpreter->mNumStackValues++ ] = 1;
  ScriptInterpreter->mNumStackValues += 1;
}

inline u32 scrGetCommandIntResult()
{
  return ScriptInterpreter->mREGValue;
}

inline f32 scrGetCommandFloatResult()
{
  return *(f32*)&ScriptInterpreter->mREGValue;
}

static TtyCmdStatus ttyAddCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  f32 a = floatParse( args[0] );
  f32 b = floatParse( args[1] );
  printf( "%f\n", a + b );
  return TTY_CMD_STATUS_OK;
}

typedef struct
{
  s16 fieldMajorId;
  s16 fieldMinorId;
  s16 envMajorId;
  s16 envMinorId;
  s16 field08;
  s16 field0a;
  s16 field0c;
  s16 field0e;
  s16 field10;
  s16 field12;
  s16 field14;
  s16 field16;
  s16 field18;
  s16 field1a;
  s16 field1c;
  s16 field1e;
} seqFieldParams;

static TtyCmdStatus ttyEchoCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  for ( u32 i = 0; i < argc; ++i )
  {
      printf( "%s ", args[i] );
  }

  printf( "\n" );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetBgmCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  s32 id = intParse( args[0] );

#if GAME_P5
  sndManPlaySfx( 0, 0, id, 0, -1, -1 );
#endif

  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetSeqCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  s32 id = intParse( args[0] );

#if GAME_P5
  if ( id == 3 )
  {
    seqFieldParams params = {};
    params.fieldMajorId = intParse( args[1] );
    params.fieldMinorId = intParse( args[2] );
    params.envMajorId = params.fieldMajorId;
    params.envMinorId = params.fieldMinorId;
    params.field08 = 0xb;
    params.field0c = -1;
    setSeq( id, &params, sizeof(seqFieldParams), 0 );
  }
  else
  {
    setSeq( id, NULL, 0, 0 );
  }
#endif

  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetCountCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 count = intParse( args[0] );
  if ( count > 512 )
  {
    *error = "Count should not be higher than 512";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  u32 value = intParse( args[1] );
  SetCountFunctionHook( count, value );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetCountCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 count = intParse( args[0] );
  if ( count > 512 )
  {
    *error = "Count should not be higher than 512";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  GetCountFunctionHook( count );
  return TTY_CMD_STATUS_OK;
}

#define BIT_MAX 8959
static TtyCmdStatus ttyBITONCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 bit = intParse( args[0] );
  if ( bit > BIT_MAX )
  {
    *error = "Bit ID should not exceed 8959";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  BIT_SET_FUNCHook( bit, 1 );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyBITOFFCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 bit = intParse( args[0] );
  if ( bit > BIT_MAX )
  {
    *error = "Bit ID should not exceed 8959";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  BIT_SET_FUNCHook( bit, 0 );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetBITCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 bit = intParse( args[0] );
  if ( bit > BIT_MAX )
  {
    *error = "Bit ID should not exceed 8959";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  BIT_CHK_FUNCHook( bit );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyHealHPCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  for (int i = 1; i <= 10; i++)
  {
    scrPushInt( i );
    BULLET_RECOVERY();
  }
  RECOVERY_ALL();
  return TTY_CMD_STATUS_OK;
}

void PartyIn( int unitID )
{
  int v0; // r30
  int i; // r30

  v0 = 0;
  while ( GetUnitIDFromPartySlot(v0) != unitID )
  {
    if ( ++v0 >= 4 )
    {
      for ( i = 0; i < 4; ++i )
      {
        if ( !GetUnitIDFromPartySlot(i) )
        {
          sub_24B274(i, unitID);
          return;
        }
      }
      sub_24B274(3, unitID);
      return;
    }
  }
  return;
}

static TtyCmdStatus ttyPartyInCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 unitID = intParse( args[0] );
  if ( unitID > 10 )
  {
    *error = "Party Member ID should not exceed 10";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  PartyIn( unitID );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyPartyOutCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 unitID = intParse( args[0] );
  if ( unitID > 10 )
  {
    *error = "Party Member ID should not exceed 10";
    return TTY_CMD_STATUS_INVALID_ARG;
  }

  u16 party2 = GetUnitIDFromPartySlot(1);
  u16 party3 = GetUnitIDFromPartySlot(2);
  u16 party4 = GetUnitIDFromPartySlot(3);

  for ( int partySlot = 1; partySlot <= 3; partySlot++ )
  {
    sub_24B274(partySlot, 0);
  }

  if ( party2 != unitID )
  {
    PartyIn(party2);
  }
  if ( party3 != unitID )
  {
    PartyIn(party3);
  }
  if ( party4 != unitID )
  {
    PartyIn(party4);
  }
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetEnemyBtlUnitCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  printf( "Enemy unit struct address 0x%08x\n", &enemyBtlUnit );
  hexDump( "Enemy unit struct", enemyBtlUnit, sizeof(btlUnit_Unit) );
  return TTY_CMD_STATUS_OK;
}

#define EnemyAffinityTBL (*((EnemyAffinityTBL**)0xD79FA8))

static TtyCmdStatus ttyGetAffinityCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 enemyID = intParse( args[0] );
  if ( enemyID > 350 )
  {
    *error = "Enemy ID should not exceed 350";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  return TTY_CMD_STATUS_OK;
}

fileHandleStruct* FutabaNavi = 0;
undefined4* LoadFutabaNaviBMDHook(void)
{
  DEBUG_LOG("LoadFutabaNaviBMDHook called\n");
  idkman* pmVar1;
  int *pmVar2;
  
  pmVar1 = MallocAndReturn(0x34);
  pmVar2 = (int *)0x0;
  DEBUG_LOG("Checking if Navi File is loaded\n");
  
  int customNaviID = GetCountFunctionHook( 9 );
  if ( customNaviID <= 1 || customNaviID == 3 || customNaviID > 10 )
  {
    customNaviID = 8;
    SetCountFunctionHook( 9, customNaviID );
  }
  char naviPath[128];

  if ( FutabaNavi != (void*)0x0  )
  {
    semaphore_WaitPost((int)FutabaNavi);
    FutabaNavi = (void*)0x0;
  }

  sprintf(naviPath, "battle/message/navi_%02d.bmd", customNaviID);
  if (pmVar1 != (idkman*)0x0) {
    FutabaNavi = open_file( naviPath, 0 );
    u64 fsResult = fsSync((int)FutabaNavi);
    if ( fsResult == 1 )
    {
      FUN_00747f48((undefined4 *)pmVar1, FutabaNavi->pointerToFile, customNaviID);
    }
    DEBUG_LOG("Navi file successfully loaded\n");
    pmVar1->ptr1 = 0x00ba76c0;
    pmVar2 = (int *)pmVar1;
  }
  return (undefined4 *)pmVar2;
}

undefined4* LoadMonaNaviBMDHook(void)
{
  DEBUG_LOG("LoadMonaNaviBMDHook called\n");
  idkman* pmVar1;
  int *pmVar2;
  
  pmVar1 = MallocAndReturn(0x34);
  pmVar2 = (int *)0x0;
  DEBUG_LOG("Checking if Navi File is loaded\n");
  if (pmVar1 != (idkman*)0x0) {
    NaviTestFile = open_file( "battle/message/navi_03.bmd", 0 );
    u64 fsResult = fsSync((int)NaviTestFile);
    if ( fsResult == 1 )
    {
      FUN_00747f48((undefined4 *)pmVar1, NaviTestFile->pointerToFile, 3);
    }
    DEBUG_LOG("Navi file successfully loaded\n");
    pmVar1->ptr1 = 0x00ba7568;
    pmVar2 = (int *)pmVar1;
  }
  return (undefined4 *)pmVar2;
}

static TtyCmdStatus ttyTestFileOpenCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  NaviTestFile = open_file( "battle/message/navi_08.bmd", 0 );
  u64 fsResult = fsSync((int)NaviTestFile);
  printf("fsSync() result %d\n", fsResult);
  if ( fsResult == 1 )
  {
    printf("Attempting to open Navi BMD file at 0x%08x\nfileStatus %d\nfileName %s\nunk1 0x%08x\nunk2 0x%08x\nunk3 0x%08x\npointerToFile 0x%08x\n", 
    NaviTestFile, 
    NaviTestFile->fileStatus,
    NaviTestFile->filename,
    NaviTestFile->unk1,
    NaviTestFile->unk2,
    NaviTestFile->unk3,
    NaviTestFile->pointerToFile);
  }
  return TTY_CMD_STATUS_OK;
}

u64 LoadNaviSoundFileHook( u64 a1, u64 a2, char* acb_path, char* awb_path, u64 a5 )
{
  printf("LoadNaviSoundFile called\n");

  char new_acb_path[128];
  char new_awb_path[128];

  int naviID = GetCountFunctionHook(9);

  if ( strcmp( acb_path, "sound/battle/spt02.acb" ) == 0 )
  {
    sprintf( new_acb_path, "sound/battle/spt%02d.acb", naviID );
    printf( "acp_path %s\n", new_acb_path );
    sprintf( new_awb_path, "sound/battle/spt%02d.awb", naviID );
    printf( "awb_path %s\n", new_awb_path );
    return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, new_acb_path, new_awb_path, a5);
  }
  else if ( strcmp( acb_path, "sound_JP/battle/spt02.acb" ) == 0 )
  {
    sprintf( new_acb_path, "sound_JP/battle/spt%02d.acb", naviID );
    printf( "acp_path %s\n", new_acb_path );
    sprintf( new_awb_path, "sound_JP/battle/spt%02d.awb", naviID );
    printf( "awb_path %s\n", new_awb_path );
    return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, new_acb_path, new_awb_path, a5);
  }
  return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, acb_path, awb_path, a5);
}

u64 FUN_00748d78Hook(u64 param_1, u64 param_2, u64 param_3, u64 param_4, u64 param_5, u64 param_6, u64 param_7, u64 param_7_00, u64 param_9)
{
  printf("FUN_00748d78Hook called\na1 -> %d\na2 -> %d\na3 -> %d\na4 -> %d\na5 -> %d\na6 -> %d\na7 -> %d\na8 -> %d\na9 -> %d\n", param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_7_00, param_9);
  if ( GetCountFunctionHook(9) == 9 )
  {
    param_3 += 100;
  }
  return SHK_CALL_HOOK(FUN_00748d78, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_7_00, param_9);
}
// List of commands that can be handled by the command listener
static TtyCmd ttyCommands[] =
{
  TTY_CMD( ttyAddCmd, "add", "Adds 2 numbers and prints the result", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "a", "The first number to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_FLOAT ), 
    TTY_CMD_PARAM( "b", "The second number to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_FLOAT )),

  TTY_CMD( ttyEchoCmd, "echo", "Prints the given input back to you", TTY_CMD_FLAG_VARARGS ),

  TTY_CMD( ttySetBgmCmd, "setbgm", "Sets the current BGM using the Cue ID", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "cueId", "The Cue ID of the BGM to play", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttySetCountCmd, "setCount", "Sets the specificed COUNT to the specified value", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "count", "COUNT number to modify", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT ), 
    TTY_CMD_PARAM( "value", "Target value for COUNT", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyGetCountCmd, "getCount", "Returns the value of the specified COUNT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "count", "COUNT to get value from", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyBITONCmd, "biton", "Enables the specified BIT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bit", "BIT to enable", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyBITOFFCmd, "bitoff", "Disables the specified BIT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bit", "BIT to disable", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyGetBITCmd, "bitget", "Returns the current state of the specified BIT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bit", "BIT to get status from", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyPartyInCmd, "partyin", "Adds a party member to current active party", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "playerID", "ID of party member to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyPartyOutCmd, "partyout", "Removes a party member from current active party", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "playerID", "ID of party member to remove", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyGetAffinityCmd, "getaffinity", "Prints affinity of input enemy", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "int", "enemy id", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyTestFileOpenCmd, "navi", "Prints the given input back to you", TTY_CMD_FLAG_VARARGS ),

  TTY_CMD( ttyGetEnemyBtlUnitCmd, "getenemy", "Prints address and contents of currently saved enemy battle struct", TTY_CMD_FLAG_NONE ),

  TTY_CMD_END(), 
};

#ifdef GAME_P5

SHK_HOOK( u64, mainUpdate, f32 deltaTime );
fileHandleStruct* testBF = 0;
static u64 mainUpdateHook( f32 deltaTime )
{
  // Process TTY commands
  ttyCmdProcess( ttyCommands );

  if (testBF == (void*)0x0)
  {
    testBF = open_file( "script/test.bf", 0 );
    u64 fsSyncResult = fsSync((int)testBF);
  }
  scrRunScript(0, testBF->pointerToFile, testBF->unk1, 0);

  return SHK_CALL_HOOK( mainUpdate, deltaTime );
}

#endif

scrCommandTableEntry exCommandTable[] =
{
  { EX_FLW_PRINTF, 1, "EX_PRINTF" },
  { EX_FLW_AI_ACT_PERSONA_SKILL, 2, "AI_ACT_PERSONA_SKILL" },
  { EX_FLW_ReturnEncounterID, 0, "GET_ENCOUNTER_ID" },
  { EX_FLW_GetEquippedPersona, 1, "GET_EQUIPPED_PERSONA" },
  { EX_FLW_PersonaEvolution, 2, "PERSONA_EVOLUTION2" },
  { EX_FLW_GET_SEQ_ID, 0, "GET_SEQ_ID" },
};

static scrCommandTableEntry* scrGetCommandFuncHook( u32 id )
{
  DEBUG_LOG("scrGetCommandFunc called on function ID 0x%04x\n", id);
  if ( id >= 0x6000 )
  {
    DEBUG_LOG("function ID 0x%x called\nName %s\nnumOfArgs %02d\n",
     id, exCommandTable[id - 0x6000].name, 
     exCommandTable[id - 0x6000].argCount);
    return exCommandTable[id & 0x0FFF].function;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandFunc, id);
  }
}

static bool scrGetCommandExistHook( u32 functionID )
{
  DEBUG_LOG("scrGetCommandExist called on function ID 0x%04x\n", functionID);
  if ( functionID >= 0x6000 )
  {
    return true;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandExist, functionID);
  }
}

static char* scrGetCommandNameHook( u32 functionID )
{
  DEBUG_LOG("scrGetCommandName called on function ID 0x%04x\n", functionID);
  if ( functionID >= 0x6000 )
  {
    return exCommandTable[functionID & 0x0FFF].name;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandName, functionID);
  }
}

static u32 scrGetCommandArgCountHook( u32 functionID )
{
  DEBUG_LOG("scrGetCommandArgCount called on function ID 0x%04x\n", functionID);
  if ( functionID >= 0x6000 )
  {
    return exCommandTable[functionID & 0x0FFF].argCount;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandArgCount, functionID);
  }
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void EXFLWInit( void )
{
  printf("Expanded Flowscript Module loaded.\n");
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( EX_FLW_setHumanLv, EX_FLW_setHumanLvHook );
  SHK_BIND_HOOK( PUT_Function, PUT_FunctionHook );
  SHK_BIND_HOOK( PUTF_Function, PUTF_FunctionHook );
  SHK_BIND_HOOK( PUTS_Function, PUTS_FunctionHook );
  SHK_BIND_HOOK( setSeq, setSeqHook );
  SHK_BIND_HOOK( SetCountFunction, SetCountFunctionHook );
  SHK_BIND_HOOK( GetCountFunction, GetCountFunctionHook );
  SHK_BIND_HOOK( FLW_AI_ACT_SKILL, FLW_AI_ACT_SKILLHook );
  SHK_BIND_HOOK( FLW_AI_ACT_ATTACK, FLW_AI_ACT_ATTACKHook );
  SHK_BIND_HOOK( BIT_SET_FUNC, BIT_SET_FUNCHook );
  SHK_BIND_HOOK( BIT_CHK_FUNC, BIT_CHK_FUNCHook );
  SHK_BIND_HOOK( BIT_CHK_FUNC_ALT, BIT_CHK_FUNC_ALTHook );
  SHK_BIND_HOOK( scrGetCommandFunc, scrGetCommandFuncHook );
  SHK_BIND_HOOK( scrGetCommandExist, scrGetCommandExistHook );
  SHK_BIND_HOOK( scrGetCommandName, scrGetCommandNameHook );
  SHK_BIND_HOOK( scrGetCommandArgCount, scrGetCommandArgCountHook );
  // Handle command handling in main update function
  SHK_BIND_HOOK( mainUpdate, mainUpdateHook );
  // Load Custom Navigator file
  SHK_BIND_HOOK( LoadFutabaNaviBMD, LoadFutabaNaviBMDHook );
  SHK_BIND_HOOK( LoadMonaNaviBMD, LoadMonaNaviBMDHook );
  SHK_BIND_HOOK( LoadNaviSoundFile, LoadNaviSoundFileHook );
  SHK_BIND_HOOK( FUN_00748d78, FUN_00748d78Hook );
}

void EXFLWShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif