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
#include "kasumi.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( bool, FUN_00425b0c, u16 unitID );
SHK_HOOK( int, FUN_00425de0, partyMemberMenu* partyMenu );
SHK_HOOK( btlUnit_Unit*, FUN_00af22e4, structB* a1 );
SHK_HOOK( void, FUN_00b20618, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1c1a0, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1c398, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1c610, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b20ec8, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1ca18, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b21980, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b21bf8, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b21e28, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b22138, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b22f60, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );
SHK_HOOK( void, FUN_00b1bf38, struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 );

static bool isPartyMemberUnlocked( u16 unitID )
{
  if ( unitID == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    return GetSavedataBitflagAlt( 0x2138 );
  }
  else return SHK_CALL_HOOK( FUN_00425b0c, unitID );
}

static btlUnit_Unit* returnBtlUnitPointer( structB* a1 )
{
  lastAccessedBtlUnit = a1 ->btlUnitPointer;
  return a1 ->btlUnitPointer;
}

static void PartyMemberFinishedLastEnemy_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  undefined8 uVar1;
  u8 bVar4;
  u64 uVar2;
  int iVar3;
  int messageIndex;
  
  uVar1 = FUN_0074805c((u64)(int)param_1,(int)param_2);
  if (((((int)uVar1 == 0) && ((param_2->field118 >> 0x16 & 1) != 0)) && (param_4 != 0)) &&
     ((param_3 == (struct_2_pointers *)0x0 || (bVar4 = FUN_002588b4(param_3->pointer_2), bVar4 == 0)
      ))) {
    uVar2 = FUN_007489a8((int)param_1,(int)param_2);
    if ((int)uVar2 == 0) {
      uVar2 = param_3->pointer_2->unitID;
      if ((uVar2) < 5) {
        if ((uVar2) < 2) {
          if ((int)uVar2 == 0) {
            return;
          }
          iVar3 = GetBitflagState(0x21c3);
          messageIndex = 299;
          if (iVar3 != 0) {
            messageIndex = 0x123;
          }
        }
        else {
          if ((uVar2) < 3) {
            iVar3 = GetBitflagState(0x21c4);
            messageIndex = 299;
            if (iVar3 != 0) {
              messageIndex = 0x124;
            }
          }
          else {
            if ((uVar2) < 4) {
              iVar3 = GetBitflagState(0x21c5);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x125;
              }
            }
            else {
              iVar3 = GetBitflagState(0x21c6);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x126;
              }
            }
          }
        }
      }
      else {
        if ((uVar2) < 8) {
          if ((uVar2) < 6) {
            iVar3 = GetBitflagState(0x21c7);
            messageIndex = 299;
            if (iVar3 != 0) {
              messageIndex = 0x127;
            }
          }
          else {
            if ((uVar2) < 7) {
              iVar3 = GetBitflagState(0x21c8);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x128;
              }
            }
            else {
              iVar3 = GetBitflagState(0x21c9);
              messageIndex = 299;
              if (iVar3 != 0) {
                messageIndex = 0x129;
              }
            }
          }
        }
        else {
          if ( uVar2 == 10 )
          {
            messageIndex = 894;
          }
          else if ( uVar2 == 9 )
          {
            iVar3 = GetBitflagState(0x21cb);
            messageIndex = 299;
            if (iVar3 != 0) 
            {
              messageIndex = 0x12a;
            }
          }
          else
          {
            return;
          }
        }
      }
      if (((messageIndex != -1) &&
          (uVar2 = FUN_0091da04(), (uVar2 + ((uVar2 & 0xffffffff) / 100) * -100 & 0xffffffff) < 0x32
          )) && (uVar2 = FUN_748ff0((int)param_1), (int)uVar2 == -1)) {
        CallNaviDialogue(param_1,messageIndex,1,0,-1,param_6,'\0',1,0.0);
      }
    }
    else {
      if (param_4 < 3) {
        if (param_4 == 0) {
          return;
        }
        if (param_4 < 2) {
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x14f;
              if (iVar3 != 0) {
                messageIndex = 0x147;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x14f;
                if (iVar3 != 0) {
                  messageIndex = 0x148;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x149;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x14a;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x14f;
                if (iVar3 != 0) {
                  messageIndex = 0x14b;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x14c;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x14f;
                  if (iVar3 != 0) {
                    messageIndex = 0x14d;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 && CONFIG_ENABLED( enableKasumi ) )
              {
                messageIndex = 898;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x14f;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x14e;
                }
              }
              else
              {
                return;
              }
            }
          }
        }
        else {
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x146;
              if (iVar3 != 0) {
                messageIndex = 0x13e;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x146;
                if (iVar3 != 0) {
                  messageIndex = 0x13f;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x140;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x141;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x146;
                if (iVar3 != 0) {
                  messageIndex = 0x142;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x143;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x146;
                  if (iVar3 != 0) {
                    messageIndex = 0x144;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 )
              {
                messageIndex = 897;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x146;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x145;
                }
              }
              else
              {
                return;
              }
            }
          }
        }
      }
      else {
        if (param_4 < 4) {
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x13d;
              if (iVar3 != 0) {
                messageIndex = 0x135;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x13d;
                if (iVar3 != 0) {
                  messageIndex = 0x136;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x137;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x138;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x13d;
                if (iVar3 != 0) {
                  messageIndex = 0x139;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x13a;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x13d;
                  if (iVar3 != 0) {
                    messageIndex = 0x13b;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 )
              {
                messageIndex = 896;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x13d;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x13c;
                }
              }
            }
          }
        }
        else {
          if (4 < param_4) {
            return;
          }
          uVar2 = param_3->pointer_2->unitID;
          if ((uVar2) < 5) {
            if ((uVar2) < 2) {
              if ((int)uVar2 == 0) {
                return;
              }
              iVar3 = GetBitflagState(0x21c3);
              messageIndex = 0x134;
              if (iVar3 != 0) {
                messageIndex = 300;
              }
            }
            else {
              if ((uVar2) < 3) {
                iVar3 = GetBitflagState(0x21c4);
                messageIndex = 0x134;
                if (iVar3 != 0) {
                  messageIndex = 0x12d;
                }
              }
              else {
                if ((uVar2) < 4) {
                  iVar3 = GetBitflagState(0x21c5);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x12e;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c6);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x12f;
                  }
                }
              }
            }
          }
          else {
            if ((uVar2) < 8) {
              if ((uVar2) < 6) {
                iVar3 = GetBitflagState(0x21c7);
                messageIndex = 0x134;
                if (iVar3 != 0) {
                  messageIndex = 0x130;
                }
              }
              else {
                if ((uVar2) < 7) {
                  iVar3 = GetBitflagState(0x21c8);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x131;
                  }
                }
                else {
                  iVar3 = GetBitflagState(0x21c9);
                  messageIndex = 0x134;
                  if (iVar3 != 0) {
                    messageIndex = 0x132;
                  }
                }
              }
            }
            else {
              if ( uVar2 == 10 )
              {
                messageIndex = 895;
              }
              else if ( uVar2 == 9 )
              {
                iVar3 = GetBitflagState(0x21cb);
                messageIndex = 0x134;
                if (iVar3 != 0) 
                {
                  messageIndex = 0x133;
                }
              }
              else
              {
                return;
              }
            }
          }
        }
      }
      if ((messageIndex != -1) && (uVar2 = FUN_748ff0((int)param_1), (int)uVar2 == -1)) {
        CallNaviDialogue(param_1,messageIndex,1,0,-1,param_6,'\0',1,0.0);
      }
    }
  }
  return;
}

static void PartyMemberWarnHealthLow_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  u64 uVar1;
  int iVar2;
  int messageIndex;
  
  if ((param_2->field118 >> 0x16 & 1) != 0) {
    if (param_4 == 1) {
      uVar1 = param_3->pointer_2->unitID;
      if ((uVar1 & 0xffffffff) < 5) {
        if ((uVar1 & 0xffffffff) < 2) {
          if ((int)uVar1 == 0) {
            return;
          }
          iVar2 = GetBitflagState(0x21c3);
          messageIndex = 0x45;
          if (iVar2 != 0) {
            messageIndex = 0x3d;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 3) {
            iVar2 = GetBitflagState(0x21c4);
            messageIndex = 0x45;
            if (iVar2 != 0) {
              messageIndex = 0x3e;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 4) {
              iVar2 = GetBitflagState(0x21c5);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x3f;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c6);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x40;
              }
            }
          }
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 8) {
          if ((uVar1 & 0xffffffff) < 6) {
            iVar2 = GetBitflagState(0x21c7);
            messageIndex = 0x45;
            if (iVar2 != 0) {
              messageIndex = 0x41;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 7) {
              iVar2 = GetBitflagState(0x21c8);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x42;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c9);
              messageIndex = 0x45;
              if (iVar2 != 0) {
                messageIndex = 0x43;
              }
            }
          }
        }
        else {
          if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
          {
            messageIndex = 874;
          }
          else if ( uVar1 == 9 )
          {
            iVar2 = GetBitflagState(0x21cb);
            messageIndex = 0x45;
            if (iVar2 != 0) 
            {
              messageIndex = 0x44;
            }
          }
          else
          {
            return;
          }
        }
      }
      if (messageIndex == -1) {
        return;
      }
    }
    else {
      messageIndex = 0x3c;
    }
    CallNaviDialogue(param_1, messageIndex, 3, 0, -1, param_6, '\0', 1, 0.0);
  }
  return;
}

static void PartyMemberGotDowned_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  u64 uVar1;
  int iVar2;
  int messageIndex;
  int iVar4;
  
  if ((param_2->field118 >> 0x16 & 1) != 0) {
    iVar4 = 3;
    if (param_4 == 1) {
      uVar1 = param_3->pointer_2->unitID;
      if ((uVar1 & 0xffffffff) < 5) {
        if ((uVar1 & 0xffffffff) < 2) {
          if ((int)uVar1 == 0) {
            return;
          }
          iVar2 = GetBitflagState(0x21c3);
          messageIndex = 0x4f;
          if (iVar2 != 0) {
            messageIndex = 0x47;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 3) {
            iVar2 = GetBitflagState(0x21c4);
            messageIndex = 0x4f;
            if (iVar2 != 0) {
              messageIndex = 0x48;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 4) {
              iVar2 = GetBitflagState(0x21c5);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x49;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c6);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x4a;
              }
            }
          }
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 8) {
          if ((uVar1 & 0xffffffff) < 6) {
            iVar2 = GetBitflagState(0x21c7);
            messageIndex = 0x4f;
            if (iVar2 != 0) {
              messageIndex = 0x4b;
            }
          }
          else {
            if ((uVar1 & 0xffffffff) < 7) {
              iVar2 = GetBitflagState(0x21c8);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x4c;
              }
            }
            else {
              iVar2 = GetBitflagState(0x21c9);
              messageIndex = 0x4f;
              if (iVar2 != 0) {
                messageIndex = 0x4d;
              }
            }
          }
        }
        else {
          if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
          {
            messageIndex = 875;
          }
          else if ( uVar1 == 9 )
          {
            iVar2 = GetBitflagState(0x21cb);
            messageIndex = 0x4f;
            if (iVar2 != 0) 
            {
              messageIndex = 0x4e;
            }
          }
          else
          {
            return;
          }
        }
      }
      if (messageIndex == 0x47) {
        iVar4 = 5;
      }
      if (messageIndex == -1) {
        return;
      }
    }
    else {
      messageIndex = 0x46;
    }
    CallNaviDialogue( param_1, messageIndex, iVar4, 0, -1, param_6, '\0', 1, 0.0);
  }
  return;
}

static void PartyMemberDodgedAttack_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  u64 uVar1;
  int iVar2;
  int messageIndex;
  
  if ((param_2->field118 >> 0x16 & 1) != 0) 
  {
    uVar1 = param_3->pointer_2->unitID;
    if ((uVar1 & 0xffffffff) < 5) 
    {
      if ((uVar1 & 0xffffffff) < 2) 
      {
        if ((int)uVar1 == 0) 
        {
          return;
        }
        iVar2 = GetBitflagState(0x21c3);
        messageIndex = 0x59;
        if (iVar2 != 0) 
        {
          messageIndex = 0x51;
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 3) 
        {
          iVar2 = GetBitflagState(0x21c4);
          messageIndex = 0x59;
          if (iVar2 != 0) 
          {
            messageIndex = 0x52;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 4) 
          {
            iVar2 = GetBitflagState(0x21c5);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x53;
            }
          }
          else 
          {
            iVar2 = GetBitflagState(0x21c6);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x54;
            }
          }
        }
      }
    }
    else 
    {
      if ((uVar1 & 0xffffffff) < 8) 
      {
        if ((uVar1 & 0xffffffff) < 6) 
        {
          iVar2 = GetBitflagState(0x21c7);
          messageIndex = 0x59;
          if (iVar2 != 0) 
          {
            messageIndex = 0x55;
          }
        }
        else {
          if ((uVar1 & 0xffffffff) < 7) 
          {
            iVar2 = GetBitflagState(0x21c8);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x56;
            }
          }
          else 
          {
            iVar2 = GetBitflagState(0x21c9);
            messageIndex = 0x59;
            if (iVar2 != 0) 
            {
              messageIndex = 0x57;
            }
          }
        }
      }
      else 
      {
        if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
        {
          messageIndex = 876;
        }
        else if ( uVar1 == 9 )
        {
          iVar2 = GetBitflagState(0x21cb);
          messageIndex = 0x59;
          if (iVar2 != 0) 
          {
            messageIndex = 0x58;
          }
        }
        else
        {
          return;
        }
      }
    }
    if ((messageIndex != -1) && (uVar1 = FUN_748ff0((int)param_1), (int)uVar1 == -1)) {
      CallNaviDialogue(param_1,messageIndex,1,0,-1,param_6,'\0',1,0.0);
    }
  }
  return;
}

static void PartyMemberKnockEnemyDown_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  //printf("Party Member Knocked and Enemy down! Character ID -> %d\nFUN_007489a8 result -> %d, a4 is -> %d\n", param_3->pointer_2->unitID, FUN_007489a8(param_1,(int)param_2), param_4);
  SHK_CALL_HOOK( FUN_00b20ec8, param_1, param_2, param_3, param_4, param_5, param_6 );
  int knockdown = param_4;
  int numberOfExistingEnemies = FUN_007489a8(param_1,(int)param_2);

  int messageIndex = -1;
  if ( param_3->pointer_2->unitID == 10 && knockdown != 0 && CONFIG_ENABLED( enableKasumi ) )
  {
    if ( knockdown ==  numberOfExistingEnemies )
    {
      messageIndex = 899;
    }
    else if ( knockdown == 4 )
    {
      messageIndex = 900;
    }
    else if ( knockdown == 3 )
    {
      messageIndex = 901;
    }
    else if ( knockdown == 2 )
    {
      messageIndex = 902;
    }
    else if ( knockdown == 1 )
    {
      messageIndex = 903;
    }
  }
  if (messageIndex != -1) 
  {
    numberOfExistingEnemies = FUN_748ff0((int)param_1);
    if ((int)numberOfExistingEnemies == -1) 
    {
      CallNaviDialogue( param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0 );
    }
  }
  return;
}

static void PartyMemberWarnAilment_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  //printf("Party Member Ailment being checked. Character ID -> %d\n", lastAccessedBtlUnit->unitID);
  btlUnit_Unit* Kasumi = GetBtlPlayerUnitFromID( 10 );
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;

  if ( !CONFIG_ENABLED( enableKasumi ) )
  {
    SHK_CALL_HOOK( FUN_00b1ca18, param_1, param_2, param_3, param_4, param_5, param_6 );
  }

  if ( Kasumi->StatusAilments > 0 )
  {
    //printf("Kasumi Ailment is 0x%x\n", Kasumi->StatusAilments);
    for ( int i = 0; i <= 11; i++)
    {
      if ( Kasumi->StatusAilments & ( 1 << i ) && hasUzukiAilmentAnnounce[i] == false )
      {
        messageIndex = 881 + i;
        hasUzukiAilmentAnnounce[i] = true;
      }
    }
  }
  else if ( Kasumi->currentHP < GetBtlUnitMaxHP( Kasumi ) / 3 && !UzukiLowHPWarn )
  {
    messageIndex = 880;
    UzukiLowHPWarn = true;
  }
  else if ( Kasumi->context.player.Buffs.BuffStatus_ATK && 
          Kasumi->context.player.BuffsDirDur.BuffDir_ATK == 14 && 
          Kasumi->context.player.BuffsDirDur.BuffDur_ATK >= 1 && 
          !UzukiDebuffAttackWarn )
  {
    messageIndex = 877;
    UzukiDebuffAttackWarn = true;
  }
  else if ( Kasumi->context.player.Buffs.BuffStatus_DEF && 
          Kasumi->context.player.BuffsDirDur.BuffDir_DEF == 14 && 
          Kasumi->context.player.BuffsDirDur.BuffDur_DEF >= 1 && 
          !UzukiDebuffDeffenseWarn )
  {
    messageIndex = 878;
    UzukiDebuffDeffenseWarn = true;
  }
  else if ( Kasumi->context.player.Buffs.BuffStatus_EVA && 
          Kasumi->context.player.BuffsDirDur.BuffDir_EVA == 14 && 
          Kasumi->context.player.BuffsDirDur.BuffDur_EVA >= 1 && 
          !UzukiDebuffSpeedWarn )
  {
    messageIndex = 879;
    UzukiDebuffSpeedWarn = true;
  }
  if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)) &&
  ((uVar9 = FUN_007490a4(param_1, messageIndex), (int)uVar9 == 0 &&
  (param_1->field2c != messageIndex)))) 
  {
    CallNaviDialogue(param_1, messageIndex, 5, 0, -1, param_6, '\0', 1, 0.0);
    param_1->field2c = messageIndex;
  }
  else SHK_CALL_HOOK( FUN_00b1ca18, param_1, param_2, param_3, param_4, param_5, param_6 );
  /*printf("Kasumi->context.player.BuffsDirDur.BuffDir_ATK -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDir_ATK);
  printf("Kasumi->context.player.BuffsDirDur.BuffDir_ACC -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDir_ACC);
  printf("Kasumi->context.player.BuffsDirDur.BuffDir_DEF -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDir_DEF);
  printf("Kasumi->context.player.BuffsDirDur.BuffDir_EVA -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDir_EVA);
  printf("Kasumi->context.player.BuffsDirDur.BuffDur_ATK -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDur_ATK);
  printf("Kasumi->context.player.BuffsDirDur.BuffDur_ACC -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDur_ACC);
  printf("Kasumi->context.player.BuffsDirDur.BuffDur_DEF -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDur_DEF);
  printf("Kasumi->context.player.BuffsDirDur.BuffDur_EVA -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDur_EVA);
  printf("Kasumi->context.player.BuffsDirDur.BuffDir_ -> %d\n", Kasumi->context.player.BuffsDirDur.BuffDir_);*/
  return;
}

static void PartyMemberMissedEnemy_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  u8 bVar4;
  u64 uVar1;
  int iVar3;
  undefined8 uVar2;
  int messageIndex;
  int iVar5;
  
  if ((param_2->field118 >> 0x16 & 1) == 0) {
    return;
  }
  if ((param_3 != 0) && (bVar4 = FUN_002588b4(param_3->pointer_2), bVar4 == 0)) {
    return;
  }
  if ((0x304 < param_2->field128 && param_2->field128 < 0x307)) {
    return;
  }
  iVar5 = 3;
  uVar1 = param_3->pointer_2->unitID;
  if ((uVar1 & 0xffffffff) < 5) {
    if ((uVar1 & 0xffffffff) < 2) {
      if ((int)uVar1 == 0) {
        return;
      }
      iVar3 = GetBitflagState(0x21c3);
      messageIndex = 0x197;
      if (iVar3 != 0) {
        messageIndex = 399;
      }
    }
    else {
      if ((uVar1 & 0xffffffff) < 3) {
        iVar3 = GetBitflagState(0x21c4);
        messageIndex = 0x197;
        if (iVar3 != 0) {
          messageIndex = 400;
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 4) {
          iVar3 = GetBitflagState(0x21c5);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x191;
          }
        }
        else {
          iVar3 = GetBitflagState(0x21c6);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x192;
          }
        }
      }
    }
  }
  else {
    if ((uVar1 & 0xffffffff) < 8) {
      if ((uVar1 & 0xffffffff) < 6) {
        iVar3 = GetBitflagState(0x21c7);
        messageIndex = 0x197;
        if (iVar3 != 0) {
          messageIndex = 0x193;
        }
      }
      else {
        if ((uVar1 & 0xffffffff) < 7) {
          iVar3 = GetBitflagState(0x21c8);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x194;
          }
        }
        else {
          iVar3 = GetBitflagState(0x21c9);
          messageIndex = 0x197;
          if (iVar3 != 0) {
            messageIndex = 0x195;
          }
        }
      }
    }
    else {
      if ( uVar1 == 10 && CONFIG_ENABLED( enableKasumi ) )
      {
        messageIndex = 906;
      }
      else if ( uVar1 == 9 )
      {
        iVar3 = GetBitflagState(0x21cb);
        messageIndex = 0x197;
        if (iVar3 != 0) 
        {
          messageIndex = 0x196;
        }
      }
      else
      {
        return;
      }
    }
  }
  
  iVar5 = 4;
  
  if (((messageIndex != -1) &&
      (uVar2 = FUN_007490a4((int)param_1,messageIndex), (int)uVar2 == 0)) &&
     (uVar1 = FUN_748ff0((int)param_1), (int)uVar1 == -1)) 
  {
    CallNaviDialogue(param_1,messageIndex,iVar5,0,-1,param_6,'\0',1,0.0);
  }
  return;
}

static void PartyMemberLandedCrit_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  SHK_CALL_HOOK( FUN_00b21e28, param_1, param_2, param_3, param_4, param_5, param_6 );
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 905;

    if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)))
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static void PartyMemberGetUp_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  SHK_CALL_HOOK( FUN_00b22138, param_1, param_2, param_3, param_4, param_5, param_6 );
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 893;

    if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)))
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static void PartyMemberExploitedWeakness_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  SHK_CALL_HOOK( FUN_00b21bf8, param_1, param_2, param_3, param_4, param_5, param_6 );
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;
  int uVar10;
  int uVar9;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 904;

    if (((messageIndex != -1) && (uVar10 = FUN_748ff0(param_1), (int)uVar10 == -1)) &&
    (param_1->field2c != messageIndex))
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
      param_1->field2c = messageIndex;
    }
  }
  return;
}

static void PartyMemberRunAway_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  SHK_CALL_HOOK( FUN_00b22f60, param_1, param_2, param_3, param_4, param_5, param_6 );
  bool bVar4;
  if ((param_2->field118 >> 0x16 & 1) == 0) 
  {
    return;
  }
  if ((param_3 != 0) && (bVar4 = FUN_002588b4(param_3->pointer_2), bVar4 == 0)) {
    return;
  }
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;

  if ( Kasumi == 10 && CONFIG_ENABLED( enableKasumi ) )
  {
    messageIndex = 907;

    if ( messageIndex != -1 )
    {
      CallNaviDialogue(param_1, messageIndex, 1, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static void PartyMemberDied_NaviDialogue( struct_2_pointers* param_1, navi_dialogue_function_a2* param_2, struct_2_pointers* param_3, u16 param_4, int param_5, int param_6 )
{
  SHK_CALL_HOOK( FUN_00b1bf38, param_1, param_2, param_3, param_4, param_5, param_6 );
  bool bVar4;
  if ((param_2->field118 >> 0x16 & 1) == 0) 
  {
    return;
  }
  int Kasumi = param_3->pointer_2->unitID;
  
  int messageIndex = -1;

  if ( Kasumi == 10 )
  {
    messageIndex = 873;

    if ( messageIndex != -1 )
    {
      CallNaviDialogue(param_1, messageIndex, 4, 0, -1, param_6, '\0', 1, 0.0);
    }
  }
  return;
}

static int BuildPartyMemberStatsMenu ( partyMemberMenu* partyMenu )
{
  int count = 1;
  
  partyMenu->partyMemberID[0] = 1;

  
  if ( isPartyMemberUnlocked(8) )
  {
    partyMenu->partyMemberID[count] = 8;
    //printf("Party Member %d added to the menu\n", 8);
    count += 1;
  }
  for ( int i = 2; i <= 7; i++)
  {
    if ( isPartyMemberUnlocked(i) )
    {
      partyMenu->partyMemberID[count] = i;
      //printf("Party Member %d added to the menu\n", i);
      count += 1;
    }
  }
  if ( isPartyMemberUnlocked(9) )
  {
    partyMenu->partyMemberID[count] = 9;
    //printf("Party Member %d added to the menu\n", 9);
    count += 1;
  }
  /*if ( isPartyMemberUnlocked(10) && CONFIG_ENABLED( enableKasumi ) )
  {
    partyMenu->partyMemberID[count] = 10;
    //printf("Party Member %d added to the menu\n", 10);
    count += 1;
  }*/

  return count;
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void KasumiInit( void )
{
  randomSetSeed( getTicks() );
  //printf("Uzuki Fixes Module loaded.\n");
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( FUN_00425b0c, isPartyMemberUnlocked);
  SHK_BIND_HOOK( FUN_00425de0, BuildPartyMemberStatsMenu);
  SHK_BIND_HOOK( FUN_00af22e4, returnBtlUnitPointer);
  SHK_BIND_HOOK( FUN_00b20618, PartyMemberFinishedLastEnemy_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1c1a0, PartyMemberWarnHealthLow_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1c398, PartyMemberGotDowned_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1c610, PartyMemberDodgedAttack_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b20ec8, PartyMemberKnockEnemyDown_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1ca18, PartyMemberWarnAilment_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b21980, PartyMemberMissedEnemy_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b21bf8, PartyMemberExploitedWeakness_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b21e28, PartyMemberLandedCrit_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b22138, PartyMemberGetUp_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b22f60, PartyMemberRunAway_NaviDialogue );
  SHK_BIND_HOOK( FUN_00b1bf38, PartyMemberDied_NaviDialogue );
}

void KasumiShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif