#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/io.h>
#include <stdint.h>

// to speed shit up, I just told AI to automatically generate bunch of irq handling functions for me to fill in later :P
// This is the same for exceptions within exceptions.c

// Remap the PIC so IRQ0-7 map to vectors 32-39 and IRQ8-15 to 40-47.
// By default after BIOS the PIC uses vectors 8-15 which overlap CPU exceptions -
// this must be done before enabling interrupts.
static void pic_remap(void) {
    uint8_t mask1 = inb(PIC1_DATA); // save existing masks
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1_CMD,  0x11); io_wait(); // ICW1: start init, expect ICW4
    outb(PIC2_CMD,  0x11); io_wait();
    outb(PIC1_DATA, 32);   io_wait(); // ICW2: master offset -> vector 32
    outb(PIC2_DATA, 40);   io_wait(); // ICW2: slave offset  -> vector 40
    outb(PIC1_DATA, 0x04); io_wait(); // ICW3: slave is on IRQ2 (bit 2)
    outb(PIC2_DATA, 0x02); io_wait(); // ICW3: slave cascade identity = 2
    outb(PIC1_DATA, 0x01); io_wait(); // ICW4: 8086 mode
    outb(PIC2_DATA, 0x01); io_wait();

    outb(PIC1_DATA, mask1); // restore saved masks
    outb(PIC2_DATA, mask2);
}

// Send End-Of-Interrupt to the PIC.
// Must be called at the END of every IRQ handler or the PIC will
// stop sending further interrupts on that line.
// For IRQs 8-15 (slave PIC), BOTH master and slave must be notified.
static void irq_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_CMD, PIC_EOI); // notify slave first for IRQs 8-15
    outb(PIC1_CMD, PIC_EOI);     // always notify master
}

// Vector 32 : IRQ0: PIT Programmable Interval Timer
// Fired by: the 8253/8254 timer chip at ~18.2 Hz by default (configurable).
// Primary use: preemptive scheduling tick, system uptime counter.
// This is the heartbeat of the OS : fired more than any other IRQ.
static void irq0_timer(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(0);
}

// Vector 33 : IRQ1: PS/2 Keyboard
// Fired by: a key press or release on the PS/2 keyboard.
// Read the scancode from port 0x60 inside the handler.
// Each physical key event fires one interrupt.
static void irq1_keyboard(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(1);
}

// Vector 34 : IRQ2: PIC Cascade
// Fired by: the slave PIC to signal an IRQ from lines 8-15.
// This is handled internally by the PIC : you never receive this directly.
// The real IRQ comes in as IRQ8-15. Safe to ignore.
static void irq2_cascade(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(2);
}

// Vector 35 : IRQ3: COM2 / Serial Port 2
// Fired by: the second serial port (COM2) when data is received or
//           the transmit buffer is empty (depending on IER config).
static void irq3_com2(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(3);
}

// Vector 36 : IRQ4: COM1 / Serial Port 1
// Fired by: the first serial port (COM1). Same conditions as IRQ3.
// If you use a serial port for kernel debug output, this fires on receive.
static void irq4_com1(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(4);
}

// Vector 37 : IRQ5: LPT2 / Sound Card
// Fired by: the second parallel port or a sound card (ISA).
// Rarely used on modern hardware : treat as free for custom use.
static void irq5_lpt2(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(5);
}

// Vector 38 : IRQ6: Floppy Disk Controller
// Fired by: the floppy disk controller when a DMA transfer completes
//           or an error occurs. Safe to ignore if you have no floppy support.
static void irq6_floppy(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(6);
}

// Vector 39 : IRQ7: LPT1 / Spurious IRQ
// Fired by: the first parallel port, OR spuriously when no real IRQ7 occurred
//           (a glitch on the interrupt line). Before handling, check the PIC's
//           ISR (In-Service Register) to confirm it's real : if bit 7 is NOT
//           set in the master ISR, it is spurious and must NOT send EOI.
static void irq7_lpt1_spurious(interrupt_frame_t *frame) {
    (void)frame;
    // TODO: check ISR before sending EOI to avoid acknowledging a spurious IRQ
    irq_eoi(7);
}

// Vector 40 : IRQ8: RTC Real-Time Clock
// Fired by: the CMOS RTC at a programmable rate (default 1024 Hz if enabled).
// Must read register C from CMOS (port 0x70/0x71) inside the handler to
// acknowledge the RTC, otherwise it will not fire again.
static void irq8_rtc(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(8);
}

// Vector 41 : IRQ9: ACPI / PCI / Free
// Fired by: ACPI SCI (System Control Interrupt) on modern systems, or
//           redirected IRQ2 on legacy systems. Also used by some PCI devices.
static void irq9_acpi(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(9);
}

// Vector 42 : IRQ10: Free / PCI
// Fired by: a PCI device assigned to this IRQ line.
// Not used by default : available for device drivers.
static void irq10_free(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(10);
}

// Vector 43 : IRQ11: Free / PCI
// Fired by: a PCI device assigned to this IRQ line.
// Not used by default : available for device drivers.
static void irq11_free(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(11);
}

// Vector 44 : IRQ12: PS/2 Mouse
// Fired by: a PS/2 mouse when it sends movement or button data.
// Read mouse data from port 0x60 (same as keyboard : check status port 0x64
// to determine whether the byte is from keyboard or mouse).
static void irq12_mouse(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(12);
}

// Vector 45 : IRQ13: FPU / Math Coprocessor Error
// Fired by: legacy x87 FPU error signalling on very old hardware.
// On modern CPUs this is routed to vector 16 (#MF) instead. Safe to ignore.
static void irq13_fpu(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(13);
}

// Vector 46 : IRQ14: Primary ATA / IDE Controller
// Fired by: the primary ATA controller when a DMA transfer completes
//           or an error occurs (drives on channels 0 and 1).
static void irq14_ata_primary(interrupt_frame_t *frame) {
    (void)frame;
    irq_eoi(14);
}

// Vector 47 : IRQ15: Secondary ATA / Spurious
// Fired by: the secondary ATA controller, OR spuriously from the slave PIC
//           (same issue as IRQ7 but on the slave). Check the slave ISR before
//           sending slave EOI : if bit 7 is NOT set, it is spurious.
//           For spurious IRQ15, send EOI to master only (cascade line).
static void irq15_ata_secondary_spurious(interrupt_frame_t *frame) {
    (void)frame;
    // TODO: check slave ISR before sending slave EOI
    irq_eoi(15);
}

// intialize all the IRQs
void irq_init(void) {
    pic_remap();

    idt_register_handler(32, irq0_timer);
    idt_register_handler(33, irq1_keyboard);
    idt_register_handler(34, irq2_cascade);
    idt_register_handler(35, irq3_com2);
    idt_register_handler(36, irq4_com1);
    idt_register_handler(37, irq5_lpt2);
    idt_register_handler(38, irq6_floppy);
    idt_register_handler(39, irq7_lpt1_spurious);
    idt_register_handler(40, irq8_rtc);
    idt_register_handler(41, irq9_acpi);
    idt_register_handler(42, irq10_free);
    idt_register_handler(43, irq11_free);
    idt_register_handler(44, irq12_mouse);
    idt_register_handler(45, irq13_fpu);
    idt_register_handler(46, irq14_ata_primary);
    idt_register_handler(47, irq15_ata_secondary_spurious);
}