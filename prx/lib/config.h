#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

// Config value type enum
// Used for dynamic serialization
typedef enum ConfigValueType
{
    CONFIG_VALUE_TYPE_BOOL,
    CONFIG_VALUE_TYPE_INT,
    CONFIG_VALUE_TYPE_FLOAT,
    CONFIG_VALUE_TYPE_STRING,
    CONFIG_VALUE_TYPE_COUNT,
} ConfigValueType;

typedef enum ConfigValueClass
{
    CONFIG_VALUE_CLASS_SINGLE,
    CONFIG_VALUE_CLASS_ARRAY
} ConfigValueClass;

const char* ConfigValueTypeNames[CONFIG_VALUE_TYPE_COUNT];

#define CONFIG_VALUE_ARRAY_MAX 32

// Config value union
// Used to dynamically store the value of a config option.
// Member names must match type suffix of ConfigValueType
#define CONFIG_VALUE_MEMBER( type, shortName, typeSuffix ) \
    type shortName##Value; \
    type shortName##Array[CONFIG_VALUE_ARRAY_MAX]; \
    type typeSuffix##_value; \
    type typeSuffix##_array[CONFIG_VALUE_ARRAY_MAX]

typedef union ConfigValue
{
    CONFIG_VALUE_MEMBER( bool, bool, BOOL );
    CONFIG_VALUE_MEMBER( int, int, INT );
    CONFIG_VALUE_MEMBER( float, float, FLOAT );
    CONFIG_VALUE_MEMBER( const char*, string, STRING );
} ConfigValue;

#undef CONFIG_VALUE_MEMBER

// Config setting struct. Describes a config setting.
typedef struct ConfigSetting
{
    const char* shortName;
    const char* longName;
    ConfigValueType valueType;
    ConfigValueClass valueClass;
    u32 valueCount;
    ConfigValue value;
} ConfigSetting;

// Config type declaration
#define CONFIG_START() \
    typedef struct { \

#define CONFIG_END() \
    } Config;

#define CONFIG_SETTING( type, shortName, longName, defaultValue ) \
    ConfigSetting setting_##shortName;

#define CONFIG_SETTING_ARRAY( type, shortName, longName, defaultValueCount, ... ) \
    ConfigSetting setting_##shortName;

#define CONFIG_OPTION CONFIG_SETTING
#define CONFIG_OPTION_ARRAY CONFIG_SETTING_ARRAY

#pragma pack(1)
#include "../config.inc"
#pragma pop

#undef CONFIG_OPTION_ARRAY
#undef CONFIG_OPTION
#undef CONFIG_SETTING_ARRAY
#undef CONFIG_SETTING

// Get the number of setting in the current config.
u32 configGetSettingCount();

// Gets the Nth setting.
ConfigSetting* configGetSettingByIndex( u32 index );

// Gets the setting with the specified short name, or NULL.
ConfigSetting* configGetSettingByName( const char* name );

// Gets the setting with the specified short name, or NULL.
ConfigSetting* configGetSettingByLongName( const char* name );

// Gets a read-only pointer to the config.
// Don't use this unless you know what you're doing
const Config* const configGet();

// Loads the current configuration from a file.
void configLoad( const char* path );

// Saves the current configuration to a file.
void configSave( const char* path );

// Config option access macros
#define CONFIG_ENABLED( name ) \
    configGet()->setting_##name.value.boolValue

#define CONFIG_BOOL( name ) \
    configGet()->setting_##name.value.boolValue

#define CONFIG_FLOAT( name ) \
    configGet()->setting_##name.value.floatValue

#define CONFIG_INT( name ) \
    configGet()->setting_##name.value.intValue

#define CONFIG_STRING( name ) \
    configGet()->setting_##name.value.stringValue

#define CONFIG_BOOL_ARRAY( name ) \
    configGet()->setting_##name.value.boolArray

#define CONFIG_FLOAT_ARRAY( name ) \
    configGet()->setting_##name.value.floatArray

#define CONFIG_INT_ARRAY( name ) \
    configGet()->setting_##name.value.intArray

#define CONFIG_STRING_ARRAY( name ) \
    configGet()->setting_##name.value.stringArray

#define CONFIG_ARRAY_COUNT( name ) \
    configGet()->setting_##name.valueCount

#endif