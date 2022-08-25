//
// Created by kosmas on 22/2/22.
//

#include <stdio.h>
#include "../../include/kernel/keyboard.h"
#include "../../include/kernel/VGADriver.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

extern unsigned char readPort(unsigned short port);
extern void writePort(unsigned short port, unsigned char data);
extern void keyboardHandler();

char keyboardMap[128] = {
   0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
   '9', '0', '-', '=', '\b',    /* Backspace */
   '\t',            /* Tab */
   'q', 'w', 'e', 'r',    /* 19 */
   't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    /* Enter key */
   0,            /* 29   - Control */
   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',    /* 39 */
   '\'', '`', 0,        /* Left shift */
   '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
   'm', ',', '.', '/', 0,                /* Right shift */
   '*',
   0,    /* Alt */
   ' ',    /* Space bar */
   0,    /* Caps lock */
   0,    /* 59 - F1 key ... > */
   0, 0, 0, 0, 0, 0, 0, 0,
   0,    /* < ... F10 */
   0,    /* 69 - Num lock*/
   0,    /* Scroll Lock */
   0,    /* Home key */
   0,    /* Up Arrow */
   0,    /* Page Up */
   '-',
   0,    /* Left Arrow */
   0,
   0,    /* Right Arrow */
   '+',
   0,    /* 79 - End key*/
   0,    /* Down Arrow */
   0,    /* Page Down */
   0,    /* Insert Key */
   0,    /* Delete Key */
   0, 0, 0,
   0,    /* F11 Key */
   0,    /* F12 Key */
   0,    /* All other keys are undefined */
};

void IOWait() {
    writePort(0x80, 0); // Some random unused port
}

void initKeyboard() {
    unsigned long keyboardAddress = (unsigned long) keyboardHandler;
    IDTSetGate(0x21, keyboardAddress, 0x09, 0x8E);

    IDTUpdate();
}

int capsLockEnabled = 0;

void keyboardHandlerMain() {
    unsigned char status;
    char keycode;

    /* Write EOI (End Of Interrupt) */
    writePort(PIC1_COMMAND_PORT, 0x20);
    IOWait();

    status = readPort(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = readPort(KEYBOARD_DATA_PORT);
        if(keycode < 0) {
            return;
        }
        switch (keycode) {
            case 0x3A:
                capsLockEnabled = !capsLockEnabled;
                return;
            case 0x0E:
                changeCursorX(-1);
                putchar(' ');
                changeCursorX(-1);
                return;
            default:
                if (capsLockEnabled) {
                    putchar(keyboardMap[keycode] - 32);
                }
                else {
                    putchar(keyboardMap[keycode]);
                }
        }
    }
}
