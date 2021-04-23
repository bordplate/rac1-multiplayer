include userconfig.mk

# dirs
BASE_DIR = $(CURDIR)
TOOLS_DIR = $(BASE_DIR)\tools

LOADER_DIR = $(BASE_DIR)\loader
LOADER_BUILD_DIR = $(LOADER_DIR)\build

PRX_DIR = $(BASE_DIR)\prx
PRX_BUILD_DIR = $(PRX_DIR)\build
PRX_OUT_DIR = $(PRX_BUILD_DIR)\tmp
PRX_BUILD_IN_DIR = $(PRX_BUILD_DIR)\tmp
PRX_BUILD_TMP_DIR = $(PRX_BUILD_DIR)\tmp
PRX_BUILD_OUT_DIR = $(PRX_BUILD_DIR)\bin
BIN2RPCS3PATCH = $(TOOLS_DIR)\bin2rpcs3patch.py

ifeq ($(GAME), P5)
# NOTE: might be different between versions...?
TOC									= 0xD01288
LOADER_INJECT_ADDR 					= 0x10250
LOADER_START_ADDR 					= 0xA3BE70 # .sub_A3BAD0 + 4
LOADER_END_ADDR 					= 0xA3BEF0 # + 0x80s
LOADER_SYS_PRX_MODULE_LOAD_ADDR 	= 0xB45E5C
LOADER_SYS_PRX_MODULE_START_ADDR 	= 0XB45D3C

# 745 instructions, around ~200 hooks
HOOK_SHARED_TEXT_BEGIN_ADDR = 0xA3BEF0 # LOADER_START_ADDR + 0x80
HOOK_SHARED_TEXT_END_ADDR 	= 0xA3CA94 

# 0xA8 / 4 = 42 words
HOOK_SHARED_DATA_BEGIN_ADDR = 0xCE2CB8
HOOK_SHARED_DATA_END_ADDR 	= 0xCE2D60

# ASM function ported to PRX to make room for the loader
HOOK_SUBSTITUTE = _shk_prx_elf_substitute/0xA3BE6C/""
else
	$(error Game target '$(GAME)' is not supported or not set)
endif

HOOKS := $(HOOK_SUBSTITUTE) $(HOOKS)

BIN2RPCS3PATCHARGS = \
	--input "$(LOADER_BUILD_DIR)\loader.text.inject.bin" "$(LOADER_BUILD_DIR)\loader.text.bin" --address $(LOADER_INJECT_ADDR) $(LOADER_START_ADDR) \
	--output "$(PATCH_FILE)" --indent 3 --replace_patch shk_elf_loader

SHKGENARGS = \
	--tools_dir "$(TOOLS_DIR)" --elf_out_dir "$(LOADER_BUILD_DIR)" --prx_out_dir "$(PRX_OUT_DIR)" \
	--toc $(TOC) --hook_shared_text_range $(HOOK_SHARED_TEXT_BEGIN_ADDR) $(HOOK_SHARED_TEXT_END_ADDR) \
	--hook_shared_data_range $(HOOK_SHARED_DATA_BEGIN_ADDR) $(HOOK_SHARED_DATA_END_ADDR) --patch_file "$(PATCH_FILE)" --hooks $(HOOKS) \
	--game $(GAME) --loader_inject_addr $(LOADER_INJECT_ADDR) --loader_text_range $(LOADER_START_ADDR) $(LOADER_END_ADDR) \
	--sys_prx_load_module_addr $(LOADER_SYS_PRX_MODULE_LOAD_ADDR) --sys_prx_start_module_addr $(LOADER_SYS_PRX_MODULE_START_ADDR)

all:
# generate build files
	cd tools && $(PYTHON) shkgen.py $(SHKGENARGS)

# build loader
	cd "$(LOADER_DIR)" && "$(MAKE)" binary LOADER_INJECT_ADDR=$(LOADER_INJECT_ADDR) LOADER_START_ADDR=$(LOADER_START_ADDR)
	$(PYTHON) "$(BIN2RPCS3PATCH)" $(BIN2RPCS3PATCHARGS)

# build injection patch
	cd "$(LOADER_BUILD_DIR)" && "$(MAKE)" -f shk_elf.gen.mk patch

# build sprx
	cd "$(PRX_DIR)" && "$(MAKE)" sprx GAME=$(GAME)
	copy "$(PRX_BUILD_OUT_DIR)\mod.sprx" "$(GAME_DIR)" /Y

clean:
	cd "$(LOADER_DIR)" && "$(MAKE)" clean
	cd "$(PRX_DIR)" && "$(MAKE)" clean

setup:
# copy userconfig from template
	-if not exist userconfig.mk copy userconfig.template.mk userconfig.mk

# create folders used during build
	-mkdir "$(LOADER_BUILD_DIR)"
	-mkdir "$(PRX_BUILD_DIR)"
	-mkdir "$(PRX_BUILD_IN_DIR)"
	-mkdir "$(PRX_BUILD_TMP_DIR)" 
	-mkdir "$(PRX_BUILD_OUT_DIR)"


