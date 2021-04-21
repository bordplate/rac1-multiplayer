#ifndef UTILS_H
#define UTILS_H

#include "common.h"

// MEMORY

void* memoryCopy( void* destination, size_t destinationLength, const void* source, size_t sourceLength, size_t count );
void* memoryClear( void* destination, size_t destinationLength );

//
// CHAR
// 

// Returns whether the character is lowercase.
bool charIsLower( char c );

// Returns whether the character is uppercase.
bool charIsUpper( char c );

// Gets the character in lowercase.
char charToLower( char c );

// Gets the character in uppercase.
char charToUpper( char c );

//
// STRING
//

// Gets the number of characters in the string.
u32 stringLength( const char* s );

// Compares two specified strings, ignoring or honoring their case, and returns an integer that indicates their relative position in the sort order.
s32 stringCompareEx( const char* s, const char* s2, bool ignoreCase );

// Compares two specified strings and returns an integer that indicates their relative position in the sort order.
s32 stringCompare( const char* s, const char* s2 );

// Concatenates one or more strings.
char* stringConcat( char* destination, size_t destinationLength, const char* source );

// Returns a value indicating whether a specified substring occurs within this string.
bool stringContainsEx( const char* s, const char* s2, bool ignoreCase );

// Returns a value indicating whether a specified substring occurs within this string.
bool stringContains( const char* s, const char* s2 );

// Copies a specified number of characters from a specified position in this instance to a specified position in an array of Unicode characters.
char* stringCopyTo( char* destination, size_t destinationLength, const char* source );

// Determines whether the end of this string instance matches a specified string.
bool stringEndsWithEx( const char* s, const char* s2, bool ignoreCase );

// Determines whether the end of this string instance matches a specified string.
bool stringEndsWith( const char* s, const char* s2 );

// Determines whether two String objects have the same value.
bool stringEqualsEx( const char* s, const char* s2, bool ignoreCase );

// Determines whether two String objects have the same value.
bool stringEquals( const char* s, const char* s2 );

// Converts the value of objects to strings based on the formats specified and inserts them into another string.
s32 stringFormat( char* destination, size_t destinationLength, const char* format, ... );

// Reports the zero-based index of the first occurrence of a specified ASCII character or string within this instance. 
// The method returns -1 if the character or string is not found in this instance.
s32 stringIndexOfEx( const char* s, const char* s2, bool ignoreCase );

// Reports the zero-based index of the first occurrence of a specified ASCII character or string within this instance. 
// The method returns -1 if the character or string is not found in this instance.
s32 stringIndexOf( const char* s, const char* s2 );

// Reports the index of the first occurrence in this instance of any character in a specified array of ASCII characters. 
// The method returns -1 if the characters in the array are not found in this instance.
s32 stringIndexOfAnyEx( const char* s, const char* anyOf, bool ignoreCase );

// Reports the index of the first occurrence in this instance of any character in a specified array of ASCII characters. 
// The method returns -1 if the characters in the array are not found in this instance.
s32 stringIndexOfAny( const char* s, const char* anyOf );

// Returns a new string in which a specified string is inserted at a specified index position in this instance.
char* stringInsert( char* destination, size_t destinationLength, const char* s, size_t startIndex, const char* s2 );

// Indicates whether the specified string is null or an empty string ("").
bool stringIsNullOrEmpty( const char* s );

// Indicates whether a specified string is null, empty, or consists only of white-space characters.
bool stringIsNullOrWhitespace( const char* s );

// Concatenates the elements of a specified strings, using the specified separator between each element.
// The last argument must be NULL.
char* stringJoinArgs( char* destination, size_t destinationLength, const char* seperator, ... );

// Concatenates the elements of a specified strings, using the specified separator between each element.
char* stringJoinArray( char* destination, size_t destinationLength, const char* seperator, const char** elements, size_t elementCount );

// Returns the length of the new string in which all occurrences of a specified ASCII character or String in the current string are replaced with another specified ASCII character or String.
char* stringReplaceEx( char* destination, size_t destinationLength, const char* source, const char* oldValue, const char* newValue, bool ignoreCase );

// Returns the length of the new string in which all occurrences of a specified ASCII character or String in the current string are replaced with another specified ASCII character or String.
char* stringReplace( char* destination, size_t destinationLength, const char* source, const char* oldValue, const char* newValue );

// Determines whether the beginning of this string instance matches the specified string.
bool stringStartsWithEx( const char* s, const char* s2, bool ignoreCase );

// Determines whether the beginning of this string instance matches the specified string.
bool stringStartsWith( const char* s, const char* s2 );

// Retrieves a substring from this instance. The substring starts at a specified character position and has a specified length.
char* stringSubstringEx( char* destination, size_t destinationLength, const char* source, size_t startIndex, size_t length );

// Retrieves a substring from this instance. The substring starts at a specified character position and has a specified length.
char* stringSubstring( char* destination, size_t destinationLength, const char* source, size_t startIndex );

// Returns a copy of this string converted to lowercase.
char* stringToLower( char* destination, size_t destinationLength, const char* source );

// Returns a copy of this string converted to uppercase.
char* stringToUpper( char* destination, size_t destinationLength, const char* source );

//
// RANDOM
//

// random prime
#define RANDOM_INITIAL_SEED 0xBBD583E3
#define RANDOM_MAX          0x7FFFFFFF

// Sets the random seed.
void randomSetSeed( u32 seed );

// Generates a random signed integer.
s32 randomInt();

// Generates a random normalized float (0-1)
f32 randomFloat();

// Generates a random signed integer between min (inclusive) and max (inclusive).
s32 randomIntBetween( s32 min, s32 max );

// Generates a random float between min (inclusive) and max (inclusive).
f32 randomFloatBetween( f32 min, f32 max );

#endif