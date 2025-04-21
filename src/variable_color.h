#ifndef VARIABLE_COLOR_H
#define VARIABLE_COLOR_H

#include "escape.h"
#include "buffer.h"

#define BASE 10
#define MAX_ESC (esc_white + esc_BRIGHT + esc_BG)

// simple and fast conversion from integer to decimal representation
// only works for positive integers from 0 to 2**16
static inline unsigned short base10_itoa(unsigned short x, char* outbuf) {
    char inbuf[6];
    register char* inbufptr = &inbuf[0];
    register unsigned short i = 0;

    // convert to decimal (reversed)
    do {
        register unsigned short rem  = x % BASE;
        inbuf[i++] = rem + '0';
        x = x/BASE;
    } while(x != 0);

    // reverse string
    unsigned short digits = i;
    while(i--)
        outbuf[i] = *inbufptr++;

    return digits;
}

// appends to buffer the escape sequence correspoinding to code
static inline int variable_color(enum ESC_Color code) {
    if(code > MAX_ESC)
        return 1;

    bp += base10_itoa(code, &buffer[bp]);
    return 0;
}

#endif /* VARIABLE_COLOR_H */
