#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/video.h>

struct terminal_char
{
	unsigned char character;
	struct color32 bgColor;
	struct color32 fgColor;
};

void terminal_initialize(void);
void terminal_clear();
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_newLine(void);
void terminal_indent(void);
void terminal_scroll();
void terminal_setBackground(struct color32 color);
void terminal_setForeground(struct color32 color);
struct color32 terminal_getBackground();
struct color32 terminal_getForeground();
bool terminal_isSafe();
bool terminal_isSafePosition(size_t x, size_t y);
struct terminal_char terminal_readEntryAt(size_t x, size_t y);
void terminal_render(int x, int y, uint32_t scale);
void terminal_setCursorPosition(size_t row, size_t column);

#endif
