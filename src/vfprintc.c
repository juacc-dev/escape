#include "escape.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "buffer.h"
#include "variable_color.h"

#define FMT '$'
#define MAX_ESC_LEN (sizeof(ESC "\x1b[100m") + 1)
#define BUF_LIMIT (BUF_SIZE - MAX_ESC_LEN)

// definition of the buffer used throughout the library
char buffer[BUF_SIZE];
unsigned bp;

static inline const char* color_directive(const char* fmt) {
    // copy of fmt before FMT ('$')
    const char* fmt_bak = fmt - 2;

    // code for the escape sequence
    // normal foreground by default
    register unsigned short code = 30;

    char ch = *fmt++;

    // background
    if(ch == 'B') {
        code += 10;
        ch = *fmt++;
    }

    // bright
    if(ch == 'h') {
        code += 60;
        ch = *fmt++;
    }

    // color
    if(ch >= '0' && ch <= '7') {
        // '0' -> 0, '7' -> 7
        code += ch - '0';

        variable_color(code);
        append_char('m');
    }

    // invalid
    else {
        // delete ESC
        bp -= sizeof(ESC) - 1;

        while(fmt_bak < fmt)
            append_char(*fmt_bak++);
    }

    return fmt;
}

// write a colored string to a file
int vfprintc(FILE* file, const char* fmt, va_list arg) {
    bp = 0;

    // for "$n"
    enum ESC_Color var;

    char ch;
    while((ch = *fmt++)) {
        if(ch == FMT) {
            append(ESC);

            switch(ch = *fmt++) {
                // "$$" -> "$"
                case FMT:
                    // delete ESC
                    bp -= sizeof(ESC) - 1;

                    append_char(FMT);
                    break;

                // color
                case 'c':
                    fmt = color_directive(fmt);
                    break;

                // variable
                case 'n':
                    var = va_arg(arg, enum ESC_Color);
                    if(variable_color(var))
                        goto invalid_format;
                    goto end_escape;


                /* modes */

                // reset all
                case 'r':
                    append_char('0');
                    goto end_escape;

                // bold
                case 'b':
                    append_char('1');
                    goto end_escape;

                // dim (low)
                case 'l':
                    append_char('2');
                    goto end_escape;

                // italic
                case 'i':
                    append_char('3');
                    goto end_escape;

                // underline
                case 'u':
                    append_char('4');
                    goto end_escape;

                // blinking
                case 'k':
                    append_char('5');
                    goto end_escape;

                // inverse
                case 'v':
                    append_char('7');
                    goto end_escape;

                // invisible
                case 's':
                    append_char('8');
                    goto end_escape;

                // strikethrough
                case 't':
                    append_char('9');
                    goto end_escape;

                // sequence introducer
                case 'E':
                    append_char('[');
                    break;


                // invalid format
                default:
                    invalid_format:
                    // delete ESC
                    bp -= sizeof(ESC) - 1;

                    append_char(FMT);
                    append_char(ch);
                    break;

                    end_escape:
                    // add 'm' to terminate valid sequences
                    append_char('m');
            }
        }
        else {
            append_char(ch);
        }

        if(bp >= BUF_LIMIT)
            fwrite(buffer, sizeof(char), bp, file);
    }

    // reset escape sequence
    append("\x1b[0m");

    fwrite(buffer, sizeof(char), bp, file);


    return bp;
}
