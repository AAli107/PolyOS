#include <kernel/keyboard.h>

static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0,
};


void keyboard_init(void)
{
}

void keyboard_handle_scancode(uint8_t scancode)
{
}

char keyboard_getchar(void)
{
    return 0;
}
