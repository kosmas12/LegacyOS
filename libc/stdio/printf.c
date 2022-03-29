//
// Created by kosmas on 21/2/22.
//

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// TODO: Set errno to EOVERFLOW in case of errors.

static bool print(const char *data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;

    for (size_t i = 0; i < length; i++) {
        if (putchar(bytes[i]) == EOF) {
            return false;
        }
    }

    return true;
}

int printf(const char *restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t allowedCharsRemaining = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%') {
                format++;
            }

            size_t amount = 1;
            while (format[amount] && format[amount] != '%') {
                amount++;
            }

            if (allowedCharsRemaining < amount) {
                return -1;
            }

            if (!print(format, amount)) {
                return -1;
            }

            format += amount;
            written += amount;
            continue;
        }

        const char *formatBeginLocation = format++;


        if (*format == 'c') {
            format++;

            char charToWrite = (char) va_arg(parameters, int /* char promotes to int */);
            if (!allowedCharsRemaining) {
                return -1;
            }

            if (!print(&charToWrite, sizeof(charToWrite))) {
                return -1;
            }

            written++;
        }
        else if (*format == 's') {
            format++;
            const char *str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (allowedCharsRemaining < len) {
                return -1;
            }
            if (!print(str, len))
                return -1;
            written += len;
        }
        else if(*format == 'd') {
            format++;

            int integer = (int) va_arg(parameters, int);
            int tempInteger = integer;

            int numDigits = 0;
            while (tempInteger != 0) {
                ++numDigits;
                tempInteger /= 10;
            }

            while (numDigits > 0) {
                int powerOf10 = 1;
                for (int i = 1; i < numDigits; ++i) {
                    powerOf10 *= 10;
                }
                numDigits--;

                int currentDigit = integer / powerOf10;
                integer -= currentDigit * powerOf10;
                char charToWrite = currentDigit + '0';
                if (!allowedCharsRemaining) {
                    return -1;
                }
                if (!print(&charToWrite, sizeof(charToWrite))) {
                    return -1;
                }
                written++;
            }
        }
        else {
            format = formatBeginLocation;
            size_t len = strlen(format);
            if (allowedCharsRemaining < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            if (!print(format, len)) {
                return -1;
            }

            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}
