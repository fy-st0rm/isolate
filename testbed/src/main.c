#include "isolate.h"
#include "testbed.h"

iso_app_def iso_init() {
	iso_window_def win_def = {
		.title = "Testbed",
		.width = 800,
		.height = 600
	};

	iso_graphics_def grap_def = {
		.api = ISO_OPENGL_API
	};

	return (iso_app_def) {
		.window_def = win_def,
		.graphics_def = grap_def,
		.fps = 60.0f
	};
}

void iso_start(iso_app* app) {
	testbed* test = iso_alloc(sizeof(testbed));
	test->app = app;

	iso_scene_def scene = {
		.name = "Testbed",
		.scene_data = test,

		.new        = testbed_new,
		.delete     = testbed_delete,
		.on_entry   = testbed_on_entry,
		.on_exit    = testbed_on_exit,
		.on_update  = testbed_on_update,
		.on_event   = testbed_on_event
	};

	iso_scene_new(app->scene_manager, scene);
}

void iso_exit(iso_app* app) {
	iso_scene_delete(app->scene_manager, "Testbed");
}

i32 main(i32 argc, char** argv) {
	return iso_run(argc, argv);
}
