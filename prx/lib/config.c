#include "lib/config.h"
#include "lib/file.h"

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
    static Config sConfig = {

#define CONFIG_END() \
    };

#define CONFIG_SETTING( type, shortName, longName, defaultValue ) \
    { #shortName, longName, CONFIG_VALUE_TYPE_##type, CONFIG_VALUE_CLASS_ATOMIC, 1, { .type##_value = defaultValue } },

#define CONFIG_SETTING_ARRAY( type, shortName, longName, defaultValueCount, ... ) \
     { #shortName, longName, CONFIG_VALUE_TYPE_##type, CONFIG_VALUE_CLASS_ARRAY, defaultValueCount, { .type##_array = { __VA_ARGS__ } } },

#define CONFIG_OPTION CONFIG_SETTING
#define CONFIG_OPTION_ARRAY CONFIG_SETTING_ARRAY

#include "../config.inc"

#undef CONFIG_OPTION_ARRAY
#undef CONFIG_OPTION
#undef CONFIG_SETTING_ARRAY
#undef CONFIG_SETTING


#define CONFIG_TRACE_LOG 0
#define CONFIG_LOG( msg, ... ) printf( "config: " msg, ##__VA_ARGS__ )
#if CONFIG_TRACE_LOG
#define CONFIG_TRACE( msg, ... ) printf( "config: " msg, ##__VA_ARGS__ )
#else
#define CONFIG_TRACE( msg, ... ) do {} while ( false )
#endif

u32 configGetSettingCount()
{
    return sizeof(Config) / sizeof(ConfigSetting);
}

ConfigSetting* configGetSettingByIndex( u32 index )
{
    assert( index < configGetSettingCount() );
    return ((ConfigSetting*)&sConfig) + index;
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

ConfigSetting* configGetSettingByLongName( const char* name )
{
    for ( size_t i = 0; i < configGetSettingCount(); i++ )
    {
        ConfigSetting* setting = configGetSettingByIndex( i );
        if ( stringEquals( setting->longName, name ) )
            return setting;
    }
    
    return NULL;
}

const Config* const configGet()
{
    return &sConfig;
}

static u8 configStringBuffer[1024];
static u32 configStringBufferIndex = 0;

static void configParseYamlValue( ConfigSetting* setting, const char* value, u32 valueIndex )
{
    switch ( setting->valueType )
    {
        case CONFIG_VALUE_TYPE_BOOL:   
            setting->value.boolArray[valueIndex] = 
                stringEqualsEx( value, "on", true ) || 
                stringEqualsEx( value, "yes", true ) || 
                stringEqualsEx( value, "true", true );

            CONFIG_TRACE( "set bool %d\n", setting->value.boolArray[valueIndex] );
            break;

        case CONFIG_VALUE_TYPE_INT:
            setting->value.intArray[valueIndex] = intParse( value );
            CONFIG_TRACE( "set int %d\n", setting->value.intArray[valueIndex] );
            break;

        case CONFIG_VALUE_TYPE_FLOAT:
            setting->value.floatArray[valueIndex] = floatParse( value );
            CONFIG_TRACE( "set float %f\n", setting->value.floatArray[valueIndex] );
            break;

        case CONFIG_VALUE_TYPE_STRING:
            {
                // handle quoted strings
                u32 valueStartIndex = 0;
                u32 valueLength = stringGetLength( value );

                if ( stringStartsWith( value, "\"" ) )
                {
                    // quoted string
                    valueStartIndex = 1;
                    valueLength -= 2;
                }

                // copy string to buffer for persistent storage
                char* p = &configStringBuffer[configStringBufferIndex];
                configStringBufferIndex += valueLength + 1;
                assert( configStringBufferIndex < sizeof(configStringBuffer ));

                memoryCopy( p, sizeof( configStringBuffer ) - configStringBufferIndex, 
                    value + valueStartIndex, valueLength );

                p[valueLength + 1] = 0;

                // set pointer to local buffer
                setting->value.stringArray[valueIndex] = p;
                CONFIG_TRACE( "set string %s\n", setting->value.stringArray[valueIndex] );
            }
            break;
    }
}

void configLoad( const char* path )
{
    CONFIG_LOG( "loading config from %s\n", path );

    const u32 settingCount = configGetSettingCount();
    CONFIG_LOG( "loading %d settings\n", settingCount );

    FileHandle handle;
    FileStatus status = fileOpen( &handle, path, FILE_MODE_READ );
    assert( status == FILE_STATUS_OK );

    while ( true )
    {
        // read line
        char line[1024];
        u64 bytesRead;
        u64 bytesWritten;
        status = fileReadLine( handle, line, sizeof( line ), &bytesRead, &bytesWritten );
        assert( status == FILE_STATUS_OK );
        if ( bytesRead == 0 )
            break;

        if ( stringIsNullOrWhitespace( line ) ||
             stringStartsWith( line, "#" ))
        {
            // skip empty and commented lines
            continue;
        }

        // remove comment
        s32 commentStartIndex = stringIndexOf( line, "#" );
        if ( commentStartIndex != -1 )
        {
            // terminate line at comment
            line[commentStartIndex] = 0;
        }

        // split line into key:value
        u32 colonIndex = stringIndexOf( line, ":" );
        assert( colonIndex != -1 );
        line[colonIndex] = 0;

        char key[1024];
        stringTrim( key, sizeof( key ), line );

        u32 valueStartIndex = colonIndex + 1;
        char value[1024];
        stringTrim( value, sizeof( value ), line + valueStartIndex );

        // parse value according to type
        ConfigSetting* setting = configGetSettingByLongName( key );
        if ( setting == NULL )
        {
            CONFIG_LOG( "ignoring unknown setting '%s'\n", key );
            continue;
        }

        if ( stringIsNullOrEmpty( value ) )
        {
            CONFIG_LOG( "ignoring missing value for setting '%s' (%s)\n",
                setting->longName, setting->shortName );
            continue;
        }

        if ( setting->valueClass == CONFIG_VALUE_CLASS_ATOMIC )
        {
            if ( stringStartsWith( value, "[" ) && stringEndsWith( value, "]" ) )
            {
                CONFIG_LOG( "ignoring invalid array value for setting '%s' (%s), expected single value\n",
                    setting->longName, setting->shortName );
                continue;
            }

            configParseYamlValue( setting, value, 0 );
        }
        else if ( setting->valueClass == CONFIG_VALUE_CLASS_ARRAY )
        {
            if ( !stringStartsWith( value, "[" ) || !stringEndsWith( value, "]" ) )
            {
                CONFIG_LOG( "ignoring invalid array value for setting '%s' (%s), expected start and end brace\n",
                    setting->longName, setting->shortName );
                continue;
            }

            u32 arrayStringLength = stringGetLength( value );
            u32 arrayContentStartIndex = stringIndexOf( value, "[" ) + 1;
            u32 arrayContentEndIndex = stringIndexOf( value, "]");
            u32 arrayValueStartIndex = arrayContentStartIndex;
            u32 arrayValueIndex = 0;
            while ( arrayValueStartIndex < arrayContentEndIndex )
            {
                CONFIG_TRACE( "%s\n", value + arrayValueStartIndex );
                s32 arrayValueEndIndex = stringIndexOf( value + arrayValueStartIndex, "," );
                if ( arrayValueEndIndex == -1 )
                {
                    // last value
                    arrayValueEndIndex = arrayContentEndIndex;
                }
                else
                {
                    // add base index
                    arrayValueEndIndex += arrayValueStartIndex;
                }

                u32 arrayValueLength = arrayValueEndIndex - arrayValueStartIndex;
                if ( arrayValueLength == 0 )
                    break;

                CONFIG_TRACE( "%d-%d %d\n", arrayValueStartIndex, arrayValueEndIndex, arrayValueLength );
            

                // copy array value portion to buffer
                char arrayValue[1024];
                stringSubstringEx( arrayValue, sizeof( arrayValue ), value, arrayValueStartIndex, arrayValueLength );
                
                // trim whitespace
                stringTrim( arrayValue, sizeof( arrayValue ), arrayValue );
                CONFIG_TRACE( "array value: '%s'\n", arrayValue );

                // parse value
                assert( arrayValueIndex < CONFIG_VALUE_ARRAY_MAX && "too many values in the settings array" );
                configParseYamlValue( setting, arrayValue, arrayValueIndex++ );
                arrayValueStartIndex = arrayValueEndIndex + 1;
            }

            setting->valueCount = arrayValueIndex;
        }
        else
        {
            assert( false && "unhandled value class" );
        }

        CONFIG_TRACE( "done processing line\n" );
    }

    CONFIG_TRACE( "close file\n" );
    fileClose( handle );

    CONFIG_TRACE( "start printing settings\n" );

    ConfigSetting* setting = configGetSettingByIndex( 0 );
    for ( size_t i = 0; i < settingCount; i++ )
    {
        CONFIG_LOG( "%s (%s): ", setting->longName, setting->shortName );
        
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
            }
        }

        printf( "\n" );
        ++setting;
    }
}

void configSave( const char* path )
{
    CONFIG_LOG( "config: saving config to %s (NOT IMPLEMENTED)\n", path );

    // TODO save from file
}
