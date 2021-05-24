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
SHK_HOOK( int, criFs_Initialize );
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

  u64 args[10];
  double arg = 0.0;
  int argCount = 0;
  bool putNewLine = true;
  bool printAsFloat = false;

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

          // skip number of digits speifier
          while ( charIsDigit( next ) )
          {
              char next = *( ++curFormat );
              if ( next == 0 )
                  goto endloop;
          }

          switch ( next )
          {
          case 'c':
          case 's':
              args[ argCount++ ] = (u64)( FLW_GetStringArg( scriptArgIndex++ ) );
              break;

          case 'd':
          case 'i':
          case 'o':
          case 'x':
          case 'X':
          case 'u':
              args[ argCount++ ] = (u64)( FLW_GetIntArg( scriptArgIndex++ ) );
              break;

          case 'f':
          case 'F':
          case 'e':
          case 'E':
          case 'a':
          case 'A':
          case 'g':
          case 'G':
              printAsFloat = true;
              arg = (double)( FLW_GetFloatArg( scriptArgIndex++ ) );
              args[ argCount ] = *(u64*)( &arg );
              ++scriptArgIndex;
              ++argCount;
              break;
          }
      }
  }

  endloop:

  // Hack in some float prints for now
  if ( printAsFloat )
  {
    // Print everything as floats
    switch ( argCount )
    {
    case 0: printf( format ); break;
    case 1: printf( format, *(f64*)args[0]); break;
    case 2: printf( format, *(f64*)args[0], *(f64*)args[1]); break;
    case 3: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2]); break;
    case 4: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3]); break;
    case 5: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4]); break;
    case 6: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5]); break;
    case 7: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6]); break;
    case 8: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6], *(f64*)args[7]); break;
    case 9: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6], *(f64*)args[7], *(f64*)args[8]); break;
    case 10: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6], *(f64*)args[7], *(f64*)args[8], *(f64*)args[9]); break;
    }
  }
  else
  {
    // Print everything as ints
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

static int EX_FLD_MODEL_ANIM(void)
{
  u32 uVar1;
  int reshnd;
  ResourceHandleStruct *pMVar4;
  int animIndex;
  int isLoop;
  double unk;
  fieldworkdataStruct *pFVar5;
  fieldworkdataStruct *pFVar6;
  u64 uVar2;
  ResourceHandleStruct *pMVar7;
  undefined8 uVar3;
  ResourceHandleStruct *pMVar8;
  bool bVar9;
  bool bVar10;
  int *piVar11;
  double animSpeed;
  
  reshnd = FLW_GetIntArg(0);
  pMVar4 = FUN_00015c34(reshnd);
  if (pMVar4 != (ResourceHandleStruct *)0x0) {
    animIndex = FLW_GetIntArg(1);
    isLoop = FLW_GetIntArg(2);
    unk = FLW_GetFloatArg(3);
    animSpeed = FLW_GetFloatArg(4);
    __builtin_clz(isLoop);
    FUN_00014a38((u32)pMVar4, 1, (u32)((u64)animIndex & 0xffffffffU), unk / 30.0, 1.0);
    FUN_000148dc(animSpeed,(u64 *)pMVar4,0);
    pFVar5 = FUN_00352f40();
    pFVar6 = FUN_00352f40();
    if (((pFVar6 == (fieldworkdataStruct *)0x0) || (isLoop = FUN_003316e0((int)pFVar6), isLoop == 0)) &&
       (pFVar5 != (fieldworkdataStruct *)0x0)) {
      FUN_00324b70((u64 *)pFVar5->field1b8, (u64 *)pMVar4, (s64)animIndex & 0xffffffffU);
      uVar2 = FUN_0032c3d4((int)pFVar5);
      pMVar7 = (ResourceHandleStruct *)uVar2;
      if (pMVar7 == (ResourceHandleStruct *)0x0) {
        pMVar8 = (ResourceHandleStruct *)0x0;
      }
      else {
        pMVar8 = (ResourceHandleStruct *)0x0;
        if ((*(u64 *)&pMVar7->field10 & 0x80000) != 0) {
          pMVar8 = pMVar7;
        }
      }
      if (pMVar4 == pMVar8) {
        FUN_002ab314(pFVar5->fieldd4);
      }
      uVar1 = (u32)((u64)*(undefined8 *)pMVar4 >> 0x3a);
      animIndex = FUN_00320b1c((int)pFVar5);
      if ((animIndex != 0) && ((uVar1 == 2 || (uVar1 == 5)))) {
        uVar2 = FUN_002e1338((u64 *)pFVar5,(u64 *)pMVar4,'\0');
        piVar11 = (int *)uVar2;
        if (piVar11 != (int *)0x0) {
          animIndex = FUN_002d128c((int)piVar11,0x80000);
          if (animIndex == 0) {
            uVar2 = FUN_002d7b6c(piVar11);
            animIndex = (int)uVar2;
            if (animIndex < 5) {
              if (animIndex < 2) {
                return 1;
              }
            }
            else {
              if (animIndex < 7) {
                if (animIndex < 6) {
                  return 1;
                }
              }
              else {
                if (animIndex != 0x12) {
                  return 1;
                }
              }
            }
            FUN_002d7b18(piVar11);
          }
          else {
            FUN_002d7b18(piVar11);
            uVar3 = FUN_002d0870(piVar11);
            if ((int)uVar3 == 0) {
              uVar3 = FUN_002d08b4(piVar11);
              bVar9 = (int)uVar3 != 0;
              bVar10 = bVar9;
            }
            else {
              bVar9 = true;
              bVar10 = false;
            }
            FUN_0003b4b8((int)pMVar4,bVar9);
            FUN_0003b510((int)pMVar4,bVar10);
            FUN_002d9d00(piVar11);
          }
        }
      }
    }
  }
  return 1;
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
    if ( fsResult == 1 && CONFIG_ENABLED( enableExternalNavi ) )
    {
      FUN_00747f48((undefined4 *)pmVar1, FutabaNavi->pointerToFile, customNaviID);
    }
    else FUN_00747f48((undefined4 *)pmVar1, 0x00df7fd4, 8); //original functionality
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
    if ( fsResult == 1 && CONFIG_ENABLED( enableExternalNavi ) )
    {
      FUN_00747f48((undefined4 *)pmVar1, NaviTestFile->pointerToFile, 3);
    }
    else FUN_00747f48((undefined4 *)pmVar1, 0x00ba7568, 3); //original functionality
    DEBUG_LOG("Navi file successfully loaded\n");
    pmVar1->ptr1 = 0x00ba7568;
    pmVar2 = (int *)pmVar1;
  }
  return (undefined4 *)pmVar2;
}

ResourceHandleStruct* ModelResourceHandle;
static TtyCmdStatus ttyTestModelResHndCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  int ResHnd = intParse( args[0] );
  ModelResourceHandle = FUN_00015c34(ResHnd);
  hexDump("Model Resource Handle Struct", ModelResourceHandle, sizeof(ResourceHandleStruct));
  return TTY_CMD_STATUS_OK;
}

u64 LoadNaviSoundFileHook( u64 a1, u64 a2, char* acb_path, char* awb_path, u64 a5 )
{
  char new_acb_path[128];
  char new_awb_path[128];

  int naviID = GetCountFunctionHook(9);

  if ( strcmp( acb_path, "sound/battle/spt02.acb" ) == 0 && CONFIG_ENABLED( enableCustomNaviSoundPack ) )
  {
    sprintf( new_acb_path, "sound/battle/spt%02d.acb", naviID );
    sprintf( new_awb_path, "sound/battle/spt%02d.awb", naviID );
    return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, new_acb_path, new_awb_path, a5);
  }
  else if ( strcmp( acb_path, "sound_JP/battle/spt02.acb" ) == 0 && CONFIG_ENABLED( enableCustomNaviSoundPack ) )
  {
    sprintf( new_acb_path, "sound_JP/battle/spt%02d.acb", naviID );
    sprintf( new_awb_path, "sound_JP/battle/spt%02d.awb", naviID );
    return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, new_acb_path, new_awb_path, a5);
  }
  return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, acb_path, awb_path, a5);
}

u64 FUN_00748d78Hook(u64 param_1, u64 param_2, u64 param_3, u64 param_4, u64 param_5, u64 param_6, u64 param_7, u64 param_7_00, u64 param_9)
{
  if ( GetCountFunctionHook(9) == 9 && CONFIG_ENABLED( enableExternalNavi )  )
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

  TTY_CMD( ttyTestModelResHndCmd, "testmodel", "Test Resource handle function", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "int", "resource handle id", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

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
  scrRunScript(0, testBF->pointerToFile, testBF->bufferSize, 0);

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
  { EX_FLD_MODEL_ANIM, 5, "EX_FLD_MODEL_ANIM" },
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

int criFs_InitializeHook( void )
{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  char acStack288 [264];

  iVar1 = FUN_00ab563c(0x00d4bd54);
  iVar1 = FUN_001a52f8(iVar1);
  if (iVar1 != 2) 
  {
    pcVar2 = FUN_001a5834();
    sprintf(acStack288,"%s/hdd.cpk",pcVar2);
    criFsBinder_BindCpk(acStack288);
  }
  
  if ( CONFIG_ENABLED( enableModCPK ) )
  {
    pcVar2 = FUN_00968be8();
    pcVar3 = FUN_00968bf4();
    iVar1 = sprintf(acStack288,"%s%s/%s.cpk",pcVar2,pcVar3, CONFIG_STRING(modCPKName));
    iVar1 = criFsBinder_BindCpk(acStack288);
  }

  u32 extraCPK = CONFIG_INT( extraModCPK );
  if ( CONFIG_ENABLED( enableModCPK ) && extraCPK > 0 )
  {
    for (int i = 0; i < extraCPK; i++)
    {
      pcVar2 = FUN_00968be8();
      pcVar3 = FUN_00968bf4();
      iVar1 = sprintf(acStack288,"%s%s/%s_%02d.cpk", pcVar2, pcVar3, CONFIG_STRING_ARRAY(extraModCPKName)[i], i + 1);
      iVar1 = criFsBinder_BindCpk(acStack288);
    }
  }

  pcVar2 = FUN_00968be8();
  pcVar3 = FUN_00968bf4();
  iVar1 = sprintf(acStack288,"%s%s/ps3.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpk(acStack288);

  pcVar2 = FUN_00968be8();
  pcVar3 = FUN_00968bf4();
  iVar1 = sprintf(acStack288,"%s%s/data.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpk(acStack288);

  return iVar1;
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
  SHK_BIND_HOOK( criFs_Initialize, criFs_InitializeHook );
}

void EXFLWShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif