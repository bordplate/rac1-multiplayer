EXE_SUFFIX = .exe
PPU_LIB_DIR = $(PS3_SDK)\target\ppu\lib
CC = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-gcc$(EXE_SUFFIX)
MAKE_FSELF = $(PS3_SDK)\host-win32\bin\make_fself$(EXE_SUFFIX)
LIBS = 

sprx:
	$(MAKE) -C ../prx sprx
	copy ..\prx\mod.sprx mod.sprx

elf: sprx
	$(CC) main.c -o main.elf

fself: elf
	$(MAKE_FSELF) main.elf main.self