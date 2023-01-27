#ifndef __ISO_LOG_H__
#define __ISO_LOG_H__

#include "iso_includes.h"
#include "iso_defines.h"

/*
 * @brief Assertion macro. Exists the program with error message with file name and line number.
 */

#define __assert(...) { fprintf(stderr, "\033[31m[ASSERTION]: %s:%d:\033[0m ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); exit(1); }
#define iso_assert(x, ...) x ? 0 : (__assert(__VA_ARGS__))

static i32 __iso_sdl_check(i32 res, const char* file, i32 line) {
	if (res < 0) {
		fprintf(stderr, "\033[31m[SDL_ERROR]: %s:%d:\033[0m ", file, line);
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(1);
	}
	return res;
}

static void* __iso_sdl_check_ptr(void* res, const char* file, i32 line) {
	if (res == NULL) {
		fprintf(stderr, "\033[31m[SDL_ERROR]: %s:%d:\033[0m ", file, line);
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(1);
	}
	return res;
}

#define iso_sdl_check(x)     __iso_sdl_check(x, __FILE__, __LINE__)
#define iso_sdl_check_ptr(x) __iso_sdl_check_ptr(x, __FILE__, __LINE__)

#endif //__ISO_LOG_H__
