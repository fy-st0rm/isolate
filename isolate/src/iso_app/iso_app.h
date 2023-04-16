#ifndef __ISO_APP_H__
#define __ISO_APP_H__

#include "iso_util/iso_defines.h"
#include "iso_window/iso_window.h"
#include "iso_graphics/iso_graphics.h"
#include "iso_util/iso_memory.h"
#include "iso_camera/iso_camera.h"
#include "iso_scene/iso_scene.h"

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
 * @mem app_data       = Extra memory that user can use it
 */

typedef struct {
	iso_window_def   window_def;
	iso_graphics_def graphics_def;
	f32 fps;
	void* app_data;
} iso_app_def;

/*
 * @brief Structure of the app.
 * @mem window         = Pointer to the iso_window.
 * @mem graphics       = Pointer to the iso_graphics.
 * @mem camera_manager = Pointer to the iso_camera_manager.
 * @mem scene_manager  = Pointer to the iso_scene_manager.
 * @mem fps            = Fps of the app.
 * @mem app_data       = Extra memory that user can use it
 */

typedef struct{
	iso_window*         window;
	iso_graphics*       graphics;
	iso_camera_manager* camera_manager;
	iso_scene_manager*  scene_manager;
	iso_app_state state;
	f32 fps;
	void* app_data;
} iso_app;

/*
 * @brief Function to construct iso_app according to the given app defination.
 * @param app_def = iso_app_def structure.
 * @return Returns the pointer to the iso_app.
 */

ISO_API iso_app* iso_app_new(iso_app_def app_def);

/*
 * @brief Function to delete iso_app.
 * @param app = Pointer to the iso_app.
 */

ISO_API void iso_app_delete(iso_app* app);

#endif // __ISO_APP_H__
