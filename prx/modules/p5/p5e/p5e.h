#ifdef GAME_P5
#ifndef P5E_H
#define P5E_H

// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

void p5eInit( void );
void p5eShutdown( void );

typedef struct
{
  u32 modelId[5];
} CharModelReplacementTable;

typedef struct
{
  u32 ptr1;
  char* fileNamePtr;
} fileAccessStruct;


CharModelReplacementTable charModelReplacementTable[10];

#endif
#endif