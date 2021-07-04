#include "file.h"

#include <sys/syscall.h>
#include <sys/fs.h>

static int sys_fs_open( const char *path, int flags, int *fd, uint64_t mode, const void *arg, uint64_t size )
{
    system_call_6( 801, (u32)path, (u64)flags, (u32)fd, (u64)mode, (u32)arg, (u64)size );
    return_to_user_prog( int );
}

static int sys_fs_read( int fd, void *buf, uint64_t nbytes, uint64_t *nread )
{
    system_call_4( 802, (u64)fd, (u32)buf, (u64)nbytes, (u32)nread );
    return_to_user_prog( int );
}

static int sys_fs_lseek( int fd, int64_t offset, int whence, uint64_t *pos )
{
    system_call_4( 818, (u64)fd, (u64)offset, (u64)whence, (u32)pos );
    return_to_user_prog( int );
}

static int sys_fs_close( int fd )
{
    system_call_1( 804, (u64)fd );
    return_to_user_prog( int );
}

//syscall(SYS_FS_OPEN, "/dev_hdd0/game/NPEA00385/USRDIR/mod/multi/host.txt", O_RDONLY, &fd, 0, 0, 0);

static int fileModeToCellFs( FileMode mode )
{
    switch ( mode )
    {
        case FILE_MODE_READ: return CELL_FS_O_RDONLY;
        case FILE_MODE_WRITE: return CELL_FS_O_WRONLY;
        case FILE_MODE_READ_WRITE: return CELL_FS_O_RDWR;
        default: assert( false && "invalid file mode" );
    }

    return 0;
}

static int cellFsErrorToFileStatus( int err )
{
    switch ( err )
    {
        case CELL_FS_OK: return FILE_STATUS_OK;
        case CELL_FS_ERROR_EEXIST: return FILE_STATUS_NOT_EXISTS;
        default: return FILE_STATUS_OPEN_FAILED;
    }   
}

static int fileSeekModeToCellSeekMode( FileSeekMode mode )
{
    switch ( mode )
    {
        case FILE_SEEK_CURRENT: return CELL_FS_SEEK_CUR;
        case FILE_SEEK_START: return CELL_FS_SEEK_SET;
        case FILE_SEEK_END: return CELL_FS_SEEK_END;
        default: assert( false && "invalid seek mode" );
    }

    return 0;
}

FileStatus fileOpen( FileHandle* handle, const char* path, FileMode mode )
{
    int fd;
    int err = sys_fs_open( path, fileModeToCellFs( mode ), &fd, 0, 0, 0 );
    FileStatus status = cellFsErrorToFileStatus( err );
    *handle = fd;
    return status;
}

FileStatus fileRead( FileHandle handle, u8* buffer, u64 count, u64* readCount )
{
    int err = sys_fs_read( handle, buffer, count, readCount );
    FileStatus status = cellFsErrorToFileStatus( err );
    return status;
}

FileStatus fileWrite( FileHandle handle, u8* buffer, u64 count, u64* writeCount )
{
    // TODO
}

FileStatus fileSeek( FileHandle handle, FileSeekMode mode, u64 offset, u64* position )
{
    int err = sys_fs_lseek( handle, offset, fileSeekModeToCellSeekMode( mode ), position );
    FileStatus status = cellFsErrorToFileStatus( err );
    return status;
}

FileStatus fileTell( FileHandle handle, u64* position )
{
    int err = fileSeek( handle, FILE_SEEK_CURRENT, 0, position );
    FileStatus status = cellFsErrorToFileStatus( err );
    return err;
}

FileStatus fileSize( FileHandle handle, u64* size )
{
    FileStatus status;

    // Save current pos
    u64 tmp;
    status = fileTell( handle, &tmp );
    if ( status != FILE_STATUS_OK ) return status;

    // Seek to end
    u64 len;
    status = fileSeek( handle, FILE_SEEK_END, 0, &len );
    if ( status != FILE_STATUS_OK ) return status;

    // Seek back to old pos
    status = fileSeek( handle, FILE_SEEK_START, tmp, &tmp );
    if ( status != FILE_STATUS_OK ) return status;

    *size = len;
    return status;
}

FileStatus fileClose( FileHandle handle )
{
    int err = sys_fs_close( handle );
    FileStatus status = cellFsErrorToFileStatus( err );
    return status;
}

FileStatus fileReadLine( FileHandle handle, char* buffer, u32 bufferSize, u64* bytesRead, u64* bytesWritten )
{
    char* bufferCursor = buffer;

    *bytesRead = 0;
    while ( true )
    {
        u64 curReadSize;
        FileStatus status = fileRead( handle, bufferCursor, 1, &curReadSize );
        if ( status != FILE_STATUS_OK ) return status;

        *bytesRead += curReadSize;

        if ( *bufferCursor == '\r' )
        {
            // skip return
            continue;
        }
        else if ( *bufferCursor == '\n' )
        {
            // overwrite newline with end of string
            *bufferCursor = 0;
            break;
        }
        else if ( curReadSize == 0 )
        {
            // write end of string
            bufferCursor++;
            *bufferCursor = 0;
            break;
        }

        *bytesWritten += curReadSize;
        bufferCursor += curReadSize;
    }

    return FILE_STATUS_OK;
}