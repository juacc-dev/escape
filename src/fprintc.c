#include "escape.h"

#include <stdio.h>
#include <stdarg.h>
#include "buffer.h"

// write a colored string to a file
int fprintc(FILE* file, const char* fmt, ...) {
    // initialize variable arguments
    va_list arg;
    va_start(arg, fmt);

    // write colored string to file
    int printed = vfprintc(file, fmt, arg);

    va_end(arg);
    return printed;
}
