#include <limine.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/video.h>

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

	// Tell the OS to kill itself when it does not have the framebuffer
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        kms();
    }
	
	// Initialize video, if it fails, then tell the OS to kill itself
    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
	if (video_initialize(framebuffer) != 0) {
		kms();
	}

	// Initialization process END

    // Print test color gradient
	for (size_t y = 0; y < framebuffer->height; y++) {
        for (size_t x = 0; x < framebuffer->width; x++) {
            uint32_t nX = x * 255 / framebuffer->width;
            uint32_t nY = y * 255 / framebuffer->height;
            video_putPixel(x, y, (nY << 8) | nX);
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