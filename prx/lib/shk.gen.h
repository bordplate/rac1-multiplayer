/**
 * @brief Register a void-returning function with 0 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V0( addr, name )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)(  );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name(  ) { ( ( name##_t )&name##_opd )(  ); }



/**
 * @brief Register a void-returning function with 1 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V1( addr, name, a0t, a0n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n ) { ( ( name##_t )&name##_opd )( a0n ); }



/**
 * @brief Register a void-returning function with 2 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V2( addr, name, a0t, a0n, a1t, a1n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n ) { ( ( name##_t )&name##_opd )( a0n, a1n ); }



/**
 * @brief Register a void-returning function with 3 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V3( addr, name, a0t, a0n, a1t, a1n, a2t, a2n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n ); }



/**
 * @brief Register a void-returning function with 4 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V4( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n ); }



/**
 * @brief Register a void-returning function with 5 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V5( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n ); }



/**
 * @brief Register a void-returning function with 6 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V6( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n ); }



/**
 * @brief Register a void-returning function with 7 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V7( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n ); }



/**
 * @brief Register a void-returning function with 8 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V8( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n ); }



/**
 * @brief Register a void-returning function with 9 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V9( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n ); }



/**
 * @brief Register a void-returning function with 10 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V10( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n ); }



/**
 * @brief Register a void-returning function with 11 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V11( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n ); }



/**
 * @brief Register a void-returning function with 12 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V12( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n ); }



/**
 * @brief Register a void-returning function with 13 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V13( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n ); }



/**
 * @brief Register a void-returning function with 14 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V14( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n, a13t, a13n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n, a13n ); }



/**
 * @brief Register a void-returning function with 15 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V15( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n, a13t, a13n, a14t, a14n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n, a13n, a14n ); }



/**
 * @brief Register a void-returning function with 16 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_V16( addr, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n, a13t, a13n, a14t, a14n, a15t, a15n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef void(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n, a15t a15n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline void name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n, a15t a15n ) { ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n, a13n, a14n, a15n ); }



/**
 * @brief Register a void-returning function with 0 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R0( addr, ret, name )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)(  );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name(  ) { return ( ( name##_t )&name##_opd )(  ); }



/**
 * @brief Register a void-returning function with 1 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R1( addr, ret, name, a0t, a0n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n ) { return ( ( name##_t )&name##_opd )( a0n ); }



/**
 * @brief Register a void-returning function with 2 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R2( addr, ret, name, a0t, a0n, a1t, a1n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n ) { return ( ( name##_t )&name##_opd )( a0n, a1n ); }



/**
 * @brief Register a void-returning function with 3 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R3( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n ); }



/**
 * @brief Register a void-returning function with 4 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R4( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n ); }



/**
 * @brief Register a void-returning function with 5 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R5( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n ); }



/**
 * @brief Register a void-returning function with 6 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R6( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n ); }



/**
 * @brief Register a void-returning function with 7 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R7( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n ); }



/**
 * @brief Register a void-returning function with 8 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R8( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n ); }



/**
 * @brief Register a void-returning function with 9 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R9( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n ); }



/**
 * @brief Register a void-returning function with 10 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R10( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n ); }



/**
 * @brief Register a void-returning function with 11 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R11( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n ); }



/**
 * @brief Register a void-returning function with 12 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R12( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n ); }



/**
 * @brief Register a void-returning function with 13 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R13( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n ); }



/**
 * @brief Register a void-returning function with 14 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R14( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n, a13t, a13n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n, a13n ); }



/**
 * @brief Register a void-returning function with 15 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R15( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n, a13t, a13n, a14t, a14n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n, a13n, a14n ); }



/**
 * @brief Register a void-returning function with 16 arguments. The function can be called using the given name.
 * @paragraph 
 * Declares the following symbols:
 * - name_addr: The address of the function.
 * - name_t: The function pointer type of the function.
 * - name_opd: The function OPD structure. This is the internal structure used for function pointers.
 * - name(...): A wrapper function that calls the function at the address. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.
 * @param aNt: The type of the function parameter.
 * @param aNn: The name of the function parameter.
 */
#define SHK_FUNCTION_R16( addr, ret, name, a0t, a0n, a1t, a1n, a2t, a2n, a3t, a3n, a4t, a4n, a5t, a5n, a6t, a6n, a7t, a7n, a8t, a8n, a9t, a9n, a10t, a10n, a11t, a11n, a12t, a12n, a13t, a13n, a14t, a14n, a15t, a15n )\
    static const void* name##_addr = ( const void* )addr;\
    typedef ret(*name##_t)( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n, a15t a15n );\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\
    static inline ret name( a0t a0n, a1t a1n, a2t a2n, a3t a3n, a4t a4n, a5t a5n, a6t a6n, a7t a7n, a8t a8n, a9t a9n, a10t a10n, a11t a11n, a12t a12n, a13t a13n, a14t a14n, a15t a15n ) { return ( ( name##_t )&name##_opd )( a0n, a1n, a2n, a3n, a4n, a5n, a6n, a7n, a8n, a9n, a10n, a11n, a12n, a13n, a14n, a15n ); }

