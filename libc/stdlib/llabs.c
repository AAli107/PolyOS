#include <stdlib.h>
#include <limits.h>

long long llabs(long long x) {
    return (x < 0) ? (x == LLONG_MIN ? LLONG_MIN : -x) : x;
}