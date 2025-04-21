#include "escape.h"

#include <stdio.h>
#include <stdarg.h>
#include "buffer.h"

// write a colored string to stdout
int printc(const char* fmt, ...) {
    // initialize variable arguments
    va_list arg;
    va_start(arg, fmt);

    // print colored string to stdout
    int printed = vfprintc(stdout, fmt, arg);

    va_end(arg);
    return printed;
}
