#include "iso_shader.h"
#include "iso_app/iso_app.h"

extern iso_app* app;

iso_shader* iso_shader_new(iso_shader_def def) {
	return app->graphics->api.shader_new(app->graphics, def);
}

void iso_shader_delete(iso_shader* shader) {
	app->graphics->api.shader_delete(app->graphics, shader);
}

void iso_shader_uniform_set(iso_shader* shader, iso_uniform_def def) {
	app->graphics->api.shader_uniform_set(app->graphics, shader, def);
}

void iso_shader_bind(iso_shader* shader) {
	app->graphics->api.shader_bind(app->graphics, shader);
}

void iso_shader_unbind() {
	app->graphics->api.shader_unbind(app->graphics);
}
