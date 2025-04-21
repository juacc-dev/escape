#include "escape.h"

#include <stdio.h>
#include <stdarg.h>
#include "buffer.h"

char format_buffer[FMT_BUF_SIZE];

// write a formatted colored string to a file
int vfprintfc(FILE* file, const char* fmt, va_list arg) {
    // call vsnprintf() first to interpret format
    int printf_status = vsnprintf(format_buffer, FMT_BUF_SIZE, fmt, arg);

    // printf returns a negative number if there was an error
    if(printf_status < 0)
        return -1;

    // finally call vfprintc() to interpret escape sequences
    return vfprintc(file, format_buffer, arg);
}
