CONFIG_START()
    CONFIG_SETTING( BOOL, debug, "Debugging features", true )

#ifdef GAME_P5
    CONFIG_SETTING( BOOL, enableEXFLWModule, "Enable Flowscripts module", true )
    CONFIG_SETTING( BOOL, enableDCModule, "Enable dc module", true )
    CONFIG_SETTING( BOOL, enableP5EModule, "Enable P5 Enhanced module", true )
    CONFIG_SETTING( BOOL, enableKasumiModule, "Enable Kasumi Fixes module", true )
    CONFIG_SETTING( BOOL, enableGlobalBF, "Global Script", true )
    CONFIG_SETTING( BOOL, enableSumire, "Sumire models", true )
    CONFIG_SETTING( BOOL, enableCutsceneOutfits, "Player Outfits in cutscenes", true )
    CONFIG_SETTING( BOOL, enableAkechiMod, "Royal Akechi patches", true )
    CONFIG_SETTING( BOOL, enablePersonaEnemies, "Persona Using Enemies", true )
    CONFIG_SETTING( BOOL, randomDLCBGM, "Randomized DLC BGM", true )
    CONFIG_SETTING( BOOL, enableExpandedBGM, "Expanded BGM Patches", true )
    CONFIG_SETTING( BOOL, enableExternalNavi, "External navigator text file", false )
    CONFIG_SETTING( BOOL, enableCustomNaviSoundPack, "Custom Navigator SoundPack", false )
    CONFIG_SETTING( BOOL, forceSingleCombatGAP, "Force Single GAP Player Combat Animations", true )
    CONFIG_SETTING( BOOL, forcePSZModel, "Force PSZ Models", false )
    CONFIG_SETTING( BOOL, enableModCPK, "mod cpk", true )
    CONFIG_SETTING( BOOL, enableKasumi, "Kasumi patches", true )
    CONFIG_SETTING( BOOL, enableMidWinterOutfits, "Midwinter Outfits", false )
    CONFIG_SETTING( BOOL, enableTTYCommands, "tty commands", true )
    CONFIG_SETTING( BOOL, enableModelDebuggingHelp, "Model Debugging Help", false )
    CONFIG_SETTING( BOOL, P5RTitleBGM, "Alternate title screens", true )
    CONFIG_SETTING( BOOL, DLCAmbush, "Ambush themes override DLC Music", false )
    CONFIG_SETTING( STRING,  modCPKName,"mod cpk name",  "mod" )
    CONFIG_SETTING( INT,  extraModCPK,  "Number of extra mod cpk", 0 )
    CONFIG_SETTING( BOOL, enableFileAccessLogging, "file access logging", true )
    CONFIG_SETTING_ARRAY( STRING, extraModCPKName, "names of extra mod cpk", 10,
    "mod", "mod", "mod", "mod", "mod", "mod", "mod", "mod", "mod", "mod")
    CONFIG_SETTING_ARRAY( INT, bitChkLogIgnore, "BIT_CHK flags to ignore when logging (1)", 100, 
    0x2101, 0x22ff, 0x2216, 0x2110, 0x2111, 
    0x2112, 0x2113, 0x106e, 0x106f, 0x1070, 
    0x2216, 0x2140, 0x2141, 0x2142, 0x2180, 
    0x2181, 0x2182, 0x2183, 0x2184, 0x2185, 
    0x2186, 0x2187, 0x2188, 0x2189, 0x218F, 
    0x1007, 0x1008, 0x1020, 0x103e, 0x107a, 
    0x1009, 0x10c9, 0x10ca, 0x10cb, 0x10cc, 
    0x1024, 0x1015, 0x1012, 0x1006, 0x1007, 
    0x10ad, 0x1c1d, 0x10ae, 0x10b0, 0x1059,
    0x1710, 0x1711, 0x1025, 0x103e, 0x2105, 
    0x103d, 0x1023, 0x1025, 0x1006, 0x10af, 
    0x10b1, 0x10b2, 0x1005, 0x1019, 0x1c1c, 
    0x0041, 0x1c21, 0x1c22, 0x1c23, 0x2131, 
    0x2103, 0x20c2, 0x2234, 0x073E, 0x20f6, 
    0x20a3, 0x074f, 0x200e, 0x22e3, 0x22e1,
    0x22e2, 0x1a8c, 0x1898, 0x1a16, 0x1258,
    0x13e8, 0x10c8, 0x1429, 0x142a, 0x107d,
    0x107d, 0x10ce, 0x125b, 0x1268, 0x13ee,
    0x157c, 0x1578, 0x1708, 0x18ef, 0x0057,
    0x1268, 0x13ec, 0x13ed, 0x159d, 0x16d5 )
    CONFIG_SETTING_ARRAY( INT, bitChkLogIgnore2, "BIT_CHK flags to ignore when logging (2)", 80, 
    0x2101, 0x22ff, 0x2216, 0x2110, 0x2111,
    0x18cb, 0x1a14, 0x1a15, 0x1bd9, 0x1bda,
    0x1bdb, 0x1bdc, 0x1bdd, 0x1bde, 0x1bdf,
    0x1a17, 0x1a18, 0x1a19, 0x1a1a, 0x1b72,
    0x1c0d, 0x1d7f, 0x1d80, 0x1d81, 0x1d83,
    0x1c38, 0x1d86, 0x1c40, 0x110a, 0x1128,
    0x0825, 0x0051, 0x0071, 0x0052, 0x2133,
    0x0053, 0x0073, 0x0054, 0x0074, 0x0055,
    0x0075, 0x0056, 0x0076, 0x0077, 0x0072,
    0x0049, 0x10d1, 0x10f9, 0x125f, 0x1260,
    0x1261, 0x1263, 0x04f0, 0x04f1, 0x04f5,
    0x04f6, 0x13f2, 0x13f4, 0x1580, 0x1584,
    0x170e, 0x170d, 0x18a6, 0x049c, 0x0821,
    0x1a8f, 0x1a8e, 0x0140, 0x0142, 0x1083,
    0x081f, 0x0a6f, 0x0069, 0x082b, 0x1016,
    0x1014, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF )
    CONFIG_SETTING_ARRAY( INT, getCountLogIgnore, "COUNT entries to ignore when logging", 20, 
     14, 135, 254, 255, 139, 
    149, 131, 151, 150, 146,
    147, 145, 148, 152, 144,
     31,  22, 155, 144, 144 )
    CONFIG_SETTING_ARRAY( INT, bitSetLogIgnore, "BIT_SET flags to ignore when logging", 10, 
    0x006b, 0x21ce, 0x2235, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF )
    CONFIG_SETTING_ARRAY( INT, fastSummonEnemy, "Fast Persona Summon Enemies", 10,
    359, 359, 359, 359, 359,
    359, 359, 359, 359, 359)
        
#endif
#ifdef GAME_TEST
    // Example config setting values
    //             type      short name        long name        default value
    CONFIG_SETTING( BOOL,    enableTestModule, "Test module",   false )
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