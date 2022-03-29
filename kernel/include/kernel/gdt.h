//
// Created by kosmas on 21/2/22.
//

#ifndef LEGACYOS_GDT_H
#define LEGACYOS_GDT_H

struct GDTEntry {
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMiddle;
    unsigned char access;
    unsigned char granularity;
    unsigned char baseHigh;
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct GDTPointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct GDTEntry GDT[3];
struct GDTPointer pointerToGDT;

/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */
extern void GDTFlush();

void GDTInstall();

#endif //LEGACYOS_GDT_H
