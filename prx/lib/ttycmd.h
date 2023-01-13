#ifndef TTYCMD_H
#define TTYCMD_H

#include "clib.h"
#include "types.h"
#include "string.h"
#include "utils.h"

/**
 * @brief The TTY command handler function result status type.
 * This enumerates the possible status codes a command handler function can return.
 */
typedef enum TtyCmdStatus
{
    /**
     * @brief The TTY command executed successfully.
     */
    TTY_CMD_STATUS_OK,

    /**
     * @brief The TTY command was unable to execute successfully, due to an invalid argument.
     */
    TTY_CMD_STATUS_INVALID_ARG,

    /**
     * @brief The TTY command was unable to execute successfully, due to an invalid number of arguments.
     * 
     */
    TTY_CMD_STATUS_INVALID_ARGC,

    /**
     * @brief The TTY command was unable to execute successfully due to an error.
     */
    TTY_CMD_STATUS_ERROR,
} TtyCmdStatus;


struct TtyCmd;

/**
 * @brief The TTY command handler function pointer type.
 * @param cmd The currently executing command
 * @param args The array of string arguments
 * @param argc The number of passed arguments
 * @param error The description of what lead to the specified error status. Use this to provide info over eg. which argument is wrong and why.
 */
typedef TtyCmdStatus(*TtyCmdHandler)( struct TtyCmd* cmd, const char** args, u32 argc, char** error );

/**
 * @brief The maximum number of TTY command parameters.
 * 
 */
#define TTY_CMD_PARAM_MAX 16

/**
 * @brief The TTY command flags. Describes how a command should be evaluated.
 */
typedef enum TtyCmdFlags
{
    /**
     * @brief Default flag. Specify this if none of the other flags are applicable.
     */
    TTY_CMD_FLAG_NONE = 0,

    /**
     * @brief The TTY command takes a variable number of arguments.
     * No error will be raised if more arguments are passed than there are defined parameters.
     * It is up to the command implementation to verify the parameters passed.
     */
    TTY_CMD_FLAG_VARARGS = 1 << 0,
} TtyCmdFlags;

/**
 * @brief TTY command string parameter type.
 */
#define TTY_CMD_PARAM_TYPE_STRING   "string"

/**
 * @brief TTY command int parameter type.
 */
#define TTY_CMD_PARAM_TYPE_INT      "int"

/**
 * @brief TTY command float parameter type.
 */
#define TTY_CMD_PARAM_TYPE_FLOAT    "float"

/**
 * @brief TTY command float parameter type.
 */
#define TTY_CMD_PARAM_TYPE_BOOL     "bool"

/**
 * @brief TTY command parameter flags.
 * 
 */
typedef enum TtyCmdParamFlags
{
    /**
     * @brief The parameter is required. If the parameter is missing, an error will be raised.
     */
    TTY_CMD_PARAM_FLAG_REQUIRED = 0,

    /**
     * @brief The parameter is optional. Optional parameters must come before required parameters.
     * No error is raised when an optional argument is missing.
     */
    TTY_CMD_PARAM_FLAG_OPTIONAL = 1 << 0,
} TtyCmdParamFlags;

/**
 * @brief Describes a TTY command parameter.
 */
typedef struct TtyCmdParam
{
    /**
     * @brief The name of the parameter.
     */
    const char* name;

    /**
     * @brief Describes the parameter's usage.
     */
    const char* description;

    /**
     * @brief Flags indicating the usage of the parameter.
     */
    TtyCmdParamFlags flags;

    /**
     * @brief String indicating the type of the parameter.
     * See TTY_CMD_PARAM_TYPE_* for the standard types.
     */
    const char* type;
} TtyCmdParam;

/**
 * @brief TTY command structure. 
 * Describes the properties of a command used for error reporting, and provides the handler function for execution.
 */
typedef struct TtyCmd
{
    /**
     * @brief The command name.
     * 
     */
    const char* name;

    /**
     * @brief The command description. Used for displaying help text.
     */
    const char* description;

    /**
     * @brief The flags indicating the usage semantics of the command.
     */
    TtyCmdFlags flags;

    /**
     * @brief Array of parameter descriptors, describing each passed argument.
     * 
     */
    TtyCmdParam params[TTY_CMD_PARAM_MAX];

    /**
     * @brief The function that handles the execution of the command.
     */
    TtyCmdHandler handler;

    /**
     * @brief User data pointer. Can be used to store data in the command;
     */
    void* user;
} TtyCmd;

/**
 * @brief Declares a TTY command
 * @param func The command function.
 * @param name The name of the command.
 * @param desc The description of the command.
 * @param flags The command flags, see TtyCmdFlags
 * @param ... Command parameters declared using TTY_CMD_PARAM 
 */
#define TTY_CMD( func, name, desc, flags, ... ) \
    { name, desc, flags, { __VA_ARGS__ }, func, NULL }

/**
 * @brief Declares a TTY command parameter
 * @param name The name of the command parameter
 * @param desc The description of the parameter
 * @param flags The parameter flags, see TtyCmdParamFlags
 * @param type The parameter value type string, see TTY_PTYPE_* for standard types. 
 * Enter your own string in case you want to specify a custom format description.
 */
#define TTY_CMD_PARAM( name, desc, flags, type ) \
    { name, desc, flags, type } 

/**
 * @brief Creates a TTY command list terminator. 
 */
#define TTY_CMD_END() \
    {}

/**
 * @brief Process any pending TTY commands.
 * 
 * @param cmds Pointer to an array of TTY command descriptors (TtyCmd). The last command must be NULL.
 */
void ttyCmdProcess( TtyCmd* cmds );

/**
 * @brief Process a single line of text containing a TTY command.
 * 
 * @param cmds Pointer to an array of TTY command descriptors (TtyCmd). The last command must be NULL.
 * @param line The line of text to process.
 */
void ttyCmdProcessLine( TtyCmd* cmds, const char* line );

/**
 * @brief Starts the TTY command listener thread.
 * 
 * @param cmds Pointer to an array of TTY command descriptors (TtyCmd). The last command must be NULL.
 */
void ttyCmdStartListenerThread( TtyCmd* cmds );

#endif

