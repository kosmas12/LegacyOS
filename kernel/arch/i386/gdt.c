//
// Created by kosmas on 21/2/22.
//

#include "../../include/kernel/gdt.h"

/* Setup a descriptor in the Global Descriptor Table */
void GDTSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity) {
    /* Setup the descriptor base address */
    GDT[num].baseLow = (base & 0xFFFF);
    GDT[num].baseMiddle = (base >> 16) & 0xFF;
    GDT[num].baseHigh = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    GDT[num].limitLow = (limit & 0xFFFF);
    GDT[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    GDT[num].granularity |= (granularity & 0xF0);
    GDT[num].access = access;
}

/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call GDTFlush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */
void GDTInstall() {
    /* Setup the GDT pointer and limit */
    pointerToGDT.limit = (sizeof(struct GDTEntry) * 3) - 1;
    pointerToGDT.base = &GDT;

    /* Our NULL descriptor */
    GDTSetGate(0, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor. */
    GDTSetGate(1, 0x0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    GDTSetGate(2, 0x0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Flush out the old GDT and install the new changes! */
    GDTFlush();
}
