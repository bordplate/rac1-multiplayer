#define PRINTF_SUPPORT_FLOAT 1

CONFIG_START()
#ifdef GAME_RC1
    //             type      short name        long name        default value
    CONFIG_SETTING( BOOL,    enableRatchetTestModule, "Test module",   true )
#endif
CONFIG_END()