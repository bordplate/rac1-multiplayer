#ifdef GAME_P5
#ifndef SCARLTZ_H
#define SCARLTZ_H

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



#endif
#endif