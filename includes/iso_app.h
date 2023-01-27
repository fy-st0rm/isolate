#ifndef __ISO_APP_H__
#define __ISO_APP_H__

#include "iso_window/iso_window.h"
#include "iso_graphics/iso_graphics.h"
#include "iso_util/iso_memory.h"

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
 * @mem window   = Pointer to the iso_window.
 * @mem graphics = Pointer to the iso_graphics.
 * @mem fps      = Max fps of the app.
 */

typedef struct {
	iso_window*   window;
	iso_graphics* graphics;
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

	// TODO: Creating iso_window and iso_graphics
	app->window = iso_window_new(app_def.window_def);

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
	// TODO: Deleting iso_window and iso_graphics
	iso_window_delete(app->window);

	iso_free(app);
}

#endif // __ISO_APP_H__
