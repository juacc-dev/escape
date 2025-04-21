#include "escape.h"

#include "buffer.h"
#include "variable_color.h"

int _esc_fcolorn(FILE *file, int nargs, ...) {
    va_list arg;
    va_start(arg, nargs);
    bp = 0;
    append(ESC);

    for(int i = 0; i < nargs; i++) {
        append_char(';');
        variable_color(va_arg(arg, enum ESC_Color));
    }
    append_char('m');
    int written = fwrite(buffer, sizeof(char), bp, file);

    return written ? written : -1;
}
