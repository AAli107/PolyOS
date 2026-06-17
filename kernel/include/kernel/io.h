#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H

#include <stdint.h>

// Used to manage hardware IRQs, remap interrupt vectors, and send EOI
#define PORT_PIC1_CMD       0x20    // master PIC command - send EOI, read ISR/IRR
#define PORT_PIC1_DATA      0x21    // master PIC data - read/write interrupt mask
#define PORT_PIC2_CMD       0xA0    // slave PIC command
#define PORT_PIC2_DATA      0xA1    // slave PIC data - read/write interrupt mask
#define PORT_PIC_EOI        0x20    // value to write to CMD port to send EOI

// Used for debug output over serial
// Base port varies per COM port; add offsets 0-7 for specific registers
#define PORT_COM1           0x3F8   // COM1 base - IRQ4
#define PORT_COM2           0x2F8   // COM2 base - IRQ3
#define PORT_COM3           0x3E8   // COM3 base - IRQ4 (shared)
#define PORT_COM4           0x2E8   // COM4 base - IRQ3 (shared)

// Used to read keyboard scancodes
#define PORT_PS2_DATA       0x60    // read scancode byte, write command data

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port) : "memory");
    return val;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif