#include "escape.h"

#include <stdio.h>
#include <stdarg.h>

static const char* const cursor_seq[] = {
    [esc_wrap_on]   = "?7h",
    [esc_wrap_off]  = "?7l",
    [esc_save]      = "s",
    [esc_restore]   = "u",
    [esc_up]        = "A",
    [esc_down]      = "B",
    [esc_right]     = "C",
    [esc_left]      = "D",
};

int esc_fcursorn(FILE* file, enum ESC_Cursor code, int nargs, ...) {
    va_list arg;
    va_start(arg, nargs);

    if(code > esc_left)
        return -1;

    int printed;

    // only escape sequences up, down, right and left
    // use an argument
    if(code < esc_up)
        printed = fprintf(file, "\x1b[%s", cursor_seq[code]);

    else {
        int d = nargs > 0 ? va_arg(arg, int) : 0;

        printed = fprintf(file, "\x1b[%d%s", d, cursor_seq[code]);
    }

    va_end(arg);
    return printed;
}
