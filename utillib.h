//===========================================================================
// Utility Library
// Copyright (c) 2022 Joshua Lee Ockert <torstenvl@gmail.com>
// https://github.com/torstenvl
//
// THIS WORK IS PROVIDED "AS IS" WITH NO EXPRESS OR IMPLIED WARRANTIES. THERE
// IS EXPRESSLY NO WARRANTY OF MERCHANTABILITY, FITNESS, NON-INFRINGEMENT, OR
// TITLE. NO AUTHOR SHALL BE LIABLE, UNDER ANY THEORY OF LAW, FOR ANY DAMAGES 
// OF ANY CHARACTER WHATSOEVER RELATING TO THE USE OF THIS WORK.
//
// Permission to use, copy, modify, and/or distribute this work for any
// purpose is hereby granted, provided this notice appears in all copies.
//---------------------------------------------------------------------------
// You may instead license this work under the MIT or Apache 2.0 licenses. 
// See the LICENSE file or visit https://apache.org/licenses/LICENSE-2.0
// 
// SPDX-License-Identifier: ISC OR MIT OR Apache-2.0
//===========================================================================

#ifndef UTILLIB_H__
#define UTILLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifdef NDEBUGUL
  #define UL__NO_DEBUG
#endif 
#ifdef NDEBUG
  #define UL__NO_DEBUG
#endif 

#define UL__NO_DEBUG
// #define UL__NEED_STRDUP
// #define UL__NEED_STACKTRACE

// #define UL__CROWDED_NAMESPACE
#ifndef UL__CROWDED_NAMESPACE
  #define BUILDSTAMP       ul__BUILDSTAMP
  #define LOG(...)         ul__LOG(__VA_ARGS__)
  #define DIE(...)         ul__DIE(__VA_ARGS__)
  #define free(...)        ul__free(__VA_ARGS__)
  #define memzero(...)     ul__memzero(__VA_ARGS__)
  #define memzero_s(...)   ul__memzero_s(__VA_ARGS__)
  #define inputline(...)   ul__inputline(__VA_ARGS__)
  #define autostr(...)     ul__autostr(__VA_ARGS__)
  #define S(...)           ul__autostr(__VA_ARGS__)
  #define STACKTRACE(...)  ul__STACKTRACE(__VA_ARGS__)
  #define DBUG(...)        ul__DBUG(__VA_ARGS__)
  #define VARTRACE(...)    ul__VARTRACE(__VA_ARGS__)
  #define stridxnomatch(...)   ul__stridxnomatch(__VA_ARGS__)
  #define stridxmatch(...)     ul__stridxmatch(__VA_ARGS__)
  #define strspanmatch(...)    ul__strspanmatch(__VA_ARGS__)
  #define strspannomatch(...)  ul__strspannomatch(__VA_ARGS__)
  #define strcharfirstptr(...) ul__strcharfirstptr(__VA_ARGS__)
  #define strcharlastptr(...)  ul__strcharlastptr(__VA_ARGS__)
  #define strstrfirstptr(...)  ul__strstrfirstptr(__VA_ARGS__)
  #define strstrlastptr(...)   ul__strstrlastptr(__VA_ARGS__)
  #define addwrap(...)         ul__addwrap(__VA_ARGS__)
  #ifdef UL__NEED_STRDUP
    #define strdup(...)      ul__strdup(__VA_ARGS__)
  #endif
#endif

#define ul__STACKTRACE(x) ((void)0)
#ifndef UL__NO_DEBUG
  #ifdef UL__NEED_STACKTRACE
    #undef ul__STACKTRACE
    #define ul__STACKTRACE(x) ul__stacktracefunc(x, __func__)
  #endif
#endif

#ifdef UL__NO_DEBUG
#define ul__DBUG(...)     ((void)0)
#define ul__VARTRACE(x)   ((void)0)
#else
#define ul__DBUG(...) (ul__LOG(__VA_ARGS__))
#define ul__VARTRACE(x) _Pragma("clang diagnostic push") \
                    _Pragma("clang diagnostic ignored \"-Wformat\"") \
                    _Pragma("gcc diagnostic push") \
                    _Pragma("gcc diagnostic ignored \"-Wformat\"") \
                    _Pragma("warning( push )")\
                    _Pragma("warning( disable : 4477 )")\
                    _Generic((x),\
             _Bool: fprintf(stderr,"%s:%d %s=%s\n",\
                                   __FILE__,__LINE__,#x,(x)?"true":"false"),\
              char: fprintf(stderr,"%s:%d %s=%c\n",__FILE__,__LINE__,#x,x),\
       signed char: fprintf(stderr,"%s:%d %s=%c\n",__FILE__,__LINE__,#x,x),\
     unsigned char: fprintf(stderr,"%s:%d %s=%c\n",__FILE__,__LINE__,#x,x),\
               int: fprintf(stderr,"%s:%d %s=%d\n",__FILE__,__LINE__,#x,x),\
      unsigned int: fprintf(stderr,"%s:%d %s=%u\n",__FILE__,__LINE__,#x,x),\
              long: fprintf(stderr,"%s:%d %s=%ld\n",__FILE__,__LINE__,#x,x),\
         long long: fprintf(stderr,"%s:%d %s=%lld\n",__FILE__,__LINE__,#x,x),\
     unsigned long: fprintf(stderr,"%s:%d %s=%lu\n",__FILE__,__LINE__,#x,x),\
unsigned long long: fprintf(stderr,"%s:%d %s=%llu\n",__FILE__,__LINE__,#x,x),\
             float: fprintf(stderr,"%s:%d %s=%f\n",__FILE__,__LINE__,#x,x),\
            double: fprintf(stderr,"%s:%d %s=%f\n",__FILE__,__LINE__,#x,x),\
       long double: fprintf(stderr,"%s:%d %s=%f\n",__FILE__,__LINE__,#x,x),\
             char*: fprintf(stderr,(x)?"%s:%d %s=\"%s\"\n":"%s:%d %s=%s\n",__FILE__,__LINE__,#x,x),\
      signed char*: fprintf(stderr,(x)?"%s:%d %s=\"%s\"\n":"%s:%d %s=%s\n",__FILE__,__LINE__,#x,x),\
    unsigned char*: fprintf(stderr,(x)?"%s:%d %s=\"%s\"\n":"%s:%d %s=%s\n",__FILE__,__LINE__,#x,x),\
           default: fprintf(stderr,"%s:%d %s is an unknown type; first eight bytes are 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",__FILE__,__LINE__,#x, (unsigned char)((unsigned char *)&x + 0), (unsigned char)((unsigned char *)&x + 1), (unsigned char)((unsigned char *)&x + 2), (unsigned char)((unsigned char *)&x + 3), (unsigned char)((unsigned char *)&x + 4), (unsigned char)((unsigned char *)&x + 5), (unsigned char)((unsigned char *)&x + 6), (unsigned char)((unsigned char *)&x + 7))\
                    _Pragma("warning( pop )")\
                    _Pragma("gcc diagnostic pop")\
                    _Pragma("clang diagnostic pop"))
#endif



//---------------------------------------------------------------------------
//              A constant containing YYYYMMDD.HHMMSS timestamp
//------------------+------------------+------------------+------------------
#define ul__BUILDSTAMP \
(char[]){ __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10], \
         (__DATE__[0]=='O' || __DATE__[0]=='N' || __DATE__[0]=='D') ? '1' : '0', \
         (__DATE__[0]=='J' && __DATE__[1]=='a') ? '1': \
         (__DATE__[0]=='F')                     ? '2': \
         (__DATE__[0]=='M' && __DATE__[2]=='r') ? '3': \
         (__DATE__[0]=='A' && __DATE__[1]=='p') ? '4': \
         (__DATE__[0]=='M' && __DATE__[2]=='y') ? '5': \
         (__DATE__[0]=='J' && __DATE__[2]=='n') ? '6': \
         (__DATE__[0]=='J' && __DATE__[2]=='l') ? '7': \
         (__DATE__[0]=='A' && __DATE__[1]=='u') ? '8': \
         (__DATE__[0]=='S')                     ? '9': \
         (__DATE__[0]=='O')                     ? '0': \
         (__DATE__[0]=='N')                     ? '1': '2' , \
         (__DATE__[4] == ' ') ? '0' : __DATE__[4], \
         __DATE__[5], \
         '.', \
         __TIME__[0], __TIME__[1], \
         __TIME__[3], __TIME__[4], \
         __TIME__[6], __TIME__[7], \
         '\0' }



//---------------------------------------------------------------------------
//             Kill the program with appropriate error messages.
//------------------+------------------+------------------+------------------
#define ul__DIE(...) do { \
    fprintf(stderr, "Died: line %zu (function %s)\n", __LINE__, __func__);  \
    fprintf(stderr, __VA_ARGS__);                                           \
    fprintf(stderr, "\n\n\n");                                              \
    exit(EXIT_FAILURE);                                                     \
    } while (0)



#define ul__LOG(...) (fprintf(stderr, "In %s:%s() on line %d: ", \
                       __FILE__, __func__, __LINE__) \
                       && fprintf(stderr, __VA_ARGS__ ) \
                       && fprintf(stderr, "\n"))
                  
//---------------------------------------------------------------------------
//             Free allocated memory and set the pointer to NULL.
//------------------+------------------+------------------+------------------
#define ul__free(x) do { \
    (free)(x);                                                              \
    x = NULL;                                                               \
    } while (0)



//---------------------------------------------------------------------------
//       Easier-to-read aliases to cryptic standard library functions
//------------------+------------------+------------------+------------------
#define ul__stridxnomatch(...)       strspn(__VA_ARGS__)
#define ul__stridxmatch(...)         strcspn(__VA_ARGS__)
#define ul__strspanmatch(...)        strspn(__VA_ARGS__)
#define ul__strspannomatch(...)      strcspn(__VA_ARGS__)
#define ul__strcharfirstptr(...)     strchr(__VA_ARGS__)
#define ul__strcharlastptr(...)      strrchr(__VA_ARGS__)
#define ul__strstrfirstptr(...)      strstr(__VA_ARGS__)
#define ul__strstrlastptr(...)       strrstr(__VA_ARGS__)



//---------------------------------------------------------------------------
//                       Initialize all memory to zero
//------------------+------------------+------------------+------------------
#define ul__memzero(x, y) \
    memset(x, 0, y)



//---------------------------------------------------------------------------
//       Actually initialize all memory to zero, do not optimize out.        
//------------------+------------------+------------------+------------------
static inline void ul__memzero_s(void *const p, const size_t z) {
    volatile unsigned char *volatile p_ = (volatile unsigned char *volatile)p;
    for (size_t i=(size_t)0U; i<z; i++) {
        p_[i] = 0U;
    }
}



//---------------------------------------------------------------------------
//      Wrapper for fgets() to get a fixed width string with truncation.
//------------------+------------------+------------------+------------------
static inline char *ul__inputline(char *buf, int n, FILE *fp) {
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
static inline char *ul__autostr(char *s) {
    static char buffer[32768];
    if (!s) return NULL;
    memmove(buffer, s, strlen(s));
    buffer[strlen(s)] = '\0';
    free(s);
    return buffer;
}



//---------------------------------------------------------------------------
//                    Add signed integers with wraparound.
//------------------+------------------+------------------+------------------
#define ul__addwrap(a, b) _Generic((a),                                   \
                                char: _Generic((b),                       \
                                            char: ul__addwrapchar,        \
                                     signed char: ul__addwrapsignedchar,  \
                                           short: ul__addwrapshort,       \
                                             int: ul__addwrapint,         \
                                            long: ul__addwraplong,        \
                                         default: ul__addwraplonglong),   \
                         signed char: _Generic((b),                       \
                                            char: ul__addwrapsignedchar,  \
                                     signed char: ul__addwrapsignedchar,  \
                                           short: ul__addwrapshort,       \
                                             int: ul__addwrapint,         \
                                            long: ul__addwraplong,        \
                                         default: ul__addwraplonglong),   \
                               short: _Generic((b),                       \
                                            char: ul__addwrapshort,       \
                                     signed char: ul__addwrapshort,       \
                                           short: ul__addwrapshort,       \
                                             int: ul__addwrapint,         \
                                            long: ul__addwraplong,        \
                                         default: ul__addwraplonglong),   \
                                 int: _Generic((b),                       \
                                            char: ul__addwrapint,         \
                                     signed char: ul__addwrapint,         \
                                           short: ul__addwrapint,       \
                                             int: ul__addwrapint,         \
                                            long: ul__addwraplong,        \
                                         default: ul__addwraplonglong),   \
                                long: _Generic((b),                       \
                                            char: ul__addwraplong,        \
                                     signed char: ul__addwraplong,        \
                                           short: ul__addwraplong,        \
                                             int: ul__addwraplong,        \
                                            long: ul__addwraplong,        \
                                         default: ul__addwraplonglong),   \
                             default: ul__addwraplonglong)(a, b)

static inline char ul__addwrapchar(char a, char b) {
    char result; 
    if (CHAR_MIN != SCHAR_MIN) result = a + b;
    else result = ((a<0 && b<CHAR_MIN-a) ? (CHAR_MAX+(b-(CHAR_MIN-a)+1)) : 
                   (a>0 && b>CHAR_MAX-a) ? (CHAR_MIN+(b-(CHAR_MAX-a)-1)) : 
                   (a + b));
    return result;
}

static inline signed char ul__addwrapsignedchar(signed char a, signed char b) {
    signed char result;
    result = ((a<0 && b<SCHAR_MIN-a) ? (SCHAR_MAX+(b-(SCHAR_MIN-a)+1)) : 
              (a>0 && b>SCHAR_MAX-a) ? (SCHAR_MIN+(b-(SCHAR_MAX-a)-1)) : 
              (a + b));
    return result;
}

static inline short ul__addwrapshort(short a, short b) {
    short result;
    result = ((a<0 && b<SHRT_MIN-a) ? (SHRT_MAX+(b-(SHRT_MIN-a)+1)) : 
             (a>0 && b>SHRT_MAX-a) ? (SHRT_MIN+(b-(SHRT_MAX-a)-1)) : 
             (a + b));
    return result;
}

static inline int ul__addwrapint(int a, int b) {
    int result;
    result = ((a<0 && b<INT_MIN-a) ? (INT_MAX+(b-(INT_MIN-a)+1)) : 
             (a>0 && b>INT_MAX-a) ? (INT_MIN+(b-(INT_MAX-a)-1)) : 
             (a + b));
    return result;
}

static inline long ul__addwraplong(long a, long b) {
    long result;
    result = ((a<0 && b<LONG_MIN-a) ? (LONG_MAX+(b-(LONG_MIN-a)+1)) : 
              (a>0 && b>LONG_MAX-a) ? (LONG_MIN+(b-(LONG_MAX-a)-1)) : 
              (a + b));
    return result;
}

static inline long long ul__addwraplonglong(long long a, long long b) {
    long long result;
    result = ((a<0 && b<LLONG_MIN-a) ? (LLONG_MAX+(b-(LLONG_MIN-a)+1)) : 
              (a>0 && b>LLONG_MAX-a) ? (LLONG_MIN+(b-(LLONG_MAX-a)-1)) : 
              (a + b));
    return result;
}


//---------------------------------------------------------------------------
//                Quick/dirty strdup() for non-POSIX systems.
//------------------+------------------+------------------+------------------
static inline char *ul__strdup(const char *s) {
    size_t n = strlen(s);
    char *r = malloc(n+1);
    if (r) { memcpy(r, s, n); r[n] = '\0'; }
    return r;
}


static inline void ul__stacktracefunc(int x, const char *funcname) {
    static int spaces = 0;
    if (x >= 1) {
        fprintf(stderr,"%*cEntering %s()\n", spaces, ' ', funcname);
        spaces+=4;
    } else {
        if (spaces >= 4) spaces-=4;
        fprintf(stderr,"%*cExiting %s()\n", spaces, ' ', funcname);
    }
}



#endif
