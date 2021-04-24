#include "config.h"
#include "string.h"

const char* ConfigValueTypeNames[] =
{
    "bool",
    "int",
    "float",
    "string"
};

// Include config values
#undef CONFIG_OPTION
#undef CONFIG_START
#undef CONFIG_END

#define CONFIG_START() \
    Config config = {

#define CONFIG_END() \
    };

#define CONFIG_OPTION( type, shortName, longName, defaultValue ) \
    { #shortName, longName, CONFIG_VALUE_TYPE_##type, CONFIG_VALUE_CLASS_SINGLE, 1, { .type##_value = defaultValue } },

#define CONFIG_OPTION_ARRAY( type, shortName, longName, defaultValueCount, ... ) \
     { #shortName, longName, CONFIG_VALUE_TYPE_##type, CONFIG_VALUE_CLASS_ARRAY, defaultValueCount, { .type##_array = { __VA_ARGS__ } } },

#include "../config.inc"

#undef CONFIG_OPTION_ARRAY
#undef CONFIG_OPTION

u32 configGetSettingCount()
{
    return sizeof(Config) / sizeof(ConfigSetting);
}

ConfigSetting* configGetSettingByIndex( u32 index )
{
    assert( index < configGetSettingCount() );
    return ((ConfigSetting*)&config) + index;
}

ConfigSetting* configGetSettingByName( const char* name )
{
    for ( size_t i = 0; i < configGetSettingCount(); i++ )
    {
        ConfigSetting* setting = configGetSettingByIndex( i );
        if ( stringEquals( setting->shortName, name ) )
            return setting;
    }
    
    return NULL;
}

const Config* const configGet()
{
    return &config;
}

void configLoad( const char* path )
{
    printf( "config: loading config from %s (NOT IMPLEMENTED)\n", path );

    const u32 settingCount = configGetSettingCount();
    printf( "config: loading %d settings\n", settingCount );

    ConfigSetting* setting = configGetSettingByIndex( 0 );
    for ( size_t i = 0; i < settingCount; i++ )
    {
        char valueBuffer[64];
        printf( "config: %s (%s): ", setting->longName, setting->shortName );
        
        if ( setting->valueCount > 0 )
        {
            for ( u32 j = 0; j < setting->valueCount; ++j )
            {
                if ( j != 0 )
                    printf( ", " );

                switch ( setting->valueType )
                {
                case CONFIG_VALUE_TYPE_BOOL:
                    printf( "%s", setting->value.boolArray[j] ? "true" : "false" );
                    break;

                case CONFIG_VALUE_TYPE_INT:
                    printf( "%d", setting->value.intArray[j] );
                    break;

                case CONFIG_VALUE_TYPE_FLOAT:
                    printf( "%f", setting->value.floatArray[j] );
                    break;

                case CONFIG_VALUE_TYPE_STRING:
                    printf( "%s", setting->value.stringArray[j] );
                    break;
                
                default:
                    break;
                }

                printf( valueBuffer );
            }
        }

        printf( "\n" );
        ++setting;
    }
    
    // TODO load from file
}

void configSave( const char* path )
{
    printf( "config: saving config to %s (NOT IMPLEMENTED)\n", path );

    // TODO save from file
}
