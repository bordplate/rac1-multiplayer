# recursive wildcard function
# courtesy of https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
TMP_DIR = $(BUILD_DIR)/tmp

-include ../userconfig.mk
-include ../os.mk

EXE_SUFFIX = .exe
PPU_LIB_DIR = $(PS3_SDK)/target/ppu/lib
CC = $(WINE) $(PS3_SDK)/host-win32/ppu/bin/ppu-lv2-gcc$(EXE_SUFFIX)
MAKE_FSELF = $(WINE) $(PS3_SDK)/host-win32/bin/make_fself$(EXE_SUFFIX)
CFILES = $(call rwildcard,.,*.c)
CPPFILES = $(call rwildcard,.,*.cpp)
DEFINES =
PRX_LDFLAGS_EXTRA =
PPU_PRX_LDFLAGS  += \
	$(PRX_LDFLAGS_EXTRA) \
	-Wl,--start-group \
	-lsysmodule_stub \
	-lsysutil_stub \
	-lsysutil_np_stub \
	-Map=$(BIN_DIR)/mod.map \
	-Wl,--end-group

CCFLAGS = $(CPPFILES) $(CFILES) $(DEFINES) -I$(PS3_SDK)/target/ppu/include -I./ -Wa,-mregnames $(PPU_PRX_LDFLAGS)
SCETOOL = $(WINE) $(PS3_SDK)/Tools/alt/scetool.exe
PKG_NPDRM = $(WINE) $(PS3_SDK)/Tools/tool/psn_package_npdrm.exe

eboot:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/PS3_GAME/USRDIR

	$(CC) $(CCFLAGS) -fpic -o $(BIN_DIR)/EBOOT.ELF
	#$(CC) $(CCFLAGS) -fpic -Wl,--oformat=fself_npdrm -Wl,--compress-output -o $(BIN_DIR)/EBOOT.BIN

	cd $(PS3_SDK)/Tools/alt && \
	$(SCETOOL) -0 SELF -1 FALSE -s TRUE -2 0007 -3 1010000001000003 -4 01000002 -5 NPDRM -A 0001000000000000 \
		-6 003005000000000 -b FREE -c EXEC -f EP9000-NPEA00385_00-RC1REMASTER00100 \
		-9 00000000000000000000000000000000000000000000003B0000000100002000 -j TRUE -g EBOOT.BIN \
		-e "$(CURDIR)/$(BIN_DIR)/EBOOT.ELF" "$(CURDIR)/$(BIN_DIR)/EBOOT.BIN"

clean:
	$(DELETE) $(DELETEFLAGS) $(BIN_DIR)$(PATH_SEP)*.*
	$(DELETE) $(DELETEFLAGS) $(TMP_DIR)$(PATH_SEP)*.*
