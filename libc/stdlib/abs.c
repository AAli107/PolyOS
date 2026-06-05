#include <stdlib.h>
#include <limits.h>

int abs(int x) {
    return (x < 0) ? (x == INT_MIN ? INT_MIN : -x) : x;
}