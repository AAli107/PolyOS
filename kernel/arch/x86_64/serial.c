#include <kernel/serial.h>
#include <kernel/io.h>

void serial_init(void) {
    outb(PORT_COM1 + 1, 0x00);
    outb(PORT_COM1 + 3, 0x80);
    outb(PORT_COM1 + 0, 0x03);
    outb(PORT_COM1 + 1, 0x00);
    outb(PORT_COM1 + 3, 0x03);
    outb(PORT_COM1 + 2, 0xC7);
    outb(PORT_COM1 + 4, 0x0B);
}

static void wait_transmit(void) {
    while ((inb(PORT_COM1 + 5) & 0x20) == 0);
}

void serial_putchar(char c) {
    wait_transmit();
    outb(PORT_COM1, c);
}