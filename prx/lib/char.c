#include "lib/char.h"

// Returns whether the character is lowercase.
bool charIsLower( char c )
{
    return c >= 'a' && c <= 'z';
}

// Returns whether the character is uppercase.
bool charIsUpper( char c )
{
    return c >= 'A' && c <= 'Z';
}

bool charIsWhitespace( char c )
{
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

// Gets the character in lowercase.
char charToLower( char c )
{
    if ( charIsUpper( c ) )
        return c - ( 'A' - 'a' );
    else
        return c;
}

// Gets the character in uppercase.
char charToUpper( char c )
{
    if ( charIsLower( c ) )
        return c + ( 'A' - 'a' );
    else
        return c;
}

bool charIsDigit( char c )
{
    return c >= '0' && c <= '9';
}

bool charIsLetter( char c )
{
    return ( c >= 'A' && c <= 'Z' ) ||
        ( c >= 'a' && c <= 'z' );
}

bool charIsHexDigit( char c )
{
    return ( c >= 'A' && c <= 'F' ) ||
        ( c >= 'a' && c <= 'f' );
}