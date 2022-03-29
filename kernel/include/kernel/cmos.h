//
// Created by kosmas on 28/3/22.
//

extern unsigned char readPort(unsigned short port);
extern void writePort(unsigned short port, unsigned char data);

#ifndef LEGACYOS_CMOS_H
#define LEGACYOS_CMOS_H

#define CMOS_INPUT_PORT 0x70
#define CMOS_OUTPUT_PORT 0x71

#define RTC_SECONDS_REGISTER 0x00
#define RTC_MINUTES_REGISTER 0x02
#define RTC_HOURS_REGISTER 0x04
#define RTC_WEEKDAY_REGISTER 0x06 // Not recommended at all and unused, left just in case
#define RTC_DAYOFMONTH_REGISTER 0x07
#define RTC_MONTH_REGISTER 0x08
#define RTC_YEAR_REGISTER 0x09
#define RTC_STATUS_A_REGISTER 0x0A
#define RTC_STATUS_B_REGISTER 0x0B

enum RTCFormat {
    // 12-hour binary format
    RTC_FORMAT_BINARY_12,
    // 24-hour binary format
    RTC_FORMAT_BINARY_24,
    // 12-hour BCD format
    RTC_FORMAT_BCD_12,
    // 24-hour BCD format
    RTC_FORMAT_BCD_24
};

typedef struct {
    int century;
    int year;
    int month;
    int dayOfMonth;
    int hours;
    int minutes;
    int seconds;
    enum RTCFormat format;
}RTCData;

enum RTCFormat getRTCFormat();
RTCData readRTCData();
unsigned char readCMOSReg(char registerAddress);
inline int BCDToBinary(int number);

#endif //LEGACYOS_CMOS_H
