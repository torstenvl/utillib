//===========================================================================
//
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
// https://opensource.org/licenses/MIT https://www.apache.org/licenses/
// SPDX-License-Identifier: ISC OR MIT OR Apache-2.0
//
//===========================================================================

#ifndef UTILLIB_H__
#define UTILLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>

static inline void ul__stacktrace(int x, const char *funcname);


//===========================================================================
//        Compile-time constants controlling Utility Library behavior
//===========================================================================
// #define UL__NEED_STACKTRACE
// #define UL__NEED_STRDUP
// #define UL__CROWDED_NAMESPACE
#ifdef NDEBUG
  #define UL__NO_DEBUG
  #undef  UL__NEED_STACKTRACE
#endif 



//===========================================================================
//                       OS and Compiler definitions
//===========================================================================
#if (!defined( OS_MAC ) && !defined( OS_LIN ) && \
     !defined( OS_BSD ) && !defined( OS_WIN ))
  #if defined( __linux__ )
    #define OS_LIN
  #elif defined( __APPLE__ )
    #define OS_MAC
  #elif defined( _WIN32 )
    #define OS_WIN
  #elif defined( __unix__ )
    #define OS_BSD
  #endif
#endif
#if (!defined( CC_CLANG ) && !defined( CC_GCC ) && !defined( CC_MSVCPP ))
  #if defined( __clang__ )
    #define CC_CLANG
  #elif defined( __GNUC__ )
    #define CC_GCC
  #elif defined( _MSC_VER )
    #define CC_MSVCPP
  #elif defined(__MINGW32__) || defined(__MINGW64__)
    #define CC_MINGW
  #endif
#endif



//===========================================================================
//                 Per-compiler pragma string definitions
//===========================================================================
#if defined( CC_CLANG )
  #define PRAGMADIAGPUSH     "clang diagnostic push"
  #define PRAGMADIAGPOP      "clang diagnostic pop"
  #define PRAGMADIAGFORMAT   "clang diagnostic ignored \"-Wformat\""
  #define PRAGMADIAGFMTLIT   "clang diagnostic ignored \"-Wformat-nonliteral\""
#elif defined( CC_GCC )
  #define PRAGMADIAGPUSH     "GCC diagnostic push"
  #define PRAGMADIAGPOP      "GCC diagnostic pop"
  #define PRAGMADIAGFORMAT   "GCC diagnostic ignored \"-Wformat\""
  #define PRAGMADIAGFMTLIT   "GCC diagnostic ignored \"-Wformat-nonliteral\""
#elif defined( CC_MSVCPP )
  #define PRAGMADIAGPUSH     "warning( push )"
  #define PRAGMADIAGPOP      "warning( pop )"
  #define PRAGMADIAGFORMAT   "warning( disable : 4477 )"
  #define PRAGMADIAGFMTLIT   "warning( disable : 4774 )"
#endif



//===========================================================================
//      Define shorter/cleaner names if we don't have namespace issues
//===========================================================================
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


//===========================================================================
//            Conditional definition of debugging functionality
//===========================================================================

#ifdef UL__NEED_STACKTRACE
  #define ul__STACKTRACE(x)   ul__stacktrace(x, __func__)
  #define FUNC_ENTER          ul__STACKTRACE(1)
  #define FUNC_VOID_RETURN    do { ul__STACKTRACE(0); return;                     } while(0)
  #define FUNC_RETURN(x)      do { ul__STACKTRACE(0); return x;                   } while(0)
  #define FUNC_VOID_DIE(...)  do { LOG(__VA_ARGS__); ul__STACKTRACE(0); return;   } while(0)
  #define FUNC_DIE(x,...)     do { LOG(__VA_ARGS__); ul__STACKTRACE(0); return x; } while(0)
#else
  #define ul__STACKTRACE(x)   ((void)0)
  #define FUNC_ENTER          ul__STACKTRACE(1)
  #define FUNC_VOID_RETURN    do { return;                      } while(0)
  #define FUNC_RETURN(x)      do { return x;                    } while(0)
  #define FUNC_VOID_DIE(...)  do { LOG(__VA_ARGS__); return;    } while(0)
  #define FUNC_DIE(x,...)     do { LOG(__VA_ARGS__); return x;  } while(0)
#endif

#ifdef UL__NO_DEBUG
  #define ul__DBUG(...)     ((void)0)
  #define ul__VARTRACE(x)   ((void)0)
#else
  #define ul__DBUG(...) (ul__LOG(__VA_ARGS__))
  #define ul__VARTRACE(x)                                                      \
                      _Pragma(PRAGMADIAGPUSH)                                  \
                      _Pragma(PRAGMADIAGFORMAT)                                \
                      _Generic((x),                                            \
               _Bool: fprintf(stderr,"%s:%d %s=%s\n",                          \
                                     __FILE__,__LINE__,#x,(x)?"true":"false"), \
                char: fprintf(stderr,"%s:%d %s=%c\n",__FILE__,__LINE__,#x,x),  \
         signed char: fprintf(stderr,"%s:%d %s=%c\n",__FILE__,__LINE__,#x,x),  \
       unsigned char: fprintf(stderr,"%s:%d %s=%c\n",__FILE__,__LINE__,#x,x),  \
                 int: fprintf(stderr,"%s:%d %s=%d\n",__FILE__,__LINE__,#x,x),  \
        unsigned int: fprintf(stderr,"%s:%d %s=%u\n",__FILE__,__LINE__,#x,x),  \
                long: fprintf(stderr,"%s:%d %s=%ld\n",__FILE__,__LINE__,#x,x), \
           long long: fprintf(stderr,"%s:%d %s=%lld\n",__FILE__,__LINE__,#x,x),\
       unsigned long: fprintf(stderr,"%s:%d %s=%lu\n",__FILE__,__LINE__,#x,x), \
  unsigned long long: fprintf(stderr,"%s:%d %s=%llu\n",__FILE__,__LINE__,#x,x),\
               float: fprintf(stderr,"%s:%d %s=%f\n",__FILE__,__LINE__,#x,x),  \
              double: fprintf(stderr,"%s:%d %s=%f\n",__FILE__,__LINE__,#x,x),  \
         long double: fprintf(stderr,"%s:%d %s=%f\n",__FILE__,__LINE__,#x,x),  \
               char*: fprintf(stderr,(x)?"%s:%d %s=\"%s\"\n":"%s:%d %s=%s\n",  \
                                         __FILE__,__LINE__,#x,x),              \
        signed char*: fprintf(stderr,(x)?"%s:%d %s=\"%s\"\n":"%s:%d %s=%s\n",  \
                                         __FILE__,__LINE__,#x,x),              \
      unsigned char*: fprintf(stderr,(x)?"%s:%d %s=\"%s\"\n":"%s:%d %s=%s\n",  \
                                         __FILE__,__LINE__,#x,x),              \
             default: fprintf(stderr,"%s:%d %s is an unknown type; first eight"\
 "bytes are 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",              \
                                     __FILE__,__LINE__,#x,                     \
                                     (unsigned char)((unsigned char *)&x + 0), \
                                     (unsigned char)((unsigned char *)&x + 1), \
                                     (unsigned char)((unsigned char *)&x + 2), \
                                     (unsigned char)((unsigned char *)&x + 3), \
                                     (unsigned char)((unsigned char *)&x + 4), \
                                     (unsigned char)((unsigned char *)&x + 5), \
                                     (unsigned char)((unsigned char *)&x + 6), \
                                     (unsigned char)((unsigned char *)&x + 7)) \
                      _Pragma(PRAGMADIAGPOP)
#endif



//===========================================================================
//              A constant containing YYYYMMDD.HHMMSS timestamp
//===========================================================================
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



//===========================================================================
//             Kill the program with appropriate error messages.
//===========================================================================
#define ul__DIE(...) ul__diefunc(__LINE__, __func__, __VA_ARGS__)

static inline int ul__diefunc(size_t line, const char *function, 
                              const char *format,           ... ) {
    va_list args;

    fprintf(stderr, "Died: line %zu (function %s)\n", line, function);
    
    va_start(args, format);
    _Pragma(PRAGMADIAGPUSH)
    _Pragma(PRAGMADIAGFMTLIT)
    vfprintf(stderr, format, args);
    _Pragma(PRAGMADIAGPOP)
    va_end(args);

    fprintf(stderr, "\n\n\n");

    exit(EXIT_FAILURE);

    // This should be unreachable, since exit() does not return. However,
    // a return statement is necessary for an int function, and an int
    // function is necessary for compilers not to complain about || DIE()
    // syntax. 
    return 0; 
}


//===========================================================================
//                             Log to stderr
//===========================================================================
#define ul__LOG(...) (fprintf(stderr, "In %s:%s() on line %d: ",            \
                       __FILE__, __func__, __LINE__)                        \
                       && fprintf(stderr, __VA_ARGS__ )                     \
                       && fprintf(stderr, "\n"))



//===========================================================================
//             Free allocated memory and set the pointer to NULL.
//===========================================================================
#define ul__free(x) do { \
    (free)(x);                                                              \
    x = NULL;                                                               \
    } while (0)



//===========================================================================
//       Easier-to-read aliases to cryptic standard library functions
//===========================================================================
#define ul__stridxnomatch(...)       strspn(__VA_ARGS__)
#define ul__stridxmatch(...)         strcspn(__VA_ARGS__)
#define ul__strspanmatch(...)        strspn(__VA_ARGS__)
#define ul__strspannomatch(...)      strcspn(__VA_ARGS__)
#define ul__strcharfirstptr(...)     strchr(__VA_ARGS__)
#define ul__strcharlastptr(...)      strrchr(__VA_ARGS__)
#define ul__strstrfirstptr(...)      strstr(__VA_ARGS__)
#define ul__strstrlastptr(...)       strrstr(__VA_ARGS__)



//===========================================================================
//                       Initialize all memory to zero.
//===========================================================================
#define ul__memzero(x, y) \
    memset(x, 0, y)



//===========================================================================
//       Actually initialize all memory to zero, do not optimize out.        
//===========================================================================
static inline void ul__memzero_s(void *const p, const size_t z) {
    volatile unsigned char *volatile p_ = (volatile unsigned char *volatile)p;
    for (size_t i=(size_t)0U; i<z; i++) {
        p_[i] = 0U;
    }
}



//===========================================================================
//      Wrapper for fgets() to get a fixed width string with truncation.
//===========================================================================
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



//===========================================================================
//   Allow quick/dirty auto storage of dynamic strings. NOT THREAD SAFE!!!
//===========================================================================
static inline char *ul__autostr(char *s) {
    static char buffer[32768];
    if (!s) return NULL;
    memmove(buffer, s, strlen(s));
    buffer[strlen(s)] = '\0';
    free(s);
    return buffer;
}



//===========================================================================
//                    Add signed integers with wraparound.
//===========================================================================
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
                                           short: ul__addwrapint,         \
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
#if SCHAR_MIN == CHAR_MIN
    else result = ((a<0 && b<CHAR_MIN-a) ? (CHAR_MAX+(b-(CHAR_MIN-a)+1)) : 
                   (a>0 && b>CHAR_MAX-a) ? (CHAR_MIN+(b-(CHAR_MAX-a)-1)) : 
                   (a + b));
#else 
    result = a + b;
#endif 
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



static inline void ul__stacktrace(int x, const char *funcname) {
    static _Thread_local int spaces = 0;
    if (x >= 1) {
        fprintf(stderr,"%*cEntering %s()\n", spaces, ' ', funcname);
        spaces+=4;
    } else {
        if (spaces >= 4) spaces-=4;
        fprintf(stderr,"%*cExiting %s()\n", spaces, ' ', funcname);
    }
}



#endif
