#include "utils.h"

void* memoryCopy( void* destination, size_t destinationLength, const void* source, size_t sourceLength, size_t count )
{
    assert( destinationLength >= count );
    assert( sourceLength >= count );
    memcpy( destination, source, count );
}

void* memoryClear( void* destination, size_t destinationLength )
{
    memset( destination, 0, destinationLength );
}

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

// Gets the character in lowercase.
char charToLower( char c )
{
    if ( charIsUpper( c ) )
        return c - ( 'A'- 'a' );
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

// Gets the number of characters in the string.
u32 stringLength( const char* s )
{
    return strlen( s );
}

static s32 _stringCompareIgnoreCase( const char* s1, const char* s2 )
{
    while ( ( *s1 ) && ( charToLower( *s1 ) == charToLower( *s2 ) ) )
    {
        ++s1;
        ++s2;
    }

    return ( *( unsigned char * )s1 - * ( unsigned char * )s2 );
}

// Compares two specified String objects, ignoring or honoring their case, and returns an integer that indicates their relative position in the sort order.
s32 stringCompareEx( const char* s, const char* s2, bool ignoreCase )
{
    if ( ignoreCase ) return _stringCompareIgnoreCase( s, s2 );
    else              return strcmp( s, s2 );
}

// Compares two specified String objects and returns an integer that indicates their relative position in the sort order.
s32 stringCompare( const char* s, const char* s2 )
{
    return stringCompareEx( s, s2, false );
}

// Concatenates one or more instances of String, or the String representations of the values of one or more instances of Object.
char* stringConcat( char* destination, size_t destinationLength, const char* source )
{
    return strncat( destination, source, destinationLength );
}

s32 stringIndexOfEx( const char* s, const char* s2, bool ignoreCase );

// Returns a value indicating whether a specified substring occurs within this string.
bool stringContainsEx( const char* s, const char* s2, bool ignoreCase )
{
    return stringIndexOfEx( s, s2, ignoreCase ) != -1;
}

// Returns a value indicating whether a specified substring occurs within this string.
bool stringContains( const char* s, const char* s2 )
{
    return stringContainsEx( s, s2, false );
}

// Copies a specified number of characters from a specified position in this instance to a specified position in an array of Unicode characters.
char* stringCopyTo( char* destination, size_t destinationLength, const char* source )
{
    return strncpy( destination, source, destinationLength );
}

// Determines whether the end of this string instance matches a specified string.
bool stringEndsWithEx( const char* s, const char* s2, bool ignoreCase )
{
    s32 idx = stringIndexOfEx( s, s2, ignoreCase );
    if ( idx == -1 ) return false;
    return idx == strlen( s ) - strlen( s2 );
}

// Determines whether the end of this string instance matches a specified string.
bool stringEndsWith( const char* s, const char* s2 )
{
    return stringEndsWithEx( s, s2, false );
}

// Determines whether two String objects have the same value.
bool stringEqualsEx( const char* s, const char* s2, bool ignoreCase )
{
    return stringCompareEx( s, s2, ignoreCase ) == 0;
}

// Determines whether two String objects have the same value.
bool stringEquals( const char* s, const char* s2 )
{
    return stringEqualsEx( s, s2, false );
}

// Converts the value of objects to strings based on the formats specified and inserts them into another string.
s32 stringFormat( char* destination, size_t destinationLength, const char* format, ... )
{
    va_list va;
    va_start( va, format );
    const int ret = vsnprintf( destination, destinationLength, format, va );
    va_end( va );
    return ret;
}

// Reports the zero-based index of the first occurrence of a specified ASCII character or string within this instance. 
// The method returns -1 if the character or string is not found in this instance.
s32 stringIndexOfEx( const char* s, const char* s2, bool ignoreCase )
{
    const char* p = strstr( s, s2 );
    if ( p == NULL ) return -1;
    return p - s; 
}

// Reports the zero-based index of the first occurrence of a specified ASCII character or string within this instance. 
// The method returns -1 if the character or string is not found in this instance.
s32 stringIndexOf( const char* s, const char* s2 )
{
    return stringIndexOfEx( s, s2, false );
}

// Reports the index of the first occurrence in this instance of any character in a specified array of ASCII characters. 
// The method returns -1 if the characters in the array are not found in this instance.
s32 stringIndexOfAnyEx( const char* s, const char* anyOf, bool ignoreCase )
{
    char buffer[2];
    buffer[1] = 0;
    
    s32 anyOfCount = stringLength( anyOf );
    for ( u32 i = 0; i < anyOfCount; ++i )
    {
        buffer[0] = anyOf[i];
        s32 foundIdx = stringIndexOfEx( s, buffer, ignoreCase );
        if ( foundIdx >= 0 )
            return foundIdx;
    }
    
    return -1;
}

// Reports the index of the first occurrence in this instance of any character in a specified array of ASCII characters. 
// The method returns -1 if the characters in the array are not found in this instance.
s32 stringIndexOfAny( const char* s, const char* anyOf )
{
    return stringIndexOfAnyEx( s, anyOf, false );
}

// Returns a new string in which a specified string is inserted at a specified index position in this instance.
char* stringInsert( char* destination, size_t destinationLength, const char* s, size_t startIndex, const char* s2 )
{
    stringCopyTo( destination, destinationLength, s );
    stringCopyTo( destination + startIndex, destinationLength, s2 );
    return destination;
}

// Indicates whether the specified string is null or an empty string ("").
bool stringIsNullOrEmpty( const char* s )
{
    return s == NULL || stringLength( s ) == 0;
}

// Indicates whether a specified string is null, empty, or consists only of white-space characters.
bool stringIsNullOrWhitespace( const char* s )
{
    if ( s == NULL ) return true;
    u32 len = stringLength( s );
    if ( len == 0 ) return true;
    
    for ( u32 i = 0; i < len; ++i )
    {
        char c = s[ i ];
        if ( c != ' ' && c != '\t' && c != '\r' && c != '\n' )
            return false;
    }
    
    return true;
}

// Concatenates the elements of a specified array or the members of a collection, using the specified separator between each element or member.
// The last argument must be NULL.
char* stringJoinArgs( char* destination, size_t destinationLength, const char* seperator, ... )
{
    va_list va;
    va_start( va, seperator );
    
    destination[0] = 0;
    size_t destinationIndex = 0;
    bool first = true;
    while ( true )
    {
        const char* s = va_arg( va, const char* );
        if ( s == NULL )
            break;
            
        if ( !first )
        {
            stringConcat( destination, max( 0, destinationLength - destinationIndex ), seperator );
            destinationIndex += strlen( seperator );
            if ( destinationIndex >= destinationLength )
                break;
        }
            
        stringConcat( destination, max( 0, destinationLength - destinationIndex ), s );
        destinationIndex += strlen( s );
        if ( destinationIndex >= destinationLength )
            break;
        
        first = false;
    }
    
    va_end( va );
    return destination;
}

// Concatenates the elements of a specified array or the members of a collection, using the specified separator between each element or member.
// The last argument must be NULL.
char* stringJoinArray( char* destination, size_t destinationLength, const char* seperator, const char** elements, size_t elementCount )
{
    destination[0] = 0;
    size_t destinationIndex = 0;
    bool first = true;
    for ( size_t i = 0; i < elementCount; ++i )
    {
        const char* s = elements[i];
        if ( !first )
        {
            stringConcat( destination, max( 0, destinationLength - destinationIndex ), seperator );
            destinationIndex += strlen( seperator );
            if ( destinationIndex >= destinationLength )
                break;
        }
            
        stringConcat( destination, max( 0, destinationLength - destinationIndex ), s );
        destinationIndex += strlen( s );
        if ( destinationIndex >= destinationLength )
            break;
        
        first = false;
    }

    return destination;
}

size_t stringLastIndexOf( const char* s, const char* s2 )
{
    size_t sLen = stringLength( s );
    size_t s2Len = stringLength( s2 );
    if ( s2Len > sLen ) return -1;
    size_t startIndex = sLen - s2Len;
    for ( size_t i = startIndex; i >= 0; --i )
    {
        if ( memcmp( s + i, s2, s2Len ) == 0 ) 
            return i;
    }

    return -1;
}

// s32 stringLastIndexOfAny( const char* s, const char* anyOf );
// void stringPadLeft( char* destination, size_t destinationLength, s32 totalWidth, char paddingChar );
// void stringPadRight( char* destination, size_t destinationLength, s32 totalWidth, char paddingChar );

// Returns a new string in which a specified number of characters from the current string are deleted.
// void stringRemove( char* destination, size_t destinationLength, const char* s, s32 startIndex );

// Returns the length of the new string in which all occurrences of a specified ASCII character or String in the current string are replaced with another specified ASCII character or String.
char* stringReplaceEx( char* destination, size_t destinationLength, const char* source, const char* oldValue, const char* newValue, bool ignoreCase )
{
    u32 sourceLength = stringLength( source );
    u32 oldValueLength = stringLength( oldValue );
    u32 newValueLength = stringLength( newValue );
    u32 sourceIndex = 0;
    u32 destinationIndex = 0;
    s32 oldValueIdx = stringIndexOfEx( source, oldValue, ignoreCase );
    while ( oldValueIdx != -1 )
    {
        u32 sourceCopyLength = ( oldValueIdx - sourceIndex );
        memoryCopy( destination + destinationIndex, destinationLength - destinationIndex, source + sourceIndex, sourceLength - sourceIndex, sourceCopyLength );
        destinationIndex += sourceCopyLength;
        sourceIndex += sourceCopyLength;
        
        memoryCopy( destination + destinationIndex, destinationLength - destinationIndex, newValue, newValueLength, newValueLength );
        destinationIndex += newValueLength;
        sourceIndex += oldValueLength;
        
        oldValueIdx = stringIndexOfEx( source + sourceIndex, oldValue, ignoreCase );
    }
    
    assert( destinationIndex < destinationLength );
    destination[ destinationIndex++ ] = 0;
    return destination;
}

char* stringReplace( char* destination, size_t destinationLength, const char* source, const char* oldValue, const char* newValue )
{
    return stringReplaceEx( destination, destinationLength, source, oldValue, newValue, false );
}

// Determines whether the beginning of this string instance matches the specified string.
bool stringStartsWithEx( const char* s, const char* s2, bool ignoreCase )
{
    return stringIndexOfEx( s, s2, ignoreCase ) == 0;
}

// Determines whether the beginning of this string instance matches the specified string.
bool stringStartsWith( const char* s, const char* s2 )
{
    return stringStartsWithEx( s, s2, false );
}

// Retrieves a substring from this instance. The substring starts at a specified character position and has a specified length.
char* stringSubstringEx( char* destination, size_t destinationLength, const char* source, size_t startIndex, size_t length )
{
    u32 sourceLength = stringLength( source );
    assert( startIndex < sourceLength );
    assert( startIndex + length <= sourceLength );
    memoryCopy( destination, destinationLength, source + startIndex, sourceLength - startIndex, length );
    destination[ length ] = 0;
    return destination;
}

// // Retrieves a substring from this instance. The substring starts at a specified character position and has a specified length.
char* stringSubstring( char* destination, size_t destinationLength, const char* source, size_t startIndex )
{
    return stringSubstringEx( destination, destinationLength, source, startIndex, stringLength( source ) - startIndex );
}

// Returns a copy of this string converted to lowercase.
char* stringToLower( char* destination, size_t destinationLength, const char* source )
{
    u32 sourceLength = stringLength( source );
    assert( destinationLength >= sourceLength + 1 );
    
    for ( u32 i = 0; i < sourceLength; ++i )
        destination[ i ] = charToLower( source[i] );
        
    destination[ sourceLength ] = 0;
    return destination;
}

// // Returns a copy of this string converted to uppercase.
char* stringToUpper( char* destination, size_t destinationLength, const char* source )
{
    u32 sourceLength = stringLength( source );
    assert( destinationLength >= sourceLength + 1 );
    
    for ( u32 i = 0; i < sourceLength; ++i )
        destination[ i ] = charToUpper( source[i] );
        
    destination[ sourceLength ] = 0;
    return destination;
}

// int main()
// {
//     assert( stringLength("hello") == 5 );
//     assert( stringStartsWith( "hello world", "hello " ) == true );
//     assert( stringEndsWith( "hello world", "o world" ) == true );
//     assert( stringIndexOf( "hello", "e" ) == 1 );
//     assert( stringIndexOfAny( "hello", "xyze" ) == 1 );
    
//     char buffer[1024];
//     stringInsert( buffer, sizeof(buffer), "hello world", 6, "warudo" );
//     assert( stringEquals( buffer, "hello warudo" ) );
    
//     stringJoin( buffer, sizeof( buffer ), "-", "hello", "world", NULL );
//     puts( buffer );
//     assert( stringEquals( buffer, "hello-world" ) );
    
//     stringReplace( buffer, sizeof( buffer ), "hello world", "world", "warudo" );
//     puts( buffer );
    
//     char* substring = stringSubstring( buffer, sizeof( buffer ), "hello world", 6 );
//     printf( "%s[END]\n", substring );
//     assert( stringEquals( substring, "world" ) );
    
//     assert( stringEquals( stringToUpper( buffer, sizeof( buffer ), "test" ), "TEST" ) );
//     assert( stringEquals( stringToLower( buffer, sizeof( buffer ), "TEST" ), "test" ) );
    
//     assert( stringIsNullOrWhitespace( " \t" ) == true );
//     assert( stringIsNullOrWhitespace( "        " ) == true );
//     assert( stringIsNullOrWhitespace( "        a" ) == false );

//     printf( "done\n" );
//     return 0;
// }

u32 sRandomSeed = RANDOM_INITIAL_SEED;

void randomSetSeed( u32 seed )
{
    sRandomSeed = seed;
}

u32 randomGetSeed()
{
    return sRandomSeed;
}

s32 randomInt()
{
    s32 a = 0x41A7;
    sRandomSeed = ( a * sRandomSeed ) % RANDOM_MAX;
    return sRandomSeed;
}

f32 randomFloat()
{
    return (f32)randomInt() / RANDOM_MAX;
}

s32 randomIntBetween( s32 min, s32 max )
{
    return (s32)( min + ( randomFloat() * ( 1 + max - min ) ) );
}

f32 randomFloatBetween( f32 min, f32 max )
{
    return (f32)( min + ( randomFloat() * ( max - min ) ) );
}

/*
Copyright 2018 Dominik Liebler

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
void hexDump( char *desc, void *addr, u32 len ) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset.
            printf("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = pc[i];
        }

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", buff);
}
