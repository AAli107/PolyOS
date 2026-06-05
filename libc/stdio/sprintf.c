#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int sprintf(char *str, const char *format, ...) {
    va_list parameters;
    va_start(parameters, format);
    int n = vsprintf(str, format, parameters);
    va_end(parameters);
    return n;
}