#include "lib/ttycmd.h"
#include <sys/tty.h>

#define TTY_TRACE_LOG 0
#define TTY_LOG( msg, ... ) printf( "ttycmd: " msg, ##__VA_ARGS__ )
#if TTY_TRACE_LOG
#define TTY_TRACE( msg, ... ) printf( "ttycmd: " msg, ##__VA_ARGS__ )
#else
#define TTY_TRACE( msg, ... ) do {} while ( false )
#endif

static TtyCmd* sCommands;

static const char* readTtyLine()
{
    static u8 buffer[1024];
    static s32 bufferIndex = 0;

    memoryClear( buffer, sizeof( buffer ) );
    u32 bytesRead;
    int err = sys_tty_read( SYS_TTYP_USER1, buffer, sizeof( buffer ), &bytesRead );
    //assert( err == CELL_OK );
    //assert( bytesRead < sizeof( buffer ) );
    return (const char*)buffer;
}

static TtyCmd* findCommandByName( TtyCmd* cmds, const char* command )
{
    TTY_TRACE( "finding command by name: %s\n", command );

    TtyCmd* cmd = cmds;
    while ( cmd->name != NULL )
    {
        if ( stringEqualsEx( command, cmd->name, true ) )
            return cmd;

        cmd++;
    }

    return NULL;
}

static s32 getParamCount( TtyCmd* cmd )
{
    s32 paramCount = 0;
    for ( u32 i = 0; i < TTY_CMD_PARAM_MAX; ++i )
    {
        if ( cmd->params[i].name == NULL )
            break;

        paramCount++;
    }

    return paramCount;
}

static s32 getRequiredParamCount( TtyCmd* cmd )
{
    if ( cmd->flags & TTY_CMD_FLAG_VARARGS )
        return -1;

    s32 paramCount = 0;
    for ( u32 i = 0; i < TTY_CMD_PARAM_MAX; ++i )
    {
        if ( cmd->params[i].name == NULL )
            break;

        if ( !( cmd->params[i].flags & TTY_CMD_PARAM_FLAG_OPTIONAL ) )
            paramCount++;
    }

    return paramCount;
}

/*
short:
add a b:    Adds 2 numbers and prints the result

long:

add a b:    Adds 2 numbers and prints the result
params:
    a: float, required      The first number to add
    b: float, required      The second number to add
*/


static void printCmdShortHelp( TtyCmd* helpCmd )
{
    TTY_TRACE( "printing short help info\n" );

    TTY_LOG( "  %s", helpCmd->name );

    s32 paramCount = getParamCount( helpCmd );
    if ( paramCount > 0 )
    {
        for ( u32 i = 0; i < paramCount; ++i )
        {
            TtyCmdParam* param = &helpCmd->params[i];

            if ( param->name != NULL )
            {
                if ( param->flags & TTY_CMD_PARAM_FLAG_OPTIONAL )
                    printf( " [%s]", param->name );
                else
                    printf( " %s", param->name );
            }
        }
    }

    if ( helpCmd->flags & TTY_CMD_FLAG_VARARGS )
    {
        printf( " ..." );
    }

    if ( helpCmd->description != NULL ) printf( "\t\t%s\n", helpCmd->description );
}


static void printCmdListShortHelp()
{
    // print short help text for all commands
   //TTY_LOG( "commands:\n" );

   //TtyCmd* helpCmd = sCommands;
   //while ( helpCmd->name != NULL )
   //{
   //    printCmdShortHelp( helpCmd );
   //    ++helpCmd;
   //}

   //TTY_LOG( "\n" );
   //TTY_LOG( "builtin commands:\n" );
   //helpCmd = sBuiltinCmds;
   //while ( helpCmd->name != NULL )
   //{
   //    printCmdShortHelp( helpCmd );
   //    ++helpCmd;
   //}
}

static TtyCmdStatus helpCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
    if ( argc == 0 )
    {
        printCmdListShortHelp();
    }
    else
    {
        // print long help text for one command
        TtyCmd* helpCmd = findCommandByName( sCommands, args[0] );
        if ( helpCmd == NULL )
        {
            TTY_LOG( "unrecognized command: %s\n", args[0] );
            return TTY_CMD_STATUS_OK;
        }

        printCmdShortHelp( helpCmd );

        TTY_LOG( "params:\n" );
        s32 paramCount = getParamCount( helpCmd );
        if ( paramCount > 0 )
        {
            for ( u32 i = 0; i < paramCount; ++i )
            {
                TtyCmdParam* param = &helpCmd->params[i];
                if ( param->name == NULL || param->type == NULL || param->description == NULL )
                    continue;

                static const char* requiredStr[] = { "optional", "required" };
                TTY_LOG( "    %s: %s %s\t\t%s\n", param->name, param->type, 
                    requiredStr[ ( param->flags & TTY_CMD_PARAM_FLAG_OPTIONAL ) == 0 ], param->description );
            }
        }

        if ( helpCmd->flags & TTY_CMD_FLAG_VARARGS )
        {
            TTY_LOG( "    ...any number of arguments\n" );
        }
    }

    return TTY_CMD_STATUS_OK;
}

#define HELP_CMD( name ) \
    TTY_CMD( helpCmd, name, "Displays the command help text", TTY_CMD_FLAG_NONE, \
        TTY_CMD_PARAM( "command", "The command to display info over", TTY_CMD_PARAM_FLAG_OPTIONAL, TTY_CMD_PARAM_TYPE_STRING ) )

static TtyCmd sBuiltinCmds[] =
{
    HELP_CMD( "help" ),
    HELP_CMD( "-h" ),
    HELP_CMD( "--help" ),
    TTY_CMD_END(),
};

static bool processCmd( TtyCmd* cmds, const char* command, const char** args, u32 argc )
{
    TTY_TRACE( "finding cmd\n" );
    TtyCmd* cmd = findCommandByName( cmds, command );
    if ( cmd == NULL )
    {
        TTY_TRACE( "cmd not found\n");
        return false;
    }

    TTY_TRACE( "checking cmd argc\n" );
    s32 paramCount = getParamCount( cmd );
    s32 requiredParamCount = getRequiredParamCount( cmd );
    if ( requiredParamCount != -1 && argc < requiredParamCount )
    {
        TTY_LOG( "command %s expected at least %d arguments, got %d\n", cmd->name, requiredParamCount, argc );
        return true;
    }

    TTY_TRACE( "running cmd handler\n" );
    //assert( cmd->handler );

    char* error;
    TtyCmdStatus status = cmd->handler( cmd, args, argc, &error );
    switch ( status )
    {
        case TTY_CMD_STATUS_OK:
            // do nothing
            break;

        case TTY_CMD_STATUS_INVALID_ARG:
            TTY_LOG( "command %s invalid argument specified: %s\n", cmd->name, error );
            break;

        case TTY_CMD_STATUS_INVALID_ARGC:
            TTY_LOG( "command %s expected %s, got %d\n", cmd->name, error, argc );
            break;

        case TTY_CMD_STATUS_ERROR:
            TTY_LOG( "command %s failed to execute due to an error: %s\n", cmd->name, error );
            break;

        default:
            TTY_LOG( "command %s returned unknown status code %d\n", cmd->name, status );
            break;
    }

    TTY_TRACE( "cmd handled\n" );
    return true;
}

static bool sFirstRun = true;

void ttyCmdProcessLine( TtyCmd* cmds, const char* line )
{
    if ( sFirstRun )
    {
        printCmdListShortHelp();
        sFirstRun = false;
    }

    u32 lineLength = stringGetLength( line );
    if ( lineLength == 0 )
    {
        return;
    }

    TTY_TRACE( "processing line: %s\n", line );

    // trim any trailing whitespace
    TTY_TRACE( "trimming line\n", line );
    char lineTrimmed[1024];
    stringTrim( lineTrimmed, sizeof( lineTrimmed ), line );

    // split the string
    TTY_TRACE( "splitting string\n", line );
    char splitBuffer[1024];
    char* splits[32];
    u32 splitCount;
    stringSplit( splitBuffer, sizeof( splitBuffer ), splits, sizeof( splits ), &splitCount, lineTrimmed, " " );
    if ( splitCount == 0 )
    {
        // need at least a command
        TTY_TRACE( "line has no command\n" );
        return;
    }

    TTY_TRACE( "parsing splits\n" );
    char* command = splits[0];
    const char** args = (const char**)&splits[1];
    u32 argc = splitCount - 1;
    bool handled = processCmd( cmds, command, args, argc );

    if ( !handled )
    {
        // try builtins
        handled = processCmd( sBuiltinCmds, command, args, argc );
        if ( !handled )
            TTY_LOG( "unrecognized command: %s\n", command );
    }
}

void ttyCmdProcess( TtyCmd* cmds )
{
    sCommands = cmds;
    const char* line = readTtyLine();
    ttyCmdProcessLine( cmds, line );
}

static void ttyCmdListenerThreadProc( u64 arg )
{
    TTY_TRACE( "enter listener threadproc\n" );

    TtyCmd* cmds = (TtyCmd*)( uintptr_t )arg;

    while ( true )
    {
        // process commands
        ttyCmdProcess( cmds );

        // sleep for 16 milliseconds (1000 / 60)
        threadSleep( 16 );
    }
}

void ttyCmdStartListenerThread( TtyCmd* cmds )
{
    TTY_TRACE( "creating listener thread\n" );
    threadCreateAndRun( ttyCmdListenerThreadProc, ( u64 )( uintptr_t )cmds, "ttyCmdListener" );
}