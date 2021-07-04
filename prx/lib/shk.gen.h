/**
 * @brief Calls a function with 0 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 */
#define SHK_FUNCTION_CALL_TOC_0( toc, addr, ret )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)(  ) )&_shk_prx_opd )(  )

/**
 * @brief Calls a function with 0 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 */
#define SHK_FUNCTION_CALL_0( addr, ret )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)(  ) )&_shk_prx_opd )(  )

/**
 * @brief Defines a static function with 0 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_0( toc, addr, ret, name )\
    static inline ret name(  ) { SHK_FUNCTION_CALL_TOC_0( toc, addr, ret ); }

/**
 * @brief Defines a function with 0 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 */
#define SHK_FUNCTION_DEFINE_TOC_0( toc, addr, ret, name )\
    inline ret name(  ) { SHK_FUNCTION_CALL_TOC_0( toc, addr, ret ); }

/**
 * @brief Defines a static function with 0 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 */
#define SHK_FUNCTION_DEFINE_STATIC_0( addr, ret, name )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_0( SHK_ELF_TOC, addr, ret, name )

/**
 * @brief Defines a function with 0 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 */
#define SHK_FUNCTION_DEFINE_0( addr, ret, name )\
    SHK_FUNCTION_DEFINE_TOC_0( SHK_ELF_TOC, addr, ret, name )



/**
 * @brief Calls a function with 1 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_1( toc, addr, ret, a1t, a1 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t ) )&_shk_prx_opd )( a1 )

/**
 * @brief Calls a function with 1 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 */
#define SHK_FUNCTION_CALL_1( addr, ret, a1t, a1 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t ) )&_shk_prx_opd )( a1 )

/**
 * @brief Defines a static function with 1 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_1( toc, addr, ret, name, a1t, a1 )\
    static inline ret name( a1t a1 ) { SHK_FUNCTION_CALL_TOC_1( toc, addr, ret, a1t, a1 ); }

/**
 * @brief Defines a function with 1 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_1( toc, addr, ret, name, a1t, a1 )\
    inline ret name( a1t a1 ) { SHK_FUNCTION_CALL_TOC_1( toc, addr, ret, a1t, a1 ); }

/**
 * @brief Defines a static function with 1 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_1( addr, ret, name, a1t, a1 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_1( SHK_ELF_TOC, addr, ret, name, a1t, a1 )

/**
 * @brief Defines a function with 1 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 */
#define SHK_FUNCTION_DEFINE_1( addr, ret, name, a1t, a1 )\
    SHK_FUNCTION_DEFINE_TOC_1( SHK_ELF_TOC, addr, ret, name, a1t, a1 )



/**
 * @brief Calls a function with 2 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_2( toc, addr, ret, a1t, a1, a2t, a2 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t ) )&_shk_prx_opd )( a1, a2 )

/**
 * @brief Calls a function with 2 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 */
#define SHK_FUNCTION_CALL_2( addr, ret, a1t, a1, a2t, a2 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t ) )&_shk_prx_opd )( a1, a2 )

/**
 * @brief Defines a static function with 2 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_2( toc, addr, ret, name, a1t, a1, a2t, a2 )\
    static inline ret name( a1t a1, a2t a2 ) { SHK_FUNCTION_CALL_TOC_2( toc, addr, ret, a1t, a1, a2t, a2 ); }

/**
 * @brief Defines a function with 2 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_2( toc, addr, ret, name, a1t, a1, a2t, a2 )\
    inline ret name( a1t a1, a2t a2 ) { SHK_FUNCTION_CALL_TOC_2( toc, addr, ret, a1t, a1, a2t, a2 ); }

/**
 * @brief Defines a static function with 2 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_2( addr, ret, name, a1t, a1, a2t, a2 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_2( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2 )

/**
 * @brief Defines a function with 2 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 */
#define SHK_FUNCTION_DEFINE_2( addr, ret, name, a1t, a1, a2t, a2 )\
    SHK_FUNCTION_DEFINE_TOC_2( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2 )



/**
 * @brief Calls a function with 3 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_3( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t ) )&_shk_prx_opd )( a1, a2, a3 )

/**
 * @brief Calls a function with 3 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 */
#define SHK_FUNCTION_CALL_3( addr, ret, a1t, a1, a2t, a2, a3t, a3 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t ) )&_shk_prx_opd )( a1, a2, a3 )

/**
 * @brief Defines a static function with 3 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_3( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3 )\
    static inline ret name( a1t a1, a2t a2, a3t a3 ) { SHK_FUNCTION_CALL_TOC_3( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3 ); }

/**
 * @brief Defines a function with 3 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_3( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3 )\
    inline ret name( a1t a1, a2t a2, a3t a3 ) { SHK_FUNCTION_CALL_TOC_3( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3 ); }

/**
 * @brief Defines a static function with 3 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_3( addr, ret, name, a1t, a1, a2t, a2, a3t, a3 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_3( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3 )

/**
 * @brief Defines a function with 3 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 */
#define SHK_FUNCTION_DEFINE_3( addr, ret, name, a1t, a1, a2t, a2, a3t, a3 )\
    SHK_FUNCTION_DEFINE_TOC_3( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3 )



/**
 * @brief Calls a function with 4 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_4( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t ) )&_shk_prx_opd )( a1, a2, a3, a4 )

/**
 * @brief Calls a function with 4 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 */
#define SHK_FUNCTION_CALL_4( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t ) )&_shk_prx_opd )( a1, a2, a3, a4 )

/**
 * @brief Defines a static function with 4 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_4( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4 ) { SHK_FUNCTION_CALL_TOC_4( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4 ); }

/**
 * @brief Defines a function with 4 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_4( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4 ) { SHK_FUNCTION_CALL_TOC_4( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4 ); }

/**
 * @brief Defines a static function with 4 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_4( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_4( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )

/**
 * @brief Defines a function with 4 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 */
#define SHK_FUNCTION_DEFINE_4( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )\
    SHK_FUNCTION_DEFINE_TOC_4( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4 )



/**
 * @brief Calls a function with 5 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_5( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5 )

/**
 * @brief Calls a function with 5 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 */
#define SHK_FUNCTION_CALL_5( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5 )

/**
 * @brief Defines a static function with 5 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_5( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5 ) { SHK_FUNCTION_CALL_TOC_5( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 ); }

/**
 * @brief Defines a function with 5 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_5( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5 ) { SHK_FUNCTION_CALL_TOC_5( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 ); }

/**
 * @brief Defines a static function with 5 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_5( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_5( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )

/**
 * @brief Defines a function with 5 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 */
#define SHK_FUNCTION_DEFINE_5( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )\
    SHK_FUNCTION_DEFINE_TOC_5( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5 )



/**
 * @brief Calls a function with 6 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_6( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6 )

/**
 * @brief Calls a function with 6 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 */
#define SHK_FUNCTION_CALL_6( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6 )

/**
 * @brief Defines a static function with 6 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_6( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6 ) { SHK_FUNCTION_CALL_TOC_6( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 ); }

/**
 * @brief Defines a function with 6 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_6( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6 ) { SHK_FUNCTION_CALL_TOC_6( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 ); }

/**
 * @brief Defines a static function with 6 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_6( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_6( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )

/**
 * @brief Defines a function with 6 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 */
#define SHK_FUNCTION_DEFINE_6( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )\
    SHK_FUNCTION_DEFINE_TOC_6( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6 )



/**
 * @brief Calls a function with 7 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_7( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7 )

/**
 * @brief Calls a function with 7 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 */
#define SHK_FUNCTION_CALL_7( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7 )

/**
 * @brief Defines a static function with 7 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_7( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7 ) { SHK_FUNCTION_CALL_TOC_7( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 ); }

/**
 * @brief Defines a function with 7 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_7( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7 ) { SHK_FUNCTION_CALL_TOC_7( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 ); }

/**
 * @brief Defines a static function with 7 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_7( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_7( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )

/**
 * @brief Defines a function with 7 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 */
#define SHK_FUNCTION_DEFINE_7( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )\
    SHK_FUNCTION_DEFINE_TOC_7( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7 )



/**
 * @brief Calls a function with 8 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_8( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8 )

/**
 * @brief Calls a function with 8 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 */
#define SHK_FUNCTION_CALL_8( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8 )

/**
 * @brief Defines a static function with 8 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_8( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8 ) { SHK_FUNCTION_CALL_TOC_8( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 ); }

/**
 * @brief Defines a function with 8 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_8( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8 ) { SHK_FUNCTION_CALL_TOC_8( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 ); }

/**
 * @brief Defines a static function with 8 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_8( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_8( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )

/**
 * @brief Defines a function with 8 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 */
#define SHK_FUNCTION_DEFINE_8( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )\
    SHK_FUNCTION_DEFINE_TOC_8( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8 )



/**
 * @brief Calls a function with 9 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_9( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9 )

/**
 * @brief Calls a function with 9 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 */
#define SHK_FUNCTION_CALL_9( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9 )

/**
 * @brief Defines a static function with 9 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_9( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9 ) { SHK_FUNCTION_CALL_TOC_9( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 ); }

/**
 * @brief Defines a function with 9 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_9( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9 ) { SHK_FUNCTION_CALL_TOC_9( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 ); }

/**
 * @brief Defines a static function with 9 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_9( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_9( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )

/**
 * @brief Defines a function with 9 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 */
#define SHK_FUNCTION_DEFINE_9( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )\
    SHK_FUNCTION_DEFINE_TOC_9( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9 )



/**
 * @brief Calls a function with 10 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_10( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 )

/**
 * @brief Calls a function with 10 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 */
#define SHK_FUNCTION_CALL_10( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 )

/**
 * @brief Defines a static function with 10 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_10( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10 ) { SHK_FUNCTION_CALL_TOC_10( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 ); }

/**
 * @brief Defines a function with 10 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_10( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10 ) { SHK_FUNCTION_CALL_TOC_10( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 ); }

/**
 * @brief Defines a static function with 10 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_10( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_10( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )

/**
 * @brief Defines a function with 10 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 */
#define SHK_FUNCTION_DEFINE_10( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )\
    SHK_FUNCTION_DEFINE_TOC_10( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10 )



/**
 * @brief Calls a function with 11 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_11( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 )

/**
 * @brief Calls a function with 11 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 */
#define SHK_FUNCTION_CALL_11( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 )

/**
 * @brief Defines a static function with 11 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_11( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11 ) { SHK_FUNCTION_CALL_TOC_11( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 ); }

/**
 * @brief Defines a function with 11 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_11( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11 ) { SHK_FUNCTION_CALL_TOC_11( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 ); }

/**
 * @brief Defines a static function with 11 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_11( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_11( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )

/**
 * @brief Defines a function with 11 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 */
#define SHK_FUNCTION_DEFINE_11( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )\
    SHK_FUNCTION_DEFINE_TOC_11( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11 )



/**
 * @brief Calls a function with 12 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_12( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12 )

/**
 * @brief Calls a function with 12 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 */
#define SHK_FUNCTION_CALL_12( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12 )

/**
 * @brief Defines a static function with 12 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_12( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12 ) { SHK_FUNCTION_CALL_TOC_12( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 ); }

/**
 * @brief Defines a function with 12 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_12( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12 ) { SHK_FUNCTION_CALL_TOC_12( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 ); }

/**
 * @brief Defines a static function with 12 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_12( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_12( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )

/**
 * @brief Defines a function with 12 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 */
#define SHK_FUNCTION_DEFINE_12( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )\
    SHK_FUNCTION_DEFINE_TOC_12( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12 )



/**
 * @brief Calls a function with 13 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_13( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13 )

/**
 * @brief Calls a function with 13 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 */
#define SHK_FUNCTION_CALL_13( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13 )

/**
 * @brief Defines a static function with 13 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_13( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13 ) { SHK_FUNCTION_CALL_TOC_13( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 ); }

/**
 * @brief Defines a function with 13 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_13( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13 ) { SHK_FUNCTION_CALL_TOC_13( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 ); }

/**
 * @brief Defines a static function with 13 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_13( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_13( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )

/**
 * @brief Defines a function with 13 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 */
#define SHK_FUNCTION_DEFINE_13( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )\
    SHK_FUNCTION_DEFINE_TOC_13( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13 )



/**
 * @brief Calls a function with 14 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_14( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t, a14t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14 )

/**
 * @brief Calls a function with 14 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 */
#define SHK_FUNCTION_CALL_14( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t, a14t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14 )

/**
 * @brief Defines a static function with 14 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_14( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13, a14t a14 ) { SHK_FUNCTION_CALL_TOC_14( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 ); }

/**
 * @brief Defines a function with 14 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_14( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13, a14t a14 ) { SHK_FUNCTION_CALL_TOC_14( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 ); }

/**
 * @brief Defines a static function with 14 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_14( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_14( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )

/**
 * @brief Defines a function with 14 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 */
#define SHK_FUNCTION_DEFINE_14( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )\
    SHK_FUNCTION_DEFINE_TOC_14( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14 )



/**
 * @brief Calls a function with 15 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_15( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t, a14t, a15t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15 )

/**
 * @brief Calls a function with 15 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 */
#define SHK_FUNCTION_CALL_15( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t, a14t, a15t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15 )

/**
 * @brief Defines a static function with 15 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_15( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13, a14t a14, a15t a15 ) { SHK_FUNCTION_CALL_TOC_15( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 ); }

/**
 * @brief Defines a function with 15 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_15( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13, a14t a14, a15t a15 ) { SHK_FUNCTION_CALL_TOC_15( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 ); }

/**
 * @brief Defines a static function with 15 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_15( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_15( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )

/**
 * @brief Defines a function with 15 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 */
#define SHK_FUNCTION_DEFINE_15( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )\
    SHK_FUNCTION_DEFINE_TOC_15( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15 )



/**
 * @brief Calls a function with 16 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 * @param a16t: The type of the 16th function parameter.
 * @param a16: The name of the 16th function parameter.
 */
#define SHK_FUNCTION_CALL_TOC_16( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t, a14t, a15t, a16t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 )

/**
 * @brief Calls a function with 16 arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 * @param a16t: The type of the 16th function parameter.
 * @param a16: The name of the 16th function parameter.
 */
#define SHK_FUNCTION_CALL_16( addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( a1t, a2t, a3t, a4t, a5t, a6t, a7t, a8t, a9t, a10t, a11t, a12t, a13t, a14t, a15t, a16t ) )&_shk_prx_opd )( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 )

/**
 * @brief Defines a static function with 16 arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 * @param a16t: The type of the 16th function parameter.
 * @param a16: The name of the 16th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_16( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )\
    static inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13, a14t a14, a15t a15, a16t a16 ) { SHK_FUNCTION_CALL_TOC_16( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 ); }

/**
 * @brief Defines a function with 16 arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 * @param a16t: The type of the 16th function parameter.
 * @param a16: The name of the 16th function parameter.
 */
#define SHK_FUNCTION_DEFINE_TOC_16( toc, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )\
    inline ret name( a1t a1, a2t a2, a3t a3, a4t a4, a5t a5, a6t a6, a7t a7, a8t a8, a9t a9, a10t a10, a11t a11, a12t a12, a13t a13, a14t a14, a15t a15, a16t a16 ) { SHK_FUNCTION_CALL_TOC_16( toc, addr, ret, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 ); }

/**
 * @brief Defines a static function with 16 arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 * @param a16t: The type of the 16th function parameter.
 * @param a16: The name of the 16th function parameter.
 */
#define SHK_FUNCTION_DEFINE_STATIC_16( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )\
    SHK_FUNCTION_DEFINE_STATIC_TOC_16( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )

/**
 * @brief Defines a function with 16 arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param a1t: The type of the 1st function parameter.
 * @param a1: The name of the 1st function parameter.
 * @param a2t: The type of the 2nd function parameter.
 * @param a2: The name of the 2nd function parameter.
 * @param a3t: The type of the 3rd function parameter.
 * @param a3: The name of the 3rd function parameter.
 * @param a4t: The type of the 4th function parameter.
 * @param a4: The name of the 4th function parameter.
 * @param a5t: The type of the 5th function parameter.
 * @param a5: The name of the 5th function parameter.
 * @param a6t: The type of the 6th function parameter.
 * @param a6: The name of the 6th function parameter.
 * @param a7t: The type of the 7th function parameter.
 * @param a7: The name of the 7th function parameter.
 * @param a8t: The type of the 8th function parameter.
 * @param a8: The name of the 8th function parameter.
 * @param a9t: The type of the 9th function parameter.
 * @param a9: The name of the 9th function parameter.
 * @param a10t: The type of the 10th function parameter.
 * @param a10: The name of the 10th function parameter.
 * @param a11t: The type of the 11th function parameter.
 * @param a11: The name of the 11th function parameter.
 * @param a12t: The type of the 12th function parameter.
 * @param a12: The name of the 12th function parameter.
 * @param a13t: The type of the 13th function parameter.
 * @param a13: The name of the 13th function parameter.
 * @param a14t: The type of the 14th function parameter.
 * @param a14: The name of the 14th function parameter.
 * @param a15t: The type of the 15th function parameter.
 * @param a15: The name of the 15th function parameter.
 * @param a16t: The type of the 16th function parameter.
 * @param a16: The name of the 16th function parameter.
 */
#define SHK_FUNCTION_DEFINE_16( addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )\
    SHK_FUNCTION_DEFINE_TOC_16( SHK_ELF_TOC, addr, ret, name, a1t, a1, a2t, a2, a3t, a3, a4t, a4, a5t, a5, a6t, a6, a7t, a7, a8t, a8, a9t, a9, a10t, a10, a11t, a11, a12t, a12, a13t, a13, a14t, a14, a15t, a15, a16t, a16 )
