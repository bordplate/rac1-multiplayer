import os
import argparse
import sys

ASM_HOOK_PATCH_TEMPLATE = \
'''
# patch func to call hook thunk
.section .text.shk_patch_{HOOK}, "x"
.global ._shk_{HOOK}
._shk_{HOOK}:
b ._shk_thunk_{HOOK}
'''

ASM_HOOK_THUNK_TEMPLATE = \
'''
# loads address(es) and calls generic thunk
.section .text.shk_shared, "x"
.global ._shk_thunk_{HOOK}
._shk_thunk_{HOOK}:
lis     r12, _shk_ptr_{HOOK}@h         # load func ptr opd address (hi)
ori     r12, r12, _shk_ptr_{HOOK}@l    # load func ptr opd address (lo)
b       ._shk_thunk             # jump to function that calls ptr in r12
'''

ASM_HOOK_SHARED_TEMPLATE = \
'''
# function hook data
.section .text.shk_shared, "x"

# calls a hook function pointer stored in r12
.global ._shk_thunk
._shk_thunk:
.set back_chain, -0x80
.set saved_toc, -0x58
.set saved_r31, -8
.set sender_lr,  0x10
stdu      r1, back_chain(r1)        # create stack frame
mflr      r0                        # put lr in r0
std       r31, 0x80+saved_r31(r1)   # save r31
std       r0, 0x80+sender_lr(r1)    # save lr
mr        r31, r1                   # save current fp in r31
lwz       r12, 0(r12)               # load opd ptr
lwz       r0, 0(r12)                # load func ptr
mtctr     r0                        # move func ptr to control register
std       r2, 0x80+saved_toc(r1)    # save toc
lwz       r2, 4(r12)                # load new toc
bctrl                               # call func tpr
ld        r2, 0x80+saved_toc(r1)    # restore toc
ld        r11, 0x80+back_chain(r1)  # load old stack ptr
ld        r0, sender_lr(r11)        # load saved lr
mtlr      r0                        # restore lr
ld        r31, saved_r31(r11)       # restore r31
mr        r1, r11                   # restore r1
blr                                 # return to caller
'''

#.section .text.shk_module_shared, "x"
ASM_HOOK_TRAMPOLINE_TEMPLATE = \
'''
.text
.global ._shk_trampoline_{HOOK}
._shk_trampoline_{HOOK}:
{INSTR0}
b ._shk_{HOOK}+(1*4)

.data
.global _shk_trampoline_{HOOK}
_shk_trampoline_{HOOK}:
.int ._shk_trampoline_{HOOK}
.int {TOC}
'''

ASM_HOOK_PTR_TEMPLATE = \
'''
.section .data.shk_shared, "w"
.global _shk_ptr_{HOOK}
_shk_ptr_{HOOK}:
.int 0xDEADBABE
'''

MK_INJECT_TEMPLATE = \
'''
PYTHON = py -3
CC = $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
CXX = $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-g++.exe
OBJCOPY = $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-objcopy.exe
OBJDUMP = $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-objdump.exe
TOOLS_DIR = {TOOLS_DIR}
IN_DIR = {IN_DIR}
TMP_DIR = {TMP_DIR}
OUT_DIR = {OUT_DIR}
SHARED_TEXT_ADDRESS = {SHARED_TEXT_ADDRESS}
SHARED_DATA_ADDRESS = {SHARED_DATA_ADDRESS}
PATCH_FILE = {PATCH_FILE}
BIN2RPCS3PATCH = $(TOOLS_DIR)/bin2rpcs3patch.py

compile:
	$(CC) "$(IN_DIR)/shk_inject.gen.s" -o "$(TMP_DIR)/shk_inject.o" -T "$(IN_DIR)/shk_inject.gen.ld" -v -Wa,-mregnames -nostartfiles -nodefaultlibs

binary: compile{HOOK_OUTPUTS}
	$(OBJCOPY) -O binary --only-section=.text.shk_shared "$(TMP_DIR)/shk_inject.o" "$(TMP_DIR)/.text.shk_shared.bin" -v
	$(OBJCOPY) -O binary --only-section=.data.shk_shared "$(TMP_DIR)/shk_inject.o" "$(TMP_DIR)/.data.shk_shared.bin" -v

patch: binary
	$(PYTHON) $(BIN2RPCS3PATCH) --input{HOOK_INPUTS} "$(TMP_DIR)/.text.shk_shared.bin" "$(TMP_DIR)/.data.shk_shared.bin" --address{HOOK_ADDRESSES} $(SHARED_TEXT_ADDRESS) $(SHARED_DATA_ADDRESS) --output "$(PATCH_FILE)" --replace_patch shk_inject --indent 3
'''

MK_INJECT_HOOK_OUTPUT_TEMPLATE = \
'''
	$(OBJCOPY) -O binary --only-section=.text.shk_patch_{HOOK} "$(TMP_DIR)/shk_inject.o" "$(TMP_DIR)/.text.shk_patch_{HOOK}.bin" -v
'''

MK_INJECT_HOOK_INPUT_TEMPLATE = \
'''
"$(TMP_DIR)/.text.shk_patch_{HOOK}.bin"
'''

ASM_HOOK_PTR_SIZE = 4

def getScriptName():
    return os.path.basename( __file__ )

def setTemplateVar( template: str, name: str, val: str ):
    if name not in template:
        raise Exception( f"Template variable {name} does not exist in template: {template}" )
    
    return template.replace( name, val )

def fillTemplate( template, **kwargs ):
    t = template
    for key, val in kwargs.items():
        t = setTemplateVar( t, f"{{{key}}}", val )
    return t

def writeAsmHookPtr( f, hook ):
    f.write( fillTemplate( ASM_HOOK_PTR_TEMPLATE, HOOK=hook ) + "\n" )
    
def writeAsmHookTrampoline( f, func, instrs, tocAddr ):
    t = fillTemplate( ASM_HOOK_TRAMPOLINE_TEMPLATE, HOOK=func, TOC=hex( tocAddr ) )
    for i, instr in enumerate( instrs ):
        t = setTemplateVar( t, f"{{INSTR{i}}}", instr )
    f.write( t + "\n" )
    
def writeAsmHookShared( f ):
    f.write( ASM_HOOK_SHARED_TEMPLATE + "\n" )

def writeAsmHookPatch( f, func ):
    f.write( fillTemplate( ASM_HOOK_PATCH_TEMPLATE, HOOK=func ) + "\n" )
    
def writeAsmHookThunk( f, func ):
    f.write( fillTemplate( ASM_HOOK_THUNK_TEMPLATE, HOOK=func ) + "\n" )
    
def writeAsmMacroSet( f, name, val ):
    f.write( f".set {name}, {val}" )

def writeLdSection( f, name, addr ):
    f.write( f"    . = {hex(addr)};\n" )
    f.write( f"    {name} : {{ *({name}) }}\n")
    
def writeLdDefSym( f, name, val ):
    f.write( f"-Wl,--defsym,{name}={val} " )
    
class HookInfo:
    def __init__( self, name: str, addr: int, replacedInstr: list ):
        self.name = name
        self.addr = addr
        self.replacedInstr = replacedInstr
        
def hexInt( s ):
    return int( s, 0 )

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument( '--tools_dir', type=str, required=True, help='path to ckit tools directory' )
    parser.add_argument( '--out_dir', type=str, required=True, help='output directory for generated files' )
    parser.add_argument( '--build_in_dir', type=str, required=True, help='input directory used for building' )
    parser.add_argument( '--build_tmp_dir', type=str, required=True, help='temporary output directory for generated files during building' )
    parser.add_argument( '--build_out_dir', type=str, required=True, help='output directory for generated files during building' )
    parser.add_argument( '--toc', type=hexInt, required=True, help='TOC (r2) address of the functions to patch' )
    parser.add_argument( '--hook_shared_text_range', type=hexInt, required=True, nargs="+", help='start/end address(es) at which shared code for hooking is stored when injected' )
    parser.add_argument( '--hook_shared_data_range', type=hexInt, required=True, nargs="+", help='start/end address(es) at which shared data for hooking is stored when injected' )
    parser.add_argument( '--hooks', type=str, nargs='+', required=True, help='space separated list of hooks in the format of HOOKNAME,0xADDRESS,REPLACED_INSTRUCTION' )
    parser.add_argument( '--patch_file', type=str, required=True, help='rpcs3 patch file in which the compiled patch is inserted' )
    args = parser.parse_args()
    
    hooks = []
    for hookStr in args.hooks:
        toks = hookStr.split( "/" )
        hooks.append( HookInfo( toks[0], int( toks[1], 0 ), [toks[2]] ) )
       
    # sort hooks by address so the linker doesn't mess up the addresses 
    hooks = sorted( hooks, key=lambda h: h.addr )
    
    # generate asm to inject into host
    with open( os.path.join( args.out_dir, "shk_inject.gen.s" ), "w" ) as f:
        f.write( f'# generated by {getScriptName()}\n' )
        
        writeAsmHookShared( f )
        for hook in hooks:
            writeAsmHookPatch( f, hook.name )
            writeAsmHookThunk( f, hook.name )
            writeAsmHookPtr( f, hook.name )
            
    # generate inject linker script
    with open( os.path.join( args.out_dir, "shk_inject.gen.ld" ), "w" ) as f:
        f.write( f'/* generated by {getScriptName()}*/\n' )
        
        f.write( "SECTIONS {\n" )
        
        # write hook patch sections
        for hook in hooks:
            writeLdSection( f, f".text.shk_patch_{hook.name}", hook.addr )
        
        # write shared hook sections
        writeLdSection( f, ".text.shk_shared", args.hook_shared_text_range[0] )
        writeLdSection( f, ".data.shk_shared", args.hook_shared_data_range[0] )
        
        f.write( "}\n" )
        
    # generate inject makefile
    with open( os.path.join( args.out_dir, "shk_inject.gen.mk" ), "w" ) as f:
        f.write( f'# generated by {getScriptName()}\n' )
        
        binaryHooksSrc = ""
        for hook in hooks:
            binaryHooksSrc += "\n" + fillTemplate( MK_INJECT_HOOK_OUTPUT_TEMPLATE.strip("\n"), HOOK=hook.name )
            
        patchHookInputsSrc = ""
        for hook in hooks:
            patchHookInputsSrc += " " + fillTemplate( MK_INJECT_HOOK_INPUT_TEMPLATE.strip("\n"), HOOK=hook.name )
        
        patchHookAddressesSrc = ""
        for hook in hooks:
            patchHookAddressesSrc += " " + hex( hook.addr )
        
        src = fillTemplate( MK_INJECT_TEMPLATE, 
            TOOLS_DIR=args.tools_dir,
            IN_DIR=args.build_in_dir,
            OUT_DIR=args.build_out_dir,
            TMP_DIR=args.build_tmp_dir,               
            HOOK_OUTPUTS=binaryHooksSrc, 
            HOOK_INPUTS=patchHookInputsSrc, 
            HOOK_ADDRESSES=patchHookAddressesSrc, 
            SHARED_TEXT_ADDRESS=hex( args.hook_shared_text_range[0] ), 
            SHARED_DATA_ADDRESS=hex( args.hook_shared_data_range[0] ),
            PATCH_FILE=args.patch_file )
        
        f.write( src + "\n" )
            
    # generate asm to include with the module
    with open( os.path.join( args.out_dir, "shk.gen.s" ), "w" ) as f:
        f.write( f'# generated by {getScriptName()}\n' )
        for hook in hooks:
            writeAsmHookTrampoline( f, hook.name, hook.replacedInstr, args.toc )
        
    # define linker symbols to be included with the module
    with open( os.path.join( args.out_dir, "shk.gen.mk" ), "w" ) as f:
        f.write( f'# generated by {getScriptName()}\n' )
        f.write( "SHK_DEFSYMS = " )
    
        # todo verify that these addresses line up
        curDataAddr = args.hook_shared_data_range[0]
        for hook in hooks:
            # write symbol for hook address (likely the function itself)
            writeLdDefSym( f, f"._shk_{hook.name}", hex( hook.addr ) )
            writeLdDefSym( f, f"_shk_ptr_{hook.name}", hex( curDataAddr ) )
            curDataAddr += ASM_HOOK_PTR_SIZE
            assert( curDataAddr < args.hook_shared_data_range[1] )
        
    pass

if __name__ == '__main__':
    main()