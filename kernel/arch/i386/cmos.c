//
// Created by kosmas on 28/3/22.
//

#include "../../include/kernel/cmos.h"
#include <string.h>

extern void IOWait();

enum RTCFormat getRTCFormat() {
    // RTC Date/Time bytes format.
    // Default is 12-hour BCD (RTC_FORMAT_BCD_12)
    enum RTCFormat format = RTC_FORMAT_BCD_12;

    unsigned char statusBRegister = readCMOSReg(RTC_STATUS_B_REGISTER);

    // If bit 1 is set (0x2), the clock uses the 24-hour format (instead of the 12-hour format).
    int uses24HourFormat = (statusBRegister & 0x2) >> 1;
    // If bit 2 is set (0x4), the clock uses the binary format (instead of the BCD format).
    int usesBinaryFormat = (statusBRegister & 0x4) >> 1;

    /* This takes advantage of C's handling of enums as integers.
       With the bitshifts, uses24HourFormat will either be 1 or 0
       Similarly, usesBinaryFormat will either be 2 or 0
       The enum is laid out such that simple addition will give us the correct value.
    */
    format += uses24HourFormat + usesBinaryFormat;

    return format;
}

inline int BCDToBinary(int number) {
    return ((number & 0xF0) >> 1) + ( (number & 0xF0) >> 3) + (number & 0xf);
}

inline int binaryToBCD(int number) {
    return(((number / 10) << 4) | (number % 10));
}

// Checks if the computer's Real Time Clock is currently updating
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

void writeCMOSReg(char registerAddress, unsigned char data) {
    __asm__("cli");
    int disableNMIBit = 1;
    writePort(CMOS_REGISTER_PORT, ((disableNMIBit) << 7) | registerAddress);
    IOWait();
    writePort(CMOS_DATA_PORT, data);
    __asm__("sti");
}

void writeRTCData(RTCData data) {
    enum RTCFormat actualFormat = getRTCFormat();

    if ((actualFormat == RTC_FORMAT_BCD_12 || actualFormat == RTC_FORMAT_BINARY_12)
    && (data.format == RTC_FORMAT_BCD_24 || data.format == RTC_FORMAT_BINARY_24)) {
        if (data.hours > 12) {
            data.hours -= 12;
            data.hours |= 0x80;
        }
    }

    if ((actualFormat == RTC_FORMAT_BCD_12 || actualFormat == RTC_FORMAT_BCD_24)
    && (data.format == RTC_FORMAT_BINARY_12 || data.format == RTC_FORMAT_BINARY_24)) {
        data.year = binaryToBCD(data.year);
        data.month = binaryToBCD(data.month);
        data.dayOfMonth = binaryToBCD(data.dayOfMonth);
        data.hours = binaryToBCD(data.hours);
        data.minutes = binaryToBCD(data.minutes);
        data.seconds = binaryToBCD(data.seconds);
    }

    writeCMOSReg(RTC_YEAR_REGISTER, data.year);
    writeCMOSReg(RTC_MONTH_REGISTER, data.month);
    writeCMOSReg(RTC_DAYOFMONTH_REGISTER, data.dayOfMonth);
    writeCMOSReg(RTC_HOURS_REGISTER, data.hours);
    writeCMOSReg(RTC_MINUTES_REGISTER, data.minutes);
    writeCMOSReg(RTC_SECONDS_REGISTER, data.seconds);

}

FloppyInfo getFloppyInfo() {
    FloppyInfo info;
    info.numDrives = 0;

    unsigned char floppyTypesRegister = readCMOSReg(FLOPPY_TYPES_REGISTER);
    // Drive 0 is the master drive, drive 1 is the slave
    info.types[0] = (floppyTypesRegister & 0xF0) >> 4;
    info.types[1] = floppyTypesRegister & 0xF;

    if (info.types[0] != NO_DRIVE) {
        ++info.numDrives;
    }

    if (info.types[1] != NO_DRIVE) {
        ++info.numDrives;
    }

    return info;
}

unsigned char readCMOSReg(char registerAddress) {
    unsigned char disableNMIBit = 1;
    writePort(CMOS_REGISTER_PORT, ((disableNMIBit) << 7) | registerAddress);
    IOWait();
    return readPort(CMOS_DATA_PORT);
}

// NOTE: This works, but was just quickly hacked together as a primitive sleep() function.
// Use `void sleep(int ms)` from timer.h instead
void sleepSeconds(int seconds) {
    RTCData data = readRTCData();

    RTCData newData = readRTCData();
    while (newData.seconds != data.seconds + seconds) {
        newData = readRTCData();
    }

}
