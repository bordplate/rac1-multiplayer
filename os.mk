ifeq ($(OS),Windows_NT)
	OS_ID := Windows
	PATH_SEP := \\
	COPY := copy
	DELETE := del
	DELETEFLAGS := /s /q

	GAME_FOR_PATHS := $(GAME)
else
	OS_ID := $(shell uname 2>/dev/null || echo Unknown)
	PATH_SEP := /
	COPY := cp
	DELETE := rm
	DELETEFLAGS := -rf

	WINE := wine
	GAME_FOR_PATHS := $(shell echo $(GAME) | tr '[:upper:]' '[:lower:]')
endif

PS3_SDK := $(shell echo $(PS3_SDK) | sed 's:\\:/:g')
RPCS3_DIR := $(shell echo $(RPCS3_DIR) | sed 's:\\:/:g')