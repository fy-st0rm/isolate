#include "iso_app/iso_app.h"

extern iso_app* app;

iso_vertex_buffer* iso_vertex_buffer_new(iso_vertex_buffer_def def) {
	return app->graphics->api.vertex_buffer_new(app->graphics, def);
}

void iso_vertex_buffer_delete(iso_vertex_buffer* vbo) {
	app->graphics->api.vertex_buffer_delete(app->graphics, vbo);
}

void iso_vertex_buffer_update(iso_vertex_buffer *vbo, iso_buffer_update_def def) {
	app->graphics->api.vertex_buffer_update(app->graphics, vbo, def);
}

void iso_vertex_buffer_bind(iso_vertex_buffer* vbo) {
	app->graphics->api.vertex_buffer_bind(app->graphics, vbo);
}

void iso_vertex_buffer_unbind() {
	app->graphics->api.vertex_buffer_unbind(app->graphics);
}
