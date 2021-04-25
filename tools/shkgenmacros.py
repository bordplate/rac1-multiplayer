
TEMPLATE_FUNC_VOID = \
'''
/**
 * @brief Register a void-returning function with {N} arguments. The function can be called using the given name.
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
#define SHK_FUNCTION_V{N}( addr, name{MACROPARAMS} )\\\n\
    static const void* name##_addr = ( const void* )addr;\\\n\
    typedef void(*name##_t)( {PARAMS} );\\\n\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\\\n\
    static inline void name( {PARAMS} ) { ( ( name##_t )&name##_opd )( {ARGS} ); }
'''

TEMPLATE_FUNC_RET = \
'''
/**
 * @brief Register a void-returning function with {N} arguments. The function can be called using the given name.
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
#define SHK_FUNCTION_R{N}( addr, ret, name{MACROPARAMS} )\\\n\
    static const void* name##_addr = ( const void* )addr;\\\n\
    typedef ret(*name##_t)( {PARAMS} );\\\n\
    static const shkOpd name##_opd = { ( void* )addr, ( void* )SHK_ELF_TOC };\\\n\
    static inline ret name( {PARAMS} ) { return ( ( name##_t )&name##_opd )( {ARGS} ); }
'''

def genMacro( count, template ):
    for i in range( count + 1 ):
        macroParams = ""
        for j in range( i ):
            macroParams += f", a{j}t, a{j}n"
        
        params = ""
        for j in range( i ):
            params += f"a{j}t a{j}n"
            if j != i - 1:
                params += ", "
            
        args = ""
        for j in range( i ):
            args += f"a{j}n"
            if j != i - 1:
                args += ", "
            
        print( template \
              .replace( "{N}", str(i) ) \
              .replace( "{MACROPARAMS}", macroParams ) \
              .replace( "{PARAMS}", params ) \
              .replace( "{ARGS}", args ) + "\n" )

def main():
    count = 16
    genMacro( count, TEMPLATE_FUNC_VOID )
    genMacro( count, TEMPLATE_FUNC_RET )
    
    pass

if __name__ == '__main__':
    main()