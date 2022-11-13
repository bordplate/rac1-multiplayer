# recursive wildcard function
# courtesy of https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)$(PATH_SEP)bin
TMP_DIR = $(BUILD_DIR)$(PATH_SEP)tmp

-include ../os.mk
include $(TMP_DIR)/shk_prx.gen.mk

GAME = RC1
EXE_SUFFIX = .exe
PPU_LIB_DIR = $(PS3_SDK)/target/ppu/lib
CC = $(WINE) $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-gcc$(EXE_SUFFIX)
MAKE_FSELF = $(WINE) $(PS3_SDK)/host-win32/bin/make_fself$(EXE_SUFFIX)
CFILES = $(call rwildcard,.,*.c)
SFILES = $(TMP_DIR)/shk_prx.gen.s runtime/init_$(GAME).s
DEFINES = -DGAME_RC1
CCFLAGS = $(SHK_PRX_DEFSYMS) $(SHK_PRX_DEFINES) $(SFILES) $(CFILES) $(DEFINES) -I$(PS3_SDK)/target/ppu/include -I./ -Wa,-mregnames -g -fno-builtin -Xlinker -Map=$(BIN_DIR)/mod.map -lnet_stub
SCETOOL = $(WINE) $(PS3_SDK)/Tools/tool/scetool.exe

elf:
	$(CC) $(CCFLAGS) -nostartfiles -relocatable -o $(BIN_DIR)/mod.elf

prx:
	$(CC) -mprx $(CCFLAGS) -zgenprx -s -o $(BIN_DIR)/mod.prx
	$(WINE) $(PS3_SDK)/host-win32/bin/ppu-lv2-prx-fixup.exe --strip-debug $(BIN_DIR)/mod.prx

preprocessed:
	$(CC) $(CCFLAGS) -E -o $(BIN_DIR)/mod.i

sprx: prx
	#$(MAKE_FSELF) $(BIN_DIR)/mod.prx $(BIN_DIR)/mod.sprx
	$(SCETOOL) -0 SELF -1 FALSE -s TRUE -2 0007 -3 1010000001000003 -4 01000002 -j FALSE -5 APP -A 0001000000000000 -6 0003005000000000 -c SPRX -9 00000000000000000000000000000000000000000000003B0000000100002000 -g mod.sprx -e $(BIN_DIR)/mod.prx $(BIN_DIR)/mod.sprx

clean:
	$(DELETE) $(DELETEFLAGS) $(BIN_DIR)$(PATH_SEP)*.*
	$(DELETE) $(DELETEFLAGS) $(TMP_DIR)$(PATH_SEP)*.*
