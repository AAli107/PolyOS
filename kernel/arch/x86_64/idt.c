#include <kernel/idt.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  flags;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static interrupt_handler_t handlers[256];

// defined in isr_stubs.S
extern void *isr_table[256];

static void set_entry(int num, void *handler, uint8_t flags) {
    uint64_t addr = (uint64_t)handler;
    idt[num].offset_low  = addr & 0xFFFF;
    idt[num].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[num].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[num].selector    = 0x08;
    idt[num].ist         = 0;
    idt[num].flags       = flags;
    idt[num].reserved    = 0;
}

void interrupt_dispatch(interrupt_frame_t *frame) {
    interrupt_handler_t h = handlers[frame->int_num];
    if (h) h(frame);
        
    if (frame->int_num < 32) {
        printf("Unhandled exception %u\n", frame->int_num);

        for (;;)
            asm volatile("hlt");
    }
}

void idt_register_handler(uint8_t num, interrupt_handler_t handler) {
    handlers[num] = handler;
}

void idt_init(void) {
    memset(handlers, 0, sizeof(handlers));

    for (int i = 0; i < 256; i++)
        set_entry(i, isr_table[i], 0x8E);  // interrupt gate, ring 0

    struct idtr idtr = {
        .limit = sizeof(idt) - 1,
        .base  = (uint64_t)idt
    };

    __asm__ volatile ("lidt %0" : : "m"(idtr) : "memory");

    printf("%s", "IDT initialized    [  ok  ]\n");
}