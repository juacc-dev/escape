#ifndef BUFFER_H
#define BUFFER_H

#include <string.h>

#define BUF_SIZE        512
#define FMT_BUF_SIZE    512

extern char buffer[BUF_SIZE];
extern unsigned bp;
extern char format_buffer[FMT_BUF_SIZE];

// append string literal to buffer
#define append(str) memcpy(&buffer[bp], str, sizeof(str) - 1);\
                    (bp += sizeof(str) - 1)

// append string to buffer
#define append_str(str) (register size_t len = strlen(str));\
                        memcpy(&buffer[bp], str, strlen(str));\
                        (bp += len)

// append char to buffer
#define append_char(ch) (buffer[bp++] = ch)

#define undo(len)   (bp -= len)  

#endif /* BUFFER_H */
