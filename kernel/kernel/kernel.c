#include <limine.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/video.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/exceptions.h>
#include <kernel/irq.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// halt and explode OS
static void kms(void) {
    for (;;) {
        asm ("hlt");
    }
}

#define KERNEL_STACK_SIZE 65536

static uint8_t kernel_stack[KERNEL_STACK_SIZE];

void kernel_main(void) {

	// Draw colored lines
	uint64_t width = video_getWidth();
	uint64_t height = video_getHeight();
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			uint32_t n = (x / 8) % 4;
			struct pixel32 pixel = {
				.b = n == 0 ? 255 : 0,
				.g = n == 1 ? 255 : 0,
				.r = n == 2 ? 255 : 0,
				.x = n == 3 ? 255 : 0
			};
			
			video_setPixel(x, y, pixel);
		}
	}

	// divide by zero test
	volatile int zero = 0;
	volatile int x = 10 / zero;

	return; // stop here cus the terminal is yet to function correctly

	terminal_initialize();

	// Don't mind the test code
	printf("Hello, kernel World!\n");
	printf("ehoi 🥀\n");
	printf("%s", "dere's a special place in memory for you\n");
	
	const char* cases[10] = {
		"1", 
		"45",
		"-452",
		"-45200000000",
		"9223372036854775807",
		"-9223372036854775808",
		"123",
		"-123",
		"69",
		"420"
	};

	for (size_t i = 0; i < 10; i++)
	{
		long long at = atoll(cases[i]);
		long long ab = llabs(at);
		printf("%s => %lld => %lld\n", cases[i], at, ab);
	}
	
	char buf[256] = {};
	sprintf(buf, "HALLO!!!%%%s%%%cgahh", "BRRAHHHHH12345", '\n');
	printf("\n%s", buf);
	printf("\nlonglong value: %lld", 9223372036854775807LL);
	printf("\nulonglong value: %llu", 18446744073709551615ULL);
	printf("\nulonglong hex: 0x%llx", 18446744073709551615ULL);
	printf("\nulonglong HEX: 0x%llX\n", 18446744073709551615ULL);

	printf("%u %u %u", (unsigned int)sizeof(int), (unsigned int)sizeof(long), (unsigned int)sizeof(long long));
}

__attribute__((noreturn))
void _start(void) {

	// Pre kernel_main initialization START

	__asm__ volatile (
		"movq %%cr0, %%rax\n"
		"andw $0xFFFB, %%ax\n"      // clear CR0.EM
		"orw  $0x2, %%ax\n"         // set CR0.MP
		"movq %%rax, %%cr0\n"
		"movq %%cr4, %%rax\n"
		"orw  $(3 << 9), %%ax\n"    // set CR4.OSFXSR and CR4.OSXMMEXCPT
		"movq %%rax, %%cr4\n"
		:
		:
		: "rax", "memory"
	);

	gdt_init(); // sets up the global descriptor table

	idt_init(); // sets up the interrupt descriptor table
	
	// registers handlers for exceptions and irqs 
	exceptions_init();
	irq_init();

	gdt_set_kernel_stack(
		(uint64_t)&kernel_stack[KERNEL_STACK_SIZE]
	);

	// Initialize video, if it fails, then tell the OS to kill itself
	if (video_initialize(&framebuffer_request) != 0) {
		kms();
	}

	// Pre kernel_main initialization END

    kernel_main();

    for (;;) {
        __asm__ volatile ("hlt");
    }
}