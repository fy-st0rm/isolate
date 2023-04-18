#ifndef __TESTBED_H__
#define __TESTBED_H__

#include "isolate.h"

static char* vert_shader = 
      "#version 440 core\n"
      "\n"
      "layout(location = 0) in vec4 position;\n"
      "void main()\n"
      "{\n"
      ""
      " gl_Position =  position;"
      ""
      ""
      "}\n";

static char* frag_shader = 
      "#version 440 core\n"
      "\n"
      "layout(location = 0) out vec4 color;"
      "void main()\n"
      "{\n"
      ""
      "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "}\n";

typedef struct {
	iso_app* app;

	iso_graphics_vertex_buffer* vbo;
	iso_graphics_index_buffer*  ibo;
	iso_graphics_shader* shader;
	iso_graphics_render_pipeline* pip;
} testbed;

void testbed_new(iso_scene* scene);
void testbed_delete(iso_scene* scene);
void testbed_on_entry(iso_scene* scene);
void testbed_on_exit(iso_scene* scene);
void testbed_on_update(iso_scene* scene, f32 dt);
void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt);

#endif
