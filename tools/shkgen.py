import os
import argparse
import sys
import tomllib


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


ASM_INJECT_PATCH_TEMPLATE = \
'''
.section .text.shk_game_patch_{PATCH}, "x"
.global ._shk_game_patch_{PATCH}
._shk_game_patch_{PATCH}:
b ._shk_game_patch_thunk_{PATCH}

.section .text.shk_elf_shared, "x"
.global ._shk_game_patch_{PATCH}
._shk_game_patch_thunk_{PATCH}:
{INSTRUCTIONS}
b ._shk_game_patch_{PATCH}+(1*4)
'''


ASM_INJECT_PATCH_IN_PLACE_TEMPLATE = \
'''
.section .text.shk_game_patch_{PATCH}, "x"
.global ._shk_game_patch_{PATCH}
._shk_game_patch_{PATCH}:
{INSTRUCTIONS}
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
GENERATEPATCHFILE = $(TOOLS_DIR)/generate_patch.py

compile:
	$(WINE) "$(CC)" "$(IN_DIR)/shk_elf.gen.s" -o "$(TMP_DIR)/shk_elf.o" -T "$(IN_DIR)/shk_elf.gen.ld" -v -Xlinker -Map="$(TMP_DIR)/shk_elf.map" -Wa,-mregnames -nostartfiles -nodefaultlibs

binary: compile{HOOK_OUTPUTS}
	$(WINE) "$(OBJCOPY)" -O binary --only-section=.text.shk_elf_shared "$(TMP_DIR)/shk_elf.o" "$(TMP_DIR)/.text.shk_elf_shared.bin" -v
	$(WINE) "$(OBJCOPY)" -O binary --only-section=.data.shk_elf_shared "$(TMP_DIR)/shk_elf.o" "$(TMP_DIR)/.data.shk_elf_shared.bin" -v

patch: binary
	$(PYTHON) "$(GENERATEPATCHFILE)" --input{HOOK_INPUTS} "$(TMP_DIR)/.text.shk_elf_shared.bin" "$(TMP_DIR)/.data.shk_elf_shared.bin" --address{HOOK_ADDRESSES} $(SHARED_TEXT_ADDRESS) $(SHARED_DATA_ADDRESS) --output "$(TMP_DIR)/patch.txt" --append
'''


MK_INJECT_HOOK_OUTPUT_TEMPLATE = \
'''
	$(WINE) "$(OBJCOPY)" -O binary --only-section=.text.{PREFIX}_{HOOK} "$(TMP_DIR)/shk_elf.o" "$(TMP_DIR)/.text.{PREFIX}_{HOOK}.bin" -v
'''


MK_INJECT_HOOK_INPUT_TEMPLATE = \
'''
"$(TMP_DIR)/.text.{PREFIX}_{HOOK}.bin"
'''


MK_LOADER_TEMPLATE = \
'''
SHK_ELF_LOADER_INJECT_ADDRESS = {LOADER_INJECT_ADDR}
SHK_ELF_LOADER_ADDRESS = {LOADER_ADDR}
SHK_ELF_LD = build/shk_elf_loader.gen.ld
'''


ASM_HOOK_PTR_SIZE = 4


class LinkerSectionInfo:
    def __init__(self, name, addr):
        self.addr = addr
        self.name = name


class HookInfo:
    def __init__(self, name: str, addr: int, replaced_instr: list, emit_trampoline: bool, toc: int):
        self.name = name
        self.addr = addr
        self.replaced_instr = replaced_instr
        self.emit_trampoline = emit_trampoline
        self.toc = toc

        # procedural data
        self.index = None
        self.ptr_offset = None


class PatchInfo:
    def __init__(self, addr: int, data: int = None, instructions: list = None, in_place: bool = True):
        self.addr: int = addr
        self.data: int = data
        self.instructions: list[str] = instructions
        self.in_place: bool = in_place


def get_script_name():
    return os.path.basename(__file__)


def set_template_var(template: str, name: str, val: str):
    if name not in template:
        raise Exception(f"Template variable {name} does not exist in template: {template}")
    
    return template.replace(name, val)


def fill_template(template, **kwargs):
    t = template
    for key, val in kwargs.items():
        t = set_template_var(t, f"{{{key}}}", val)
    return t


def write_asm_inject_hook_shared_data(f):
    f.write(fill_template(ASM_INJECT_HOOK_PTR_TABLE_TEMPLATE) + "\n")


def write_asm_prx_hook_trampoline(f, hook):
    if hook.emit_trampoline:
        t = fill_template(ASM_PRX_HOOK_TRAMPOLINE_TEMPLATE, HOOK=hook.name, TOC=hex(hook.toc))
        for i, instr in enumerate(hook.replaced_instr):
            t = set_template_var(t, f"{{INSTR{i}}}", instr)
        f.write(t + "\n")
    else:
        t = fill_template(ASM_PRX_HOOK_TRAMPOLINE_EMPTY_TEMPLATE, HOOK=hook.name)
        f.write(t + "\n")


def write_asm_inject_hook_shared_text(f):
    f.write(ASM_INJECT_HOOK_SHARED_TEMPLATE + "\n")


def write_asm_inject_hook_patch(f, hook):
    f.write(fill_template(ASM_INJECT_HOOK_PATCH_TEMPLATE, HOOK=hook.name) + "\n")


def write_asm_inject_hook_thunk(f, hook):
    f.write(fill_template(ASM_INJECT_HOOK_THUNK_TEMPLATE, HOOK=hook.name, HOOK_PTR_OFFSET=hex(hook.ptr_offset)) + "\n")


def write_asm_patch_in_place(f, name: str, instructions: list[str]):
    f.write(fill_template(ASM_INJECT_PATCH_IN_PLACE_TEMPLATE, PATCH=name, INSTRUCTIONS="\n".join(instructions)))


def write_asm_patch(f, name: str, instructions: list[str]):
    f.write(fill_template(ASM_INJECT_PATCH_TEMPLATE,
                          PATCH=name,
                          INSTRUCTIONS="\n".join(instructions)
           ))


def write_asm_macro_set(f, name, val):
    f.write(f".set {name}, {val}")
    
def write_ld_comment(f, val):
    f.write(f"/* {val} */\n")


def write_ld_sections_begin(f):
    f.write("SECTIONS {\n")


def write_ld_sections_end(f):
    f.write("}\n")


def write_ld_section(f, name, addr):
    f.write(f"    . = {hex(addr)};\n")
    f.write(f"    {name} : {{ *({name}) }}\n")


def write_ld_symbol(f, name, addr):
    f.write(f"{name} = {addr};\n")


def write_ld_def_sym(f, name, val):
    f.write(f"-Wl,--defsym,{name}={val} ")


def hex_int(s):
    return int(s, 0)


def is_none_or_empty_str(s):
    if s == None: return True
    if isinstance(s, str) and s == "": return True
    return False


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--tools_dir', type=str, required=True, help='path to ckit tools directory')
    parser.add_argument('--elf_out_dir', type=str, required=True, help='output directory for generated ELF files')
    parser.add_argument('--prx_out_dir', type=str, required=True, help='output directory for generated PRX files')
    parser.add_argument('--loader_inject_addr', type=hex_int, required=True, help='address at which the loader jumped to')
    parser.add_argument('--loader_text_range', type=hex_int, required=True, nargs="+", help='address at which the loader code is placed')
    parser.add_argument('--sys_prx_load_module_addr', type=hex_int, required=True, help='address of sys_prx_load_module')
    parser.add_argument('--sys_prx_start_module_addr', type=hex_int, required=True, help='address of sys_prx_start_module')
    parser.add_argument('--sce_np_drm_is_available2_addr', type=hex_int, required=True, help='address of sce_np_drm_is_available2_addr')
    parser.add_argument('--toc', type=hex_int, required=True, help='default TOC (r2) address of the functions to patch')
    parser.add_argument('--hook_shared_text_range', type=hex_int, required=True, nargs="+", help='start/end address(es) at which shared code for hooking is stored when injected')
    parser.add_argument('--hook_shared_data_range', type=hex_int, required=True, nargs="+", help='start/end address(es) at which shared data for hooking is stored when injected')
    parser.add_argument('--hooks', type=str, nargs='+', required=False, help='space separated list of hooks in the format of HOOKNAME,0xADDRESS,REPLACED_INSTRUCTION')
    parser.add_argument('--game', type=str, required=True, help='name of the game target')
    parser.add_argument('--hooks_file', type=str, nargs="+", required=False, help='path(s) to toml hooks file(s)')
    parser.add_argument('--patch_file', type=str, nargs="+", required=False, help='path(s) to toml patch file(s)')
    args = parser.parse_args()
    
    # generate loader makefile
    with open(os.path.join(args.elf_out_dir, "shk_elf_loader.gen.mk"), "w") as f:
        src = fill_template(MK_LOADER_TEMPLATE,
                            LOADER_INJECT_ADDR=hex(args.loader_inject_addr),
                            LOADER_ADDR=hex(args.loader_text_range[0]))
        f.write(src + "\n")
        
    # generate loader linker script
    loader_sections = [
        LinkerSectionInfo(".text.inject", args.loader_inject_addr),
        LinkerSectionInfo(".text", args.loader_text_range[0])
    ]

    # sort sections by address so the linker doesn't mess up the addresses
    loader_sections = sorted(loader_sections, key=lambda s: s.addr)
    
    with open(os.path.join(args.elf_out_dir, "shk_elf_loader.gen.ld"), "w") as f:
        write_ld_comment(f, f"generated by {get_script_name()}")
        write_ld_symbol(f, ".sys_prx_load_module", hex(args.sys_prx_load_module_addr))
        write_ld_symbol(f, ".sys_prx_start_module", hex(args.sys_prx_start_module_addr))
        write_ld_symbol(f, ".sceNpDrmIsAvailable2", hex(args.sce_np_drm_is_available2_addr))
        write_ld_sections_begin(f)
        for section in loader_sections:
            write_ld_section(f, section.name, section.addr)
        write_ld_sections_end(f)
    
    sections = [
        LinkerSectionInfo(".text.shk_elf_shared", args.hook_shared_text_range[0]),
        LinkerSectionInfo(".data.shk_elf_shared", args.hook_shared_data_range[0])
    ]

    hooks = []
    if args.hooks is not None and len(args.hooks) > 0:
        for hook_str in args.hooks:
            toks = hook_str.split("/")
            name = toks[0]
            addr = int(toks[1], 0)
            replaced_instr = [toks[2]]
            emit_trampoline = True
            
            if len(replaced_instr) == 0 or replaced_instr[0] == "":
                emit_trampoline = False
            
            hook = HookInfo(name, addr, replaced_instr, emit_trampoline, args.toc)
            hooks.append(hook)
            # we add a section for every hook patch
            sections.append(LinkerSectionInfo(f".text.shk_elf_patch_{hook.name}", hook.addr))

    if args.hooks_file is not None and len(args.hooks_file) > 0:
        for hooks_file in args.hooks_file:
            with open(hooks_file, "rb") as f:
                toml_dict = tomllib.load(f)
                for key, value in toml_dict.items():
                    if "addr" not in value or is_none_or_empty_str(value["addr"]):
                        raise Exception(f"Hook {key}: Missing address")
                    
                    addr = value["addr"]
                    replaced_instr = None
                    if "replaced_instr" in value and not is_none_or_empty_str(value["replaced_instr"]):
                        replaced_instr = value["replaced_instr"]
                    
                    emit_trampoline = True
                    if "emit_trampoline" in value and not value["emit_trampoline"]:
                        emit_trampoline = False
                    
                    # if no replaced instr, then emit_trampoline must be false
                    if replaced_instr is None and emit_trampoline:
                        raise Exception(f"Hook {key}: Replaced instruction must be specified when emit_trampoline: on")
                    
                    toc = args.toc
                    if "toc" in value and not is_none_or_empty_str(value["toc"]):
                        toc = value["toc"]
                    
                    hook = HookInfo(key, addr, [replaced_instr], emit_trampoline, toc)
                    hooks.append(hook)
                    # we add a section for every hook patch
                    sections.append(LinkerSectionInfo(f".text.shk_elf_patch_{hook.name}", hook.addr))

    patches: list[PatchInfo] = []

    if args.patch_file is not None and len(args.patch_file) > 0:
        for patch_file in args.patch_file:
            with open(patch_file, "rb") as f:
                toml_dict = tomllib.load(f)
                for key, value in toml_dict.items():
                    try:
                        addr = int(key, base=0)  # Infer base
                    except ValueError:
                        print(f"{patch_file}: {key} could not be parsed as an address.")
                        exit(-1)

                    patch = None

                    if type(value) is str:
                        patch = PatchInfo(addr, instructions=[value])

                    if type(value) is int:
                        patch = PatchInfo(addr, data=value)

                    if type(value) is list:
                        patch = PatchInfo(addr, instructions=value)

                    if type(value) is dict:
                        if "instructions" not in value:
                            print(f"{patch_file}:{key} Missing instructions")
                            exit(-1)

                        instructions = value["instructions"]

                        if type(instructions) is str:
                            instructions = [value["instructions"]]

                        if type(instructions) is not list:
                            print(f"{patch_file}:{key} Instructions must be a list or a str, not a {type(instructions)}")
                            exit(-1)

                        in_place = value.get("in_place", True) == True

                        patch = PatchInfo(addr, instructions=instructions, in_place=in_place)

                    if patch is None:
                        print(f"{patch_file}: {key} is not a valid patch.")
                        print(f"\tValue: {value}. Type {type(value)}")

                        exit(-1)

                    patches.append(patch)
                    sections.append(LinkerSectionInfo(f".text.shk_game_patch_0x{patch.addr:X}", patch.addr))

    # sort sections by address so the linker doesn't mess up the addresses 
    sections = sorted(sections, key=lambda s: s.addr)
    
    # precalculate values for hooks
    for i in range(len(hooks)):
        hook = hooks[i]
        hook.index = i
        hook.ptr_offset = i * 4
    
    # generate asm to inject into host
    with open(os.path.join(args.elf_out_dir, "shk_elf.gen.s"), "w") as f:
        f.write(f'# generated by {get_script_name()}\n')
        
        write_asm_inject_hook_shared_text(f)
        write_asm_inject_hook_shared_data(f)
        for hook in hooks:
            write_asm_inject_hook_patch(f, hook)
            write_asm_inject_hook_thunk(f, hook)

        for patch in patches:
            if patch.instructions is not None and patch.in_place:
                write_asm_patch_in_place(f, f"0x{patch.addr:X}", patch.instructions)
            elif patch.instructions is not None:
                write_asm_patch(f, f"0x{patch.addr:X}", patch.instructions)
            elif patch.data is not None:
                write_asm_patch_in_place(f, f"0x{patch.addr:X}", [f".int {patch.data}"])
            else:
                print(f"Failed to apply patch 0x{patch.addr:X}")
                exit(-1)
            
    # generate inject linker script
    with open(os.path.join(args.elf_out_dir, "shk_elf.gen.ld"), "w") as f:
        f.write(f'/* generated by {get_script_name()}*/\n')
        
        f.write("SECTIONS {\n")
        
        # write sections
        for section in sections:
            write_ld_section(f, section.name, section.addr)
        
        f.write("}\n")
        
    # generate inject makefile
    with open(os.path.join(args.elf_out_dir, "shk_elf.gen.mk"), "w") as f:
        f.write(f'# generated by {get_script_name()}\n')
        
        binary_hooks_src = ""
        for hook in hooks:
            binary_hooks_src += "\n" + fill_template(MK_INJECT_HOOK_OUTPUT_TEMPLATE.strip("\n"), PREFIX="shk_elf_patch", HOOK=hook.name)

        for patch in patches:
            binary_hooks_src += "\n" + fill_template(MK_INJECT_HOOK_OUTPUT_TEMPLATE.strip("\n"), PREFIX="shk_game_patch", HOOK=f"0x{patch.addr:X}")
            
        patch_hook_inputs_src = ""
        for hook in hooks:
            patch_hook_inputs_src += " " + fill_template(MK_INJECT_HOOK_INPUT_TEMPLATE.strip("\n"), PREFIX="shk_elf_patch", HOOK=hook.name)

        for patch in patches:
            patch_hook_inputs_src += " " + fill_template(MK_INJECT_HOOK_INPUT_TEMPLATE.strip("\n"), PREFIX="shk_game_patch", HOOK=f"0x{patch.addr:X}")
        
        patch_hook_addresses_src = ""
        for hook in hooks:
            patch_hook_addresses_src += " " + hex(hook.addr)

        for patch in patches:
            patch_hook_addresses_src += " " + hex(patch.addr)
        
        src = fill_template(MK_INJECT_TEMPLATE,
                            TOOLS_DIR=args.tools_dir,
                            IN_DIR=args.elf_out_dir,
                            OUT_DIR=args.elf_out_dir,
                            TMP_DIR=args.elf_out_dir,
                            HOOK_OUTPUTS=binary_hooks_src,
                            HOOK_INPUTS=patch_hook_inputs_src,
                            HOOK_ADDRESSES=patch_hook_addresses_src,
                            SHARED_TEXT_ADDRESS=hex(args.hook_shared_text_range[0]),
                            SHARED_DATA_ADDRESS=hex(args.hook_shared_data_range[0]))
        
        f.write(src + "\n")
            
    # generate asm to include with the module
    with open(os.path.join(args.prx_out_dir, "shk_prx.gen.s"), "w") as f:
        f.write(f'# generated by {get_script_name()}\n')
        
        # write hook handler ptr table
        entries_src = ""
        for hook in hooks:
            entries_src += fill_template(ASM_PRX_HOOK_PTR_TABLE_ENTRY_TEMPLATE, HOOK=hook.name)
            
        f.write(fill_template(ASM_PRX_HOOK_PTR_TABLE_TEMPLATE, ENTRIES=entries_src) + "\n")
        
        # write hook trampolines
        for hook in hooks:
            write_asm_prx_hook_trampoline(f, hook)
                
    # define linker symbols to be included with the module
    with open(os.path.join(args.prx_out_dir, "shk_prx.gen.mk"), "w") as f:
        f.write(f'# generated by {get_script_name()}\n')
        
        # write defsyms
        f.write("SHK_PRX_DEFSYMS = ")
    
        # todo verify that these addresses line up
        cur_data_addr = args.hook_shared_data_range[0]
        
        # write symbol for eboot->prx pointer for hook handler function pointer table
        write_ld_def_sym(f, f"_shk_elf_prx_ptr_table", hex(cur_data_addr))
        
        # write symbol for main ELF TOC
        write_ld_def_sym(f, f"_shk_elf_toc", hex(args.toc))
        
        for hook in hooks:
            # write symbol for hook address (likely the function itself)
            write_ld_def_sym(f, f"._shk_elf_{hook.name}", hex(hook.addr))
            
        f.write("\n")
        # write defines
        f.write(f"SHK_PRX_DEFINES = -DGAME_{args.game}")


if __name__ == '__main__':
    main()