#include "../lib/common.h"
#include "../lib/shk.h"
#include "runtime.h"

extern void* _shk_elf_prx_ptr_table;
extern void* _shk_prx_ptr_table;

extern void runtimeSubstitute1Impl();

SHK_HOOK( void, runtimeSubstitute1 );

void runtimeInit()
{
    printf( "mod sprx runtime init\n" );

    // initialize pointer to prx function pointer table in elf
    *(void**)&_shk_elf_prx_ptr_table = &_shk_prx_ptr_table;
    SHK_BIND_HOOK( runtimeSubstitute1, runtimeSubstitute1Impl );
}