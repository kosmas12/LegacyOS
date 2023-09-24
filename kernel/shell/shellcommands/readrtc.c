//
// Created by kosmas on 5/4/2023.
//

#include "shellcommands.h"
#include "../../include/kernel/cmos.h"
#include <stdio.h>

void readRTC() {
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

    printf("The system date and time is configured to be ");

    // Make sure these are always printed with 2 digits e.g. 9 is changed to 09 and 12 is kept the same

    if (dateTimeData.dayOfMonth < 10) {
        printf("0");
    }
    printf("%d/", dateTimeData.dayOfMonth);

    if (dateTimeData.month < 10) {
        printf("0");
    }
    printf("%d/", dateTimeData.month);

    printf("%d, ", dateTimeData.year);

    if (dateTimeData.hours < 10) {
        printf("0");
    }
    printf("%d:", dateTimeData.hours);

    if (dateTimeData.minutes < 10) {
        printf("0");
    }
    printf("%d:", dateTimeData.minutes);

    if (dateTimeData.seconds < 10) {
        printf("0");
    }
    printf("%d\n", dateTimeData.seconds);
}
