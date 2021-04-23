#include "modules/test/testmodule.h"

static ModuleInfo sModules[] =
{
    //  short name  long name           toggle setting name     init function       shutdown function       module dependencies
#ifdef GAME_TEST
    {   "test",     "test module",      "enableTestModule",     testModuleInit,     testModuleShutdown,     { "test2" } },
    {   "test2",    "test module 2",    "enableTestModule",     NULL,               NULL,                   { "test" } },  
#endif
#ifdef GAME_P5
#endif
};