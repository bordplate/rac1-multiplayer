#ifndef CLIB_H
#define CLIB_H

#include "types.h"
#include <stddef.h>

#define XSTRINGIZE(s) STRINGIZE(s)
#define STRINGIZE(s) #s

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// assert.h
/* If NDEBUG is set, assert() is a null operation. */
#undef assert

void _Assert( const char* pCond, const char* pFile, int line );

#ifdef NDEBUG
#define assert( ignore ) ( (void) 0 )
#else
#define assert( cond ) \
    do { if ( !(cond) ) { _Assert( XSTRINGIZE(cond), __FILE__, __LINE__ ); }} while ( false )
#endif

// stdlib.h
// memory.h
// string.h
#define RAND_MAX 32767

#ifdef __cplusplus
extern "C" {
#endif

extern void* malloc(size_t len);
extern void free(void* ptr);

int abs( int j );
void * bsearch( const void * key, const void * base, size_t nmemb, size_t size, int ( *compar )( const void *, const void * ) );
long int labs( long int j );
int rand( void );
void srand( unsigned int seed );
void * memchr( const void * s, int c, size_t n );
int memcmp( const void * s1, const void * s2, size_t n );
void * memcpy( void * s1, const void * s2, size_t n );
void * memmove( void * s1, const void * s2, size_t n );
void * memset( void * s, int c, size_t n );
char * strcat( char * s1, const char * s2 );
char * strchr( const char * s, int c );
int strcmp( const char * s1, const char * s2 );
int strcoll( const char * s1, const char * s2 );
char * strcpy( char * s1, const char * s2 );
size_t strcspn( const char * s1, const char * s2 );
size_t strlen( const char * s );
char * strncat( char * s1, const char * s2, size_t n );
int strncmp( const char * s1, const char * s2, size_t n );
char * strncpy( char * s1, const char *  s2, size_t n );
char * strpbrk( const char * s1, const char * s2 );
char * strrchr( const char * s, int c );
size_t strspn( const char * s1, const char * s2 );
char * strstr( const char * s1, const char * s2 );
size_t strxfrm( char * s1, const char * s2, size_t n );
size_t mbstowcs(wchar_t* dst, const char* src, size_t len);
int isspace( int c );
int isdigit( int c );
int atoi( const char * str );
double pow( double x, double y );

// stdio.h
void _putchar( char c );

#ifdef __cplusplus
}
#endif

#include "external/printf/printf.h"
//extern void printf(const char* format, ...);

// endian.h

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __PDP_ENDIAN 3412

#define BIG_ENDIAN __BIG_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#define PDP_ENDIAN __PDP_ENDIAN
#define BYTE_ORDER __BYTE_ORDER

#define __bswap16(x) ((uint16_t)((((uint16_t)(x)&0xff00) >> 8) | (((uint16_t)(x)&0x00ff) << 8)))

#define __bswap32(x)                                                                     \
	((uint32_t)((((uint32_t)(x)&0xff000000) >> 24) | (((uint32_t)(x)&0x00ff0000) >> 8) | \
				(((uint32_t)(x)&0x0000ff00) << 8) | (((uint32_t)(x)&0x000000ff) << 24)))

#define __bswap64(x)                                            \
	((uint64_t)((((uint64_t)(x)&0xff00000000000000ULL) >> 56) | \
				(((uint64_t)(x)&0x00ff000000000000ULL) >> 40) | \
				(((uint64_t)(x)&0x0000ff0000000000ULL) >> 24) | \
				(((uint64_t)(x)&0x000000ff00000000ULL) >> 8) |  \
				(((uint64_t)(x)&0x00000000ff000000ULL) << 8) |  \
				(((uint64_t)(x)&0x0000000000ff0000ULL) << 24) | \
				(((uint64_t)(x)&0x000000000000ff00ULL) << 40) | \
				(((uint64_t)(x)&0x00000000000000ffULL) << 56)))

#if __BYTE_ORDER == __LITTLE_ENDIAN

// Definitions from musl libc
//#define htobe16(x) __bswap16(x)
//#define be16toh(x) __bswap16(x)
//#define betoh16(x) __bswap16(x)
//#define htobe32(x) __bswap32(x)
//#define be32toh(x) __bswap32(x)
//#define betoh32(x) __bswap32(x)
//#define htobe64(x) __bswap64(x)
//#define be64toh(x) __bswap64(x)
//#define betoh64(x) __bswap64(x)
//#define htole16(x) (uint16_t)(x)
//#define le16toh(x) (uint16_t)(x)
//#define letoh16(x) (uint16_t)(x)
//#define htole32(x) (uint32_t)(x)
//#define le32toh(x) (uint32_t)(x)
//#define letoh32(x) (uint32_t)(x)
//#define htole64(x) (uint64_t)(x)
//#define le64toh(x) (uint64_t)(x)
//#define letoh64(x) (uint64_t)(x)

// From Apple Open Source Libc
//#define ntohs(x) __bswap16(x)
//#define htons(x) __bswap16(x)
//#define ntohl(x) __bswap32(x)
//#define htonl(x) __bswap32(x)
//#define ntohll(x) __bswap64(x)
//#define htonll(x) __bswap64(x)
//#define NTOHL(x) (x) = ntohl((uint32_t)x)
//#define NTOHS(x) (x) = ntohs((uint16_t)x)
//#define NTOHLL(x) (x) = ntohll((uint64_t)x)
//#define HTONL(x) (x) = htonl((uint32_t)x)
//#define HTONS(x) (x) = htons((uint16_t)x)
//#define HTONLL(x) (x) = htonll((uint64_t)x)

#else // BIG_ENDIAN

// Definitions from musl libc
//#define htobe16(x) (uint16_t)(x)
//#define be16toh(x) (uint16_t)(x)
//#define betoh16(x) (uint16_t)(x)
//#define htobe32(x) (uint32_t)(x)
//#define be32toh(x) (uint32_t)(x)
//#define betoh32(x) (uint32_t)(x)
//#define htobe64(x) (uint64_t)(x)
//#define be64toh(x) (uint64_t)(x)
//#define betoh64(x) (uint64_t)(x)
//#define htole16(x) __bswap16(x)
//#define le16toh(x) __bswap16(x)
//#define letoh16(x) __bswap16(x)
//#define htole32(x) __bswap32(x)
//#define le32toh(x) __bswap32(x)
//#define letoh32(x) __bswap32(x)
//#define htole64(x) __bswap64(x)
//#define le64toh(x) __bswap64(x)
//#define letoh64(x) __bswap64(x)
//
//// From Apple Open Source libc
//#define ntohl(x) ((uint32_t)(x))
//#define ntohs(x) ((uint16_t)(x))
//#define htonl(x) ((uint32_t)(x))
//#define htons(x) ((uint16_t)(x))
//#define ntohll(x) ((uint64_t)(x))
//#define htonll(x) ((uint64_t)(x))
//
//#define NTOHL(x) (x)
//#define NTOHS(x) (x)
//#define NTOHLL(x) (x)
//#define HTONL(x) (x)
//#define HTONS(x) (x)
//#define HTONLL(x) (x)

#endif // endian check

#endif

/*
Implementations taken from PDCLib (https://github.com/DevSolar/pdclib)

License:
PDCLib is distributed unter the Creative Commons CC0 License. You
should have received a copy of the full legal text of this license
as part of this distribution (COPYING.CC0). It is also available at

https://creativecommons.org/publicdomain/zero/1.0/legalcode

The following is a human-readable summary of that license.

                       No Copyright

The person who associated a work with this deed has dedicated the
work to the public domain by waiving all of his or her rights to
the work worldwide under copyright law, including all related and
neighboring rights, to the extent allowed by law.

You can copy, modify, distribute and perform the work, even for
commercial purposes, all without asking permission. See Other
Information below.

                    Other Information

In no way are the patent or trademark rights of any person affected
by CC0, nor are the rights that other persons may have in the work
or in how the work is used, such as publicity or privacy rights.

Unless expressly stated otherwise, the person who associated a work
with this deed makes no warranties about the work, and disclaims
liability for all uses of the work, to the fullest extent permitted
by applicable law.

When using or citing the work, you should not imply endorsement by
the author or the affirmer.
*/

/*
endian.h implementation adapted from: https://github.com/embeddedartistry/libc

MIT License

Copyright (c) 2017 Embedded Artistry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/