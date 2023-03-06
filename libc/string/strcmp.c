//
// Created by kosmas on 5/3/2023.
//

#include <string.h>

int strcmp(const char *str1, const char *str2) {
    size_t shortestStringLength = 0;

    if (strlen(str1) > strlen(str2)) {
        shortestStringLength = strlen(str2);
    }
    else {
        shortestStringLength = strlen(str1);
    }

    for (int i = 0; i <= shortestStringLength; ++i) {
        if (str1[i] > str2[i]) {
            return 1;
        }
        else if (str1[i] < str2[i]) {
            return -1;
        }
    }
    return 0;
}
