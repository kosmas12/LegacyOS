//
// Created by kosmas on 7/9/26.
//

#include <stdio.h>
#include "shellcommands.h"
#include "../../include/kernel/VGADriver.h"
#include "../shell.h"

void paletteSwap() {
    uint8_t foreground = 255;
    uint8_t background = 255;

    printf("Standard color options: black(00), blue(01), green(02), cyan(03), red(04), magenta(05), brown(06), gray(08), "
           "yellow(14), white(15).\n");
    printf("Light color options: gray(07), blue(09), green(10), cyan(11), red(12), magenta(13)\n");

    printf("Please enter the numbers EXACTLY as shown, always 2 digits long.\n");

    while (foreground > 15) {
        printf("Enter foreground color number: ");
        foreground = getNumericalInput(2);
    }

    while (background > 15) {
        printf("Enter background color number: ");
        background = getNumericalInput(2);
    }

    setVGAColorEntry(background, foreground);

    for (int y = 0; y < getVGACurrentHeight(); ++y) {
        for (int x = 0; x < getVGACurrentWidth(); ++x) {
            VGAPutCharacterEntryAt(VGAGetCharacterAt(x, y), generateVGAColorEntry(background, foreground), x, y);
        }
    }

}