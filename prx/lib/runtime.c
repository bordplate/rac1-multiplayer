#include "common.h"
#include "shk.h"
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

    // bind hook to function that was substituted (copied to the prx) to make room for the loader
    SHK_BIND_HOOK( runtimeSubstitute1, runtimeSubstitute1Impl );
}