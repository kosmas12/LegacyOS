//
// Created by kosmas on 21/1/22.
//

#ifndef LEGACYOS_VGADRIVER_H
#define LEGACYOS_VGADRIVER_H

#include <stdint.h>
#include <stddef.h>

enum VGAColor {
    VGA_COLOR_BLACK = 0x0,
    VGA_COLOR_BLUE = 0x1,
    VGA_COLOR_GREEN = 0x2,
    VGA_COLOR_CYAN = 0x3,
    VGA_COLOR_RED = 0x4,
    VGA_COLOR_MAGENTA = 0x5,
    VGA_COLOR_BROWN = 0x6,
    VGA_COLOR_LIGHT_GRAY = 0x7,
    VGA_COLOR_DARK_GRAY = 0x8,
    VGA_COLOR_LIGHT_BLUE = 0x9,
    VGA_COLOR_LIGHT_GREEN = 0xA,
    VGA_COLOR_LIGHT_CYAN = 0xB,
    VGA_COLOR_LIGHT_RED = 0xC,
    VGA_COLOR_LIGHT_MAGENTA = 0xD,
    VGA_COLOR_YELLOW = 0xE,
    VGA_COLOR_WHITE = 0xF
};

uint8_t currentlyUsedVGAColorEntry;

static size_t VGA_WIDTH;
static size_t VGA_HEIGHT;
size_t cursorX;
size_t cursorY;
uint16_t *VGABuffer;

uint8_t generateVGAColorEntry(enum VGAColor background, enum VGAColor foreground);
void VGAPutCharacter(char character);
void VGAWriteString(char *string);
void VGAScroll();
void VGAInit(enum VGAColor background, enum VGAColor foreground);


#endif //LEGACYOS_VGADRIVER_H
