#ifndef MODULELIST_H
#define MODULELIST_H

#include "testmodule.h"

typedef void(*ModuleInitFunc)();
typedef void(*ModuleShutdownFunc)();

typedef struct
{
    // Short name used to refer to the module.
    const char* shortName;

    // Long name of the module.
    const char* longName;

    // Name of the setting used to enable or disable this module.
    const char* toggleSettingName;

    // Function used to initialize the module
    ModuleInitFunc init;

    // Function used to shut down the module.
    ModuleShutdownFunc shutdown;
} ModuleInfo;

ModuleInfo Modules[] =
{
    { "test", "test module", "enableTestModule", testModuleInit, testModuleShutdown },
};

#endif