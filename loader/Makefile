-include ../userconfig.mk
-include ../os.mk

CC = $(WINE) $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
CXX = $(WINE) $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-g++.exe
OBJCOPY = $(WINE) $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-objcopy.exe
OBJDUMP = $(WINE) $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-objdump.exe
BIN2RPCS3PATCH = ../tools/bin2rpcs3patch.py
BUILD_DIR = build
SCETOOL = $(WINE) $(PS3_SDK)/Tools/scetool.2.9.exe

APPLYPATCH = ../tools/apply_patch.py

# include generated variables
include build/shk_elf_loader.gen.mk

asm:
	$(CXX) loader.cpp -o $(BUILD_DIR)/loader.cpp.o -T $(SHK_ELF_LD) -v -Wa,-mregnames -nostartfiles -nodefaultlibs -O3 -mno-toc -fno-exceptions
	$(OBJDUMP) -M intel -S --disassemble $(BUILD_DIR)/loader.cpp.o > $(BUILD_DIR)/loader.cpp.s

compile:
	$(CXX) loader.s -o $(BUILD_DIR)/loader.o -T $(SHK_ELF_LD) -v -Wa,-mregnames -nostartfiles -nodefaultlibs -O3 -mno-toc -fno-exceptions

binary: compile
	$(OBJCOPY) -O binary --only-section=.text.inject $(BUILD_DIR)/loader.o $(BUILD_DIR)/loader.text.inject.bin -v
	$(OBJCOPY) -O binary --only-section=.text $(BUILD_DIR)/loader.o $(BUILD_DIR)/loader.text.bin -v

patch:
	cd $(PS3_SDK)/Tools && \
	$(WINE) $(SCETOOL) -d "$(RPCS3_DIR)/dev_hdd0/game/NPEA00385/USRDIR/EBOOT.BIN" "$(CURDIR)/$(BUILD_DIR)/EBOOT.ELF"

	cd "$(CURDIR)"

	$(PYTHON) $(APPLYPATCH) --patch "$(BUILD_DIR)/patch.txt" --input "$(BUILD_DIR)/EBOOT.ELF" --output "$(BUILD_DIR)/EBOOT.patched.ELF"

	cd $(PS3_SDK)/Tools && \
	$(WINE) $(SCETOOL) \
		--sce-type=SELF \
		--compress-data=TRUE \
		--skip-sections=TRUE \
		--self-ctrl-flags=4000000000000000000000000000000000000000000000000000000000000002 \
		--key-revision=04 \
		--self-auth-id=1010000001000003 \
		--self-app-version=0001000000000000 \
		--self-vendor-id=01000002 \
		--self-type=APP \
		--self-fw-version=0003004000000000 \
		--encrypt $(CURDIR)/$(BUILD_DIR)/EBOOT.patched.ELF $(CURDIR)/$(BUILD_DIR)/EBOOT.mp.BIN


clean:
	$(DELETE) $(DELETEFLAGS) build$(PATH_SEP)*
