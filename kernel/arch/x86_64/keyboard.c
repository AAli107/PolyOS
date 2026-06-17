#include <kernel/keyboard.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0,
};

static const char scancode_to_ascii_shift[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0, ' ', 0,
};

#define LSHIFT_PRESSED 0x2A
#define LSHIFT_RELEASED 0xAA
#define RSHIFT_PRESSED 0x36
#define RSHIFT_RELEASED 0xB6

#define KB_BUFFER_SIZE 256
static char kb_buffer[KB_BUFFER_SIZE];
static size_t kb_head = 0;
static size_t kb_tail = 0;
static bool shiftHeld = false;

static void kb_buffer_push(char c) {
    size_t next = (kb_head + 1) % KB_BUFFER_SIZE;
    if (next == kb_tail) return;
    kb_buffer[kb_head] = c;
    kb_head = next;
}

void keyboard_init(void)
{
    kb_head = 0;
    kb_tail = 0;
    shiftHeld = false;
    printf("%s", "-> Keyboard initialized.\n");
}

void keyboard_handle_scancode(uint8_t scancode)
{
    if (scancode == LSHIFT_PRESSED || scancode == RSHIFT_PRESSED) {
        shiftHeld = true;
        return;
    }
    if (scancode == LSHIFT_RELEASED || scancode == RSHIFT_RELEASED) {
        shiftHeld = false;
        return;
    }

    bool released = scancode & 0x80;
    if (released) return;

    uint8_t index = scancode & 0x7F;

    char c = shiftHeld ? scancode_to_ascii_shift[index] : scancode_to_ascii[index];
    if (c != 0)
        kb_buffer_push(c);
}

char keyboard_getchar(void)
{
    if (kb_head == kb_tail) return 0;
    char c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;
    return c;
}
