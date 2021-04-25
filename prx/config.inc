CONFIG_START()
    CONFIG_SETTING( BOOL, debug, "Debugging features", false )
#ifdef GAME_TEST
    // Example config setting values
    //             type      short name        long name        default value
    CONFIG_SETTING( BOOL,    enableTestModule, "Test module",   true )
    CONFIG_SETTING( INT,     testInt,          "Test int",      1234 )
    CONFIG_SETTING( FLOAT,   testFloat,        "Test float",    1234.012f )
    CONFIG_SETTING( STRING,  testMessage,      "Test message",  "Hello World" )

    // Example config setting array values
    // Default max array size is 256.
    //                    value type  short name       long name                         # default values  default values
    CONFIG_SETTING_ARRAY( BOOL,       testBoolArray,   "Test bool array",                0 )
    CONFIG_SETTING_ARRAY( INT,        testIntArray,    "Test int array",                 0 )
    CONFIG_SETTING_ARRAY( FLOAT,      testFloatAray,   "Test float array",               0 )
    CONFIG_SETTING_ARRAY( STRING,     testRmotd,       "Test random message of the day", 4,                "Hi", "Hello", "Hello World", "Bonjour" )
#endif
CONFIG_END()