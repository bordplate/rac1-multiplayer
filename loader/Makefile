PYTHON = py -3
CC = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-gcc.exe
CXX = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-g++.exe
OBJCOPY = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-objcopy.exe
OBJDUMP = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-objdump.exe
BIN2RPCS3PATCH = ..\tools\bin2rpcs3patch.py
INJECT_ADDRESS = 0x10250
LOADER_ADDRESS = 0xB44AA0
BUILD_DIR = build

asm:
	$(CXX) loader.cpp -o $(BUILD_DIR)\loader.cpp.o -T loader.ld -v -Wa,-mregnames -nostartfiles -nodefaultlibs -O3 -mno-toc -fno-exceptions
	$(OBJDUMP) -M intel -S --disassemble $(BUILD_DIR)\loader.cpp.o > $(BUILD_DIR)\loader.cpp.s

compile:
	$(CXX) loader.s -o $(BUILD_DIR)\loader.o -T loader.ld -v -Wa,-mregnames -nostartfiles -nodefaultlibs -O3 -mno-toc -fno-exceptions

binary: compile
	$(OBJCOPY) -O binary --only-section=.text.inject $(BUILD_DIR)\loader.o $(BUILD_DIR)\loader.text.inject.bin -v
	$(OBJCOPY) -O binary --only-section=.text $(BUILD_DIR)\loader.o $(BUILD_DIR)\loader.text.bin -v

patch: binary
	$(PYTHON) $(BIN2RPCS3PATCH) --input $(BUILD_DIR)\loader.text.inject.bin --address $(INJECT_ADDRESS) --output $(BUILD_DIR)\loader.yaml --indent 3
	$(PYTHON) $(BIN2RPCS3PATCH) --input $(BUILD_DIR)\loader.text.bin --address $(LOADER_ADDRESS) --output $(BUILD_DIR)\loader.yaml --append --indent 3

clean:
	del build\* /s /q
