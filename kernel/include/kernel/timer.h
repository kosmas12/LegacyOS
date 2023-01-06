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

/* Creates a PIT command byte using:
    * The selected channel
    * The selected access and operations modes
    * The numerical mode for the channel
*/
inline unsigned char makePITCommand(int channel, int accessMode, int operationMode, int bcd);

// Initializes the Programmable Interval Timer, with `hz` giving the times per second we want the IRQ to fire
void timerInit(int hz);

/* Sets the Programmable Interval Timer's reload value, necessary for it to precisely fire when we want it to.
   The reload value is calculated as follows:
      * PIT_FREQ_HZ = 1193180
      * RELOAD_VALUE = PIT_FREQ_HZ/DESIRED_HZ
*/
void timerSetReloadValue(int hz);

// Sleeps for a given amount of milliseconds
void sleep(int ms);

// Installs the timer interrupt
void timerInstall();

#endif //LEGACYOS_TIMER_H
