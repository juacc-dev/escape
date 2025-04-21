#include "escape.h"

#include <stdio.h>
#include <stdarg.h>
#include "buffer.h"

// write a formatted colored string to a file
int fprintfc(FILE* file, const char* fmt, ...) {
    // initialize variable arguments
    va_list arg;
    va_start(arg, fmt);

    // print formatted colored string to file
    int printed = vfprintfc(file, fmt, arg);

    va_end(arg);
    return printed;
}
