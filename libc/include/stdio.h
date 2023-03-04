//
// Created by kosmas on 21/2/22.
//

#ifndef STDIO_H
#define STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char * __restrict, ...);
int putchar(int);
int puts(const char*);
int getchar(void);

#ifdef __cplusplus
}
#endif

#endif //STDIO_H
