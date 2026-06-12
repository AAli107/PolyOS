#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t int_num, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
} interrupt_frame_t;

typedef void (*interrupt_handler_t)(interrupt_frame_t *frame);

void idt_init(void);
void idt_register_handler(uint8_t num, interrupt_handler_t handler);

#endif