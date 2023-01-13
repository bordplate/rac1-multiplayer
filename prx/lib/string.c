#include "lib/string.h"
#include "types.h"

static s32 _stringCompareIgnoreCase( const char* s1, const char* s2 )
{
    //while ( ( *s1 ) && ( charToLower( *s1 ) == charToLower( *s2 ) ) )
    //{
    //    ++s1;
    //    ++s2;
    //}
//
    //return ( *( unsigned char* )s1 - *( unsigned char* )s2 );
    return 0;
}

// Gets the number of characters in the string.
u32 stringGetLength( const char* s )
{
    return strlen( s );
}

// Compares two specified String objects, ignoring or honoring their case, and returns an integer that indicates their relative position in the sort order.
s32 stringCompareEx( const char* s, const char* s2, bool ignoreCase )
{
    if ( ignoreCase )
        return _stringCompareIgnoreCase( s, s2 );
    else
        return strcmp( s, s2 );
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
    s32 idx = stringLastIndexOfEx( s, s2, ignoreCase );
    if ( idx == -1 )
        return false;
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
// s32 stringIndexOfEx( const char* s, const char* s2, bool ignoreCase )
// {
//     const char* p = strstr( s, s2 );
//     if ( p == NULL )
//         return -1;
//     return p - s;
// }

// Reports the zero-based index of the first occurrence of a specified ASCII character or string within this instance.
// The method returns -1 if the character or string is not found in this instance.
s32 stringIndexOfEx( const char* s, const char* s2, bool ignoreCase )
{
    //u32 sLen = stringGetLength( s );
    //u32 s2Len = stringGetLength( s2 );
//
    //for ( u32 i = 0; i < sLen; ++i )
    //{
    //    if ( i + s2Len > sLen )
    //        break;
//
    //    bool matches;
    //    for ( u32 j = 0; j < s2Len; ++j )
    //    {
    //        if ( ignoreCase )
    //            matches = charToLower( s[ i + j ] ) == charToLower( s2[ j ] );
    //        else
    //            matches = s[ i + j ] == s2[ j ];
//
    //        if ( !matches )
    //            break;
    //    }
//
    //    if ( matches )
    //        return i;
    //}

    return -1;
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
    char buffer[ 2 ];
    buffer[ 1 ] = 0;

    s32 anyOfCount = stringGetLength( anyOf );
    for ( u32 i = 0; i < anyOfCount; ++i )
    {
        buffer[ 0 ] = anyOf[ i ];
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
    return s == NULL || stringGetLength( s ) == 0;
}

// Indicates whether a specified string is null, empty, or consists only of white-space characters.
bool stringIsNullOrWhitespace( const char* s )
{
    if ( s == NULL )
        return true;
    u32 len = stringGetLength( s );
    if ( len == 0 )
        return true;

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

    destination[ 0 ] = 0;
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
    destination[ 0 ] = 0;
    size_t destinationIndex = 0;
    bool first = true;
    for ( size_t i = 0; i < elementCount; ++i )
    {
        const char* s = elements[ i ];
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

size_t stringLastIndexOfEx( const char* s, const char* s2, bool ignoreCase )
{
    //size_t sLen = stringGetLength( s );
    //size_t s2Len = stringGetLength( s2 );
    //if ( s2Len > sLen )
    //    return -1;
//
    //size_t startIndex = sLen - s2Len;
    //for ( size_t i = startIndex; i >= 0; --i )
    //{
    //    if ( i + s2Len > sLen )
    //        break;
//
    //    bool matches;
    //    for ( u32 j = 0; j < s2Len; ++j )
    //    {
    //        if ( ignoreCase )
    //            matches = charToLower( s[ i + j ] ) == charToLower( s2[ j ] );
    //        else
    //            matches = s[ i + j ] == s2[ j ];
//
    //        if ( !matches )
    //            break;
    //    }
//
    //    if ( matches )
    //        return i;
    //}

    return -1;
}

size_t stringLastIndexOf( const char* s, const char* s2 )
{
    return stringLastIndexOfEx( s, s2, false );
}

char* stringTrimLeft( char* destination, size_t destinationLength, const char* source )
{
    //// count whitespace chars on the left
    //u32 sourceLength = stringGetLength( source );
    //u32 wsCount = 0;
    //for ( u32 i = 0; i < sourceLength; ++i )
    //{
    //    if ( charIsWhitespace( source[ i ] ) )
    //        wsCount++;
    //    else
    //        break;
    //}
//
    //return stringSubstring( destination, destinationLength, source, wsCount );
    return 0;
}

char* stringTrimRight( char* destination, size_t destinationLength, const char* source )
{
    //// count whitespace chars on the right
    //u32 sourceLength = stringGetLength( source );
    //u32 wsCount = 0;
    //for ( u32 i = sourceLength - 1; i >= 0; --i )
    //{
    //    if ( charIsWhitespace( source[ i ] ) )
    //        wsCount++;
    //    else
    //        break;
    //}
//
    //return stringSubstringEx( destination, destinationLength, source, 0, sourceLength - wsCount );
    return 0;
}

char* stringTrim( char* destination, size_t destinationLength, const char* source )
{
    stringTrimLeft( destination, destinationLength, source );
    stringTrimRight( destination, destinationLength, destination );
    return destination;
}


// s32 stringLastIndexOfAny( const char* s, const char* anyOf );
// void stringPadLeft( char* destination, size_t destinationLength, s32 totalWidth, char paddingChar );
// void stringPadRight( char* destination, size_t destinationLength, s32 totalWidth, char paddingChar );

// Returns a new string in which a specified number of characters from the current string are deleted.
// void stringRemove( char* destination, size_t destinationLength, const char* s, s32 startIndex );

// Returns the length of the new string in which all occurrences of a specified ASCII character or String in the current string are replaced with another specified ASCII character or String.
char* stringReplaceEx( char* destination, size_t destinationLength, const char* source, const char* oldValue, const char* newValue, bool ignoreCase )
{
    u32 sourceLength = stringGetLength( source );
    u32 oldValueLength = stringGetLength( oldValue );
    u32 newValueLength = stringGetLength( newValue );
    u32 sourceIndex = 0;
    u32 destinationIndex = 0;
    s32 oldValueIdx = stringIndexOfEx( source, oldValue, ignoreCase );
    while ( oldValueIdx != -1 )
    {
        u32 sourceCopyLength = ( oldValueIdx - sourceIndex );
        memoryCopy( destination + destinationIndex, destinationLength - destinationIndex, source + sourceIndex, sourceCopyLength );
        destinationIndex += sourceCopyLength;
        sourceIndex += sourceCopyLength;

        memoryCopy( destination + destinationIndex, destinationLength - destinationIndex, newValue, newValueLength );
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
    u32 sourceLength = stringGetLength( source );
    assert( startIndex < sourceLength );
    assert( startIndex + length <= sourceLength );
    memoryCopy( destination, destinationLength, source + startIndex, length );
    destination[ length ] = 0;
    return destination;
}

// // Retrieves a substring from this instance. The substring starts at a specified character position and has a specified length.
char* stringSubstring( char* destination, size_t destinationLength, const char* source, size_t startIndex )
{
    return stringSubstringEx( destination, destinationLength, source, startIndex, stringGetLength( source ) - startIndex );
}

// Returns a copy of this string converted to lowercase.
char* stringToLower( char* destination, size_t destinationLength, const char* source )
{
    //u32 sourceLength = stringGetLength( source );
    //assert( destinationLength >= sourceLength + 1 );
//
    //for ( u32 i = 0; i < sourceLength; ++i )
    //    destination[ i ] = charToLower( source[ i ] );
//
    //destination[ sourceLength ] = 0;
    return destination;
}

// // Returns a copy of this string converted to uppercase.
char* stringToUpper( char* destination, size_t destinationLength, const char* source )
{
    //u32 sourceLength = stringGetLength( source );
    //assert( destinationLength >= sourceLength + 1 );
//
    //for ( u32 i = 0; i < sourceLength; ++i )
    //    destination[ i ] = charToUpper( source[ i ] );
//
    //destination[ sourceLength ] = 0;
    return destination;
}

static void stringSplitRange( char* outputBuffer, size_t outputBufferSize, u32* outputBufferIndex, 
    char** splits, size_t splitsBufferSize, u32* splitIndex,
    u32 seperatorIndex, u32 seperatorLength, u32* splitStartIndex, const char* input )
{
    u32 splitLength = seperatorIndex - *splitStartIndex;
    u32 splitLengthTerm = splitLength + 1;

    assert( *outputBufferIndex + splitLengthTerm < outputBufferSize );
    memcpy( outputBuffer + *outputBufferIndex, input + *splitStartIndex, splitLength );

    // set split pointer to buffer
    assert( *splitIndex < splitsBufferSize );
    splits[ *splitIndex ] = &outputBuffer[ *outputBufferIndex ];
    *splitIndex = *splitIndex + 1;

    // write null terminator to buffer
    *outputBufferIndex = *outputBufferIndex + splitLength;
    outputBuffer[ *outputBufferIndex ] = 0;
    *outputBufferIndex = *outputBufferIndex + 1;
    
    // next split begins right after seperator
    *splitStartIndex = seperatorIndex + seperatorLength;
}

void stringSplit( char* outputBuffer, size_t outputBufferSize, 
    char** splits, size_t splitsBufferSize, u32* splitCount,
    const char* input, const char* seperator )
{
    u32 inputLength = stringGetLength( input );
    u32 seperatorLength = stringGetLength( seperator );
    
    u32 inputSplitStartIndex = 0;
    u32 splitIndex = 0;
    u32 outputBufferIndex = 0;

    for ( u32 i = 0; i < inputLength; ++i )
    {
        if ( i + seperatorLength > inputLength - 1 )
            break;
        
        // find occurence of seperator in string
        bool matches = true;
        for ( u32 j = 0; j < seperatorLength; ++j )
        {
            matches = input[i + j] == seperator[ j ];
            if ( !matches )
                break;
        }

        if ( matches )
        {
            stringSplitRange( outputBuffer, outputBufferSize, &outputBufferIndex, 
                splits, splitsBufferSize, &splitIndex,
                i, seperatorLength, &inputSplitStartIndex, input );
        }
    }
    
    stringSplitRange( outputBuffer, outputBufferSize, &outputBufferIndex, 
        splits, splitsBufferSize, &splitIndex,
        inputLength, 0, &inputSplitStartIndex, input );
    
    *splitCount = splitIndex;
}

// int main()
// {
//     assert( stringGetLength("hello") == 5 );
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