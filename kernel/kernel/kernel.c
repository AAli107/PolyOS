#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>

// Fun fact, the reason I decided to do OS dev is cus I wanted to learn smth cool :)
// Shoutout to "osdev.wiki" for giving out the knowledge and resources for developing operating systems.
// As you can see as you look around, the project is just basically "Meaty Skeleton" but with extra flare! ✨
// I don't expect to go far, but who knows? Maybe I'd lock in in the future if I stay working on it.
void kernel_main(void) {
	// Initialization process
	terminal_initialize();

	// Don't mind the test code
	printf("Hello, kernel World!\n");
	printf("Hello, k\ternel World!\n");
	printf("Hello, ke\trnel World!\n");
	printf("Hello, ker\tnel World!\n");
	printf("Hello, kern\tel World!\n");
	printf("Hello, kerne\tl World!\n");
	printf("Hello, kernel\t World!\n");
	printf("Hello, kernel \tWorld!\n");
	printf("ehoi 🥀\n");
	printf("%s", "dere's a special place in memory for you\n");
	printf("%c\n", 'F');
	int i = atoi("-123");
	int j = abs(i);
	printf("%d => %d", i, j);

	char buf[256] = {};
	sprintf(buf, "HALLO!!!%%%s%%%cgahh", "BRRAHHHHH12345", '\n');
	printf("\n%s", buf);
	printf("\nint value: %d", 2147483647);
	printf("\nuint value: %u", 4294967295);
	printf("\nuint hex: 0x%x", 4294967295);
	printf("\nuint HEX: 0x%X", 4294967295);
}
