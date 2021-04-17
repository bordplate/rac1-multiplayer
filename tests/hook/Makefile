PYTHON = py -3
CC = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-gcc.exe
CXX = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-g++.exe
OBJCOPY = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-objcopy.exe
OBJDUMP = $(PS3_SDK)\host-win32\ppu\bin\ppu-lv2-objdump.exe
BIN2RPCS3PATCH = ..\tools\bin2rpcs3patch.py
INJECT_ADDRESS = 0x6CF04
HOOK_TEXT_ADDRESS = 0xb44b20
HOOK_DATA_ADDRESS = 0xCE2CB8

compile:
	$(CC) hook.s -o hook.o -T hook.ld -v -Wa,-mregnames -nostartfiles -nodefaultlibs

binary: compile
	$(OBJCOPY) -O binary --only-section=.text.func_patch hook.o hook.text.func_patch.bin -v
	$(OBJCOPY) -O binary --only-section=.text.func_hook hook.o hook.text.func_hook.bin -v
	$(OBJCOPY) -O binary --only-section=.data.func_hook hook.o hook.data.func_hook.bin -v

patch: binary
	$(PYTHON) $(BIN2RPCS3PATCH) --input hook.text.func_patch.bin hook.text.func_hook.bin hook.data.func_hook.bin --address $(INJECT_ADDRESS) $(HOOK_TEXT_ADDRESS) $(HOOK_DATA_ADDRESS) --output hook.yaml --indent 3