//
// Created by dionisis on 3/3/2023.
//

#include "../../kernel/include/kernel/keyboard.h"

int getchar(void) {
    return popChar();
}
