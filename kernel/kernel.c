#include <stdio.h>

#include "include/kernel/VGADriver.h"
#include "include/kernel/gdt.h"
#include "include/kernel/idt.h"
#include "include/kernel/keyboard.h"
#include "include/kernel/cmos.h"
#include "include/kernel/timer.h"

int kernel_main() {
    GDTInstall();
    IDTInstall();
    VGAInit(VGA_COLOR_RED, VGA_COLOR_WHITE);
    initKeyboard();
    timerInstall();

    printf("Welcome to LegacyOS!\n\n");

    RTCData dateTimeData = readRTCData();

    printf("The computer's RTC (Real Time Clock) is set to use a ");

    switch (dateTimeData.format) {
        case RTC_FORMAT_BCD_12:
            printf("12-hour BCD format.\n");
            break;
        case RTC_FORMAT_BCD_24:
            printf("24-hour BCD format.\n");
            break;
        case RTC_FORMAT_BINARY_12:
            printf("12-hour binary format.\n");
            break;
        case RTC_FORMAT_BINARY_24:
            printf("24-hour binary format.\n");
            break;
        default:
            printf("format that is... not valid? How'd we even end up here?\n");
            break;
    }

    printf("The system date and time is configured to be %d/%d/%d, %d:%d:%d\n", dateTimeData.dayOfMonth,
           dateTimeData.month,
           dateTimeData.year,
           dateTimeData.hours,
           dateTimeData.minutes,
           dateTimeData.seconds);

    while (1) {
        char character = getchar();
        printf("%c", character);
    }

    return 0;
}
