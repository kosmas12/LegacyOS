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

/* Defines an IDT entry */
struct IDTEntry {
    unsigned short baseLow;
    unsigned short selector;   /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short baseHigh;
} __attribute__((packed));

struct IDTPointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct IDTEntry idt[256];
struct IDTPointer pointerToIDT;


void IDTSetGate(unsigned char num, unsigned long base, unsigned short selector, unsigned char flags);
/* This exists in 'start.asm', and is used to load our IDT */
extern void IDTLoad();
void IDTUpdate();
void IDTInstall();

#endif //LEGACYOS_IDT_H
