#include "lib/ttycmd.h"

#define TTY_TRACE_LOG 0
#define TTY_LOG( msg, ... ) printf( "ttycmd: " msg, ##__VA_ARGS__ )
#if TTY_TRACE_LOG
#define TTY_TRACE( msg, ... ) printf( "ttycmd: " msg, ##__VA_ARGS__ )
#else
#define TTY_TRACE( msg, ... ) do {} while ( false )
#endif

void ttyCmdProcessLine( TtyCmd* cmds, const char* line )
{
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

    TtyCmd* cmd = cmds;
    bool handled = false;
    while ( cmd->name != NULL )
    {
        TTY_TRACE( "checking cmd\n" );

        if ( stringEqualsEx( command, cmd->name, true ) )
        {
            TTY_TRACE( "checking cmd argc\n" );
            if ( cmd->argc != -1 && cmd->argc != argc )
            {
                TTY_LOG( "command %s expected %d arguments, got %d\n", cmd->name, cmd->argc, argc );
                return;
            }

            TTY_TRACE( "running cmd handler\n" );
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
            handled = true;
            break;
        }

        cmd++;
    }

    if ( !handled )
    {
        TTY_LOG( "unrecognized command: %s\n", command );
    }
}

void ttyCmdProcess( TtyCmd* cmds )
{
    const char* line = ttyReadLine();
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