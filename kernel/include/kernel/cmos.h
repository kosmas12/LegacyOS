//
// Created by kosmas on 28/3/22.
//

extern unsigned char readPort(unsigned short port);
extern void writePort(unsigned short port, unsigned char data);

#ifndef LEGACYOS_CMOS_H
#define LEGACYOS_CMOS_H

#define CMOS_REGISTER_PORT 0x70
#define CMOS_DATA_PORT 0x71

#define RTC_SECONDS_REGISTER 0x00
#define RTC_MINUTES_REGISTER 0x02
#define RTC_HOURS_REGISTER 0x04
#define RTC_WEEKDAY_REGISTER 0x06 // Not recommended at all and unused, left just in case
#define RTC_DAYOFMONTH_REGISTER 0x07
#define RTC_MONTH_REGISTER 0x08
#define RTC_YEAR_REGISTER 0x09
#define RTC_STATUS_A_REGISTER 0x0A
#define RTC_STATUS_B_REGISTER 0x0B

#define FLOPPY_TYPES_REGISTER 0x10

// Enum used to make code that deals with RTC formats more readable
enum RTCFormat {
    // 12-hour BCD format
    RTC_FORMAT_BCD_12 = 0,
    // 24-hour BCD format
    RTC_FORMAT_BCD_24 = 1,
    // 12-hour binary format
    RTC_FORMAT_BINARY_12 = 2,
    // 24-hour binary format
    RTC_FORMAT_BINARY_24 = 3
};

// Enum used to make code that deals with floppy media more readable
enum FloppyType {
    NO_DRIVE = 0,
    FIVE_360KB = 1,
    FIVE_1200KB = 2,
    THREE_720KB = 3,
    THREE_1440KB = 4,
    THREE_2880KB = 5
};

/* Special data structure to hold data that is meant to be sent/received to/from the computer's Real Time Clock.
   This structure can hold:
      * The century
      * The year
      * The month
      * The day of the month
      * The hours
      * The minutes
      * The seconds
      * The numerical format used in the data (BCD or binary, 12-hour or 24-hour)
*/
typedef struct {
    unsigned char century;
    unsigned char year;
    unsigned char month;
    unsigned char dayOfMonth;
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
    enum RTCFormat format;
}RTCData;

/* Special data structure to hold info about the media that is supported the system's floppy drives.
   This structure can hold:
      * The number of drives
      * The type of media for all 6 drives a system usually can have
   NOTE: While systems can have up to 3 controllers, CMOS memory only has info for the first one.
   This means that, until further research, only the media types of the first 2 drives are guaranteed to be valid
*/
typedef struct {
    unsigned char numDrives;
    enum FloppyType types[6];
}FloppyInfo;

// Gets the format that the computer's Real Time Clock uses to store the time
enum RTCFormat getRTCFormat();

// Reads the data from the computer's Real Time Clock
RTCData readRTCData();

// Write data to the computer's Real Time Clock
void writeRTCData(RTCData data);

// Get info about the media support for the connected floppy drives
FloppyInfo getFloppyInfo();

// Reads the specified CMOS register
unsigned char readCMOSReg(char registerAddress);

// Fast function to convert BCD number to binary
inline int BCDToBinary(int number);

// Waits `seconds` using the computer's Real Time Clock for timekeeping
void sleepSeconds(int seconds);

#endif //LEGACYOS_CMOS_H
