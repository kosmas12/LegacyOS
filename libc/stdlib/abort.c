//
// Created by kosmas on 21/2/22.
//

#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
    // TODO: Add proper kernel panic.
	printf("kernel: panic: abort()\n");
        asm volatile("hlt");
#else
    // TODO: Abnormally terminate the process with a SIGABRT.
    printf("abort() called\n");
#endif
    while (1);
    __builtin_unreachable();
}
