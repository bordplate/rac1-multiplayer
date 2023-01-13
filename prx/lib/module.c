
#include "clib.h"
#include "string.h"
#include "module.h"

#include "../modules.inc"

#ifdef __cplusplus
extern "C" {
#endif

u32 moduleGetModuleCount()
{
    return sizeof( sModules ) / sizeof( ModuleInfo );
}

ModuleInfo* moduleGetModuleByIndex( u32 index )
{
    assert( index < moduleGetModuleCount() );
    return &sModules[index];
}

ModuleInfo* moduleGetModuleByName( const char* name )
{
    ModuleInfo* module = moduleGetModuleByIndex( 0 );
    for ( u32 i = 0; i < moduleGetModuleCount(); ++i )
    {
        //if ( stringEquals( module->shortName, name ) )
            return module;

        module++;
    }

    return NULL;
}

ModuleInitStatus moduleInitModule( ModuleInfo* module )
{
    if ( module->flags & MODULE_FLAG_INIT )
    {
        // module has already been loaded before
        if ( module->flags & MODULE_FLAG_ENABLED )
            return MODULE_INIT_STATUS_ENABLED;
        else
            return MODULE_INIT_STATUS_DISABLED;
    }

    // set init flag early to prevent infinite recursion
    *(int*)&module->flags |= MODULE_FLAG_INIT;

    // mark as enabled for now to satisfy recursive loads
    // if an error occurs while resolving dependencies, the flag is unset
    *(int*)&module->flags |= MODULE_FLAG_ENABLED;

    // init dependencies first
    printf( "module: module '%s' (%s) loading dependencies\n", module->longName, module->shortName );
    const char** dependency = module->dependencies;
    while ( *dependency != NULL )
    {
        printf( "module: module '%s' (%s) loading dependency %s\n", module->longName, module->shortName, *dependency );

        ModuleInfo* dependencyModule = moduleGetModuleByName( *dependency );
        if ( !dependencyModule )
        {
            printf( "module: module '%s' (%s) is not loaded because dependency module '%s' is not found\n", 
                module->longName, module->shortName, *dependency );
            return MODULE_INIT_STATUS_DEPENDENCY_INVALID;
        }

        ModuleInitStatus status = moduleInitModule( dependencyModule );

        // handle status
        switch ( status )
        {
            case MODULE_INIT_STATUS_DEPENDENCY_DISABLED:
                printf( "module: module '%s' (%s) not loaded because dependency module %s (%s) failed to load a dependency\n",
                     module->longName, module->shortName, dependencyModule->longName, dependencyModule->shortName );

                *(int*)&module->flags &= ~MODULE_FLAG_ENABLED;
                return MODULE_INIT_STATUS_DEPENDENCY_DISABLED;

            case MODULE_INIT_STATUS_DEPENDENCY_INVALID:
                printf( "module: module '%s' (%s) not loaded because dependency module %s (%s) failed to load a dependency\n",
                     module->longName, module->shortName, dependencyModule->longName, dependencyModule->shortName );

                *(int*)&module->flags &= ~MODULE_FLAG_ENABLED;
                return MODULE_INIT_STATUS_DEPENDENCY_INVALID;

            case MODULE_INIT_STATUS_DISABLED:
                printf( "module: module '%s' (%s) not loaded because dependency module %s (%s) is disabled\n",
                     module->longName, module->shortName, dependencyModule->longName, dependencyModule->shortName );

                *(int*)&module->flags &= ~MODULE_FLAG_ENABLED;
                return MODULE_INIT_STATUS_DEPENDENCY_DISABLED;

            case MODULE_INIT_STATUS_ENABLED:
                // good
                break;
        }

        dependency++;
    }

    // call module init
    printf( "module: initialising module '%s' (%s)\n", module->longName, module->shortName );
    if ( module->init ) module->init();
}

void moduleShutdownModule( ModuleInfo* module )
{
    if ( module->flags & MODULE_FLAG_ENABLED && 
        !( module->flags & MODULE_FLAG_SHUTDOWN ) )
    {
        printf( "module: shutting down module %s (%s)\n", module->longName, module->shortName );
        if ( module->shutdown ) module->shutdown();
        *(int*)&module->flags |= MODULE_FLAG_SHUTDOWN;
    }
}

#ifdef __cplusplus
}
#endif