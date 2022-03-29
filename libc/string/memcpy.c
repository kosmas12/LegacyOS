//
// Created by kosmas on 21/2/22.
//

#include <string.h>

void *memcpy(void *restrict destinationPointer, const void *restrict sourcePointer, size_t size) {
    unsigned char *destinationAsBytes = (unsigned char *) destinationPointer;
    const unsigned char *sourceAsBytes = (const unsigned char *) sourcePointer;

    for (size_t i = 0; i < size; i++) {
        destinationAsBytes[i] = sourceAsBytes[i];
    }

    return destinationPointer;
}
