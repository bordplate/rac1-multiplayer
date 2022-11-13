#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

/**
 * @brief Config value type enum.
 * Used for dynamic serialization.
 */
typedef enum ConfigValueType
{
    CONFIG_VALUE_TYPE_BOOL,
    CONFIG_VALUE_TYPE_INT,
    CONFIG_VALUE_TYPE_FLOAT,
    CONFIG_VALUE_TYPE_STRING,
    CONFIG_VALUE_TYPE_COUNT,
} ConfigValueType;

/**
 * @brief The class of a config value type.
 * It is used to differentiate between different usages of a value type, such as an atomic value or an array.
 */
typedef enum ConfigValueClass
{
    /**
     * @brief The value represents a single, atomic value.
     * 
     */
    CONFIG_VALUE_CLASS_ATOMIC,

    /**
     * @brief The value represents an array value.
     * 
     */
    CONFIG_VALUE_CLASS_ARRAY
} ConfigValueClass;

const char* ConfigValueTypeNames[CONFIG_VALUE_TYPE_COUNT];

/**
 * @brief The max number of elements on a config setting array.
 * 
 */
#define CONFIG_VALUE_ARRAY_MAX 256

#define CONFIG_VALUE_MEMBER( type, shortName, typeSuffix ) \
    type shortName##Value; \
    type shortName##Array[CONFIG_VALUE_ARRAY_MAX]; \
    type typeSuffix##_value; \
    type typeSuffix##_array[CONFIG_VALUE_ARRAY_MAX]

/**
 * @brief Represents a config setting value.
 * Used to dynamically store the value of a config setting.
 * Member names must match type suffix of ConfigValueType.
 */
typedef union ConfigValue
{
    CONFIG_VALUE_MEMBER( bool, bool, BOOL );
    CONFIG_VALUE_MEMBER( int, int, INT );
    CONFIG_VALUE_MEMBER( float, float, FLOAT );
    CONFIG_VALUE_MEMBER( const char*, string, STRING );
} ConfigValue;

#undef CONFIG_VALUE_MEMBER

/**
 * @brief Config setting struct. Describes a config setting.
 * 
 */
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

//#pragma pack(1)
#include "../config.inc"
//#pragma pop

#undef CONFIG_OPTION_ARRAY
#undef CONFIG_OPTION
#undef CONFIG_SETTING_ARRAY
#undef CONFIG_SETTING

/**
 * @brief Get the number of settings in the current config.
 * @return u32 The number of settings.
 */
u32 configGetSettingCount();

/**
 * @brief Gets the Nth setting, where N is the index specified.
 * 
 * @param index The index of the setting to retrieve.
 * @return ConfigSetting* A pointer to the config setting structure.
 */
ConfigSetting* configGetSettingByIndex( u32 index );

/**
 * @brief Gets the setting with the specified short name, or NULL if not found.
 * @param name The short name of the config setting.
 * @return ConfigSetting* A pointer to the config setting if found, or NULL if not found.
 */
ConfigSetting* configGetSettingByName( const char* name );

/**
 * @brief Gets the setting with the specified long name, or NULL if not found.
 * @param name The long name of the config setting.
 * @return ConfigSetting* A pointer to the config setting if found, or NULL if not found.
 */
ConfigSetting* configGetSettingByLongName( const char* name );

/**
 * @brief Gets a read-only pointer to the config.
 * @return const Config* const 
 */
const Config* const configGet();

/**
 * @brief Loads the current configuration from a file.
 * @param path The fully qualified file path of the config file.
 */
void configLoad( const char* path );

/**
 * @brief Saves the current configuration to a file.
 * @param path The fully qualified file path of the config file to save to.
 */
void configSave( const char* path );

// Config option access macros

/**
 * @brief Checks if the specified config setting is enabled.
 * @param name The config setting short name.
 * @return bool
 */
#define CONFIG_ENABLED( name ) \
    configGet()->setting_##name.value.boolValue

/**
 * @brief Retrieves the boolean value of the specified config setting.
 * @param name The config setting short name.
 * @return bool
 */
#define CONFIG_BOOL( name ) \
    configGet()->setting_##name.value.boolValue

/**
 * @brief Retrieves the float value of the specified config setting.
 * @param name The config setting short name.
 * @return float
 */
#define CONFIG_FLOAT( name ) \
    configGet()->setting_##name.value.floatValue

/**
 * @brief Retrieves the integer value of the specified config setting.
 * @param name The config setting short name.
 * @return int
 */
#define CONFIG_INT( name ) \
    configGet()->setting_##name.value.intValue

/**
 * @brief Retrieves the string value of the specified config setting.
 * @param name The config setting short name.
 * @return const char*
 */
#define CONFIG_STRING( name ) \
    configGet()->setting_##name.value.stringValue

/**
 * @brief Retrieves the boolean value array of the specified config setting.
 * @param name The config setting short name.
 * @return const bool[]
 */
#define CONFIG_BOOL_ARRAY( name ) \
    configGet()->setting_##name.value.boolArray

/**
 * @brief Retrieves the float value array of the specified config setting.
 * @param name The config setting short name.
 * @return const float[]
 */
#define CONFIG_FLOAT_ARRAY( name ) \
    configGet()->setting_##name.value.floatArray

/**
 * @brief Retrieves the integer value array of the specified config setting.
 * @param name The config setting short name.
 * @return const int[]
 */
#define CONFIG_INT_ARRAY( name ) \
    configGet()->setting_##name.value.intArray

/**
 * @brief Retrieves the string value array of the specified config setting.
 * @param name The config setting short name.
 * @return const char*[]
 */
#define CONFIG_STRING_ARRAY( name ) \
    configGet()->setting_##name.value.stringArray

/**
 * @brief Retrieves the number of stored array values of the specified config setting.
 * @param name The config setting short name.
 * @return u32
 */
#define CONFIG_ARRAY_COUNT( name ) \
    configGet()->setting_##name.valueCount

#endif