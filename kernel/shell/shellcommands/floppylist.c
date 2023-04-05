//
// Created by dionisis on 5/4/2023.
//

#include "shellcommands.h"
#include "../../include/kernel/cmos.h"
#include <stdio.h>

void floppyList() {
    FloppyInfo info = getFloppyInfo();

    printf("Number of drives connected to the system: %d\n", info.numDrives);

    for (int i = 0; i < info.numDrives; ++i) {
        printf("Drive %d - ", i);
        switch (info.types[i]) {
            case FIVE_360KB:
                printf("5.25\" 360KB");
                break;
            case FIVE_1200KB:
                printf("5.25\" 1.2MB");
                break;
            case THREE_720KB:
                printf("3.5\" 720KB");
                break;
            case THREE_1440KB:
                printf("3.5\" 1.44MB");
                break;
            case THREE_2880KB:
                printf("3.5\" 2.88MB");
                break;
            case NO_DRIVE:
                printf("No drive");
                break;
        }
        printf("\n");
    }
}
