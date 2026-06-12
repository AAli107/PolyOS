.section .text

# Stubs for exceptions WITHOUT error code — push dummy 0
.macro ISR_NOERR num
.global __isr_stub_\num
__isr_stub_\num:
    pushq $0
    pushq $\num
    jmp isr_common
.endm

# Stubs for exceptions WITH error code — CPU already pushed it
.macro ISR_ERR num
.global __isr_stub_\num
__isr_stub_\num:
    pushq $\num
    jmp isr_common
.endm

# CPU exceptions 0-31
ISR_NOERR 0    # Divide by zero
ISR_NOERR 1    # Debug
ISR_NOERR 2    # NMI
ISR_NOERR 3    # Breakpoint
ISR_NOERR 4    # Overflow
ISR_NOERR 5    # Bound range exceeded
ISR_NOERR 6    # Invalid opcode
ISR_NOERR 7    # Device not available
ISR_ERR   8    # Double fault
ISR_NOERR 9    # Coprocessor segment overrun
ISR_ERR   10   # Invalid TSS
ISR_ERR   11   # Segment not present
ISR_ERR   12   # Stack segment fault
ISR_ERR   13   # General protection fault
ISR_ERR   14   # Page fault
ISR_NOERR 15
ISR_NOERR 16   # x87 FPU error
ISR_ERR   17   # Alignment check
ISR_NOERR 18   # Machine check
ISR_NOERR 19   # SIMD FP exception
ISR_NOERR 20   # Virtualization exception
ISR_ERR   21   # Control protection
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_ERR   29   # VMM communication
ISR_ERR   30   # Security exception
ISR_NOERR 31

# IRQs 32-255 — none have error codes, generate with a loop
.altmacro
.macro ISR_NOERR_N num
.global __isr_stub_\num
__isr_stub_\num:
    pushq $0
    pushq $\num
    jmp isr_common
.endm

.set i, 32
.rept 224
    ISR_NOERR_N %i
    .set i, i+1
.endr

# Common handler — saves all regs, calls C, restores, returns
isr_common:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    mov %rsp, %rdi       # first arg = pointer to interrupt_frame_t
    call interrupt_dispatch

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

    add $16, %rsp        # skip int_num and error_code
    iretq

.macro ISR_TABLE_ENTRY num
    .quad __isr_stub_\num
.endm

# Table of all 256 stub addresses — used by idt_init
.global isr_table
isr_table:
.set i, 0
.rept 256
    ISR_TABLE_ENTRY %i
    .set i, i+1
.endr