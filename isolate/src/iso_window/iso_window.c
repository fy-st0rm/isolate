#include "iso_window.h"


iso_window* iso_window_new(iso_window_def window_def) {
	iso_log_info("Constructing iso_window...\n");

	iso_window* window = iso_alloc(sizeof(iso_window));

	// Initializing sdl
	iso_sdl_check(SDL_Init(SDL_INIT_EVERYTHING));
	iso_sdl_check(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG));

	// Copying the window data
	window->title = iso_str_new(window_def.title);

	window->width  = window_def.width;
	window->height = window_def.height;

	// Creating sdl window and renderer
	window->sdl_window = iso_sdl_check_ptr(SDL_CreateWindow(
		window->title->c_str, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window->width, window->height, window_def.flags
	));

	window->sdl_renderer = iso_sdl_check_ptr(SDL_CreateRenderer(
		window->sdl_window, -1,
		SDL_RENDERER_ACCELERATED
	));

	iso_log_sucess("Created iso_window:\n\tTitle: `%s`\n\tRes: %dx%d\n", window->title->c_str, window->width, window->height);
	return window;
}

void iso_window_delete(iso_window* window) {
	iso_log_info("Deleting iso_window...\n");

	// Deleting sdl window and renderer
	SDL_DestroyWindow(window->sdl_window);
	SDL_DestroyRenderer(window->sdl_renderer);

	// Cleaning iso_window memory
	iso_str_delete(window->title);
	iso_free(window);

	// Quiting sdl
	IMG_Quit();
	SDL_Quit();

	iso_log_sucess("Deleted iso_window.\n");
}
