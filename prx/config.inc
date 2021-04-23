CONFIG_START()
    CONFIG_OPTION( BOOL, debug, "Enable debugging features", false )
#ifdef GAME_TEST
    CONFIG_OPTION( BOOL, enableTestModule, "Enables the test module", true )
    CONFIG_OPTION( STRING, motd, "Message of the day", "Hello World" )
#endif
CONFIG_END()