# NOTE: might be different between versions...?
TOC									= 0xD01288 # static throughout
LOADER_INJECT_ADDR 					= 0x10250  # nop in main()
LOADER_START_ADDR 					= 0xA3BE70 # 0xA3BE6C + 4 + align16
LOADER_END_ADDR 					= 0xA3BEF0 # LOADER_START_ADDR + 0x80
LOADER_SYS_PRX_MODULE_LOAD_ADDR 	= 0xB45E5C # cellPrxForUser _sys_heap_delete_heap import
LOADER_SYS_PRX_MODULE_START_ADDR 	= 0XB45D3C # cellPrxForUser _sys_heap_malloc import

# 745 instructions, around ~200 hooks
HOOK_SHARED_TEXT_BEGIN_ADDR = $(LOADER_END_ADDR)
HOOK_SHARED_TEXT_END_ADDR 	= 0xA3CA94 # HOOK_SHARED_TEXT_BEGIN_ADDR + 0xBA4

# 0xA8 / 4 = 42 words
HOOK_SHARED_DATA_BEGIN_ADDR = 0xCE2CB8
HOOK_SHARED_DATA_END_ADDR 	= 0xCE2D60 # HOOK_SHARED_DATA_BEGIN_ADDR + 0xA8