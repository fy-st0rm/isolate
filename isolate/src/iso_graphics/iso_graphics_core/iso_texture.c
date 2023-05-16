#include "iso_app/iso_app.h"

extern iso_app* app;

iso_texture* iso_texture_new_from_file(iso_texture_from_file_def def) {
	return app->graphics->api.texture_new_from_file(app->graphics, def);
}

iso_texture* iso_texture_new_from_data(iso_texture_from_data_def def) {
	return app->graphics->api.texture_new_from_data(app->graphics, def);
}

void iso_texture_delete(iso_texture* texture) {
	app->graphics->api.texture_delete(app->graphics, texture);
}

void iso_texture_update(iso_texture* texture, iso_texture_update_def def) {
	app->graphics->api.texture_update(app->graphics, texture, def);
}

void iso_texture_bind(iso_texture* texture) {
	app->graphics->api.texture_bind(app->graphics, texture);
}

void iso_texture_unbind(iso_texture* texture) {
	app->graphics->api.texture_unbind(app->graphics, texture);
}
