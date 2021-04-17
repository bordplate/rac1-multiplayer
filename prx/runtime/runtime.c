#include "../lib/common.h"
#include "../lib/shk.h"
#include "runtime.h"

extern void runtimeSubstitute1Impl();

SHK_HOOK( void, runtimeSubstitute1 );

void runtimeInit()
{
    SHK_BIND_HOOK( runtimeSubstitute1, runtimeSubstitute1Impl );
}