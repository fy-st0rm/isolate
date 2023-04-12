#include "iso_app.h"

iso_app* iso_app_new(iso_app_def app_def) {
	iso_log_info("Constructing the application\n");

	iso_app* app = iso_alloc(sizeof(iso_app));

	// Creating iso_window
	app_def.window_def.flags |= app_def.graphics_def.api;
	app->window = iso_window_new(app_def.window_def);

	// Creating iso_graphics
	app->graphics = iso_graphics_new(app_def.graphics_def);
	app->graphics->api.init(app->window);

	// Creating iso_camera_manager
	app->camera_man = iso_camera_manager_new();

	// Initializing the app state
	app->state = ISO_APP_RUNNING;

	app->fps = 0.0f;

	iso_log_sucess("Created application\n\n");
	return app;
}

void iso_app_delete(iso_app* app) {
	iso_log_info("Deleting application\n");

	// Cleaning memory
	iso_graphics_delete(app->graphics);
	iso_camera_manager_delete(app->camera_man);
	iso_window_delete(app->window);

	iso_free(app);

	iso_log_sucess("Deleted application\n\n");
}
