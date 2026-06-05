#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);
// ABS functions
int abs(int x);
long int labs(long int x);
long long llabs(long long x);
// string to int
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);
// string to float
double atof(const char *str);

#ifdef __cplusplus
}
#endif

#endif
