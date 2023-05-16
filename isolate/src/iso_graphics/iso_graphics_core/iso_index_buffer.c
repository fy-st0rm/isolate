#include "iso_app/iso_app.h"

extern iso_app* app;

ISO_API iso_index_buffer* iso_index_buffer_new(iso_index_buffer_def def) {
	return app->graphics->api.index_buffer_new(app->graphics, def);
}

ISO_API void iso_index_buffer_delete(iso_index_buffer* ibo) {
	app->graphics->api.index_buffer_delete(app->graphics, ibo);
}

ISO_API void iso_index_buffer_update(iso_index_buffer* ibo, iso_buffer_update_def def) {
	app->graphics->api.index_buffer_update(app->graphics, ibo, def);
}

ISO_API void iso_index_buffer_bind(iso_index_buffer* ibo) {
	app->graphics->api.index_buffer_bind(app->graphics, ibo);
}

ISO_API void iso_index_buffer_unbind() {
	app->graphics->api.index_buffer_unbind(app->graphics);
}
