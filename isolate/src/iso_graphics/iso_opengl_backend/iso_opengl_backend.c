#include "iso_opengl_backend.h"

/*
 * @brief Function to initialize opengl backend
 * @param window = Pointer to the iso_window
 */

void iso_gl_init(iso_window* window) {
	iso_log_info("Initializing opengl backend...\n");

	// Setting some attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	// Creating OPENGL context
	iso_sdl_check_ptr(SDL_GL_CreateContext(window->sdl_window));

	if (glewInit() != GLEW_OK) iso_assert(0, "Failed to initialize GLEW.\n");

	// Setting up alpha channels and blendings
	GLCall(glEnable(GL_BLEND));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	iso_log_sucess("Opengl has been initialized\n");
}

/*
 * @brief Function to update the window for opengl
 * @param window = Pointer to the iso_window
 */

void iso_gl_update(iso_window* window) {
	SDL_GL_SwapWindow(window->sdl_window);
}

/*
 * @brief Function to clear the window
 * @param window = Pointer to the iso_window
 * @param color  = Color of the background
 */

void iso_gl_clear_window(iso_window* window, iso_vec4 color) {
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
