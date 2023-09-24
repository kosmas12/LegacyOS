//
// Created by kosmas on 5/4/2023.
//

#include "shellcommands.h"
#include "../../include/kernel/cmos.h"
#include "../shell.h"
#include <stdio.h>

void writeRTC() {
    RTCData dataToWrite;
    dataToWrite.format = RTC_FORMAT_BINARY_24;

    printf("Please enter the year (last 2 digits): ");
    dataToWrite.year = getNumericalInput(2);

    printf("Please enter the month (01-12): ");
    dataToWrite.month = getNumericalInput(2);

    printf("Please enter the day of the month (01-31): ");
    dataToWrite.dayOfMonth = getNumericalInput(2);

    printf("Please enter the hours (00-23): ");
    dataToWrite.hours = getNumericalInput(2);

    printf("Please enter the minutes (00-59): ");
    dataToWrite.minutes = getNumericalInput(2);

    printf("Please enter the seconds (00-59): ");
    dataToWrite.seconds = getNumericalInput(2);

    writeRTCData(dataToWrite);

}
