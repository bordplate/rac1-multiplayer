#include "utils.h"

void* memoryCopy( void* destination, size_t destinationLength, const void* source, size_t count )
{
    assert( destinationLength >= count );
    memcpy( destination, source, count );
}

void* memoryClear( void* destination, size_t destinationLength )
{
    memset( destination, 0, destinationLength );
}

s32 intParse( const char* s )
{
    u32 len = stringGetLength( s );
    u32 startIndex = 0;
    u32 base = 10;
    u32 sign = 1;

    if ( stringStartsWith( s, "-" ) )
    {
        // negative
        startIndex += 1;
        sign = -1;
        s++;
    }

    if ( stringStartsWith( s, "0x" ) )
    {
        // hex
        startIndex += 2;
        base = 16;
        s += 2;
    }

    u32 position = len - startIndex - 1;
    u32 parsedValue = 0;
    while ( true )
    {
        char c = *s++;
        u32 cval = 0;
        if ( charIsDigit( c ) )
            cval = c - '0';
        else if ( base == 16 && charIsHexDigit( c ) )
            cval = charToLower( c ) - 'a' + 10;
        else
            break;

        parsedValue += cval * pow( base, position );
        position--;
    }

    parsedValue *= sign;

    return parsedValue;
}

u32 uintParse( const char* s )
{
    return ( u32 )intParse( s );
}

/*
 * strtod implementation.
 * author: Yasuhiro Matsumoto
 * license: public domain
 */

static
char*
skipwhite( q ) char* q;
{
    char* p = q;
    while ( charIsWhitespace( *p ) )
        ++p;
    return p;
}
#define vim_isdigit(x) charIsDigit(x)

static
double
vim_strtod( str, end )
const char* str;
char** end;
{
    double d = 0.0;
    int sign;
    int n = 0;
    const char* p, * a;

    a = p = str;
    p = skipwhite( p );

    /* decimal part */
    sign = 1;
    if ( *p == '-' )
    {
        sign = -1;
        ++p;
    }
    else if ( *p == '+' )
        ++p;
    if ( vim_isdigit( *p ) )
    {
        d = ( double )( *p++ - '0' );
        while ( *p && vim_isdigit( *p ) )
        {
            d = d * 10.0 + ( double )( *p - '0' );
            ++p;
            ++n;
        }
        a = p;
    }
    else if ( *p != '.' )
        goto done;
    d *= sign;

    /* fraction part */
    if ( *p == '.' )
    {
        double f = 0.0;
        double base = 0.1;
        ++p;

        if ( vim_isdigit( *p ) )
        {
            while ( *p && vim_isdigit( *p ) )
            {
                f += base * ( *p - '0' );
                base /= 10.0;
                ++p;
                ++n;
            }
        }
        d += f * sign;
        a = p;
    }

    /* exponential part */
    if ( ( *p == 'E' ) || ( *p == 'e' ) )
    {
        int e = 0;
        ++p;

        sign = 1;
        if ( *p == '-' )
        {
            sign = -1;
            ++p;
        }
        else if ( *p == '+' )
            ++p;

        if ( vim_isdigit( *p ) )
        {
            while ( *p == '0' )
                ++p;
            if ( *p == '\0' )
                --p;
            e = ( int )( *p++ - '0' );
            while ( *p && vim_isdigit( *p ) )
            {
                e = e * 10 + ( int )( *p - '0' );
                ++p;
            }
            e *= sign;
        }
        else if ( !vim_isdigit( *( a - 1 ) ) )
        {
            a = str;
            goto done;
        }
        else if ( *p == 0 )
            goto done;

        if ( d == 2.2250738585072011 && e == -308 )
        {
            d = 0.0;
            a = p;
            goto done;
        }
        if ( d == 2.2250738585072012 && e <= -308 )
        {
            d *= 1.0e-308;
            a = p;
            goto done;
        }
        d *= pow( 10.0, ( double )e );
        a = p;
    }
    else if ( p > str && !vim_isdigit( *( p - 1 ) ) )
    {
        a = str;
        goto done;
    }

done:
    if ( end )
        *end = ( char* )a;
    return d;
}

f32 floatParse( const char* s )
{
    char* end;
    f64 val = vim_strtod( s, &end );
    return ( f32 )val;
}



/*
Copyright 2018 Dominik Liebler

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
void hexDump( char* desc, void* addr, u32 len )
{
    int i;
    unsigned char buff[ 17 ];
    unsigned char* pc = ( unsigned char* )addr;

    // Output description if given.
    if ( desc != NULL )
        printf( "%s:\n", desc );

    // Process every byte in the data.
    for ( i = 0; i < len; i++ )
    {
        // Multiple of 16 means new line (with line offset).

        if ( ( i % 16 ) == 0 )
        {
            // Just don't print ASCII for the zeroth line.
            if ( i != 0 )
                printf( "  %s\n", buff );

            // Output the offset.
            printf( "  %04x ", i );
        }

        // Now the hex code for the specific character.
        printf( " %02x", pc[ i ] );

        // And store a printable ASCII character for later.
        if ( ( pc[ i ] < 0x20 ) || ( pc[ i ] > 0x7e ) )
        {
            buff[ i % 16 ] = '.';
        }
        else
        {
            buff[ i % 16 ] = pc[ i ];
        }

        buff[ ( i % 16 ) + 1 ] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ( ( i % 16 ) != 0 )
    {
        printf( "   " );
        i++;
    }

    // And print the final ASCII bit.
    printf( "  %s\n", buff );
}
