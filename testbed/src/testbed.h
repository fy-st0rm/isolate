#ifndef __TESTBED_H__
#define __TESTBED_H__

#include "isolate.h"

typedef struct {
	iso_app* app;
} testbed;

void testbed_new(iso_scene* scene);
void testbed_delete(iso_scene* scene);
void testbed_on_entry(iso_scene* scene);
void testbed_on_exit(iso_scene* scene);
void testbed_on_update(iso_scene* scene, f32 dt);
void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt);

#endif
