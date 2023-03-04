//
// Created by kosmas on 22/2/22.
//

#include <stdio.h>
#include "../../include/kernel/keyboard.h"
#include "../../include/kernel/VGADriver.h"

#define PS2_DATA_PORT 0x60
// Command port is the same as status port
#define PS2_COMMAND_PORT 0x64
// Status port is the same as the command port
#define PS2_STATUS_PORT 0x64

#define COMMAND_READ_CONFIGURATION_BYTE 0x20
#define COMMAND_WRITE_CONFIGURATION_BYTE 0x60
#define COMMAND_DISABLE_SECOND_PS2 0xA7
#define COMMAND_ENABLE_SECOND_PS2 0xA8
#define COMMAND_TEST_SECOND_PS2 0xA9
#define COMMAND_TEST_PS2_CONTROLLER 0xAA
#define COMMAND_TEST_FIRST_PS2 0xAB
#define COMMAND_DISABLE_FIRST_PS2 0xAD
#define COMMAND_ENABLE_FIRST_PS2 0xAE
#define COMMAND_RESET 0xFF
// By default, the controller writes to the first PS/2 port
#define COMMAND_WRITE_BYTE_TO_SECOND_PS2 0xD4

#define PS2_CONTROLLER_TEST_PASSED 0x55
#define PS2_CONTROLLER_TEST_FAIL 0xFC
#define PS2_PORT_TEST_PASSED 0x00
#define PS2_PORT_TEST_CLOCK_STUCK_LOW 0x01
#define PS2_PORT_TEST_CLOCK_STUCK_HIGH 0x02
#define PS2_PORT_TEST_DATA_STUCK_LOW 0x03
#define PS2_PORT_TEST_DATA_STUCK_HIGH 0x04

#define PS2_DEVICE_COMMAND_ACKNOWLEDGED 0xFA

extern unsigned char readPort(unsigned short port);
extern void writePort(unsigned short port, unsigned char data);
extern void keyboardHandler();

char buffer[256];
volatile uint8_t bufferFilled = 0;

char popChar() {
    // This loop will stop when the keyboard interrupt is triggered or if there already is data
    while (bufferFilled == 0);
    bufferFilled--;
    return buffer[0];
}

void pushChar(char character) {
    buffer[bufferFilled++] = character;
}

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

void resetPS2Device(int device) {
    while (readPort(PS2_STATUS_PORT) & 2);
    if (device == 2) {
        writePort(PS2_COMMAND_PORT, COMMAND_WRITE_BYTE_TO_SECOND_PS2);
        IOWait();
    }
    while (readPort(PS2_STATUS_PORT) & 2);
    writePort(PS2_DATA_PORT, COMMAND_RESET);
    IOWait();
    while ((readPort(PS2_STATUS_PORT) & 1) == 0);

    uint8_t response = 0;
    if (readPort(PS2_DATA_PORT) == PS2_DEVICE_COMMAND_ACKNOWLEDGED) {
        while ((readPort(PS2_STATUS_PORT) & 1) == 0);
        response = readPort(PS2_DATA_PORT);
        switch (response) {
            case 0xAA:
                if (device == 1) {
                    printf("First PS/2 device successfully reset.\n");
                }
                else {
                    printf("Second PS/2 device successfully reset.\n");
                }
                break;
            case 0xFC:
            case 0xFD:
                if (device == 1) {
                    printf("First PS/2 device reset failed.\n");
                }
                else {
                    printf("Second PS/2 device reset failed.\n");
                }
                break;
            case 0xFE:
                resetPS2Device(device);
                break;
            default:
                if (device == 1) {
                    printf("Unknown response byte received from first PS/2 device %d", response);
                }
                else {
                    printf("Unknown response byte received from second PS/2 device %d", response);
                }
                break;
        }
    }
    else {
        printf("Response to reset command: %d", response);
        while (1) {
            __asm__("hlt");
        }
    }
}

void initKeyboard() {
    __asm__("cli");
    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_COMMAND_PORT, COMMAND_READ_CONFIGURATION_BYTE);
    IOWait();
    while ((readPort(PS2_STATUS_PORT) & 1) == 0);
    uint8_t configurationByte = readPort(PS2_DATA_PORT);

    // Clear bits 0 and 1 to disable IRQs
    configurationByte &= ~0x1;
    configurationByte &= ~0x2;
    if ((configurationByte & 0x20) >> 5) {
        printf("The detected PS/2 controller is a dual channel controller.\n");
    }
    else {
        printf("The detected PS/2 controller is a single channel controller.\n");
    }

    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_COMMAND_PORT, COMMAND_DISABLE_FIRST_PS2);
    IOWait();
    if ((configurationByte & 0x20) >> 5) {
        while (readPort(PS2_STATUS_PORT) & 2);
        writePort(PS2_COMMAND_PORT, COMMAND_DISABLE_SECOND_PS2);
        IOWait();
    }

    // Discard any leftover data from previously
    while ((readPort(PS2_STATUS_PORT) & 1) != 0) {
        readPort(PS2_DATA_PORT);
    }

    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_COMMAND_PORT, COMMAND_WRITE_CONFIGURATION_BYTE);
    IOWait();
    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_DATA_PORT, configurationByte);
    IOWait();

    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_COMMAND_PORT, COMMAND_TEST_PS2_CONTROLLER);
    IOWait();
    while ((readPort(PS2_STATUS_PORT) & 1) == 0);
    if (readPort(PS2_DATA_PORT) == PS2_CONTROLLER_TEST_PASSED) {
        printf("PS/2 controller self test passed.\n");
    }

    int portTests = 0;
    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_COMMAND_PORT, COMMAND_TEST_FIRST_PS2);
    IOWait();
    while ((readPort(PS2_STATUS_PORT) & 1) == 0);
    if (readPort(PS2_DATA_PORT) == PS2_PORT_TEST_PASSED) {
        printf("First PS/2 port test passed.\n");
        while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
        writePort(PS2_COMMAND_PORT, COMMAND_ENABLE_FIRST_PS2);
        IOWait();
        portTests += 1;
    }
    else {
        printf("First PS/2 port test failed.\n");
    }

    if ((configurationByte & 0x20) >> 5) {
        while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
        writePort(PS2_COMMAND_PORT, COMMAND_TEST_SECOND_PS2);
        IOWait();
        while ((readPort(PS2_STATUS_PORT) & 1) == 0);
        if (readPort(PS2_DATA_PORT) == PS2_PORT_TEST_PASSED) {
            printf("Second PS/2 port test passed.\n");
            while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
            writePort(PS2_COMMAND_PORT, COMMAND_ENABLE_SECOND_PS2);
            IOWait();
            portTests += 2;
        }
        else {
            printf("Second PS/2 port test failed.\n");
        }
    }

    if (portTests & 1) {
        printf("First PS/2 port enabled.\n");
    }
    if (portTests & 2) {
        printf("Second PS/2 port enabled.\n");
    }
    if (!portTests) {
        printf("No usable PS/2 ports. Halting...\n");
        while (1) {
            __asm__("hlt");
        }
    }

    resetPS2Device(1);
    if ((configurationByte & 0x20) >> 5) {
        resetPS2Device(2);
    }

    while ((readPort(PS2_STATUS_PORT) & 1) != 0) {
        readPort(PS2_DATA_PORT);
    }

    unsigned long keyboardAddress = (unsigned long) keyboardHandler;
    IDTSetGate(0x21, keyboardAddress, 0x09, 0x8E);
    IDTUpdate();

    // Enable PS/2 IRQ1 and translation (set bits 0 and 6)
    configurationByte |= 0x1;
    configurationByte |= 0x40;
    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_COMMAND_PORT, COMMAND_WRITE_CONFIGURATION_BYTE);
    IOWait();
    while ((readPort(PS2_STATUS_PORT) & 2) >> 1);
    writePort(PS2_DATA_PORT, configurationByte);
    IOWait();

    __asm__("sti");

}

int capsLockEnabled = 0;

void keyboardHandlerMain() {
    unsigned char status;
    char keycode;

    /* Write EOI (End Of Interrupt) */
    writePort(PIC1_COMMAND_PORT, 0x20);
    IOWait();

    status = readPort(PS2_STATUS_PORT);
    /* Lowest bit will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = readPort(PS2_DATA_PORT);
        if(keycode < 0) {
            return;
        }
        switch (keycode) {
            case 0x3A:
                capsLockEnabled = !capsLockEnabled;
                return;
            case 0x0E:
                pushChar('\b');
                return;
            default:
                if (capsLockEnabled && (keyboardMap[keycode] - 32 >= 'A' && keyboardMap[keycode] - 32 <= 'Z')) {
                    pushChar(keyboardMap[keycode] - 32);
                }
                else {
                    pushChar(keyboardMap[keycode]);
                }
        }
    }
}
