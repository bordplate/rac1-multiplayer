#ifndef MODULELIST_H
#define MODULELIST_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type of the function used to initialize a module.
 * 
 */
typedef void(*ModuleInitFunc)();

/**
 * @brief Type of the function used to shut down a module.
 * 
 */
typedef void(*ModuleShutdownFunc)();

/**
 * @brief Runtime module flags.
 * 
 */
typedef enum ModuleFlags {
    MODULE_FLAG_INIT = 1 << 0,
    MODULE_FLAG_SHUTDOWN = 1 << 1,
    MODULE_FLAG_ENABLED = 1 << 2,
} ModuleFlags;

/**
 * @brief Module initialization status. Used to resolve errors.
 * 
 */
typedef enum ModuleInitStatus {
    MODULE_INIT_STATUS_ENABLED,
    MODULE_INIT_STATUS_DISABLED,
    MODULE_INIT_STATUS_DEPENDENCY_INVALID,
    MODULE_INIT_STATUS_DEPENDENCY_DISABLED
} ModuleInitStatus;

/**
 * @brief Describes a module.
 * 
 */
typedef struct ModuleInfo {
    /**
     * @brief Short name used to refer to the module.
     */
    const char *shortName;

    /**
     * @brief Long name of the module.
     */
    const char *longName;

    /**
     * @brief Name of the setting used to enable or disable this module.
     */
    const char *toggleSettingName;

    /**
     * @brief Function used to initialize the module.
     */
    ModuleInitFunc init;

    /**
     * @brief Function used to shut down the module.
     */
    ModuleShutdownFunc shutdown;

    /**
     * @brief Short names of the modules this module depends on.
     * Module dependencies are loaded before the module itself, and if any of the dependencies
     * fail to load due to being disabled or due to an invalid configuration, the module won't be loaded.
     */
    const char *dependencies[32];

    /**
     * @brief Runtime flags.
     */
    ModuleFlags flags;
} ModuleInfo;

/**
 * @brief Returns the number of declared modules (not enabled).
 * 
 * @return u32 
 */
u32 moduleGetModuleCount();

/**
 * @brief Returns the module with the given index.
 * 
 * @param index The index of the module to retrieve.
 * @return ModuleInfo* 
 */
ModuleInfo *moduleGetModuleByIndex(u32 index);

/**
 * @brief Returns the module with the given (short) name.
 * 
 * @param name The short name of the module.
 * @return ModuleInfo* or NULL if not found.
 */
ModuleInfo *moduleGetModuleByName(const char *name);

/**
 * @brief Initializes the specified module.
 * 
 * @param module 
 * @return ModuleInitStatus 
 */
ModuleInitStatus moduleInitModule(ModuleInfo *module);

/**
 * @brief Shut down/deinitialize the specified module.
 * 
 * @param module 
 */
void moduleShutdownModule(ModuleInfo *module);

#ifdef __cplusplus
}
#endif


#endif