#include "iso_entry.h"

i32 iso_run(i32 argc, char** argv) {
	srand(time(NULL));

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
	f32 unit_frame = 1000.0f / app_def.fps;
	f32 dt = 0.0f;
	f32 frame_cnt = 0.0f;

	// Checking if scene exists or not
	iso_assert(iso_str_len(app->scene_manager->current_scene) != 0, "No scene created. At least one scene is required.\n");

	// Main loop
	time_t start_time = time(NULL);
	while (app->state == ISO_APP_RUNNING) {
		u32 start_tick = SDL_GetTicks();

		iso_assert(iso_str_len(app->scene_manager->current_scene) != 0, "No active scene found. Try switching scenes.\n");

		// Getting the current scene
		iso_scene* scene;
		iso_hmap_get(app->scene_manager->scenes, app->scene_manager->current_scene, scene);

		// Event handler
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				app->state = ISO_APP_CLOSED;
			}

			scene->on_event(scene, event, dt);
		}

		// Update handler
		scene->on_update(scene, dt);
		frame_cnt++;

		// Capping the frames
		dt = SDL_GetTicks() - start_tick;
		if (unit_frame > dt) {
			SDL_Delay(unit_frame - dt);
		}

		// Calculating delta time
		dt = SDL_GetTicks() - start_tick;

		// Updating the graphics
		app->graphics->api.update(app->window);

		// Counting fps
		time_t final_time = time(NULL);
		if (final_time - start_time == 1) {
			app->fps = frame_cnt;
			start_time = final_time;
			frame_cnt = 0;
		}
	}

	// Reseting the current scene
	iso_scene_manager_reset(app->scene_manager);

	// Exit of the app
	iso_exit(app);

	// Cleaning app
	iso_app_delete(app);

	// Alerting incase of memory leaks
	iso_memory_alert();

	return 0;
}
