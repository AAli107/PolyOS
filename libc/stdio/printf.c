#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	char buf[1024];
    va_list parameters;
    va_start(parameters, format);
    int n = vsprintf(buf, format, parameters);
    va_end(parameters);
    if (n < 0)
        return -1;
    if (!print(buf, n))
        return -1;
    return n;
}
