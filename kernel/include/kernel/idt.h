//
// Created by kosmas on 21/2/22.
//

#ifndef LEGACYOS_IDT_H
#define LEGACYOS_IDT_H

#define PIC1_DATA_PORT 0x21
#define PIC1_COMMAND_PORT 0x20
#define PIC2_DATA_PORT 0xA1
#define PIC2_COMMAND_PORT 0xA0
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

/* Special data structure to hold an Interrupt Descriptor entry for the Interrupt Descriptor Table.
   This structure MUST hold:
      * The Interrupt Service Routine's base address
      * A Segment Selector which points to a code segment in the Global Descriptor Table
      * The Gate Type, which defines what the Descriptor is supposed to serve
      * The privilege level needed to run the interrupt

   This structure NEEDS to be packed
*/
struct IDTEntry {
    unsigned short baseLow;
    unsigned short selector;   /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short baseHigh;
} __attribute__((packed));


/* Special data structure to hold a pointer to an Interrupt Descriptor Table.
   This structure MUST hold:
      * The Interrupt Descriptor Table's size, decremented by 1
      * The Interrupt Descriptor Table's base address in the computer's memory

   This structure NEEDS to be packed
*/
struct IDTPointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// Creates an Interrupt Descriptor entry
void IDTSetGate(unsigned char num, unsigned long base, unsigned short selector, unsigned char flags);

// Loads the Interrupt Descriptor Table to the processor
extern void IDTLoad();

// Updates the processor's Interrupt Descriptor Table. Really just calls `IDTLoad()`
void IDTUpdate();

// Creates the initial Interrupt Descriptor Table and loads it to the processor
void IDTInstall();

#endif //LEGACYOS_IDT_H
