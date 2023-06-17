#ifndef STRING_H
#define STRING_H

#include "types.h"
#include "utils.h"
#include "clib.h"

#define nullptr 0

#ifdef __cplusplus

#include "logger.h"
#include "vector.h"

class String {
private:
    char* m_str;
    size_t m_length;

public:
    // Constructors
    String() : m_str(nullptr), m_length(0) { set(""); }
    String(const char* str) : m_str(nullptr), m_length(0) { set(str); }
    String(const String& other) : m_str(nullptr), m_length(0) { set(other.m_str); }

    // Static method for formatting
    static String format(const char* format, ...) {
        va_list args;
        va_start(args, format);
        String string = String();
        string.setf_args(format, args);
        va_end(args);
        return string;
    }
    ~String() { delete[] m_str; }

    // Modifiers
    void set(const char* str) {
        delete[] m_str;
        m_length = strlen(str);
        m_str = new char[m_length + 1];
        strcpy(m_str, str);
    }

    void setf(const char* format, ...) {
        va_list args;
        va_start(args, format);

        setf_args(format, args);

        va_end(args);
    }

    void setf_args(const char* format, va_list args) {
        // Determine the length of the formatted string
        size_t length = vsnprintf(nullptr, 0, format, args);

        // Allocate a buffer for the formatted string
        char* buffer = new char[length + 1];

        // Print the formatted string into the buffer
        vsnprintf(buffer, length + 1, format, args);

        // Set the value of the string using the buffer
        set(buffer);

        // Clean up
        delete[] buffer;
    }

    // Accessors
    size_t length() const { return m_length; }
    const char* c_str() const { return m_str; }

    Vector<String> split(const char* delimiter) {
        Vector<String> splits;
        size_t start = 0;
        size_t end = 0;
        size_t del_len = strlen(delimiter);

        while (end <= m_length) {
            if (strncmp(m_str + end, delimiter, del_len) == 0 || end == m_length) {
                size_t length = end - start;
                char* substr = new char[length + 1];
                strncpy(substr, m_str + start, length);
                substr[length] = '\0';
                splits.push_back(String(substr));
                delete[] substr;
                start = end + del_len;
                end = start;
            } else {
                ++end;
            }
        }

        return splits;
    }

    String slice(int start_index, int end_index) {
        size_t length = end_index - start_index;
        char* substr = new char[end_index - start_index + 1];
        strncpy(substr, m_str + start_index, length);
        substr[length] = '\0';

        String slice_string = String(substr);
        delete[] substr;

        return slice_string;
    }

    // Operators
    String& operator=(const String& other) {
        set(other.m_str);
        return *this;
    }

    String& operator=(const char *other) {
        set(other);
        return *this;
    }

    // Operator for concatenation with another String object
    String& operator+=(const String& other) {
        size_t new_length = m_length + other.m_length;
        char* new_str = new char[new_length + 1];
        strcpy(new_str, m_str);
        strcat(new_str, other.m_str);

        delete[] m_str;
        m_str = new_str;
        m_length = new_length;

        return *this;
    }

    // Operator for concatenation with a C-style string
    String& operator+=(const char* other) {
        String other_str(other);
        return *this += other_str;
    }

    // Operator for concatenation of two String objects, returning a new String
    String operator+(const String& other) const {
        String result(*this);
        result += other;
        return result;
    }

    // Operator for concatenation of a String object and a C-style string, returning a new String
    String operator+(const char* other) const {
        String other_str(other);
        return *this + other_str;
    }

    bool operator==(const String& other) {
        return strcmp(this->c_str(), other.c_str()) == 0;
    }

    char& operator[](size_t index) {
        return m_str[index];
    }

    const char& operator[](size_t index) const {
        return m_str[index];
    }
};

extern "C" {
#endif


/**
 * @brief Gets the number of characters in the string.
 * 
 * @param s The first string. The input string.
 * @return u32 
 */
u32 stringGetLength(const char *s);

/**
 * @brief Compares two specified strings, ignoring or honoring their case, and returns an integer that indicates their relative position in the sort order.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return s32 
 */
s32 stringCompareEx(const char *s, const char *s2, bool ignoreCase);

/**
 * @brief Compares two specified strings and returns an integer that indicates their relative position in the sort order.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @return s32 
 */
s32 stringCompare(const char *s, const char *s2);

/**
 * @brief Concatenates one or more strings.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @return char* The destination buffer pointer.
 */
char *stringConcat(char *destination, size_t destinationLength, const char *source);

/**
 * @brief Returns a value indicating whether a specified substring occurs within this string.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return bool
 */
bool stringContainsEx(const char *s, const char *s2, bool ignoreCase);

/**
 * @brief Returns a value indicating whether a specified substring occurs within this string.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @return bool
 */
bool stringContains(const char *s, const char *s2);

/**
 * @brief Copies a specified number of characters from a specified position in this instance to a specified position in an array of Unicode characters.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @return char* The destination buffer pointer.
 */
char *stringCopyTo(char *destination, size_t destinationLength, const char *source);

/**
 * @brief Determines whether the end of this string instance matches a specified string.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return bool
 */
bool stringEndsWithEx(const char *s, const char *s2, bool ignoreCase);

/**
 * @brief Determines whether the end of this string instance matches a specified string.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @return bool
 */
bool stringEndsWith(const char *s, const char *s2);

/**
 * @brief Determines whether two String objects have the same value.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return bool
 */
bool stringEqualsEx(const char *s, const char *s2, bool ignoreCase);

/**
 * @brief Determines whether two String objects have the same value.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @return bool
 */
bool stringEquals(const char *s, const char *s2);

/**
 * @brief Converts the value of objects to strings based on the formats specified and inserts them into another string.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param format 
 * @param ... 
 * @return s32 
 */
s32 stringFormat(char *destination, size_t destinationLength, const char *format, ...);

/**
 * @brief Reports the zero-based index of the first occurrence of a specified ASCII character or string within this instance. 
 * @param s The first string. 
 * @param s2 The second string. 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return s32 Index of the first occurence, or -1 if the character or string is not found in this instance.
 */
s32 stringIndexOfEx(const char *s, const char *s2, bool ignoreCase);

/**
 * @brief Reports the zero-based index of the first occurrence of a specified ASCII character or string within this instance. 
 * @param s The first string. 
 * @param s2 The second string. 
 * @return s32 Index of the first occurence, or -1 if the character or string is not found in this instance.
 */
s32 stringIndexOf(const char *s, const char *s2);

/**
 * @brief Reports the index of the first occurrence in this instance of any character in a specified array of ASCII characters. 
 * 
 * @param s The first string. 
 * @param anyOf 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return s32 Index of first occurence or -1 if not found.
 */
s32 stringIndexOfAnyEx(const char *s, const char *anyOf, bool ignoreCase);

/**
 * @brief Reports the index of the first occurrence in this instance of any character in a specified array of ASCII characters. 
 * 
 * @param s The first string. 
 * @param anyOf 
 * @return s32 Index of first occurence or -1 if not found.
 */
s32 stringIndexOfAny(const char *s, const char *anyOf);

/**
 * @brief Reports the index of the last occurence of the second string in the first string.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return size_t 
 */
size_t stringLastIndexOfEx(const char *s, const char *s2, bool ignoreCase);

/**
 * @brief Returns a new string in which a specified string is inserted at a specified index position in this instance.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param s The first string. 
 * @param startIndex 
 * @param s2 The second string. 
 * @return char* The destination buffer pointer.
 */
char *stringInsert(char *destination, size_t destinationLength, const char *s, size_t startIndex, const char *s2);

/**
 * @brief Indicates whether the specified string is null or an empty string ("").
 * 
 * @param s The first string. 
 * @return bool
 */
bool stringIsNullOrEmpty(const char *s);

/**
 * @brief  Indicates whether a specified string is null, empty, or consists only of white-space characters.
 * 
 * @param s The first string. 
 * @return bool
 */
bool stringIsNullOrWhitespace(const char *s);

/**
 * @brief Concatenates the elements of a specified strings, using the specified separator between each element.
 * The last argument must be NULL.
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param seperator 
 * @param ... 
 * @return char* The destination buffer pointer.
 */
char *stringJoinArgs(char *destination, size_t destinationLength, const char *seperator, ...);

/**
 * @brief Concatenates the elements of a specified strings, using the specified separator between each element.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param seperator 
 * @param elements 
 * @param elementCount 
 * @return char* The destination buffer pointer.
 */
char *stringJoinArray(char *destination, size_t destinationLength, const char *seperator, const char **elements,
                      size_t elementCount);

/**
 * @brief Returns the length of the new string in which all occurrences of a specified ASCII character or String in the current string are replaced with another specified ASCII character or String.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @param oldValue 
 * @param newValue 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return char* The destination buffer pointer.
 */
char *stringReplaceEx(char *destination, size_t destinationLength, const char *source, const char *oldValue,
                      const char *newValue, bool ignoreCase);

/**
 * @brief Returns the length of the new string in which all occurrences of a specified ASCII character or String in the current string are replaced with another specified ASCII character or String.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @param oldValue 
 * @param newValue 
 * @return char* The destination buffer pointer.
 */
char *stringReplace(char *destination, size_t destinationLength, const char *source, const char *oldValue,
                    const char *newValue);

/**
 * @brief Determines whether the beginning of this string instance matches the specified string.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @param ignoreCase Boolean indicating whether to ignore case or not.
 * @return bool
 */
bool stringStartsWithEx(const char *s, const char *s2, bool ignoreCase);

/**
 * @brief Determines whether the beginning of this string instance matches the specified string.
 * 
 * @param s The first string. 
 * @param s2 The second string. 
 * @return bool
 */
bool stringStartsWith(const char *s, const char *s2);

/**
 * @brief Retrieves a substring from this instance. The substring starts at a specified character position and has a specified length.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @param startIndex 
 * @param length 
 * @return char* The destination buffer pointer.
 */
char *
stringSubstringEx(char *destination, size_t destinationLength, const char *source, size_t startIndex, size_t length);

/**
 * @brief Retrieves a substring from this instance. The substring starts at a specified character position and has a specified length.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @param startIndex 
 * @return char* The destination buffer pointer.
 */
char *stringSubstring(char *destination, size_t destinationLength, const char *source, size_t startIndex);

/**
 * @brief Returns a copy of this string converted to lowercase.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @return char* The destination buffer pointer.
 */
char *stringToLower(char *destination, size_t destinationLength, const char *source);

/**
 * @brief Returns a copy of this string converted to uppercase.
 * 
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.
 * @param source The string to process.
 * @return char* The destination buffer pointer.
 */
char *stringToUpper(char *destination, size_t destinationLength, const char *source);

/**
 * @brief Trims any whitespace characters from the string.
 *
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.The total size of the output buffer.
 * @param source The string to process.The string to process.
 * @return char* The destination buffer pointer.The destination buffer pointer.
 */
char *stringTrim(char *destination, size_t destinationLength, const char *source);

/**
 * @brief Trims any whitespace characters from the left side of the string.
 *
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.The total size of the output buffer.
 * @param source The string to process.The string to process.
 * @return char* The destination buffer pointer.The destination buffer pointer.
 */
char *stringTrimLeft(char *destination, size_t destinationLength, const char *source);

/**
 * @brief Trims any whitespace characters from the right side of the string.
 *
 * @param destination The output buffer to write the result to.
 * @param destinationLength The total size of the output buffer.The total size of the output buffer.
 * @param source The string to process.The string to process.
 * @return char* The destination buffer pointer.The destination buffer pointer.
 */
char *stringTrimRight(char *destination, size_t destinationLength, const char *source);

/**
 * @brief Splits a string using the given seperator.
 * 
 * @param outputBuffer The output buffer to which the split strings are written.
 * @param outputBufferSize The size of the output buffer.
 * @param splits The array of string pointers to where the start of each string.
 * @param splitsBufferSize The size of the string pointer array.
 * @param splitCount The number of total splits
 * @param input The input string
 * @param seperator The seperator string
 */
void stringSplit(char *outputBuffer, size_t outputBufferSize,
                 char **splits, size_t splitsBufferSize, u32 *splitCount,
                 const char *input, const char *seperator);

#ifdef __cplusplus
}
#endif

#endif