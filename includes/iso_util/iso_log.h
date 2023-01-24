#ifndef __ISO_LOG_H__
#define __ISO_LOG_H__

#include <stdio.h>

#define __assert(...) { printf("\033[31m[ASSERTION]: %s:%d:\033[0m ", __FILE__, __LINE__); printf(__VA_ARGS__); exit(1); }
#define iso_assert(x, ...) x ? 0 : (__assert(__VA_ARGS__))

#endif //__ISO_LOG_H__
