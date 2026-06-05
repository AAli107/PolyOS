#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static void fillBuffer(char* buf, int written, const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++) {
        int index = written + i;
        buf[index] = bytes[i];
    }
}

static char *uint_to_str(char *end, unsigned int value, int base, bool isCapital) {
    static const char digits[] = "0123456789abcdef";
    static const char digitsCapital[] = "0123456789ABCDEF";

    const char* finalDigits = isCapital ? digitsCapital : digits;

    *end = '\0';
    do {
        *--end = finalDigits[value % base];
        value /= base;
    } while (value);
    return end;
}

static char *int_to_str(char *end, int value, int base) {
    int negative = value < 0;
    unsigned int uvalue = negative ? -(unsigned int)value : (unsigned int)value;
    char *result = uint_to_str(end, uvalue, base, false);
    if (negative)
        *--result = '-';
    return result;
}

int vsprintf(char *str, const char *format, va_list ap) {
    int written = 0;
    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;
        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            fillBuffer(str, written, format, amount);
            format += amount;
            written += amount;
            continue;
        }
        const char* format_begun_at = format++;

        switch (*format)
        {
        case 'c':
            {
                format++;
                char c = (char) va_arg(ap, int);
                if (!maxrem) {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                fillBuffer(str, written, &c, sizeof(c));
                written++;
            }
            break;
        case 's':
            {
                format++;
                const char* s = va_arg(ap, const char*);
                size_t len = strlen(s);
                if (maxrem < len) {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                fillBuffer(str, written, s, len);
                written += len;
            }
            break;
        case 'd':
            {
                format++;
                char buf[12] = {};
                const char* s = int_to_str(buf+11, va_arg(ap, int), 10);
                size_t len = strlen(s);
                if (maxrem < len) {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                fillBuffer(str, written, s, len);
                written += len;
            }
            break;
        case 'u':
        case 'x':
        case 'X':
            {                
                bool isCapitalHex = *format == 'X';
                bool isHex = isCapitalHex || *format == 'x';
                format++;
                char buf[11] = {};
                const char* s = uint_to_str(buf+10, va_arg(ap, unsigned int), 
                    isHex ? 16 : 10, isCapitalHex);
                size_t len = strlen(s);
                if (maxrem < len) {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                fillBuffer(str, written, s, len);
                written += len;
            }
            break;
        
        default:
            {
                format = format_begun_at;
                size_t len = strlen(format);
                if (maxrem < len) {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                fillBuffer(str, written, format, len);
                written += len;
                format += len;
            }
            break;
        }
    }
    str[written] = '\0';
    return written;
}