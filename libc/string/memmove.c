//
// Created by kosmas on 21/2/22.
//

#include <string.h>

void *memmove(void *destinationPointer, const void *sourcePointer, size_t numBytes) {
    unsigned char *destinationAsBytes = (unsigned char *) destinationPointer;
    const unsigned char *sourceAsBytes = (const unsigned char *) sourcePointer;

    if (destinationAsBytes < sourceAsBytes) {
        for (size_t i = 0; i < numBytes; i++) {
            destinationAsBytes[i] = sourceAsBytes[i];
        }
    } else {
        for (size_t i = numBytes; i != 0; i--) {
            destinationAsBytes[i - 1] = sourceAsBytes[i - 1];
        }
    }
    return destinationPointer;
}
