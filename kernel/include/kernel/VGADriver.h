//
// Created by kosmas on 21/1/22.
//

#ifndef LEGACYOS_VGADRIVER_H
#define LEGACYOS_VGADRIVER_H

#include <stdint.h>

// RESEARCHME: Are these the actual names?
#define CURSOR_COMMAND_PORT 0x3D4
#define CURSOR_DATA_PORT 0x3D5

#define CURSOR_SET_LOW_BYTE 0xF
#define CURSOR_SET_HIGH_BYTE 0xE

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

// Generates a VGA color entry byte from the given background and foreground colors
uint8_t generateVGAColorEntry(enum VGAColor background, enum VGAColor foreground);

void setVGAColorEntry(enum VGAColor background, enum VGAColor foreground);

uint8_t getVGACurrentHeight();
uint8_t getVGACurrentWidth();

inline void setCursorPosition(int x, int y);

// Puts a character to the VGA buffer
void VGAPutCharacter(char character);

void VGAPutCharacterEntryAt(char character, uint8_t color, uint8_t x, uint8_t y);

void VGAClearScreen();

// Writes a whole NULL-terminated string to the VGA buffer
void VGAWriteString(char *string);

// Scrolls the VGA buffer
void VGAScroll();

// Initializes VGA text mode
void VGAInit(enum VGAColor background, enum VGAColor foreground);

// Change the X position of the text cursor by `places`
void changeCursorX(int places);

// Change the Y position of the text cursor by `places`
void changeCursorY(int places);

// Get the character written in position (x, y) on the screen
char VGAGetCharacterAt(uint8_t x, uint8_t y);

// Get the color of the pixel in position (x, y) on the screen
enum VGAColor VGAGetColorAt(uint8_t x, uint8_t y);


#endif //LEGACYOS_VGADRIVER_H
