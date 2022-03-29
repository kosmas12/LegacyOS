//
// Created by kosmas on 28/3/22.
//

#include "../../include/kernel/cmos.h"
#include <string.h>

extern void IOWait();

// Get the format the RTC uses for its data
enum RTCFormat getRTCFormat() {
    // RTC Date/Time bytes format.
    // Default is 12-hour BCD (RTC_FORMAT_BCD_12)
    enum RTCFormat format = RTC_FORMAT_BCD_12;

    char statusBRegister = readCMOSReg(RTC_STATUS_B_REGISTER);

    // If bit 1 is set (0x2), the clock uses the 24-hour format (instead of the 12-hour format).
    int uses24HourFormat = statusBRegister & 0x2;
    // If bit 2 is set (0x4), the clock uses the binary format (instead of the BCD format).
    int usesBinaryFormat = statusBRegister & 0x4;

    if (usesBinaryFormat) {
        format = RTC_FORMAT_BINARY_12;
        if(uses24HourFormat) {
            format = RTC_FORMAT_BINARY_24;
        }
    }
    else {
        if (uses24HourFormat) {
            // Remember, if we are here this means we are using BCD instead of binary
            format = RTC_FORMAT_BCD_24;
        }
    }

    return format;
}

// Quick BCD -> Binary conversion function
inline int BCDToBinary(int number) {
    return ((number & 0xF0) >> 1) + ( (number & 0xF0) >> 3) + (number & 0xf);
}

inline int checkRTCUpdateFlag() {
    // If updating, the highest bit of Status A will be set (0x80)
    return readCMOSReg(RTC_STATUS_A_REGISTER) & 0x80;
}

RTCData readRTCData() {
    RTCData data;
    // Copy of last values read, to verify we weren't affected by an update while reading
    RTCData lastData;

    data.format = getRTCFormat();

    while (checkRTCUpdateFlag());
    data.year = readCMOSReg(RTC_YEAR_REGISTER);
    data.month = readCMOSReg(RTC_MONTH_REGISTER);
    data.dayOfMonth = readCMOSReg(RTC_DAYOFMONTH_REGISTER);
    data.hours = readCMOSReg(RTC_HOURS_REGISTER);
    data.minutes = readCMOSReg(RTC_MINUTES_REGISTER);
    data.seconds = readCMOSReg(RTC_SECONDS_REGISTER);

    do {
        memcpy(&lastData, &data, sizeof(RTCData));
        while (checkRTCUpdateFlag());
        data.year = readCMOSReg(RTC_YEAR_REGISTER);
        data.month = readCMOSReg(RTC_MONTH_REGISTER);
        data.dayOfMonth = readCMOSReg(RTC_DAYOFMONTH_REGISTER);
        data.hours = readCMOSReg(RTC_HOURS_REGISTER);
        data.minutes = readCMOSReg(RTC_MINUTES_REGISTER);
        data.seconds = readCMOSReg(RTC_SECONDS_REGISTER);
    } while (!memcmp(&data, &lastData, sizeof(RTCData)));

    if (data.format == RTC_FORMAT_BCD_12 || data.format == RTC_FORMAT_BCD_24) {
        data.year = BCDToBinary(data.year);
        data.month = BCDToBinary(data.month);
        data.dayOfMonth = BCDToBinary(data.dayOfMonth);
        data.hours = BCDToBinary(data.hours);
        data.minutes = BCDToBinary(data.minutes);
        data.seconds = BCDToBinary(data.seconds);
    }


    // When using 12-hour time, hour 12 is actually midnight (24-hour time 0)
    if (data.hours == 12 && (data.format == RTC_FORMAT_BCD_12 || data.format == RTC_FORMAT_BINARY_12) ) {
        data.hours = 0;
    }

    // When using 12-hour time, if the highest bit is set (0x80), the time is PM.
    // We need to mask off that bit to get the actual hour
    // We also need to add 12 in order to get the 24-hour time, if we want to
    if (data.hours & 0x80 && (data.format == RTC_FORMAT_BCD_12 || data.format == RTC_FORMAT_BINARY_12)) {
        data.hours = data.hours & 0x80;
        data.hours += 12;
    }

    return data;
}

unsigned char readCMOSReg(char registerAddress) {
    int disableNMIBit = 1;
    writePort(CMOS_INPUT_PORT, ((disableNMIBit) << 7) | registerAddress);
    IOWait();
    return readPort(CMOS_OUTPUT_PORT);
}
