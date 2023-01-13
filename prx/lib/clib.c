#include <sys/tty.h>
#include "clib.h"

// stdlib
int abs( int j )
{
    return ( j >= 0 ) ? j : -j;
}

void * bsearch( const void * key, const void * base, size_t nmemb, size_t size, int ( *compar )( const void *, const void * ) )
{
    const void * pivot;
    int rc;
    size_t corr;

    while ( nmemb )
    {
        /* algorithm needs -1 correction if remaining elements are an even number. */
        corr = nmemb % 2;
        nmemb /= 2;
        pivot = ( const char * )base + ( nmemb * size );
        rc = compar( key, pivot );

        if ( rc > 0 )
        {
            base = ( const char * )pivot + size;
            /* applying correction */
            nmemb -= ( 1 - corr );
        }
        else if ( rc == 0 )
        {
            return ( void * )pivot;
        }
    }

    return NULL;
}

long int labs( long int j )
{
    return ( j >= 0 ) ? j : -j;
}

int _PDCLIB_seed = 0xBBD583E3;

int rand( void )
{
    _PDCLIB_seed = _PDCLIB_seed * 1103515245 + 12345;
    return ( int )( _PDCLIB_seed / 65536 ) % 32768;
}

void srand( unsigned int seed )
{
    _PDCLIB_seed = seed;
}

// string
void * memchr( const void * s, int c, size_t n )
{
    const unsigned char * p = ( const unsigned char * ) s;

    while ( n-- )
    {
        if ( *p == ( unsigned char ) c )
        {
            return ( void * ) p;
        }

        ++p;
    }

    return NULL;
}

int memcmp( const void * s1, const void * s2, size_t n )
{
    const unsigned char * p1 = ( const unsigned char * ) s1;
    const unsigned char * p2 = ( const unsigned char * ) s2;

    while ( n-- )
    {
        if ( *p1 != *p2 )
        {
            return *p1 - *p2;
        }

        ++p1;
        ++p2;
    }

    return 0;
}

void * memcpy( void * s1, const void * s2, size_t n )
{
    char * dest = ( char * ) s1;
    const char * src = ( const char * ) s2;

    while ( n-- )
    {
        *dest++ = *src++;
    }

    return s1;
}

void * memmove( void * s1, const void * s2, size_t n )
{
    char * dest = ( char * ) s1;
    const char * src = ( const char * ) s2;

    if ( dest <= src )
    {
        while ( n-- )
        {
            *dest++ = *src++;
        }
    }
    else
    {
        src += n;
        dest += n;

        while ( n-- )
        {
            *--dest = *--src;
        }
    }

    return s1;
}

void * memset( void * s, int c, size_t n )
{
    unsigned char * p = ( unsigned char * ) s;

    while ( n-- )
    {
        *p++ = ( unsigned char ) c;
    }

    return s;
}

char * strcat( char * s1, const char * s2 )
{
    char * rc = s1;

    if ( *s1 )
    {
        while ( *++s1 )
        {
            /* EMPTY */
        }
    }

    while ( ( *s1++ = *s2++ ) )
    {
        /* EMPTY */
    }

    return rc;
}

char * strchr( const char * s, int c )
{
    do
    {
        if ( *s == ( char ) c )
        {
            return ( char * ) s;
        }
    } while ( *s++ );

    return NULL;
}

int strcmp( const char * s1, const char * s2 )
{
    while ( ( *s1 ) && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
    }

    return ( *( unsigned char * )s1 - * ( unsigned char * )s2 );
}

int strcoll( const char * s1, const char * s2 )
{
    /* FIXME: This should access _PDCLIB_lc_collate. */
    return strcmp( s1, s2 );
}

char * strcpy( char * s1, const char * s2 )
{
    char * rc = s1;

    while ( ( *s1++ = *s2++ ) )
    {
        /* EMPTY */
    }

    return rc;
}

size_t strcspn( const char * s1, const char * s2 )
{
    size_t len = 0;
    const char * p;

    while ( s1[len] )
    {
        p = s2;

        while ( *p )
        {
            if ( s1[len] == *p++ )
            {
                return len;
            }
        }

        ++len;
    }

    return len;
}

size_t strlen( const char * s )
{
    size_t rc = 0;

    while ( s[rc] )
    {
        ++rc;
    }

    return rc;
}

char * strncat( char * s1, const char * s2, size_t n )
{
    char * rc = s1;

    while ( *s1 )
    {
        ++s1;
    }

    while ( n && ( *s1++ = *s2++ ) )
    {
        --n;
    }

    if ( n == 0 )
    {
        *s1 = '\0';
    }

    return rc;
}

int strncmp( const char * s1, const char * s2, size_t n )
{
    while ( n && *s1 && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }

    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return ( *( unsigned char * )s1 - * ( unsigned char * )s2 );
    }
}

char * strncpy( char * s1, const char *  s2, size_t n )
{
    char * rc = s1;

    while ( n && ( *s1++ = *s2++ ) )
    {
        /* Cannot do "n--" in the conditional as size_t is unsigned and we have
           to check it again for >0 in the next loop below, so we must not risk
           underflow.
        */
        --n;
    }

    /* Checking against 1 as we missed the last --n in the loop above. */
    while ( n-- > 1 )
    {
        *s1++ = '\0';
    }

    return rc;
}

char * strpbrk( const char * s1, const char * s2 )
{
    const char * p1 = s1;
    const char * p2;

    while ( *p1 )
    {
        p2 = s2;

        while ( *p2 )
        {
            if ( *p1 == *p2++ )
            {
                return ( char * ) p1;
            }
        }

        ++p1;
    }

    return NULL;
}

char * strrchr( const char * s, int c )
{
    size_t i = 0;

    while ( s[i++] )
    {
        /* EMPTY */
    }

    do
    {
        if ( s[--i] == ( char ) c )
        {
            return ( char * ) s + i;
        }
    } while ( i );

    return NULL;
}

size_t strspn( const char * s1, const char * s2 )
{
    size_t len = 0;
    const char * p;

    while ( s1[ len ] )
    {
        p = s2;

        while ( *p )
        {
            if ( s1[len] == *p )
            {
                break;
            }

            ++p;
        }

        if ( ! *p )
        {
            return len;
        }

        ++len;
    }

    return len;
}

char * strstr( const char * s1, const char * s2 )
{
    const char * p1 = s1;
    const char * p2;

    while ( *s1 )
    {
        p2 = s2;

        while ( *p2 && ( *p1 == *p2 ) )
        {
            ++p1;
            ++p2;
        }

        if ( ! *p2 )
        {
            return ( char * ) s1;
        }

        ++s1;
        p1 = s1;
    }

    return NULL;
}

size_t strxfrm( char * s1, const char * s2, size_t n )
{
    size_t len = strlen( s2 );

    if ( len < n )
    {
        /* Cannot use strncpy() here as the filling of s1 with '\0' is not part
           of the spec.
        */
        /* FIXME: This should access _PDCLIB_lc_collate. */
        while ( n-- && ( *s1++ = ( unsigned char )*s2++ ) )
        {
            /* EMPTY */
        }
    }

    return len;
}

double pow( double x, double y )
{
    if (y == 0)
    {
        return 1;
    }
    else if (y < 0)
    {
        return 1 / pow(x, -y);
    }
    else if ((int)y % 2 == 0)
    {
        float halfPow = pow(x, y / 2);
        return halfPow * halfPow;
    }
    else
    {
        return x * pow(x, y - 1);
    }
}

void _Assert( const char* pCond, const char* pFile, s32 line )
{
    //printf( "%s:%d assertion failed! %s\n", pFile, line, pCond );
}

// for printf implementation
/*extern "C"*/ void _putchar( char c )
{
    static u8 buffer[1024];
    static s32 bufferIndex = 0;
    if ( c == 0 ) return;

    assert( bufferIndex < sizeof( buffer ) );
    buffer[ bufferIndex++ ] = c;

    if ( c == '\n' )
    {
        // flush
        u32 writeLen;
        sys_tty_write( SYS_TTYP_PPU_STDOUT, buffer, bufferIndex, &writeLen );
        memset( buffer, sizeof( buffer ), 0 );
        bufferIndex = 0;
    }
}
