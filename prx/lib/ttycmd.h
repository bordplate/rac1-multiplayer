#ifndef TTYCMD_H
#define TTYCMD_H

#include "lib/common.h"

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
     * @brief The number of arguments the command takes.
     * Specify -1 to allow the function to take an arbitrary number of arguments.
     */
    s32 argc;

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

