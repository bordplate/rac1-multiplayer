include userconfig.mk

# dirs
BASE_DIR = $(CURDIR)
LOADER_DIR = $(BASE_DIR)\loader
LOADER_BUILD_DIR = $(LOADER_DIR)\build
TOOLS_DIR = $(BASE_DIR)\tools
PRX_DIR = $(BASE_DIR)\prx
PRX_BUILD_DIR = $(PRX_DIR)\build
PRX_OUT_DIR = $(PRX_BUILD_DIR)\tmp
PRX_BUILD_IN_DIR = $(PRX_BUILD_DIR)\tmp
PRX_BUILD_TMP_DIR = $(PRX_BUILD_DIR)\tmp
PRX_BUILD_OUT_DIR = $(PRX_BUILD_DIR)\bin
BIN2RPCS3PATCH = $(TOOLS_DIR)\bin2rpcs3patch.py

# addresses
#mod name			first - last instruction addr				
#mod cpk:			B44A9C-B44AB8
#file access log:	B44ABC-B44B4C
#bgmorder:			B44B50-B44B94
#bgmrandom:			B44B50-B44B94
#fixscriptprint:	B44B98-B44BF4
#60 fps:			b44e00-b44e50

# NOTE: might be different between versions...?
TOC 						= 0xD01288
# NOTE: if you change these, also change them in the loader linker script
INJECT_ADDR 				= 0x10250
LOADER_START_ADDR 			= 0xA3BE70 # .sub_A3BAD0 + 4
LOADER_END_ADDR 			= 0xA3BEF0 # + 0x80s

# 745 instructions, around ~200 hooks
HOOK_SHARED_TEXT_BEGIN_ADDR = 0xA3BEF0 # LOADER_START_ADDR + 0x80
HOOK_SHARED_TEXT_END_ADDR 	= 0xA3CA94 

# 0x1FC / (3 * 4) = 42
# TODO use this
#HOOK_SHARED_TEXT_2_BEGIN_ADDR = 0xB44E60
#HOOK_SHARED_TEXT_2_END_ADDR = 0xB4505C

# 0xA8 / 4 = 42
HOOK_SHARED_DATA_BEGIN_ADDR = 0xCE2CB8
HOOK_SHARED_DATA_END_ADDR 	= 0xCE2D60

# 0xA8 / 4 = 42
# TODO use this
#HOOK_SHARED_DATA_2_BEGIN_ADDR = 0xB46434
#HOOK_SHARED_DATA_2_END_ADDR = 0xB464DC

BIN2RPCS3PATCHARGS = \
	--input "$(LOADER_BUILD_DIR)\loader.text.inject.bin" "$(LOADER_BUILD_DIR)\loader.text.bin" --address $(INJECT_ADDR) $(LOADER_START_ADDR) \
	--output "$(PATCH_FILE)" --indent 3 --replace_patch shk_elf_loader

SHKGENARGS = \
	--tools_dir "$(TOOLS_DIR)" --out_dir "$(PRX_OUT_DIR)" --build_in_dir "$(PRX_BUILD_IN_DIR)" --build_tmp_dir "$(PRX_BUILD_TMP_DIR)" \
	--build_out_dir "$(PRX_BUILD_OUT_DIR)" --toc $(TOC) --hook_shared_text_range $(HOOK_SHARED_TEXT_BEGIN_ADDR) $(HOOK_SHARED_TEXT_END_ADDR) \
	--hook_shared_data_range $(HOOK_SHARED_DATA_BEGIN_ADDR) $(HOOK_SHARED_DATA_END_ADDR) --patch_file "$(PATCH_FILE)" --hooks $(HOOKS)

all:
# build loader
	cd "$(LOADER_DIR)" && "$(MAKE)" binary INJECT_ADDR=$(INJECT_ADDR) LOADER_START_ADDR=$(LOADER_START_ADDR)
	$(PYTHON) "$(BIN2RPCS3PATCH)" $(BIN2RPCS3PATCHARGS)

# generate shk files
	cd tools && $(PYTHON) shkgen.py $(SHKGENARGS)

# build injection patch
	cd "$(PRX_BUILD_TMP_DIR)" && "$(MAKE)" -f shk_elf.gen.mk patch

# build sprx
	cd "$(PRX_DIR)" && "$(MAKE)" sprx
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


