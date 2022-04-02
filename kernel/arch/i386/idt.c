//
// Created by kosmas on 21/2/22.
//

#include "../../include/kernel/idt.h"
#include <string.h>

void IDTSetGate(unsigned char num, unsigned long base, unsigned short selector, unsigned char flags) {

    idt[num].baseLow = (base & 0xFFFF);
    idt[num].baseHigh = (base & 0xFFFF0000) >> 16;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].selector = selector;

}

// New function used for clarification, because seeing IDTLoad() out of nowhere would seem weird
void IDTUpdate() {
    IDTLoad();
}

extern unsigned char readPort(unsigned short port);
extern void writePort(unsigned short port, unsigned char data);
extern void IOWait();
/* Installs the IDT */
void IDTInstall() {

    /* ICW1 - begin initialization */
    writePort(PIC1_COMMAND_PORT , ICW1_INIT | ICW1_ICW4);
    IOWait();
    writePort(PIC2_COMMAND_PORT , ICW1_INIT | ICW1_ICW4);
    IOWait();

    /* ICW2 - remap offset address of IDT */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * The first 32 interrupts are "reserved" for CPU exceptions
    */
    writePort(PIC1_DATA_PORT , 0x20);
    IOWait();
    writePort(PIC2_DATA_PORT , 0x28);
    IOWait();

    writePort(PIC1_DATA_PORT , 0x4);
    IOWait();
    writePort(PIC2_DATA_PORT , 0x2);
    IOWait();

    /* ICW4 - environment info */
    writePort(PIC1_DATA_PORT , ICW4_8086);
    IOWait();
    writePort(PIC2_DATA_PORT , ICW4_8086);
    IOWait();
    /* Initialization finished */

    /* mask interrupts */
    writePort(PIC1_DATA_PORT , 0);
    IOWait();
    writePort(PIC2_DATA_PORT , 0);
    IOWait();

    /* 0xFC is 11111100 - enables only IRQ0 (PIT) IRQ1 (keyboard) */
    writePort(PIC1_DATA_PORT , 0xFC);
    IOWait();

    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    pointerToIDT.limit = (sizeof(struct IDTEntry) * 256) - 1;
    pointerToIDT.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct IDTEntry) * 256);

    /* Add any new ISRs to the IDT here using IDTSetGate() */

    /* Points the processor's internal register to the new IDT */
    IDTLoad();
}
