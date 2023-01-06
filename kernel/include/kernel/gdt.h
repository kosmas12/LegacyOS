//
// Created by kosmas on 21/2/22.
//

#ifndef LEGACYOS_GDT_H
#define LEGACYOS_GDT_H

/* Special data structure to hold a segment entry for the Global Descriptor Table.
   This structure MUST hold:
      * The segment's base address in the computer's memory
      * The size of the segment
      * The access byte for the segment
      * The flags for the segment

   This structure NEEDS to be packed
*/
struct GDTEntry {
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMiddle;
    unsigned char access;
    unsigned char granularity;
    unsigned char baseHigh;
} __attribute__((packed));

/* Special data structure to hold a pointer to a Global Descriptor Table.
   This structure MUST hold:
      * The Global Descriptor Table's size, decremented by 1
      * The Global Descriptor Table's base address in the computer's memory

   This structure NEEDS to be packed
*/
struct GDTPointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// Updates the processor to store the new Global Descriptor Table
extern void GDTFlush();

// Installs our operating system's GDT to the processor
void GDTInstall();

#endif //LEGACYOS_GDT_H
