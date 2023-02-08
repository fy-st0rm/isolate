#ifndef __ISO_APP_H__
#define __ISO_APP_H__

#include "iso_window/iso_window.h"
#include "iso_graphics/iso_graphics.h"
#include "iso_util/iso_memory.h"
#include "iso_camera/iso_camera.h"

/*
 * @brief Enum that holds the state of the app.
 */

typedef enum {
	ISO_APP_CLOSED,
	ISO_APP_RUNNING
} iso_app_state;

/*
 * @brief App defination structure. Defines on how the app should be.
 * @mem window_def   = Defination for construction of iso_window.
 * @mem graphics_def = Defination for construction of iso_graphics.
 * @mem fps          = Max fps for the app.
 */

typedef struct {
	iso_window_def   window_def;
	iso_graphics_def graphics_def;
	f32 fps;
} iso_app_def;

/*
 * @brief Structure of the app.
 * @mem window     = Pointer to the iso_window.
 * @mem graphics   = Pointer to the iso_graphics.
 * @mem camera_man = Pointer to the iso_camera_manager.
 * @mem fps        = Max fps of the app.
 */

typedef struct {
	iso_window*         window;
	iso_graphics*       graphics;
	iso_camera_manager* camera_man;
	iso_app_state state;
	f32 fps;
} iso_app;

/*
 * @brief Function to construct iso_app according to the given app defination.
 * @param app_def = iso_app_def structure.
 * @return Returns the pointer to the iso_app.
 */

static iso_app* iso_app_new(iso_app_def app_def) {
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

	app->fps = app_def.fps;

	return app;
}

/*
 * @brief Function to delete iso_app.
 * @param app = Pointer to the iso_app.
 */

static void iso_app_delete(iso_app* app) {
	// Cleaning memory
	iso_window_delete(app->window);
	iso_graphics_delete(app->graphics);
	iso_camera_manager_delete(app->camera_man);

	iso_free(app);
}

#endif // __ISO_APP_H__
