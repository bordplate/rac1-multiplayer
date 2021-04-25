#ifndef CHAR_H
#define CHAR_H

#include "lib/common.h"

// Returns whether the character is lowercase.
bool charIsLower( char c );

// Returns whether the character is uppercase.
bool charIsUpper( char c );

/**
 * @brief Returns whether the character is a whitespace character.
 * 
 * @param c The character.
 * @return true The character is a whitespace character.
 * @return false The character is not a whitespace character.
 */
bool charIsWhitespace( char c );

// Gets the character in lowercase.
char charToLower( char c );

// Gets the character in uppercase.
char charToUpper( char c );

bool charIsDigit( char c );

bool charIsLetter( char c );

bool charIsHexDigit( char c );

#endif