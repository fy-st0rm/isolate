#ifndef __ISO_ENTRY_H__
#define __ISO_ENTRY_H__

#include "iso_util/iso_memory.h"
#include "iso_app.h"

/*
 * @brief Function that will initialize the app
 */

iso_app_def iso_init();

/*
 * @brief Function that will be called at the start of the app
 * @param app = Pointer to the iso_app
 */

void iso_start(iso_app* app);

/*
 * @brief Function that will be called during events
 * @param app   = Pointer to the iso_app
 * @param event = SDL_Event handler
 */

void iso_event(iso_app* app, SDL_Event event);

/*
 * @brief Function that will be called every frame
 * @param app = Pointer to the iso_app
 * @param dt  = Delta time
 */

void iso_update(iso_app* app, f32 dt);

/*
 * @brief Function that will be called at the end of the app
 */

void iso_exit(iso_app* app);

/*
 * @brief Program entry point of the program
 * @param argc = Number of command line arguments.
 * @param argv = List of command line arguments.
 */

int main(int argc, char** argv) {

	// Initialize the engine memory
	iso_memory_init();

	// Init
	iso_app_def app_def = iso_init();

	// Creating the app
	iso_app* app = iso_app_new(app_def);

	// App start
	iso_start(app);

	SDL_Event event;

	// Frame handler
	f32 unit_frame = 1000.0f / app->fps;
	f32 dt = 0.0f;

	// Main loop
	while (app->state == ISO_APP_RUNNING) {
		u32 start_tick = SDL_GetTicks();

		// Event handler
		while (SDL_PollEvent(&event)) {
			iso_event(app, event);
		}

		// Update handler
		iso_update(app, dt);

		// Capping the frames
		dt = SDL_GetTicks() - start_tick;
		if (unit_frame > dt) {
			SDL_Delay(unit_frame - dt);
		}

		// Calculating delta time
		dt = SDL_GetTicks() - start_tick;

		// Updating the graphics
		app->graphics->api.update(app->window);
	}

	// Exit of the app
	iso_exit(app);

	// Cleaning app
	iso_app_delete(app);

	// Alerting incase of memory leaks
	iso_memory_alert();

	return 0;
}

#endif //__ISO_ENTRY_H__
