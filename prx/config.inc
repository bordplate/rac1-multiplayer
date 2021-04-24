CONFIG_START()
    CONFIG_OPTION( BOOL, debug, "Enable debugging features", false )
#ifdef GAME_TEST
    // Example config option values
    //             type     name              description                default value
    CONFIG_OPTION( BOOL,    enableTestModule, "Enables the test module", true )
    CONFIG_OPTION( STRING,  motd,             "Message of the day",      "Hello World" )

    // Example config option array values
    // Default max array size is 32.
    //                   value type  name             description                                 default value count  default values
    CONFIG_OPTION_ARRAY( STRING,     rmotd,           "Random messages of the day",               4,                   "Hi", "Hello", "Hello World", "Bonjour" )
    CONFIG_OPTION_ARRAY( INT,        favoriteNumbers, "Tell us all about your favorite numbers",  0 )
#endif
CONFIG_END()