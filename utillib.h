//===========================================================================
//
//  Copyright (c) 2022 Joshua Lee Ockert <torstenvl@gmail.com>
//  https://github.com/torstenvl/
//
//  This work is permissively licensed.  See LICENSE.
//  SPDX-License-Identifier: ISC OR MIT OR Apache-2.0
//
//===========================================================================

#ifndef UTILLIB_H__
#define UTILLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// #define UL_CROWDED_NAMESPACE
#ifndef UL_CROWDED_NAMESPACE
#define DIE(...)         _ul_DIE(__VA_ARGS__)
#define memzero(...)     _ul_memzero(__VA_ARGS__)
#define inputline(...)   _ul_inputline(__VA_ARGS__)
#define autostr(...)     _ul_autostr(__VA_ARGS__)
#ifndef strdup
#define strdup(...)      _ul_strdup(__VA_ARGS__)
#endif
#endif



//---------------------------------------------------------------------------
//             Kill the program with appropriate error messages.
//------------------+------------------+------------------+------------------
#define _ul_DIE(...) do { \    
    fprintf(stderr, "Died: line %zu (function %s)\n", __LINE__, __func__);  \
    fprintf(stderr, __VA_ARGS__);                                           \
    fprintf(stderr, "\n\n\n");                                              \
    exit(EXIT_FAILURE);                                                     \
    } while (0);



//---------------------------------------------------------------------------
//       Actually initialize all memory to zero, do not optimize out.        
//------------------+------------------+------------------+------------------
static inline void _ul_memzero(void *const p, const size_t z) {
    volatile unsigned char *volatile p_ = (volatile unsigned char *volatile)p;
    for (size_t i=(size_t)0U; i<z; i++) {
        p_[i] = 0U;
    }
}



//---------------------------------------------------------------------------
//      Wrapper for fgets() to get a fixed width string with truncation.
//------------------+------------------+------------------+------------------
static inline char *_ul_inputline(char *buf, size_t n, FILE *fp) {
    if (fgets(buf, n, fp) == NULL) return NULL;
    if (strlen(buf) < 1) return buf;     // Go ahead and return empty string
    if (buf[strlen(buf)-1] == '\n') {    // If we got the newline, NUL it
        buf[strlen(buf)-1] = '\0';
    } else {                             // If we didn't get the newline
        scanf("%*[^\n]");                //   Read everything UP TO newline
        scanf("%*c");                    //   And then one more byte
    }

    return buf;
}



//---------------------------------------------------------------------------
//   Allow quick/dirty auto storage of dynamic strings. NOT THREAD SAFE!!!
//------------------+------------------+------------------+------------------
static inline char *_ul_autostr(char *s) {
    static char buffer[32768];
    if (!s) return NULL;
    memmove(buffer, s, strlen(s));
    buffer[strlen(s)] = '\0';
    free(s);
    return buffer;
}



//---------------------------------------------------------------------------
//                Quick/dirty strdup() for non-POSIX systems.
//------------------+------------------+------------------+------------------
static inline char *_ul_strdup(const char *s) {
    size_t n = strlen(s);
    char *r = malloc(n+1);
    if (r) { memcpy(r, s, n); r[n] = '\0'; }
    return r;
}



#endif
