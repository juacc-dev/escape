#ifndef ESCAPE_H
#define ESCAPE_H

/*  #\\~                                                                ~//#  *
 *  \                                ESCAPE                                /  *
 *  /                                 v0.5                                 \  *
 *  \                     A library to use ANSI escape                     /  *
 *  /                           sequences easily                           \  *
 *  #\\~                                                                ~//#  */

/*
 *  TL;DR: e.g. "$c2" is green, "$cBh1" is bright red background, "$b" is bold.
 *
 *  Functions ending in -c (like printc()) which accept a format string work as
 *  follows:
 *
 *  The string may contain directives which will not be printed but rather
 *  interpreted and will cause to print an escape sequence.
 *
 *  A directive may specify a color or mode. Directives can also be variable and
 *  passed as another argument to the function.
 *
 *  '$' indicates the start of a directive, similar to '%' in printf(). "$$"
 *  prints a single '$'.
 *
 *  'c' indicated a color. The color directive has a special syntax:
 *  "$c['B']['h']<n>". <n> is a number from 0 to 7 which specifies the color
 *  where:
 *      0 - black
 *      1 - red
 *      2 - green
 *      3 - yellow
 *      4 - blue
 *      5 - magenta
 *      6 - cyan
 *      7 - white
 *  'B' stands for "Background" and indicated that the color will be set for the
 *  background rather than the foreground.
 *  'h' is for the "bright" (High) version of the color, it can be combined with
 *  'B'
 *
 *  'n' indicates a variable sequence that that is specified in a separate
 *  argument passed to the function. e.g. `printc("$n", bold);`. Any value from
 *  the Color enum can be passed as a parameter except for "BRIGHT" and "BG",
 *  which are  added (with the "+" operator) to a color. e.g.
 *  `printc("$n", green + BRIGHT);`.
 *
 *  'E' indicates an escape sequence introducer, it has the same effect as
 *  putting a literal "\x1b[" in the string.
 *
 *  All the sequences for bold, italic and similar are specified by a single
 *  character:
 *      'r' - reset
 *      'b' - bold
 *      'l' - dim
 *      'i' - italic
 *      'u' - underline
 *      'k' - blinking
 *      'v' - inverse
 *      's' - invisible
 *      't' - strikethrough
 *
 *  Non-valid directives will be printed as they appear in the string. e.g
 *  "$abc" will print literally "$abc".
 *
 *  A reset escape sequence will be printed at the end of the string.
 *
 *
 *  Functions ending in -fc (like printfc()) will combine printf() with
 *  printc(). Any arguments for format (interpreted by printf()) need to be
 *  passed before any for color since the input string is first parsed by
 *  printf() and then by printc(). e.g `printfc("%d $n%c\n", 12, 'A', red);`.
 *  This means that `printfc("%ssome color\n", "$c3");` or even
 *  `printfc("%ssome other color color\n", "$n", blue);` will work fine.
 *
 *
 *  The functions color() and fcolor() take a Color enum and print the
 *  corresponding escape sequence. The set color or mode won't be cleared until
 *  a reset sequence is printed specifically. A call to printc() or similar
 *  functions will print such a sequence automatically.
 *
 *
 *  The functions cursor() and fcursor() work the same as color() and fcolor()
 *  except that they take a Cursor enum as input.
 */

// Add C++ bindings
#ifdef __cplusplus
#include <cstdio>
#include <cstdarg>

extern "C" {
namespace esc {

#else
#include <stdio.h>
#include <stdarg.h>

#endif /* __cplusplus */

// Get number of variadic arguments, support for zero arguments.
#define _ESC_NARGS(...) (sizeof((int[]){0, __VA_ARGS__}) / sizeof(int) - 1)

#define ESC "\x1b["
// Escape sequences supported by the printc() and color() family of functions.
// Each enum value corresponds to the number inside a sequence.
// e.g. code esc_red (value 31) maps to "\x1b[31m".
enum ESC_Color {
    // Modes
    esc_reset         = 0,
    esc_bold          = 1,
    esc_dim           = 2,
    esc_italic        = 3,
    esc_underline     = 4,
    esc_blinking      = 5, // 6 is not valid
    esc_inverse       = 7,
    esc_invisible     = 8,
    esc_strikethrough = 9,

    // Colors
    esc_black   = 30,
    esc_red     = 31,
    esc_green   = 32,
    esc_yellow  = 33,
    esc_blue    = 34,
    esc_magenta = 35,
    esc_cyan    = 36,
    esc_white   = 37,

    // Color modifiers.
    // Should be added to a color, e.g green + BRIGHT is bright green.
    // red + BRIGHT + BG is bright red background.
    // Should not be used with modes.
    esc_BG     = 10,
    esc_BRIGHT = 60,
};

// Escape sequences supported by and cursor(), fcursor() and fcursorn().
enum ESC_Cursor {
    esc_wrap_on,
    esc_wrap_off,
    esc_save,
    esc_restore,
    esc_up,
    esc_down,
    esc_right,
    esc_left,
};

/* ##  printc functions  ## */

// Write a colored string to a file.
// Variadic, not meant for direct use.
int vfprintc(FILE* file, const char* fmt, va_list arg);

// Write a formatted colored string to a file.
// Arguments for format shuold be passed before any for color.
// Variadic, not meant for direct use.
int vfprintfc(FILE* file, const char* fmt, va_list arg);

// Write a number of bytes of a colored string to a buffer.
// Not meant for direct use.
// Not implemented.
// int vsnprintc(char* buf, size_t n, const char* fmt, va_list arg);

// Write a number of bytes of formatted colored output to a buffer.
// Arguments for format shuold be passed before any for color.
// Not meant for direct use.
// Not implemented.
// int vsnprintfc(char* buf, size_t n, const char* fmt, va_list arg);

// Write a colored string to a file.
int fprintc(FILE* file, const char* fmt, ...);

// Write a formatted colored string to a file.
// Arguments for format shuold be passed before any for color.
int fprintfc(FILE* file, const char* fmt, ...);

// write a colored string to stdout.
int printc(const char* fmt, ...);

// Print a formatted colored string.
// Arguments for format shuold be passed before any for color.
int printfc(const char* fmt, ...);

/* ##  direct escape sequences  ## */

// Write a number of escape sequence to a file.
// Number of argument has to be passed, not meant for direct use.
int _esc_fcolorn(FILE* file, int nargs, ...);

#ifndef __cplusplus
// Write escape sequences to a file.
// Multiple codes can be passed. e.g. `esc_fcolor(stderr, red, bold)`.
#define esc_fcolor(file, ...) \
    _esc_fcolorn(file, _ESC_NARGS(__VA_ARGS__), __VA_ARGS__)

// Write escape sequences to a stdout.
// Multiple codes can be passed. e.g. `esc_color(red, bold)`.
#define esc_color(...) \
    _esc_fcolorn(stdout, _ESC_NARGS(__VA_ARGS__), __VA_ARGS__)
#else
// Write escape sequences to a file.
// Multiple codes can be passed. e.g. `fcolor(stderr, red, bold)`.
#define fcolor(file, ...) \
    _esc_fcolorn(file, _ESC_NARGS(__VA_ARGS__), __VA_ARGS__)

// Write escape sequences to a stdout.
// Multiple codes can be passed. e.g. `color(red, bold)`.
#define color(...) _esc_fcolorn(stdout, _ESC_NARGS(__VA_ARGS__), __VA_ARGS__)
#endif

/* ##  cursor movement  ## */

// Write a escape sequence related to cursor control to a file.
// If moving the cursor, an additional integer can be passed to specify specify
// the distance.
// Number of argument has to be passed, not meant for direct use
int _esc_fcursorn(FILE* file, enum ESC_Cursor, int nargs, ...);

// write a escape sequence related to cursor control to a file.
// if moving the cursor, an additional integer can be passed to specify specify
// the distance.
#define esc_fcursor(file, code, ...) \
    _esc_fcursorn(file, code, _ESC_NARGS(__VA_ARGS__), ##__VA_ARGS__)

// move, save or restore the position of the cursor.
// if moving the cursor, an additional integer can be passed to specify specify
// the distance.
#define esc_cursor(code, ...) \
    _esc_fcursorn(stdout, code, _ESC_NARGS(__VA_ARGS__), ##__VA_ARGS__)

#ifdef __cplusplus

// C++ would return int when adding enums and make color() not accept int
// as parameter. This overload allows to call color(red + BG)
inline enum Color operator+(enum Color color, enum Color modifier) {
    return Color((int) color + (int) modifier);
}

} /* namespace esc */
} /* extern "C" */

#endif /* __cplusplus */

#endif /* ESCAPE_H */
