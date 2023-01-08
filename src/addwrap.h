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

#ifndef ADDWRAP_H__
#define ADDWRAP_H__

#include <limits.h>

// #define ulCROWDEDNAMESPACE
#ifndef ulCROWDEDNAMESPACE
  #define addwrap(...)         uladdwrap(__VA_ARGS__)
#endif

//===========================================================================
//                    Add signed integers with wraparound.
//===========================================================================
#define uladdwrap(a, b) _Generic((a),                                   \
                                char: _Generic((b),                     \
                                            char: uladdwrapchar,        \
                                     signed char: uladdwrapsignedchar,  \
                                           short: uladdwrapshort,       \
                                             int: uladdwrapint,         \
                                            long: uladdwraplong,        \
                                         default: uladdwraplonglong),   \
                         signed char: _Generic((b),                     \
                                            char: uladdwrapsignedchar,  \
                                     signed char: uladdwrapsignedchar,  \
                                           short: uladdwrapshort,       \
                                             int: uladdwrapint,         \
                                            long: uladdwraplong,        \
                                         default: uladdwraplonglong),   \
                               short: _Generic((b),                     \
                                            char: uladdwrapshort,       \
                                     signed char: uladdwrapshort,       \
                                           short: uladdwrapshort,       \
                                             int: uladdwrapint,         \
                                            long: uladdwraplong,        \
                                         default: uladdwraplonglong),   \
                                 int: _Generic((b),                     \
                                            char: uladdwrapint,         \
                                     signed char: uladdwrapint,         \
                                           short: uladdwrapint,         \
                                             int: uladdwrapint,         \
                                            long: uladdwraplong,        \
                                         default: uladdwraplonglong),   \
                                long: _Generic((b),                     \
                                            char: uladdwraplong,        \
                                     signed char: uladdwraplong,        \
                                           short: uladdwraplong,        \
                                             int: uladdwraplong,        \
                                            long: uladdwraplong,        \
                                         default: uladdwraplonglong),   \
                             default: ul__addwraplonglong)(a, b)

static inline char uladdwrapchar(char a, char b) {
#if SCHAR_MIN == CHAR_MIN
    char result;
    result = ((a<0 && b<CHAR_MIN-a) ? (CHAR_MAX+(b-(CHAR_MIN-a)+1)) :
              (a>0 && b>CHAR_MAX-a) ? (CHAR_MIN+(b-(CHAR_MAX-a)-1)) :
              (a + b));
    return result;
#else
    // If char is unsigned, wrapping is built into the compiler.
    return a + b;
#endif
}

static inline signed char uladdwrapsignedchar(signed char a, signed char b) {
    signed char result;
    result = ((a<0 && b<SCHAR_MIN-a) ? (SCHAR_MAX+(b-(SCHAR_MIN-a)+1)) :
              (a>0 && b>SCHAR_MAX-a) ? (SCHAR_MIN+(b-(SCHAR_MAX-a)-1)) :
              (a + b));
    return result;
}

static inline short uladdwrapshort(short a, short b) {
    short result;
    result = ((a<0 && b<SHRT_MIN-a) ? (SHRT_MAX+(b-(SHRT_MIN-a)+1)) :
             (a>0 && b>SHRT_MAX-a) ? (SHRT_MIN+(b-(SHRT_MAX-a)-1)) :
             (a + b));
    return result;
}

static inline int uladdwrapint(int a, int b) {
    int result;
    result = ((a<0 && b<INT_MIN-a) ? (INT_MAX+(b-(INT_MIN-a)+1)) :
             (a>0 && b>INT_MAX-a) ? (INT_MIN+(b-(INT_MAX-a)-1)) :
             (a + b));
    return result;
}

static inline long uladdwraplong(long a, long b) {
    long result;
    result = ((a<0 && b<LONG_MIN-a) ? (LONG_MAX+(b-(LONG_MIN-a)+1)) :
              (a>0 && b>LONG_MAX-a) ? (LONG_MIN+(b-(LONG_MAX-a)-1)) :
              (a + b));
    return result;
}

static inline long long uladdwraplonglong(long long a, long long b) {
    long long result;
    result = ((a<0 && b<LLONG_MIN-a) ? (LLONG_MAX+(b-(LLONG_MIN-a)+1)) :
              (a>0 && b>LLONG_MAX-a) ? (LLONG_MIN+(b-(LLONG_MAX-a)-1)) :
              (a + b));
    return result;
}


#endif
