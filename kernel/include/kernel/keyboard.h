#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
void keyboard_handle_scancode(uint8_t scancode);

// returns the next buffered character, or 0 if none available
char keyboard_getchar(void);

#endif