// ===========================================================================
//
//                                 Util Lib
//    A collection of utilities to make C programming and debugging easier
//
// ---------------------------------------------------------------------------
//
//   Copyright (c) 2022 Joshua Lee Ockert <https://github.com/torstenvl/>
//
//   THIS WORK IS PROVIDED "AS IS" WITH NO WARRANTY OF ANY KIND. THE IMPLIED
//   WARRANTIES OF MERCHANTABILITY, FITNESS, NON-INFRINGEMENT, AND TITLE ARE
//   EXPRESSLY DISCLAIMED. NO AUTHOR SHALL BE LIABLE UNDER ANY THEORY OF LAW
//   FOR ANY DAMAGES OF ANY KIND RESULTING FROM THE USE OF THIS WORK.
//
//   Permission to use, copy, modify, and/or distribute this work for any
//   purpose is hereby granted, provided this notice appears in all copies.
//
//   SPDX-License-Identifier: ISC
//
// ===========================================================================

#ifndef UTILLIB_H__
#define UTILLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
// #include <stdarg.h>

// ---------------------------------------------------------------------------
//        Compile-time constants controlling Utility Library behavior
// ---------------------------------------------------------------------------

// #define ulCROWDEDNAMESPACE
// #define ulNEEDSTRDUP
// #define ulNEEDTRACE
#define ulNEEDDBUG

#ifdef NDEBUG
  #undef  ulNEEDDBUG
  #undef  ulNEEDTRACE
#endif



//===========================================================================
//                 Per-compiler pragma string definitions
//===========================================================================
#if defined( __clang__ )
  #define PRAGMADIAGPUSH     "clang diagnostic push"
  #define PRAGMADIAGPOP      "clang diagnostic pop"
  #define PRAGMADIAGFORMAT   "clang diagnostic ignored \"-Wformat\""
  #define PRAGMADIAGFMTLIT   "clang diagnostic ignored \"-Wformat-nonliteral\""
#elif defined( __GNUC__ )
  #define PRAGMADIAGPUSH     "GCC diagnostic push"
  #define PRAGMADIAGPOP      "GCC diagnostic pop"
  #define PRAGMADIAGFORMAT   "GCC diagnostic ignored \"-Wformat\""
  #define PRAGMADIAGFMTLIT   "GCC diagnostic ignored \"-Wformat-nonliteral\""
#elif defined( _MSC_VER )
  #define PRAGMADIAGPUSH     "warning( push )"
  #define PRAGMADIAGPOP      "warning( pop )"
  #define PRAGMADIAGFORMAT   "warning( disable : 4477 )"
  #define PRAGMADIAGFMTLIT   "warning( disable : 4774 )"
#endif



//===========================================================================
//      Define shorter/cleaner names if we don't have namespace issues
//===========================================================================
#ifndef ulCROWDEDNAMESPACE

  #define LOG(...)             ulLOG(__VA_ARGS__)
  #define DBUG(...)            ulDBUG(__VA_ARGS__)
  #define VARDBUG(...)         ulVARDBUG(__VA_ARGS__)
  #define CRASH(...)           ulCRASH(__VA_ARGS__)
  #define TRACE(...)           ulTRACE(__VA_ARGS__)
  #define ENTER                ulENTER
  #define RETURN(x)            ulRETURN(x)
  #define VOIDRETURN           ulVOIDRETURN
  #define DIE(x,...)           ulDIE(x,__VA_ARGS__)
  #define VOIDDIE(...)         ulVOIDDIE(__VA_ARGS__)

  #define free(...)            ulfree(__VA_ARGS__)
  #define memzero(...)         ulmemzero(__VA_ARGS__)
  #define memzerosecure(...)   ulmemzerosecure(__VA_ARGS__)

  #define inputline(...)       ulinputline(__VA_ARGS__)
  #define autostr(...)         ulautostr(__VA_ARGS__)
  #define S(...)               ulautostr(__VA_ARGS__)

  #define stridxnomatch(...)   ulstridxnomatch(__VA_ARGS__)
  #define stridxmatch(...)     ulstridxmatch(__VA_ARGS__)
  #define strcharfirstptr(...) ulstrcharfirstptr(__VA_ARGS__)
  #define strcharlastptr(...)  ulstrcharlastptr(__VA_ARGS__)
  #define strstrfirstptr(...)  ulstrstrfirstptr(__VA_ARGS__)
  #define strstrlastptr(...)   ulstrstrlastptr(__VA_ARGS__)
  
  #ifdef ulNEED_STRDUP
  #define strdup(...)          ulstrdup(__VA_ARGS__)
  #endif

  #define cdpt_from_utf8(s)          ulcdpt_from_utf8(s)
  #define cdpt_from_utf16(h, l)      ulcdpt_from_utf16(h, l)
  #define utf16_from_cdpt(c, h, l)   ulutf16_from_cdpt(c, h, l)
  #define utf8_from_cdpt(c)          ulutf8_from_cdpt(c)

#endif










//===========================================================================
//                    Logging and debugging functionality
//===========================================================================

#define ulLOG(...) (\
    fprintf(stderr, "In %s:%s() line %d: ", __FILE__, __func__, __LINE__)  &&\
    fprintf(stderr, __VA_ARGS__)                                           &&\
    fprintf(stderr, "\n")\
    )

#ifndef ulNEEDDBUG
#define ulDBUG(...) ((void)0)
#else
#define ulDBUG(...) (ulLOG(__VA_ARGS__))
#endif

#ifndef ulNEEDDBUG
#define ulVARDBUG(x)   ((void)0)
#else
#define ulVARTRACE(x)                                                        \
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
           default: fprintf(stderr,"%s:%d %s is an unknown type; first four" \
                                   "bytes are 0x%x 0x%x 0x%x 0x%x\n"         \
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

#define ulCRASH(...) (\
    fprintf(stderr, "CRASH: line %d (function %s)\n", __LINE__, __func__)  &&\
    fprintf(stderr, __VA_ARGS__)                                           &&\
    fprintf(stderr, "\n\n\n")                                              &&\
    (exit(EXIT_FAILURE), 1)\
    )

#ifdef ulNEEDTRACE
static inline void ultrace(int x, const char *funcname) {
    static _Thread_local int spaces = 0;
    if (x >= 0) {
        fprintf(stderr,"%*cEntering %s()\n", spaces, ' ', funcname);
        spaces+=4;
    } else {
        if (spaces >= 4) spaces-=4;
        fprintf(stderr,"%*cExiting %s()\n",  spaces, ' ', funcname);
    }
}
#define ulTRACE(x)     ultrace(x, __func__)
#define ulENTER        ulTRACE(1)
#define ulRETURN(x)    do{                   ulTRACE(0); return x; }while(0)
#define ulVOIDRETURN   do{                   ulTRACE(0); return;   }while(0)
#define ulDIE(x,...)   do{ LOG(__VA_ARGS__); ulTRACE(0); return x; }while(0)
#define ulVOIDDIE(...) do{ LOG(__VA_ARGS__); ulTRACE(0); return;   }while(0)
#else
#define ulTRACE(x)     ((void)0)
#define ulENTER        ((void)0)
#define ulVOIDRETURN   do{                               return;   }while(0)
#define ulRETURN(x)    do{                               return x; }while(0)
#define ulVOIDDIE(...) do{ LOG(__VA_ARGS__);             return;   }while(0)
#define ulDIE(x,...)   do{ LOG(__VA_ARGS__);             return x; }while(0)
#endif










//===========================================================================
//                            Memory functionality
//===========================================================================
#define ulfree(x)        do { (free)(x); x = NULL; } while (0)
#define ulmemzero(x, y)  memset(x, 0, y)

static inline void ulmemzerosecure(void *const p, const size_t z) {
    volatile unsigned char *volatile p_ = (volatile unsigned char *volatile)p;
    for (size_t i=(size_t)0U; i<z; i++) {
        p_[i] = 0U;
    }
}










//===========================================================================
//                              String functions
//===========================================================================
#define ul__stridxnomatch(...)       strspn(__VA_ARGS__)
#define ul__stridxmatch(...)         strcspn(__VA_ARGS__)
#define ul__strspanmatch(...)        strspn(__VA_ARGS__)
#define ul__strspannomatch(...)      strcspn(__VA_ARGS__)
#define ul__strcharfirstptr(...)     strchr(__VA_ARGS__)
#define ul__strcharlastptr(...)      strrchr(__VA_ARGS__)
#define ul__strstrfirstptr(...)      strstr(__VA_ARGS__)
#define ul__strstrlastptr(...)       strrstr(__VA_ARGS__)



// Wrap fgets() for standard human input
// -------------------------------------
static inline char *ulinputline(char *buf, int n, FILE *fp) {
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



// Allow quick/dirty auto storage of dynamic strings
// -------------------------------------------------
static inline char *ulautostr(char *s) {
    _Thread_local static char buffer[16384];
    if (!s) return NULL;
    size_t len = (16383 < strlen(s)) ? 16383 : strlen(s);
    memmove(buffer, s, len);
    buffer[len] = '\0';
    free(s);
    return buffer;
}



// Quick/dirty strdup() for non-POSIX systems
// ------------------------------------------
static inline char *ulstrdup(const char *s) {
    size_t n = strlen(s);
    char *r = malloc(n+1);
    if (r) { memcpy(r, s, n); r[n] = '\0'; }
    return r;
}


// Limited UTF-8 and UTF-16 Conversion Functions
// ---------------------------------------------

static inline int32_t ulcdpt_from_utf8(const unsigned char *s) {
    if (s[0] <= 0x7F) return s[0];

    if ((0xC0 <= s[0] && s[0] <= 0xDF) &&     //  1100 0000 —— 1101 1111
        (0x80 <= s[1] && s[1] <= 0xBF))       //  1000 0000 —— 1011 1111
            return ((s[0] & 0x1F) <<  6) |         // 000xxxxx______
                   ((s[1] & 0x3F) <<  0) ;         //       00xxxxxx

    if ((0xE0 <= s[0] && s[0] <= 0xEF) &&     //  1110 0000 —— 1110 1111
        (0x80 <= s[1] && s[1] <= 0xBF) &&     //  1000 0000 —— 1011 1111
        (0x80 <= s[2] && s[2] <= 0xBF))       //  1000 0000 —— 1011 1111
            return ((s[0] & 0x0F) << 12) |         // xxxx____________
                   ((s[1] & 0x3F) <<  6) |         //   00xxxxxx______
                   ((s[2] & 0x3F) <<  0) ;         //         00xxxxxx

    if ((0xF0 <= s[0] && s[0] <= 0xF7) &&     //  1111 0000 —— 1111 0111
        (0x80 <= s[1] && s[1] <= 0xBF) &&     //  1000 0000 —— 1011 1111
        (0x80 <= s[2] && s[2] <= 0xBF) &&     //  1000 0000 —— 1011 1111
        (0x80 <= s[3] && s[3] <= 0xBF))       //  1000 0000 —— 1011 1111
            return ((s[0] & 0x07) << 18) |         // xxx__________________
                   ((s[1] & 0x3F) << 12) |         //  00xxxxxx____________
                   ((s[2] & 0x3F) <<  6) |         //        00xxxxxx______
                   ((s[3] & 0x3F) <<  0) ;         //              00xxxxxx

    /* Default -- maybe it isn't UTF-8? */
    return s[0];
}


static inline int32_t ulcdpt_from_utf16(uint16_t hi, uint16_t lo) {
    if (0xD800 <= hi && hi <= 0xDBFF) {
        if (0xDC00 <= lo && lo <= 0xDFFF) {
            // We have a valid surrogate pair, so convert it.
            // Unicode 15.0, Tbl 3-5 says UTF-16 surrogate pairs in form:
            //         110110wwwwyyyyyy
            //                   110111xxxxxxxxxx
            // map to scalar code points of value:
            //              uuuuuyyyyyyxxxxxxxxxx     (uuuuu=wwww+1)
            return ((lo & 0x03FF)        |
                   ((hi & 0x003F) << 10) |
                   ((hi & 0x03C0) << 10) +
                   ((     0x0040) << 10));
        }
    }
    else if (!(0xDC00 <= lo && lo <= 0xDFFF)) {
        return lo;
    }

    return '?';
}


static inline void ulutf16_from_cdpt(int32_t cdpt, uint16_t *hi, uint16_t *lo) {
    if (0 <= cdpt && cdpt <= 65535) {
        *hi = 0;
        *lo = (uint16_t)cdpt;
        return;
    }

    else {
        *lo = 0xDC00 | (cdpt & 0x03FF);
        *hi = (uint16_t)(cdpt >> 10);
        *hi = *hi - (1<<6);
        *hi = *hi | 0xD800;
        return;
    }
}


static inline unsigned char *ulutf8_from_cdpt(int32_t c) {
    _Thread_local static unsigned char u[5];
    if (c<0 || (0xD800 <= c&&c <= 0xDBFF)) u[0]= '\0';
    else if (c < 0x80) {               // Up to 7 bits
        u[0]= (unsigned char)(c>>0   & 0x7F);  // 7 bits –> 0xxxxxxx
        u[1]= '\0';
    }
    else if (c < 0x800) {              // Up to 11 bits
        u[0]= (unsigned char)((c>>6  & 0x1F)|0xC0);  // 5 bits –> 110xxxxx
        u[1]= (unsigned char)((c     & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[2]= '\0';
    }
    else if (c < 0x10000) {            // Up to 16 bits
        u[0]= (unsigned char)((c>>12 & 0x0F)|0xE0);  // 4 bits –> 1110xxxx
        u[1]= (unsigned char)((c>>6  & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[2]= (unsigned char)((c     & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[3]= '\0';
    }
    else if (c < 0x110000) {           // Up to 21 bits
        u[0]= (unsigned char)((c>>18 & 0x07)|0xF0);  // 3 bits –> 11110xxx
        u[1]= (unsigned char)((c>>12 & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[2]= (unsigned char)((c>>6  & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[3]= (unsigned char)((c     & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[4]= '\0';
    }
    else u[0]= '\0';
    return u;
}

#endif
