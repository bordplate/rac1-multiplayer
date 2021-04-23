#ifndef MODULELIST_H
#define MODULELIST_H

#include "lib/common.h"
#include "testmodule.h"

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

    ModuleFlags flags;
} ModuleInfo;

ModuleInfo Modules[] =
{
    //  short name  long name           toggle setting name     init function       shutdown function       module dependencies
    {   "test",     "test module",      "enableTestModule",     testModuleInit,     testModuleShutdown,     { "test2" } },
    {   "test2",    "test module 2",    "enableTestModule",     NULL,               NULL,                   { "test" } },
};

inline u32 moduleGetModuleCount()
{
    return sizeof( Modules ) / sizeof( ModuleInfo );
}

inline ModuleInfo* moduleGetModuleByIndex( u32 index )
{
    assert( index < moduleGetModuleCount() );
    return &Modules[index];
}

inline ModuleInfo* moduleGetModuleByName( const char* name )
{
    ModuleInfo* module = moduleGetModuleByIndex( 0 );
    for ( u32 i = 0; i < moduleGetModuleCount(); ++i )
    {
        if ( stringEquals( module->shortName, name ) )
            return module;

        module++;
    }

    return NULL;
}

#endif