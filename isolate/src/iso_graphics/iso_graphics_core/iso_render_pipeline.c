#include "iso_app/iso_app.h"

extern iso_app* app;

ISO_API iso_render_pipeline* iso_render_pipeline_new(iso_render_pipeline_def def) {
	return app->graphics->api.render_pipeline_new(app->graphics, def);
}

ISO_API void iso_render_pipeline_delete(iso_render_pipeline* pip) {
	app->graphics->api.render_pipeline_delete(app->graphics, pip);
}

ISO_API void iso_render_pipeline_begin(iso_render_pipeline* pip) {
	app->graphics->api.render_pipeline_begin(app->graphics, pip);
}

ISO_API void iso_render_pipeline_end(iso_render_pipeline* pip, i32 indices_cnt) {
	app->graphics->api.render_pipeline_end(app->graphics, pip, indices_cnt);
}
