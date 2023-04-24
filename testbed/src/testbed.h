#ifndef __TESTBED_H__
#define __TESTBED_H__

#include "isolate.h"

//#define ORTHO
#define WIDTH 800
#define HEIGHT 600

static char* vert_shader = 
      "#version 440 core\n"
      "\n"
      "layout(location = 0) in vec3 position;\n"
      "uniform mat4 mvp;\n"
      "void main()\n"
      "{\n"
      ""
      " gl_Position =  mvp * vec4(position, 1.0f);"
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

      iso_camera* o_cam;
      iso_camera* p_cam;
      iso_camera* cam;
} testbed;

void testbed_new(iso_scene* scene);
void testbed_delete(iso_scene* scene);
void testbed_on_entry(iso_scene* scene);
void testbed_on_exit(iso_scene* scene);
void testbed_on_update(iso_scene* scene, f32 dt);
void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt);

#endif
