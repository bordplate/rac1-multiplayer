// SHK - Static HooK library for PS3/Cell PPU
// Author: TGE
// Designed for hooking PS3 games for use with RPCS3.
// The SHK library solves a longstanding issue with RPCS3: it's inability to handle runtime code generation.
// Due to it's highly optimized LLVM recompilation model, all of the code has to be known at game boot.
// This architecture makes it incompatible with runtime hooking methods used by most hooking applications.

#ifndef SHK_H
#define SHK_H

#include "common.h"

// TODO: unhardcode TOC

// ELF TOC pointer
extern void* _shk_elf_toc;
#define SHK_ELF_TOC (&_shk_elf_toc) 

#ifdef __cplusplus
    #define SHK_API extern "C"
#else
    #define SHK_API 
#endif

// PPC ABI OPD structure used to refer to functions.
typedef struct
{
    void* function;
    void* TOC;
} shkOpd;

/**
 * @brief Registers an SHK hook.
 * @details 
 * - Declares extern trampoline function used for calling the original/bypassing the hook.
 * - Declares extern pointer to the dispatch function pointer used by the function patch.
 *   It's a workaround for not being able to call functions in the PRX directly
 *   In the start function of the PRX, the address of the hook handler is written to this pointer
 *   and later in the game code when the hooked function gets called, the game code has been patched so that it loads this pointer
 *   and calls it as a function.
 * - Declares function type used by hook handler/function being hooked
 * - Declares volatile pointer to trampoline function used to enforce indirect function calls.
 *   Direct calls from the PRX to a function in the game makes RPCS3 crash when using LLVM due to direct call optimization.
 *   Indirect calls are processed via the slower path which seems to work (tested on RPCS3 0.0.15-12057-d10584ac Alpha)
 * @param ret Return type of the hooked function
 * @param name Name of the hook
 * @param ... Parameter list, including both type and parameter name
 */
#define SHK_HOOK( ret, name, ... ) \
    extern ret _shk_prx_trampoline_##name( __VA_ARGS__ ); \
    extern volatile void* _shk_prx_ptr_##name; \
    typedef ret(*_shk_prx_hook_##name##_t)( __VA_ARGS__ ); \
    static volatile _shk_prx_hook_##name##_t _shk_prx_trampoline_##name##_ptr = _shk_prx_trampoline_##name 

/**
 * @brief Binds an SHK hook to the specified handler.
 * @param name Name of the hook.
 * @param handler Callback used to handle the hook. Must match the calling convention of the hook.
 */
#define SHK_BIND_HOOK( name, handler ) \
    _shk_prx_ptr_##name = (void*)( handler )

/**
 * @brief Calls the original unhooked function of a function that is hooked by SHK.
 * @param name Name of the hook
 * @param ... Argument list
 */
#define SHK_CALL_HOOK( name, ... ) \
    _shk_prx_trampoline_##name##_ptr( __VA_ARGS__ )

// // Registers a function for calling.
// // Prepares a function wrapper (OPD) to make the function eligible for calling from a PRX.
// // Parameters:
// // - addr: Address of the function
// // - ret: Return type of the function
// // - name: Name of the function
// // TODO: unhardcode TOC
// #define SHK_FUNCTION_( addr, ret, name, ... ) \
//     const void* _shk_prx_ptr_##name = ( const void* )addr; \
//     typedef ret(*_shk_##name##_t)( __VA_ARGS__ ); \
//     const shkOpd _shk_opd_##name = { ( void* )addr, ( void* )SHK_ELF_TOC }

// #define SHK_CALL_FUNCTION( name, ... ) \
//     ( ( _shk_##name##_t )&_shk_opd_##name )( __VA_ARGS__ )

#include "shk.gen.h"

#endif
