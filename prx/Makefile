# recursive wildcard function
# courtesy of https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)\bin
TMP_DIR = $(BUILD_DIR)\tmp

include $(TMP_DIR)\shk_prx.gen.mk

GAME = p5
EXE_SUFFIX = .exe
PPU_LIB_DIR = $(PS3_SDK)\target\ppu\lib
CC = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-gcc$(EXE_SUFFIX)
MAKE_FSELF = $(PS3_SDK)\host-win32\bin\make_fself$(EXE_SUFFIX)
CFILES = $(call rwildcard,.,*.c)
SFILES = $(TMP_DIR)/shk_prx.gen.s runtime/init_$(GAME).s modules/test/testmodule.s
DEFINES = -DGAME_TEST
CCFLAGS = $(SHK_PRX_DEFSYMS) $(SHK_PRX_DEFINES) $(SFILES) $(CFILES) $(DEFINES) -I./ -Wa,-mregnames -g -fno-builtin -Xlinker -Map=$(BIN_DIR)\mod.map

elf:
	$(CC) $(CCFLAGS) -nostartfiles -o $(BIN_DIR)\mod.elf

prx:
	$(CC) -mprx $(CCFLAGS) -o $(BIN_DIR)\mod.prx

preprocessed:
	$(CC) $(CCFLAGS) -E -o $(BIN_DIR)\mod.i

sprx: prx
	$(MAKE_FSELF) $(BIN_DIR)\mod.prx $(BIN_DIR)\mod.sprx

clean:
	del $(BIN_DIR)\*.* /s /q
	del $(TMP_DIR)\*.* /s /q