#include <stdlib.h>
#include <limits.h>

long int labs(long int x) {
    return (x < 0) ? (x == LONG_MIN ? LONG_MIN : -x) : x;
}