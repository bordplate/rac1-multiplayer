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
    { #shortName, longName, CONFIG_VALUE_TYPE_##type, { .type##_value = defaultValue } },

#include "../config.inc"

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

char* configValueToString( char* dest, size_t destLen, ConfigValueType type, ConfigValue* value )
{
    switch ( type )
    {
    case CONFIG_VALUE_TYPE_BOOL:
        stringFormat( dest, destLen, "%s", value->BOOL_value ? "true" : "false" );
        break;

    case CONFIG_VALUE_TYPE_INT:
        stringFormat( dest, destLen, "%d", value->INT_value );
        break;

    case CONFIG_VALUE_TYPE_FLOAT:
        stringFormat( dest, destLen, "%f", value->FLOAT_value );
        break;

    case CONFIG_VALUE_TYPE_STRING:
        stringFormat( dest, destLen, "%s", value->STRING_value );
        break;
    
    default:
        break;
    }

    return dest;
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
        printf( "config: %s (%s): %s\n", setting->longName, setting->shortName,
            configValueToString( valueBuffer, sizeof( valueBuffer ), setting->valueType, &setting->value ) );
        ++setting;
    }
    
    // TODO load from file
}

void configSave( const char* path )
{
    printf( "config: saving config to %s (NOT IMPLEMENTED)\n", path );

    // TODO save from file
}
