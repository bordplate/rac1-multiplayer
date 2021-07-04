TEMPLATE_PARAMDOC = \
'''
 * @param a{N}t: The type of the {RANK} function parameter.
 * @param a{N}: The name of the {RANK} function parameter.'''

TEMPLATE_FUNC = \
'''
/**
 * @brief Calls a function with {N} arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.{PARAMDOC}
 */
#define SHK_FUNCTION_CALL_TOC_{N}( toc, addr, ret{MACROPARAMS} )\\\n\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)toc }; return ( ( ret(*)( {PARAMTYPES} ) )&_shk_prx_opd )( {ARGS} )

/**
 * @brief Calls a function with {N} arguments and returns the result.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.{PARAMDOC}
 */
#define SHK_FUNCTION_CALL_{N}( addr, ret{MACROPARAMS} )\\\n\
    shkOpd _shk_prx_opd = { ( void* )addr, ( void*)SHK_ELF_TOC }; return ( ( ret(*)( {PARAMTYPES} ) )&_shk_prx_opd )( {ARGS} )

/**
 * @brief Defines a static function with {N} arguments using the specified TOC address. The function can be called using the given name.
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.{PARAMDOC}
 */
#define SHK_FUNCTION_DEFINE_STATIC_TOC_{N}( toc, addr, ret, name{MACROPARAMS} )\\\n\
    static inline ret name( {PARAMS} ) { SHK_FUNCTION_CALL_TOC_{N}( toc, addr, ret{MACROPARAMS} ); }

/**
 * @brief Defines a function with {N} arguments using the specified TOC address. The function can be called using the given name. 
 * @param toc: The TOC (r2) address for this function.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.{PARAMDOC}
 */
#define SHK_FUNCTION_DEFINE_TOC_{N}( toc, addr, ret, name{MACROPARAMS} )\\\n\
    inline ret name( {PARAMS} ) { SHK_FUNCTION_CALL_TOC_{N}( toc, addr, ret{MACROPARAMS} ); }

/**
 * @brief Defines a static function with {N} arguments using the default TOC address. The function can be called using the given name. 
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.{PARAMDOC}
 */
#define SHK_FUNCTION_DEFINE_STATIC_{N}( addr, ret, name{MACROPARAMS} )\\\n\
    SHK_FUNCTION_DEFINE_STATIC_TOC_{N}( SHK_ELF_TOC, addr, ret, name{MACROPARAMS} )

/**
 * @brief Defines a function with {N} arguments using the default TOC address.. The function can be called using the given name.
 * @param addr: The function address
 * @param ret: The return type of the function.
 * @param name: The name of the function. This must be a valid C name.{PARAMDOC}
 */
#define SHK_FUNCTION_DEFINE_{N}( addr, ret, name{MACROPARAMS} )\\\n\
    SHK_FUNCTION_DEFINE_TOC_{N}( SHK_ELF_TOC, addr, ret, name{MACROPARAMS} )
'''

def formatRank( i ):
    if i == 1: return "1st"
    if i == 2: return "2nd"
    if i == 3: return "3rd"
    return f"{i}th"

def genMacro( count, template ):
    for i in range( count + 1 ):
        macroParams = ""
        for j in range( i ):
            macroParams += f", a{1 +j}t, a{1 +j}"
        
        params = ""
        for j in range( i ):
            params += f"a{1 + j}t a{1 + j}"
            if j != i - 1:
                params += ", "
            
        args = ""
        for j in range( i ):
            args += f"a{1 + j}"
            if j != i - 1:
                args += ", "
                
        paramDoc = ""
        for j in range( i ):
            paramDoc += TEMPLATE_PARAMDOC.replace( "{N}", str(1 + j) ).replace( "{RANK}", formatRank(1 + j) )
            
        paramTypes = ""
        for j in range( i ):
            paramTypes += f"a{1 + j}t"
            if j != i - 1:
                paramTypes += ", "

        print( template \
              .replace( "{N}", str(i) ) \
              .replace( "{PARAMDOC}", paramDoc ) \
              .replace( "{MACROPARAMS}", macroParams ) \
              .replace( "{PARAMS}", params ) \
              .replace( "{PARAMTYPES}", paramTypes )
              .replace( "{ARGS}", args ) + "\n" )

def main():
    count = 16
    genMacro( count, TEMPLATE_FUNC )
    
    pass

if __name__ == '__main__':
    main()