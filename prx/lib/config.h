#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

// Config value type enum
// Used for dynamic serialization
typedef enum
{
    CONFIG_VALUE_TYPE_BOOL,
    CONFIG_VALUE_TYPE_INT,
    CONFIG_VALUE_TYPE_FLOAT,
    CONFIG_VALUE_TYPE_STRING,
    CONFIG_VALUE_TYPE_COUNT,
} ConfigValueType;

const char* ConfigValueTypeNames[CONFIG_VALUE_TYPE_COUNT];

// Config value union
// Used to dynamically store the value of a config option.
// Member names must match type suffix of ConfigValueType
#define CONFIG_VALUE_MEMBER( type, shortName, typeSuffix ) \
    type shortName##Value; \
    type typeSuffix##_value

typedef union
{
    CONFIG_VALUE_MEMBER( bool, bool, BOOL );
    CONFIG_VALUE_MEMBER( int, int, INT );
    CONFIG_VALUE_MEMBER( float, float, FLOAT );
    CONFIG_VALUE_MEMBER( const char*, string, STRING );
} ConfigValue;

#undef CONFIG_VALUE_MEMBER

// Config setting struct. Describes a config setting.
typedef struct
{
    const char* shortName;
    const char* longName;
    ConfigValueType valueType;
    ConfigValue value;
} ConfigSetting;

// Config type declaration
#define CONFIG_START() \
    typedef struct { \

#define CONFIG_END() \
    } Config;

#define CONFIG_OPTION( type, shortName, longName, defaultValue ) \
    ConfigSetting option_##shortName;

#pragma pack(1)
#include CONFIG_SET_FILENAME
#pragma pop

#undef CONFIG_OPTION

// Get the number of setting in the current config.
u32 configGetSettingCount();

// Gets the Nth setting.
ConfigSetting* configGetSettingByIndex( u32 index );

// Gets the setting with the specified short name, or NULL.
ConfigSetting* configGetSettingByName( const char* name );

// Gets a read-only pointer to the config.
// Don't use this unless you know what you're doing
const Config* const configGet();

// Loads the current configuration from a file.
void configLoad( const char* path );

// Saves the current configuration to a file.
void configSave( const char* path );

char* configValueToString( char* dest, size_t destLen, ConfigValueType type, ConfigValue* value );

// Config option access macros
#define CONFIG_ENABLED( name ) \
    configGet()->option_##name.value.boolValue

#define CONFIG_BOOL( name ) \
    configGet()->option_##name.value.boolValue

#define CONFIG_FLOAT( name ) \
    configGet()->option_##name.value.floatValue

#define CONFIG_INT( name ) \
    configGet()->option_##name.value.intValue

#define CONFIG_STRING( name ) \
    configGet()->option_##name.value.stringValue

#endif