#ifndef __ISO_WINDOW_H__
#define __ISO_WINDOW_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"

/*
 * @brief Window defination structure. Defines on how the window should be.
 * @mem title  = Title of the window.
 * @mem width  = Width of the window.
 * @mem height = Height of the window.
 */

typedef struct {
	char* title;
	i32 width, height;
} iso_window_def;

/*
 * @brief Structure of the iso_window
 * @mem title        = Title of the window
 * @mem width        = Width of the window
 * @mem height       = Height of the window
 * @mem sdl_window   = Pointer to the SDL_Window
 * @mem sdl_renderer = Pointer to the SDL_Renderer
 */

typedef struct {
	char* title;
	i32 width, height;
	SDL_Window* sdl_window;
	SDL_Renderer* sdl_renderer;
} iso_window;

/*
 * @brief Function to contruct new window using the window defination.
 * @param window_def = window defination structure
 * @return Returns the pointer to the iso_window
 */

static iso_window* iso_window_new(iso_window_def window_def) {
	iso_window* window = iso_alloc(sizeof(iso_window));

	// Initializing sdl
	iso_sdl_check(SDL_Init(SDL_INIT_EVERYTHING));

	// Copying the window data
	window->title = iso_alloc(strlen(window_def.title));
	strcpy(window->title, window_def.title);

	window->width  = window_def.width;
	window->height = window_def.height;

	// Creating sdl window and renderer
	window->sdl_window   = iso_sdl_check_ptr(SDL_CreateWindow(window_def.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_def.width, window_def.height, 0));
	window->sdl_renderer = iso_sdl_check_ptr(SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED));

	return window;
}

/*
 * @brief Function to delete the window.
 * @param window = Pointer to the iso_window
 */

static void iso_window_delete(iso_window* window) {
	// Deleting sdl window and renderer
	SDL_DestroyWindow(window->sdl_window);
	SDL_DestroyRenderer(window->sdl_renderer);

	// Cleaning iso_window memory
	iso_free(window->title);
	iso_free(window);

	// Quiting sdl
	SDL_Quit();
}



#endif // __ISO_WINDOW_H__
