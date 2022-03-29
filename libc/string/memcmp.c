//
// Created by kosmas on 21/2/22.
//

#include <string.h>

int memcmp(const void *pointer1, const void *pointer2, size_t numBytes) {
    const unsigned char *pointer1AsBytes = (const unsigned char *) pointer1;
    const unsigned char *pointer2AsBytes = (const unsigned char *) pointer2;

    for (size_t i = 0; i < numBytes; i++) {
        if (pointer1AsBytes[i] < pointer2AsBytes[i]) {
            return -1;
        }
        else if (pointer2AsBytes[i] < pointer1AsBytes[i]) {
            return 1;
        }
    }
    return 0;
}
