#include <stdio.h>

#include "include/kernel/VGADriver.h"
#include "include/kernel/gdt.h"
#include "include/kernel/idt.h"
#include "include/kernel/keyboard.h"
#include "include/kernel/cmos.h"
#include "include/kernel/timer.h"
#include "include/kernel/ata.h"
#include "shell/shell.h"

int kernel_main() {
    GDTInstall();
    IDTInstall();
    VGAInit(VGA_COLOR_BLACK, VGA_COLOR_CYAN);
    initKeyboard();
    timerInstall();
    ATAInit();

    printf("Welcome to LegacyOS!\n\n");

    shell();

    return 0;
}
