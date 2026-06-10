#include <limine.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/video.h>
#include <kernel/gdt.h>

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

void kernel_main(void) {
	// Initialization process BEGIN
	
	gdt_init();

	// Initialize video, if it fails, then tell the OS to kill itself
	if (video_initialize(&framebuffer_request) != 0) {
		kms();
	}

	// Initialization process END

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
    kernel_main();
    for (;;) {
        __asm__ volatile ("hlt");
    }
}