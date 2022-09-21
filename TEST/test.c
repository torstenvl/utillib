#include <stdio.h>
#include "utillib.h"

int main(void) {
    int64_t a = 0x7fffffffffffffffL;
    int64_t b = 0x7fffffffffffffffL;
    long long result = addwrap(a, b);

    printf("The result should be -2, and is %lld\n", result);

    return 0;
}
