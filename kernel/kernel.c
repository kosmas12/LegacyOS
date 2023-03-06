#include <stdio.h>
#include <string.h>

#include "include/kernel/VGADriver.h"
#include "include/kernel/gdt.h"
#include "include/kernel/idt.h"
#include "include/kernel/keyboard.h"
#include "include/kernel/cmos.h"
#include "include/kernel/timer.h"

void resetShellCommand(char command[256], int *index) {
    for (int i = 0; i < 256; ++i) {
        command[i] = '\0';
    }
    *index = 0;
}

int getNumericalInput(int digits) {
    int input = 0;

    char character = getchar();
    int multipleOf10 = 1;

    for (int i = 1; i < digits; ++i) {
        multipleOf10 *= 10;
    }

    while (character != '\n') {
        if (character >= '0' && character <= '9') {
            putchar(character);
            input += (character - '0') * multipleOf10;
            multipleOf10 /= 10;
        }
        character = getchar();
    }
    putchar('\n');

    return input;
}

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

    printf("The system date and time is configured to be %d/%d/%d, %d:%d:%d\n", dateTimeData.dayOfMonth,
           dateTimeData.month,
           dateTimeData.year,
           dateTimeData.hours,
           dateTimeData.minutes,
           dateTimeData.seconds);
}

void clearScreen() {
    VGAClearScreen();
}

void printHelp() {
    printf("writertc - Configure the time in the Real Time Clock\n");
    printf("readrtc - Read the Real Time Clock's configured time\n");
    printf("clear - Clear the screen\n");
    printf("help - Print this help prompt\n");
}

void processShellCommand(char command[256], int *index) {
    if (strcmp(command, "writertc") == 0) {
        writeRTC();
    }
    else if (strcmp(command, "readrtc") == 0) {
        readRTC();
    }
    else if (strcmp(command, "clear") == 0) {
        clearScreen();
    }
    else if (strcmp(command, "help") == 0) {
        printHelp();
    }
    else {
        printf("Comamnd %s not found\n", command);
    }
    resetShellCommand(command, index);
}

int kernel_main() {
    GDTInstall();
    IDTInstall();
    VGAInit(VGA_COLOR_RED, VGA_COLOR_WHITE);
    initKeyboard();
    timerInstall();

    printf("Welcome to LegacyOS!\n\n");

    char shellCommand[256] = {'\0'};
    int index = 0;

    readRTC();

    printf("\n");
    printf("Use the 'help' command for info on how to use the shell.\n");
    printf(">>> ");
    while (1) {
        char character = getchar();
        printf("%c", character);
        if (character != '\b') {
            shellCommand[index++] = character;
        }
        if (character == '\n') {
            shellCommand[index - 1] = '\0';
            if (index == 1) {
                index = 0;
                shellCommand[0] = '\0';
            }
            else {
                processShellCommand(shellCommand, &index);
            }
            printf(">>> ");
        }
    }

    return 0;
}
