//
// Created by dionisis on 5/4/2023.
//

#include "shellcommands.h"
#include <stdio.h>

void printHelp() {
    printf("clear - Clear the screen\n");
    printf("floppylist - List all floppy drives and what media they support/have inserted\n");
    printf("readrtc - Read the Real Time Clock's configured time\n");
    printf("writertc - Configure the time in the Real Time Clock\n");
    printf("help - Print this help prompt\n");
}
