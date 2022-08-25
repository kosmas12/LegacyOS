//
// Created by kosmas on 31/3/22.
//

#include <stdio.h>
#include "../../include/kernel/timer.h"
#include "../../include/kernel/idt.h"

extern void writePort(unsigned short port, unsigned char data);
extern unsigned char readPort(unsigned short port);

volatile int passedTicks = 0;

inline unsigned char makePITCommand(int channel, int accessMode, int operationMode, int bcd) {
    unsigned char command = 0;

    command |= (bcd & 1);
    command |= ((operationMode & 7) << 1);
    command |= ((accessMode & 3) << 4);
    command |= ((channel & 3) << 6);
    return command;
}

void timerInit(int hz) {
    int divisor = PIT_INPUT_CLOCK_HZ/hz;

    unsigned char command = makePITCommand(0, 3, 2, 0);

    writePort(PIT_COMMAND_REGISTER, command);
    writePort(PIT_DATA_REGISTER_0, divisor & 0xFF);
    writePort(PIT_DATA_REGISTER_0, divisor >> 8);

}

void timerSetReloadValue(int hz) {
    int divisor = PIT_INPUT_CLOCK_HZ/hz;

    writePort(PIT_DATA_REGISTER_0, divisor & 0xFF);
    writePort(PIT_DATA_REGISTER_0, divisor >> 8);
}

extern void IOWait();
void increment1ms() {
    writePort(PIC1_COMMAND_PORT, 0x20);
    IOWait();
    ++passedTicks;
}

void wait(int ms) {
    int currentPassedTicks = passedTicks;
    while (passedTicks < currentPassedTicks + ms);
}

extern void timerHandler();
void timerInstall() {
    unsigned long timerHandlerAddress = (unsigned long) timerHandler;

    IDTSetGate(0x20, timerHandlerAddress, 0x09, 0x8E);
    IDTUpdate();

    timerInit(1000);
}
