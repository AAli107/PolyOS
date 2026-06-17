#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/serial.h>

#define SAFETY_CHECK if (!terminal_isSafe()) return

#define VGA_WIDTH 80
#define VGA_HEIGHT 40

static size_t terminal_row;
static size_t terminal_column;
static terminal_char terminal_buffer[VGA_WIDTH * VGA_HEIGHT];
static bool dirty_buffer[VGA_WIDTH * VGA_HEIGHT];
static struct color32 _backgroundColor;
static struct color32 _foregroundColor;

static void terminal_putTerminalCharAt(const terminal_char tc, size_t x, size_t y) {
	int index = y * VGA_WIDTH + x;
	terminal_buffer[index] = tc;
	dirty_buffer[index] = true;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	_backgroundColor = (struct color32){
		.b = 0,
		.g = 0,
		.r = 0,
		.a = 255
	};
	_foregroundColor = (struct color32){
		.b = 192,
		.g = 192,
		.r = 192,
		.a = 255
	};
	terminal_clear();

    printf("%s", "-> Terminal initialized.\n");
}

void terminal_clear()
{
	SAFETY_CHECK;

	terminal_char clearChar = {
		.character = ' ',
		.bgColor = _backgroundColor,
		.fgColor = _foregroundColor
	};

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			terminal_putTerminalCharAt(clearChar, x, y);
		}
	}
}

void terminal_putentryat(unsigned char c, struct color32 fgColor, struct color32 bgColor, size_t x, size_t y) {
	if (!terminal_isSafePosition(x, y)) return;
	
	terminal_putTerminalCharAt(
		(terminal_char) {
			.character = c,
			.bgColor = bgColor,
			.fgColor = fgColor
		}, x, y
	);
}

void terminal_putchar(char c) 
{	
	SAFETY_CHECK;

	unsigned char uc = c;
	terminal_putentryat(uc, _foregroundColor, _backgroundColor, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			terminal_row = VGA_HEIGHT - 1;
			terminal_scroll();
		}
	}
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
	{
		switch (data[i]) {
			case '\n': terminal_newLine();  break;
			case '\t': terminal_indent();   break;
			default:   terminal_putchar(data[i]); break;
		}
		serial_putchar(data[i]);
	}
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_newLine(void)
{
	SAFETY_CHECK;

    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
        terminal_row--;
        terminal_scroll();
    }
}

void terminal_indent(void) {
	SAFETY_CHECK;

    size_t new_col = terminal_column + (4 - (terminal_column % 4));
    if (new_col >= VGA_WIDTH)
        terminal_newLine();
    else terminal_column = new_col;
}

void terminal_scroll()
{
    SAFETY_CHECK;

    for (size_t y = 0; y < (VGA_HEIGHT - 1); y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = terminal_readEntryAt(x, y + 1);
        }
    }

	terminal_char clearChar = {
		.character = ' ',
		.bgColor = _backgroundColor,
		.fgColor = _foregroundColor
	};

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = clearChar;
    }

	memset(dirty_buffer, true, VGA_WIDTH * VGA_HEIGHT);
}

void terminal_setBackground(struct color32 color) {
	_backgroundColor = color;
}

void terminal_setForeground(struct color32 color) {
	_foregroundColor = color;
}

struct color32 terminal_getBackground() {
	return _backgroundColor;
}

struct color32 terminal_getForeground() {
	return _foregroundColor;
}

bool terminal_isSafe()
{
	return terminal_isSafePosition(terminal_column, terminal_row);
}

bool terminal_isSafePosition(size_t x, size_t y)
{
	return x < VGA_WIDTH && y < VGA_HEIGHT;
}

terminal_char terminal_readEntryAt(size_t x, size_t y)
{
	if (!terminal_isSafePosition(x, y)) return (terminal_char){0};
	return terminal_buffer[y * VGA_WIDTH + x];
}

void terminal_render(int startX, int startY, uint32_t scale)
{
	if (!video_isInit()) return;

	uint32_t glyph_width = font_getWidth();
	uint32_t glyph_height = font_getHeight();

	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			int index = y * VGA_WIDTH + x;
			if (dirty_buffer[index]) {
				terminal_char tChar = terminal_buffer[index];
				font_drawChar(
					startX + (x * glyph_width * scale),
					startY + (y * glyph_height * scale),
					tChar.character,
					*(const uint32_t*)&tChar.fgColor,
					*(const uint32_t*)&tChar.bgColor,
					scale
				);
				dirty_buffer[index] = false;
			}
		}
	}
}

void terminal_setCursorPosition(size_t row, size_t column)
{
	terminal_column = column;
	terminal_row = row;
}