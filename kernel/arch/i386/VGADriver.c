//
// Created by kosmas on 21/2/22.
//

#include <stddef.h>
#include <stdint.h>

static uint8_t VGA_WIDTH;
static uint8_t VGA_HEIGHT;
uint8_t cursorX;
uint8_t cursorY;
uint16_t *VGABuffer;
uint8_t currentlyUsedVGAColorEntry;

#include "../../include/kernel/VGADriver.h"

uint8_t generateVGAColorEntry(enum VGAColor background, enum VGAColor foreground) {
    return (background << 4) | foreground;
}

extern unsigned char readPort(unsigned short port);
extern void writePort(unsigned short port, unsigned char data);

inline void setCursorPosition(int x, int y) {
    uint16_t position = y * VGA_WIDTH + x;

    writePort(CURSOR_COMMAND_PORT, CURSOR_SET_LOW_BYTE);
    writePort(CURSOR_DATA_PORT, position & 0xFF);
    writePort(CURSOR_COMMAND_PORT, CURSOR_SET_HIGH_BYTE);
    writePort(CURSOR_DATA_PORT, (position & 0xFF00) >> 8);
}

void VGAPutCharacter(char character) {
    if (cursorX == VGA_WIDTH || character == '\n') {
        cursorX = 0;
        cursorY++;
        if (cursorY == VGA_HEIGHT) {
            VGAScroll();
            --cursorY;
            setCursorPosition(cursorX, cursorY);
        }
        setCursorPosition(cursorX, cursorY);
        if (character == '\n') {
            return;
        }
    }
    if (character == '\b') {
        if (cursorX > 0) {
            cursorX--;
        }
        else {
            if (cursorY > 0) {
                --cursorY;
                cursorX = VGA_WIDTH - 1;
            }
        }
        VGABuffer[cursorY * VGA_WIDTH + cursorX] = ' ' | (currentlyUsedVGAColorEntry << 8);
        setCursorPosition(cursorX, cursorY);
        return;
    }
    if (cursorY == VGA_HEIGHT) {
        VGAScroll();
        --cursorY;
        setCursorPosition(cursorX, cursorY);
    }
    VGABuffer[cursorY * VGA_WIDTH + cursorX++] = character | (currentlyUsedVGAColorEntry << 8);
    setCursorPosition(cursorX, cursorY);
}

void VGAWriteString(char *string) {
    int i = 0;
    while (string[i]) {
        VGAPutCharacter(string[i++]);
    }
}

void VGAPutCharacterEntryAt(char character, uint8_t color, uint8_t x, uint8_t y) {
    VGABuffer[y * VGA_WIDTH + x] = character | (color << 8);
}

void VGAScroll() {
    for (int y = 0; y < VGA_HEIGHT; ++y) {
        for (int x = 0; x < VGA_WIDTH; ++x) {
            VGABuffer[y * VGA_WIDTH + x] = VGABuffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    for (int i = 0; i < VGA_WIDTH; ++i) {
        VGAPutCharacterEntryAt(' ', generateVGAColorEntry(VGA_COLOR_RED, VGA_COLOR_WHITE), i, VGA_HEIGHT - 1);
    }
}

void VGAInit(enum VGAColor background, enum VGAColor foreground) {
    VGA_WIDTH = 80;
    VGA_HEIGHT = 25;
    cursorX = 0;
    cursorY = 0;
    VGABuffer = (uint16_t *) 0xB8000;
    currentlyUsedVGAColorEntry = generateVGAColorEntry(background, foreground);

    for (size_t i = 0; i < VGA_HEIGHT; ++i) {
        for (size_t j = 0; j < VGA_WIDTH; ++j) {
            VGABuffer[i * VGA_WIDTH + j] = ' ' | (currentlyUsedVGAColorEntry << 8);
        }
    }
}

void changeCursorX(int places) {
    cursorX += places;
}

void changeCursorY(int places) {
    cursorY += places;
}
