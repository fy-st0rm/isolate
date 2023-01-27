#include "iso.h"

iso_app_def iso_init() {
	iso_window_def window_def = {
		.title  = "Window",
		.width  = 800,
		.height = 600
	};

	iso_graphics_def graphics_def = {
		.api = ISO_OPENGL_API,
		.renderer_amt = 1
	};

	return (iso_app_def) {
		.window_def = window_def,
		.graphics_def = graphics_def,
		.fps = 60.0f
	};
}

void iso_start(iso_app* app) {
}

void iso_event(iso_app* app, SDL_Event event) {
	if (event.type == SDL_QUIT) {
		app->state = ISO_APP_CLOSED;
	}
}

void iso_update(iso_app* app, f32 dt) {
}

void iso_exit() {
}
