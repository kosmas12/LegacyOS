//
// Created by kosmas on 21/2/22.
//

#include <string.h>

void *memset(void *bufferPointer, int value, size_t numBytes) {
    unsigned char *bufferAsBytes = (unsigned char *) bufferPointer;

    for (size_t i = 0; i < numBytes; i++) {
        bufferAsBytes[i] = (unsigned char) value;
    }

    return bufferPointer;
}
