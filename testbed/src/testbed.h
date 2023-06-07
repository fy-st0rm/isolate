#ifndef __TESTBED_H__
#define __TESTBED_H__

#include "isolate.h"
#include "controller.h"

#include "object.h"
#include "cube.h"

//#define ORTHO

typedef struct {
	iso_app* app;

	iso_vertex_buffer* vbo;
	iso_shader* shader;
	iso_texture* tex;
	iso_render_pipeline* pip;

	controller* con;

	object* cube;

} testbed;

void testbed_new(iso_scene* scene);
void testbed_delete(iso_scene* scene);
void testbed_on_entry(iso_scene* scene);
void testbed_on_exit(iso_scene* scene);
void testbed_on_update(iso_scene* scene, f32 dt);
void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt);

#endif
