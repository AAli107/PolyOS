#include <kernel/gdt.h>
#include <string.h>
#include <stdint.h>

// Size : 8 bytes
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  flags_limit;
    uint8_t  base_high;
} __attribute__((packed));

// Size : 16 bytes
struct tss_descriptor {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  flags_limit;
    uint8_t  base_high;
    uint32_t base_upper;
    uint32_t reserved;
} __attribute__((packed));

// the TSS structure
struct tss {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
} __attribute__((packed));

// global descriptor table register data
struct gdtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

/* What each index in the gdt array represents
 * 0    =   null
 * 1    =   kcode
 * 2    =   kdata
 * 3    =   ucode
 * 4    =   udata
 * 5 6  =   tss
 */
static struct gdt_entry gdt[7];
static struct tss tss;

// Helper : Sets a specific entry at "i" index
static void set_entry(int i, uint8_t access, uint8_t flags) {
    gdt[i].limit_low  = 0xFFFF;
    gdt[i].base_low   = 0;
    gdt[i].base_mid   = 0;
    gdt[i].access     = access;
    gdt[i].flags_limit = flags | 0x0F;
    gdt[i].base_high  = 0;
}

static void set_tss_descriptor(void) {
    uint64_t base  = (uint64_t)&tss;
    uint32_t limit = sizeof(tss) - 1;
    struct tss_descriptor *d = (struct tss_descriptor *)&gdt[5];
    d->limit_low    = limit & 0xFFFF;
    d->base_low     = base & 0xFFFF;
    d->base_mid     = (base >> 16) & 0xFF;
    d->access       = 0x89;
    d->flags_limit  = (limit >> 16) & 0x0F;
    d->base_high    = (base >> 24) & 0xFF;
    d->base_upper   = (base >> 32) & 0xFFFFFFFF;
    d->reserved     = 0;
}

void gdt_init(void) {
    memset(&tss, 0, sizeof(tss));
    tss.iopb_offset = sizeof(tss);

    // set all the entries as well as the TSS
    set_entry(0, 0x00, 0x00);  // null
    set_entry(1, 0x9A, 0xA0);  // kernel code - ring 0, 64-bit
    set_entry(2, 0x92, 0xC0);  // kernel data - ring 0
    set_entry(3, 0xFA, 0xA0);  // user code   - ring 3, 64-bit
    set_entry(4, 0xF2, 0xC0);  // user data   - ring 3
    set_tss_descriptor();      // TSS at slots 5+6

    struct gdtr gdtr = {
        .limit = sizeof(gdt) - 1,
        .base  = (uint64_t)&gdt
    };

    // feed in the gdtr struct values into the system
    __asm__ volatile (
        "lgdt %0\n"
        "push $0x08\n"
        "lea 1f(%%rip), %%rax\n"
        "push %%rax\n"
        "lretq\n"
        "1:\n"
        "mov $0x10, %%eax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "mov $0x28, %%ax\n"
        "ltr %%ax\n"
        :
        : "m"(gdtr)
        : "rax", "memory"
    );
}


void gdt_set_kernel_stack(uint64_t rsp0) {
    tss.rsp0 = rsp0;
}