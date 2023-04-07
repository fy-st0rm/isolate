#ifndef __ISO_LOG_H__
#define __ISO_LOG_H__

#include "iso_includes.h"
#include "iso_defines.h"

/*
 * @brief Assertion macro. Exists the program with error message with file name and line number.
 */

#define __assert(...) ({ fprintf(stderr, "\033[31m[ASSERTION]: %s:%d:\033[0m ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); exit(1); })
#define iso_assert(x, ...) (x) ? 0 : (__assert(__VA_ARGS__))


/*
 * #brief Loging color codes
 */

#define ISO_LOG_NORMAL 0
#define ISO_LOG_ERROR  91
#define ISO_LOG_SUCESS 92
#define ISO_LOG_WARN   93
#define ISO_LOG_INFO   96

/*
 * @brief Loging macros
 */

#ifdef ISO_BUILD_DEBUG
	#define iso_log_typed(type, ...)           \
		({                                       \
			printf("\033[%dm", type);              \
			printf(__VA_ARGS__);                   \
			printf("\033[%dm", ISO_LOG_NORMAL);    \
		})
#else
	#define iso_log_typed(type, ...)
#endif

#define iso_log(...)                              \
	({                                              \
		iso_log_typed(ISO_LOG_NORMAL, "[LOG]: ");     \
		iso_log_typed(ISO_LOG_NORMAL, __VA_ARGS__);   \
	})

#define iso_log_sucess(...)                       \
	({                                              \
		iso_log_typed(ISO_LOG_SUCESS, "[SUCESS]: ");  \
		iso_log_typed(ISO_LOG_SUCESS, __VA_ARGS__);   \
	})

#define iso_log_warn(...)                         \
	({                                              \
		iso_log_typed(ISO_LOG_WARN, "[WARN]: ");      \
		iso_log_typed(ISO_LOG_WARN, __VA_ARGS__);     \
	})

#define iso_log_error(...)                        \
	({                                              \
		iso_log_typed(ISO_LOG_ERROR, "[ERROR]: ");    \
		iso_log_typed(ISO_LOG_ERROR, __VA_ARGS__);    \
	})

#define iso_log_info(...)                         \
	({                                              \
		iso_log_typed(ISO_LOG_INFO, "[INFO]: ");      \
		iso_log_typed(ISO_LOG_INFO, __VA_ARGS__);     \
	})

/*
 * @brief Function to check for sdl int return value.
 * @param res = Int Return value by the function
 * @param file = Name of the file in which function as called
 * @param line = Line number of the called function
 */

static i32 __iso_sdl_check(i32 res, const char* file, i32 line) {
	if (res < 0) {
		fprintf(stderr, "\033[31m[SDL_ERROR]: %s:%d:\033[0m ", file, line);
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(1);
	}
	return res;
}

/*
 * @brief Function to check for sdl pointer return value.
 * @param res = Pointer Return value by the function
 * @param file = Name of the file in which function as called
 * @param line = Line number of the called function
 */

static void* __iso_sdl_check_ptr(void* res, const char* file, i32 line) {
	if (res == NULL) {
		fprintf(stderr, "\033[31m[SDL_ERROR]: %s:%d:\033[0m ", file, line);
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(1);
	}
	return res;
}

/*
 * @brief Macros to check for sdl error
 */

#define iso_sdl_check(x)     __iso_sdl_check(x, __FILE__, __LINE__)
#define iso_sdl_check_ptr(x) __iso_sdl_check_ptr(x, __FILE__, __LINE__)

/*
 * @brief Macros and functions for opengl error handling
 */

#define GLCall(x)\
	(\
		clear_gl_error(), \
		x\
	);\
	iso_assert(gl_error_log(#x, __FILE__, __LINE__), "Opengl failed.\n");\

static void clear_gl_error() {
	while(glGetError());
}

static bool gl_error_log(const char* function, const char* file, int line) {
	GLenum error;
	while ((error = glGetError())) {
		fprintf(stderr, "[Error code]: %d\n", error);
		fprintf(stderr, "[Error message]: %s\n", gluErrorString(error));
		fprintf(stderr, "[Opengl error]: %s %s: %d\n", function ,file, line);
		return false;
	}
	return true;
}


#endif //__ISO_LOG_H__
