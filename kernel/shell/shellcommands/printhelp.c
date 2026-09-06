//
// Created by kosmas on 5/4/2023.
//

#include "shellcommands.h"
#include <stdio.h>

void printHelp() {
    printf("clear - Clear the screen\n");
    printf("floppylist - List all floppy drives and what media they support/have inserted\n");
    printf("readrtc - Read the Real Time Clock's configured time\n");
    printf("writertc - Configure the time in the Real Time Clock\n");
    printf("pcilist - List PCI devices on your computer\n");
    printf("paletteswap - Change the VGA color palette\n");
    printf("help - Print this help prompt\n");
}
