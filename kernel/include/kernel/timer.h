//
// Created by kosmas on 31/3/22.
//

#ifndef LEGACYOS_TIMER_H
#define LEGACYOS_TIMER_H

#define PIT_INPUT_CLOCK_HZ 1193180
#define PIT_DATA_REGISTER_0 0x40
#define PIT_DATA_REGISTER_1 0x41
#define PIT_DATA_REGISTER_2 0x42
#define PIT_COMMAND_REGISTER 0x43

inline unsigned char makePITCommand(int channel, int accessMode, int operationMode, int bcd);
void timerInit(int hz);
void timerSetReloadValue(int hz);
void wait(int ms);
void timerInstall();

#endif //LEGACYOS_TIMER_H
