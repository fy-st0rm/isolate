#include "testbed.h"

void testbed_new(iso_scene* scene) {
	iso_log("Testbed created\n");
}

void testbed_delete(iso_scene* scene) {
	iso_free(scene->scene_data);
	iso_log("Testbed Destroyed\n");
}

void testbed_on_entry(iso_scene* scene) {
}

void testbed_on_exit(iso_scene* scene) {
}

void testbed_on_update(iso_scene* scene, f32 dt) {
}

void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt) {
}
