//
// Created by kosmas on 21/2/22.
//

#include <stdio.h>

#if defined(__is_libk)
#include <kernel/VGADriver.h>
#endif

int putchar(int charAsInt) {
#if defined(__is_libk)
    char character = (char) charAsInt;
	VGAPutCharacter(character);
#else
    // Nothing we can do yet for user space
#endif
    return charAsInt;
}

