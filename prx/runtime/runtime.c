#include "../lib/common.h"
#include "../lib/shk.h"
#include "runtime.h"

extern void* _shk_elf_prx_ptr_table;
extern void* _shk_prx_ptr_table;

extern void runtimeSubstitute1Impl();

SHK_HOOK( void, runtimeSubstitute1 );

void runtimeInit()
{
    // set 
    printf( "_shk_elf_prx_ptr_table = 0x%08X\n", _shk_elf_prx_ptr_table );
    printf( "&_shk_elf_prx_ptr_table = 0x%08X\n", &_shk_elf_prx_ptr_table );
    printf( "_shk_prx_ptr_table = 0x%08X\n", _shk_prx_ptr_table );
    printf( "&_shk_prx_ptr_table = 0x%08X\n", &_shk_prx_ptr_table );

    *(void**)&_shk_elf_prx_ptr_table = &_shk_prx_ptr_table;
    SHK_BIND_HOOK( runtimeSubstitute1, runtimeSubstitute1Impl );
}