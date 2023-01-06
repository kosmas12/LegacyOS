//
// Created by kosmas on 22/2/22.
//

#ifndef LEGACYOS_KEYBOARD_H
#define LEGACYOS_KEYBOARD_H

#include "idt.h"

// Initializes the keyboard (PIC masking, appropriate IDT entry, etc.)
void initKeyboard();

#endif //LEGACYOS_KEYBOARD_H
