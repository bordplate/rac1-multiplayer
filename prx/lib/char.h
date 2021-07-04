#ifndef CHAR_H
#define CHAR_H

#include "lib/common.h"

/**
 * @brief Returns whether the character is lowercase.
 * 
 * @param c The character value.
 * @return true The character is lowercase.
 * @return false The character is uppercase.
 */
bool charIsLower( char c );

/**
 * @brief Returns whether the character is uppercase.
 * 
 * @param c The character value.
 * @return true The character is uppercase.
 * @return false The character is lowercase.
 */
bool charIsUpper( char c );

/**
 * @brief Returns whether the character is a whitespace character.
 * 
 * @param c The character.
 * @return true The character is a whitespace character.
 * @return false The character is not a whitespace character.
 */
bool charIsWhitespace( char c );

/**
 * @brief Returns the character in lowercase.
 * 
 * @param c The character value.
 * @return char The character value converted to lowercase, if applicable.
 */
char charToLower( char c );

/**
 * @brief Returns the character in uppercase.
 * 
 * @param c The character value.
 * @return char The character value converted to uppercase, if applicable.
 */
char charToUpper( char c );

/**
 * @brief Returns whether the character is a digit (0-9),
 * 
 * @param c The character value.
 * @return true The character is a digit.
 * @return false The character is not a digit.
 */
bool charIsDigit( char c );

/**
 * @brief Returns whether the character is a letter (a-zA-Z)
 * 
 * @param c The character value.
 * @return true The character is a letter.
 * @return false The character is not a letter.
 */
bool charIsLetter( char c );

/**
 * @brief Returns whether the character is a hex digit (A-F).
 * 
 * @param c The character value.
 * @return true The character is a hex digit.
 * @return false The character is not a hex digit.
 */
bool charIsHexDigit( char c );

#endif