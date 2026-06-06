#include <stdlib.h>
#include <stdbool.h>

long atol(const char *str) {
    if (!str)
        return 0;
        
    long value = 0;
    bool isNegative = str[0] == '-';
    
    int i = isNegative ? 1 : 0;
    while (str[i] != '\0')
    {
        int charValue = str[i] - 48;
        if (charValue < 0 || charValue > 9)
            return 0;
        value *= 10;
        value += charValue;
        i++;
    }

    if (isNegative)
        value = -value;
    return value;
}