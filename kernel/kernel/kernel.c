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
	printf("ehoi 🥀\n");
	printf("%s", "dere's a special place in memory for you\n");
	
	const char* cases[10] = {
		"1", 
		"45",
		"-452",
		"-45200000000",
		"2147483647",
		"-2147483648",
		"123",
		"-123",
		"69",
		"420"
	};

	for (size_t i = 0; i < 10; i++)
	{
		long at = atol(cases[i]);
		long ab = labs(at);
		printf("%s => %ld => %ld\n", cases[i], at, ab);
	}
	
	char buf[256] = {};
	sprintf(buf, "HALLO!!!%%%s%%%cgahh", "BRRAHHHHH12345", '\n');
	printf("\n%s", buf);
	printf("\nlonglong value: %lld", 9223372036854775807);
	printf("\nulonglong value: %llu", 18446744073709551615);
	printf("\nulonglong hex: 0x%llx", 18446744073709551615);
	printf("\nulonglong HEX: 0x%llX\n", 18446744073709551615);

	printf("%u %u %u", (unsigned int)sizeof(int), (unsigned int)sizeof(long), (unsigned int)sizeof(long long));
}
