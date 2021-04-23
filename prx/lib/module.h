#ifndef MODULELIST_H
#define MODULELIST_H

#include "common.h"

typedef void(*ModuleInitFunc)();
typedef void(*ModuleShutdownFunc)();

typedef enum
{
    MODULE_FLAG_INIT = 1 << 0,
    MODULE_FLAG_SHUTDOWN = 1 << 1,
    MODULE_FLAG_ENABLED = 1 << 2,
} ModuleFlags;

typedef enum 
{
    MODULE_INIT_STATUS_ENABLED,
    MODULE_INIT_STATUS_DISABLED,
    MODULE_INIT_STATUS_DEPENDENCY_INVALID,
    MODULE_INIT_STATUS_DEPENDENCY_DISABLED
} ModuleInitStatus;

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

    // Modules this module depends on
    const char* dependencies[32];

    // Runtime flags
    ModuleFlags flags;
} ModuleInfo;

// Returns the number of declared modules (not enabled).
u32 moduleGetModuleCount();

// Returns the module with the given index.
ModuleInfo* moduleGetModuleByIndex( u32 index );

// Returns the module with the given (short) name.
ModuleInfo* moduleGetModuleByName( const char* name );

// Initializes the specified module
ModuleInitStatus moduleInitModule( ModuleInfo* module );

// Shut down/deinitialize the specified module
void moduleShutdownModule( ModuleInfo* module );

#endif