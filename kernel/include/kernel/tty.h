#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void terminal_initialize(void);
void terminal_clear();
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_newLine(void);
void terminal_indent(void);
void terminal_scroll();
bool terminal_isSafe();
bool terminal_isSafePosition(size_t x, size_t y);
uint16_t terminal_readEntryAt(size_t x, size_t y);

#endif
