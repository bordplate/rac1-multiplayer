# recursive wildcard function
# courtesy of https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
TMP_DIR = $(BUILD_DIR)/tmp

-include ../userconfig.mk
-include ../os.mk
include $(TMP_DIR)/shk_prx.gen.mk


GAME = RC1
EXE_SUFFIX = .exe
PPU_LIB_DIR = $(PS3_SDK)/target/ppu/lib
CC = $(WINE) $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-g++$(EXE_SUFFIX)
MAKE_FSELF = $(WINE) $(PS3_SDK)/host-win32/bin/make_fself$(EXE_SUFFIX)
CFILES = $(call rwildcard,.,*.c)
CPPFILES = $(call rwildcard,.,*.cpp)
SFILES = $(TMP_DIR)/shk_prx.gen.s runtime/init_$(GAME).s
DEFINES = -DGAME_RC1
PRX_LDFLAGS_EXTRA =
PPU_PRX_LDFLAGS  += \
	$(PRX_LDFLAGS_EXTRA) \
	-Wl,--start-group \
	-lsyscall \
	-llv2_stub \
	-lc_stub \
	-lfs_stub \
	-lstdc++ \
	-lm_stub \
	-lnet_stub \
	-lsysutil_stub \
	-Map=$(BIN_DIR)/mod.map \
	-Wl,--end-group

CCFLAGS = $(SHK_PRX_DEFSYMS) $(SHK_PRX_DEFINES) rc1/globals.ld $(SFILES) $(CPPFILES) $(CFILES) $(DEFINES) -I$(PS3_SDK)/target/ppu/include -I./ -fno-exceptions -fno-rtti -Wa,-mregnames $(PPU_PRX_LDFLAGS)
SCETOOL = $(WINE) $(PS3_SDK)/Tools/alt/scetool.exe

elf:
	$(CC) $(CCFLAGS) -nostartfiles -Wl,-relocatable -o $(BIN_DIR)/mod.elf

prx:
	#$(CC) $(CCFLAGS) -nostartfiles -Wl,-relocatable -o $(BIN_DIR)/mod.elf
	$(CC) $(CCFLAGS) -s -o $(BIN_DIR)/mod.prx -mprx-with-runtime -zgenprx -zlevel=all
	$(WINE) $(PS3_SDK)/host-win32/bin/ppu-lv2-prx-fixup.exe --stub-fix-only $(BIN_DIR)/mod.prx

preprocessed:
	$(CC) $(CCFLAGS) -E -o $(BIN_DIR)/mod.i

sprx: prx
	#$(MAKE_FSELF) $(BIN_DIR)/mod.prx $(BIN_DIR)/mod.sprx
	cd $(PS3_SDK)/Tools/alt && \
		$(SCETOOL) -0 SELF -1 FALSE -s TRUE -2 0007 -3 1010000001000003 -4 01000002 -j FALSE -5 APP -A 0001000000000000 -6 0003005000000000 -c SPRX -9 00000000000000000000000000000000000000000000003B0000000100002000 -g mod.sprx -e $(CURDIR)/$(BIN_DIR)/mod.prx $(CURDIR)/$(BIN_DIR)/mod.sprx
#		$(SCETOOL) \
#        		--sce-type=SELF \
#        		--compress-data=TRUE \
#        		--skip-sections=TRUE \
#        		--self-ctrl-flags=4000000000000000000000000000000000000000000000000000000000000002 \
#        		--key-revision=04 \
#        		--self-auth-id=1010000001000003 \
#        		--self-app-version=0001000000000000 \
#        		--self-vendor-id=01000002 \
#        		--self-type=APP \
#        		--self-fw-version=0003004000000000 \
#        		--encrypt $(CURDIR)/$(BIN_DIR)/mod.prx $(CURDIR)/$(BIN_DIR)/mod.sprx

clean:
	$(DELETE) $(DELETEFLAGS) $(BIN_DIR)$(PATH_SEP)*.*
	$(DELETE) $(DELETEFLAGS) $(TMP_DIR)$(PATH_SEP)*.*
