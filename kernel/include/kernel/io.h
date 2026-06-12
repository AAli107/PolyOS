#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H

#include <stdint.h>

#define PIC1_CMD    0x20    // master PIC command port
#define PIC1_DATA   0x21    // master PIC data port (IMR read/write)
#define PIC2_CMD    0xA0    // slave PIC command port
#define PIC2_DATA   0xA1    // slave PIC data port
#define PIC_EOI     0x20    // End-Of-Interrupt command

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port) : "memory");
    return val;
}

void io_wait(void) {
    outb(0x80, 0);
}

#endif