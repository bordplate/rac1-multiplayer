#ifdef GAME_P5
#ifndef EXFLW_H
#define EXFLW_H

typedef struct
{
    void* function;
    u32 argCount;
    const char* name;
} scrCommandTableEntry;

void EXFLWInit( void );
void EXFLWShutdown( void );

#endif
#endif