BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)\bin
TMP_DIR = $(BUILD_DIR)\tmp

include $(TMP_DIR)\shk_prx.gen.mk

EXE_SUFFIX = .exe
PPU_LIB_DIR = $(PS3_SDK)\target\ppu\lib
CC = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-gcc$(EXE_SUFFIX)
MAKE_FSELF = $(PS3_SDK)\host-win32\bin\make_fself$(EXE_SUFFIX)
CFILES = lib\runtime.c lib\common.c lib\external\printf\printf.c mod.c
SFILES = $(TMP_DIR)\shk_prx.gen.s lib\runtime.s mod.s
CCFLAGS = $(SHK_PRX_DEFSYMS) $(SFILES) $(CFILES) -Wa,-mregnames -g -fno-builtin -Xlinker -Map=$(BIN_DIR)\mod.map

elf:
	$(CC) $(CCFLAGS) -nostartfiles -o $(BIN_DIR)\mod.elf

prx:
	$(CC) -mprx $(CCFLAGS) -o $(BIN_DIR)\mod.prx

sprx: prx
	$(MAKE_FSELF) $(BIN_DIR)\mod.prx $(BIN_DIR)\mod.sprx

clean:
	del $(BIN_DIR)\*.* /s /q
	del $(TMP_DIR)\*.* /s /q