//
// Created by kosmas on 21/2/22.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../../include/kernel/VGADriver.h"

uint8_t generateVGAColorEntry(enum VGAColor background, enum VGAColor foreground) {
    return (background << 4) | foreground;
}

void VGAPutCharacter(char character) {
    if (cursorX == VGA_WIDTH || character == '\n') {
        cursorX = 0;
        cursorY++;
        if (character == '\n') {
            return;
        }
    }
    if (cursorY == VGA_HEIGHT) {
        VGAScroll();
    }
    VGABuffer[cursorY * VGA_WIDTH + cursorX++] = character | (currentlyUsedVGAColorEntry << 8);
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
    --cursorY;
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
