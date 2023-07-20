import os
import argparse
import sys
import yaml

# limited to one instruction to make hooking stubs possible
ASM_INJECT_HOOK_PATCH_TEMPLATE = \
'''
# patch func to call hook thunk
.section .text.shk_elf_patch_{HOOK}, "x"
.global ._shk_elf_{HOOK}
._shk_elf_{HOOK}:
b ._shk_elf_thunk_{HOOK}
'''

# loads ptr offset and calls generic code that executes it
ASM_INJECT_HOOK_THUNK_TEMPLATE = \
'''
# loads address(es) and calls generic thunk
.section .text.shk_elf_shared, "x"
.global ._shk_elf_thunk_{HOOK}
._shk_elf_thunk_{HOOK}:
li      r0, {HOOK_PTR_OFFSET}  # 0xFFFF / 4 = 16383 hooks.... should suffice
b       ._shk_elf_thunk         # jump to function that calls the function index in r0
'''

# maybe this could be moved to the prx too
ASM_INJECT_HOOK_SHARED_TEMPLATE = \
'''
# function hook data
.section .text.shk_elf_shared, "x"

# calls a hook function index stored in r0
.global ._shk_elf_thunk
._shk_elf_thunk:
.set back_chain, -0x80
.set saved_toc, -0x58
.set saved_r31, -8
.set sender_lr,  0x10
# prolog
stdu      r1, back_chain(r1)                    # create stack frame
std       r31, 0x80+saved_r31(r1)               # save r31
mflr      r31                                   # put lr in r31
std       r31, 0x80+sender_lr(r1)               # save lr
# body
lis       r31, _shk_elf_prx_ptr_table@h         # load elf func ptr table ptr
ori       r31, r31, _shk_elf_prx_ptr_table@l    # cont.
lwz       r31, 0(r31)                           # load prx func ptr table
add       r31, r0, r31                          # add ptr offset
lwz       r31, 0(r31)                           # load opd ptr
lwz       r0, 0(r31)                            # load func ptr
mtctr     r0                                    # move func ptr to control register
std       r2, 0x80+saved_toc(r1)                # save toc
lwz       r2, 4(r31)                            # load new toc
mr        r31, r1                               # save current fp in r31
bctrl                                           # call func tpr
# epilog
ld        r2, 0x80+saved_toc(r1)                # restore toc
ld        r31, 0x80+back_chain(r1)              # load old stack ptr
ld        r0, sender_lr(r31)                    # load saved lr
mtlr      r0                                    # restore lr
mr        r1, r31                               # restore r1
ld        r31, saved_r31(r31)                   # restore r31
blr                                             # return to caller
'''

#.section .text.shk_module_shared, "x"
ASM_PRX_HOOK_TRAMPOLINE_TEMPLATE = \
'''
.text
.global ._shk_prx_trampoline_{HOOK}
._shk_prx_trampoline_{HOOK}:
{INSTR0}
b ._shk_elf_{HOOK}+(1*4)

.data
.global _shk_prx_trampoline_{HOOK}
_shk_prx_trampoline_{HOOK}:
.int ._shk_prx_trampoline_{HOOK}
.int {TOC}
'''

ASM_PRX_HOOK_TRAMPOLINE_EMPTY_TEMPLATE = \
'''
.text
.global ._shk_prx_trampoline_{HOOK}
._shk_prx_trampoline_{HOOK}:

.data
.global _shk_prx_trampoline_{HOOK}
_shk_prx_trampoline_{HOOK}:
'''


# pointer to function pointer lookup table (in PRX memory)
ASM_INJECT_HOOK_PTR_TABLE_TEMPLATE = \
'''
.section .data.shk_elf_shared, "w"
.global _shk_elf_prx_ptr_table
_shk_elf_prx_ptr_table:
.int 0x0BADF00D
'''

ASM_PRX_HOOK_PTR_TABLE_TEMPLATE = \
'''
# table used to look up function pointers for hooking
.data
.global _shk_prx_ptr_table
_shk_prx_ptr_table:
{ENTRIES}
'''

# maybe automatically bind to handler?
# would need to specify handler name in build config
ASM_PRX_HOOK_PTR_TABLE_ENTRY_TEMPLATE = \
'''
.global _shk_prx_ptr_{HOOK} 
_shk_prx_ptr_{HOOK}:
.int _shk_prx_trampoline_{HOOK}
'''

MK_INJECT_TEMPLATE = \
'''
-include ../../userconfig.mk
-include ../../os.mk

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
GENERATEPATCHFILE = $(TOOLS_DIR)/generate_patch.py

compile:
	$(WINE) "$(CC)" "$(IN_DIR)/shk_elf.gen.s" -o "$(TMP_DIR)/shk_elf.o" -T "$(IN_DIR)/shk_elf.gen.ld" -v -Xlinker -Map="$(TMP_DIR)/shk_elf.map" -Wa,-mregnames -nostartfiles -nodefaultlibs

binary: compile{HOOK_OUTPUTS}
	$(WINE) "$(OBJCOPY)" -O binary --only-section=.text.shk_elf_shared "$(TMP_DIR)/shk_elf.o" "$(TMP_DIR)/.text.shk_elf_shared.bin" -v
	$(WINE) "$(OBJCOPY)" -O binary --only-section=.data.shk_elf_shared "$(TMP_DIR)/shk_elf.o" "$(TMP_DIR)/.data.shk_elf_shared.bin" -v

patch: binary
	#$(PYTHON) "$(BIN2RPCS3PATCH)" --input{HOOK_INPUTS} "$(TMP_DIR)/.text.shk_elf_shared.bin" "$(TMP_DIR)/.data.shk_elf_shared.bin" --address{HOOK_ADDRESSES} $(SHARED_TEXT_ADDRESS) $(SHARED_DATA_ADDRESS) --output "$(PATCH_FILE)" --replace_patch shk_elf_inject_{GAME} --indent 3
	$(PYTHON) "$(GENERATEPATCHFILE)" --input{HOOK_INPUTS} "$(TMP_DIR)/.text.shk_elf_shared.bin" "$(TMP_DIR)/.data.shk_elf_shared.bin" --address{HOOK_ADDRESSES} $(SHARED_TEXT_ADDRESS) $(SHARED_DATA_ADDRESS) --output "$(TMP_DIR)/patch.txt" --append
'''

MK_INJECT_HOOK_OUTPUT_TEMPLATE = \
'''
	$(WINE) "$(OBJCOPY)" -O binary --only-section=.text.shk_elf_patch_{HOOK} "$(TMP_DIR)/shk_elf.o" "$(TMP_DIR)/.text.shk_elf_patch_{HOOK}.bin" -v
'''

MK_INJECT_HOOK_INPUT_TEMPLATE = \
'''
"$(TMP_DIR)/.text.shk_elf_patch_{HOOK}.bin"
'''

MK_LOADER_TEMPLATE = \
'''
SHK_ELF_LOADER_INJECT_ADDRESS = {LOADER_INJECT_ADDR}
SHK_ELF_LOADER_ADDRESS = {LOADER_ADDR}
SHK_ELF_LD = build/shk_elf_loader.gen.ld
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

def writeAsmInjectHookSharedData( f ):
    f.write( fillTemplate( ASM_INJECT_HOOK_PTR_TABLE_TEMPLATE ) + "\n" )
    
def writeAsmPrxHookTrampoline( f, hook ):
    if hook.emitTrampoline:   
        t = fillTemplate( ASM_PRX_HOOK_TRAMPOLINE_TEMPLATE, HOOK=hook.name, TOC=hex( hook.toc ) )
        for i, instr in enumerate( hook.replacedInstr ):
            t = setTemplateVar( t, f"{{INSTR{i}}}", instr )
        f.write( t + "\n" )
    else:
        t = fillTemplate( ASM_PRX_HOOK_TRAMPOLINE_EMPTY_TEMPLATE, HOOK=hook.name )
        f.write( t + "\n" )   
    
def writeAsmInjectHookSharedText( f ):
    f.write( ASM_INJECT_HOOK_SHARED_TEMPLATE + "\n" )

def writeAsmInjectHookPatch( f, hook ):
    f.write( fillTemplate( ASM_INJECT_HOOK_PATCH_TEMPLATE, HOOK=hook.name ) + "\n" )
    
def writeAsmInjectHookThunk( f, hook ):
    f.write( fillTemplate( ASM_INJECT_HOOK_THUNK_TEMPLATE, HOOK=hook.name, HOOK_PTR_OFFSET=hex( hook.ptrOffset ) ) + "\n" )
    
def writeAsmMacroSet( f, name, val ):
    f.write( f".set {name}, {val}" )
    
def writeLdComment( f, val ):
    f.write( f'/* {val} */\n' )
    
def writeLdSectionsBegin( f ):
    f.write( "SECTIONS {\n")
    
def writeLdSectionsEnd( f ):
    f.write( "}\n")

def writeLdSection( f, name, addr ):
    f.write( f"    . = {hex(addr)};\n" )
    f.write( f"    {name} : {{ *({name}) }}\n")
    
def writeLdSymbol( f, name, addr ):
    f.write( f"{name} = {addr};\n" )
    
def writeLdDefSym( f, name, val ):
    f.write( f"-Wl,--defsym,{name}={val} " )
    
class LinkerSectionInfo:
    def __init__( self, name, addr ):
        self.addr = addr
        self.name = name
    
class HookInfo:
    def __init__( self, name: str, addr: int, replacedInstr: list, emitTrampoline: bool, toc: int ):
        self.name = name
        self.addr = addr
        self.replacedInstr = replacedInstr
        self.emitTrampoline = emitTrampoline
        self.toc = toc
        
        # procedural data
        self.index = None
        self.ptrOffset = None
        
def hexInt( s ):
    return int( s, 0 )

def isNoneOrEmptyStr( s ):
    if s == None: return True
    if isinstance( s, str ) and s == "": return True
    return False

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument( '--tools_dir', type=str, required=True, help='path to ckit tools directory' )
    parser.add_argument( '--elf_out_dir', type=str, required=True, help='output directory for generated ELF files' )
    parser.add_argument( '--prx_out_dir', type=str, required=True, help='output directory for generated PRX files' )
    parser.add_argument( '--loader_inject_addr', type=hexInt, required=True, help='address at which the loader jumped to')
    parser.add_argument( '--loader_text_range', type=hexInt, required=True, nargs="+", help='address at which the loader code is placed')
    parser.add_argument( '--sys_prx_load_module_addr', type=hexInt, required=True, help='address of sys_prx_load_module')
    parser.add_argument( '--sys_prx_start_module_addr', type=hexInt, required=True, help='address of sys_prx_start_module')
    parser.add_argument( '--sce_np_drm_is_available2_addr', type=hexInt, required=True, help='address of sce_np_drm_is_available2_addr')
    parser.add_argument( '--toc', type=hexInt, required=True, help='default TOC (r2) address of the functions to patch' )
    parser.add_argument( '--hook_shared_text_range', type=hexInt, required=True, nargs="+", help='start/end address(es) at which shared code for hooking is stored when injected' )
    parser.add_argument( '--hook_shared_data_range', type=hexInt, required=True, nargs="+", help='start/end address(es) at which shared data for hooking is stored when injected' )
    parser.add_argument( '--hooks', type=str, nargs='+', required=False, help='space separated list of hooks in the format of HOOKNAME,0xADDRESS,REPLACED_INSTRUCTION' )
    parser.add_argument( '--patch_file', type=str, required=True, help='rpcs3 patch file in which the compiled patch is inserted' )
    parser.add_argument( '--game', type=str, required=True, help='name of the game target')
    parser.add_argument( '--hooks_file', type=str, nargs="+", required=False, help='path(s) to yaml hooks file(s)')
    args = parser.parse_args()
    
    # generate loader makefile
    with open( os.path.join( args.elf_out_dir, "shk_elf_loader.gen.mk" ), "w" ) as f:
        src = fillTemplate( MK_LOADER_TEMPLATE, 
                     LOADER_INJECT_ADDR=hex( args.loader_inject_addr ),
                     LOADER_ADDR=hex( args.loader_text_range[0] ))
        f.write( src + "\n" )
        
    # generate loader linker script
    loaderSections = []
    loaderSections.append( LinkerSectionInfo( ".text.inject", args.loader_inject_addr ) )
    loaderSections.append( LinkerSectionInfo( ".text", args.loader_text_range[0] ) )
    # sort sections by address so the linker doesn't mess up the addresses 
    loaderSections = sorted( loaderSections, key=lambda s: s.addr )
    
    with open( os.path.join( args.elf_out_dir, "shk_elf_loader.gen.ld" ), "w" ) as f:
        writeLdComment( f, f"generated by {getScriptName()}" )
        writeLdSymbol( f, ".sys_prx_load_module", hex( args.sys_prx_load_module_addr ) )
        writeLdSymbol( f, ".sys_prx_start_module", hex( args.sys_prx_start_module_addr ) )
        writeLdSymbol( f, ".sceNpDrmIsAvailable2", hex( args.sce_np_drm_is_available2_addr ) )
        writeLdSectionsBegin( f )
        for section in loaderSections:
            writeLdSection( f, section.name, section.addr )
        writeLdSectionsEnd( f )
    
    sections = []
    sections.append( LinkerSectionInfo( ".text.shk_elf_shared", args.hook_shared_text_range[0] ) )
    sections.append( LinkerSectionInfo( ".data.shk_elf_shared", args.hook_shared_data_range[0] ) )
    
    hooks = []
    if args.hooks != None and len( args.hooks ) > 0:
        for hookStr in args.hooks:
            toks = hookStr.split( "/" )
            name = toks[0]
            addr = int( toks[1], 0 )
            replacedInstr = [toks[2]]
            emitTrampoline = True
            
            if len( replacedInstr ) == 0 or replacedInstr[0] == "":
                emitTrampoline = False            
            
            hook = HookInfo( name, addr, replacedInstr, emitTrampoline, args.toc ) 
            hooks.append( hook )
            # we add a section for every hook patch
            sections.append( LinkerSectionInfo( f".text.shk_elf_patch_{hook.name}", hook.addr ))
        
    if args.hooks_file != None and len( args.hooks_file ) > 0:
        for hooksFile in args.hooks_file:
            with open( hooksFile ) as f:
                yamlDict = yaml.load( f, Loader=yaml.SafeLoader )
                for key, value in yamlDict.items():
                    if "addr" not in value or isNoneOrEmptyStr( value["addr"] ):
                        raise Exception( f"Hook {key}: Missing address" )
                    
                    addr = value["addr"]
                    replacedInstr = None
                    if "replacedInstr" in value and not isNoneOrEmptyStr( value["replacedInstr"] ):
                        replacedInstr = value["replacedInstr"]
                    
                    emitTrampoline = True
                    if "emitTrampoline" in value and not value["emitTrampoline"]:
                        emitTrampoline = False
                    
                    # if no replaced instr, then emitTrampoline must be false
                    if replacedInstr == None and emitTrampoline:
                        raise Exception( f"Hook {key}: Replaced instruction must be specified when emitTrampoline: on" ) 
                    
                    toc = args.toc
                    if "toc" in value and not isNoneOrEmptyStr( value["toc"] ):
                        toc = value["toc"]
                    
                    hook = HookInfo( key, addr, [replacedInstr], emitTrampoline, toc )
                    hooks.append( hook )
                    # we add a section for every hook patch
                    sections.append( LinkerSectionInfo( f".text.shk_elf_patch_{hook.name}", hook.addr ))
                
       
    # sort sections by address so the linker doesn't mess up the addresses 
    sections = sorted( sections, key=lambda s: s.addr )
    
    # precalculate values for hooks
    for i in range( len( hooks ) ):
        hook = hooks[i]
        hook.index = i
        hook.ptrOffset = i * 4
    
    # generate asm to inject into host
    with open( os.path.join( args.elf_out_dir, "shk_elf.gen.s" ), "w" ) as f:
        f.write( f'# generated by {getScriptName()}\n' )
        
        writeAsmInjectHookSharedText( f )
        writeAsmInjectHookSharedData( f )
        for hook in hooks:
            writeAsmInjectHookPatch( f, hook )
            writeAsmInjectHookThunk( f, hook )
            
    # generate inject linker script
    with open( os.path.join( args.elf_out_dir, "shk_elf.gen.ld" ), "w" ) as f:
        f.write( f'/* generated by {getScriptName()}*/\n' )
        
        f.write( "SECTIONS {\n" )
        
        # write sections
        for section in sections:
            writeLdSection( f, section.name, section.addr )
        
        f.write( "}\n" )
        
    # generate inject makefile
    with open( os.path.join( args.elf_out_dir, "shk_elf.gen.mk" ), "w" ) as f:
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
            IN_DIR=args.elf_out_dir,
            OUT_DIR=args.elf_out_dir,
            TMP_DIR=args.elf_out_dir,               
            HOOK_OUTPUTS=binaryHooksSrc, 
            HOOK_INPUTS=patchHookInputsSrc, 
            HOOK_ADDRESSES=patchHookAddressesSrc, 
            SHARED_TEXT_ADDRESS=hex( args.hook_shared_text_range[0] ), 
            SHARED_DATA_ADDRESS=hex( args.hook_shared_data_range[0] ),
            PATCH_FILE=args.patch_file,
            GAME=args.game )
        
        f.write( src + "\n" )
            
    # generate asm to include with the module
    with open( os.path.join( args.prx_out_dir, "shk_prx.gen.s" ), "w" ) as f:
        f.write( f'# generated by {getScriptName()}\n' )
        
        # write hook handler ptr table
        entriesSrc = ""
        for hook in hooks:
            entriesSrc += fillTemplate( ASM_PRX_HOOK_PTR_TABLE_ENTRY_TEMPLATE, HOOK=hook.name )
            
        f.write( fillTemplate( ASM_PRX_HOOK_PTR_TABLE_TEMPLATE, ENTRIES=entriesSrc ) + "\n" )
        
        # write hook trampolines
        for hook in hooks:
            writeAsmPrxHookTrampoline( f, hook )
                
    # define linker symbols to be included with the module
    with open( os.path.join( args.prx_out_dir, "shk_prx.gen.mk" ), "w" ) as f:
        f.write( f'# generated by {getScriptName()}\n' )
        
        # write defsyms
        f.write( "SHK_PRX_DEFSYMS = " )
    
        # todo verify that these addresses line up
        curDataAddr = args.hook_shared_data_range[0]
        
        # write symbol for eboot->prx pointer for hook handler function pointer table
        writeLdDefSym( f, f"_shk_elf_prx_ptr_table", hex( curDataAddr ) )
        
        # write symbol for main ELF TOC
        writeLdDefSym( f, f"_shk_elf_toc", hex( args.toc ) )
        
        for hook in hooks:
            # write symbol for hook address (likely the function itself)
            writeLdDefSym( f, f"._shk_elf_{hook.name}", hex( hook.addr ) )
            
        f.write( "\n" )
        # write defines
        f.write( f"SHK_PRX_DEFINES = -DGAME_{args.game}" )
        
        
    pass

if __name__ == '__main__':
    main()