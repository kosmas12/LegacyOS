//
// Created by dionisis on 5/4/2023.
//

#include "shell.h"
#include <stdio.h>
#include <string.h>
#include "shellcommands/shellcommands.h"

void resetShellCommand(char command[256], int *index) {
    for (int i = 0; i < 256; ++i) {
        command[i] = '\0';
    }
    *index = 0;
}

int getNumericalInput(int digits) {
    int input = 0;

    char character = getchar();
    int multipleOf10 = 1;

    for (int i = 1; i < digits; ++i) {
        multipleOf10 *= 10;
    }

    int lastThingAdded = 0;
    while (character != '\n') {
        if (character >= '0' && character <= '9') {
            putchar(character);
            lastThingAdded = (character - '0') * multipleOf10;
            input += lastThingAdded;
            multipleOf10 /= 10;
        }
        character = getchar();
    }
    putchar('\n');

    return input;
}

void processShellCommand(char command[256], int *index) {
    if (strcmp(command, "writertc") == 0) {
        writeRTC();
    }
    else if (strcmp(command, "readrtc") == 0) {
        readRTC();
    }
    else if (strcmp(command, "clear") == 0) {
        clearScreen();
    }
    else if (strcmp(command, "help") == 0) {
        printHelp();
    }
    else if (strcmp(command, "floppylist") == 0) {
        floppyList();
    }
    else {
        printf("Command %s not found\n", command);
    }
    resetShellCommand(command, index);
}

void shell() {
    // Print the current time as a sort of init message
    readRTC();

    char shellCommand[256] = {'\0'};
    int index = 0;

    printf("\n");
    printf("Use the 'help' command for info on how to use the shell.\n");
    printf(">>> ");
    while (1) {
        char character = getchar();
        printf("%c", character);
        if (character != '\b' && index < 256) {
            shellCommand[index++] = character;
        }
        else {
            shellCommand[index--] = '\0';
        }
        if (character == '\n') {
            shellCommand[index - 1] = '\0';
            if (index == 1) {
                index = 0;
                shellCommand[0] = '\0';
            }
            else {
                processShellCommand(shellCommand, &index);
            }
            printf(">>> ");
        }
    }
}
