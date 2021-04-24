CONFIG_START()
    CONFIG_SETTING( BOOL, debug, "Enable debugging features", false )
#ifdef GAME_TEST
    // Example config setting values
    //             type     name              description                default value
    CONFIG_SETTING( BOOL,    enableTestModule, "Enables the test module", true )
    CONFIG_SETTING( STRING,  motd,             "Message of the day",      "Hello World" )

    // Example config setting array values
    // Default max array size is 32.
    //                   value type  name             description                                  default value count  default values
    CONFIG_SETTING_ARRAY( STRING,     rmotd,           "Random messages of the day",               4,                   "Hi", "Hello", "Hello World", "Bonjour" )
    CONFIG_SETTING_ARRAY( INT,        favoriteNumbers, "Tell us all about your favorite numbers",  0 )
#endif
CONFIG_END()