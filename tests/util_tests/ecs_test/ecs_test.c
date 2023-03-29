#define ISO_IMPLEMENTATION
#include "isolate.h"

iso_app_def iso_init() { return (iso_app_def){}; }
void iso_start(iso_app* app) {}
void iso_update(iso_app* app, f32 dt) {}
void iso_event(iso_app* app, SDL_Event event) {}
void iso_exit(iso_app* app) {}

typedef struct {
	i32 x, y;
} transform;

typedef struct {
	char* t;
} tag;

int main(int argc, char** argv) {
	srand(time(NULL));
	iso_memory_init();

	iso_ecs* ecs = iso_ecs_new(100);

	// Basic
	{
		iso_entity ent = iso_entity_new(ecs);
		printf("Entity: %d\n", ent);

		transform* comp = iso_entity_add_component(ecs, ent, transform, 10, 20);
		printf("Transform: %d %d\n", comp->x, comp->y);

		// Deletes entity with its components as well
		iso_entity_delete(ecs, ent);
	}

	// Entity component manipulation
	{
		iso_entity ent = iso_entity_new(ecs);
		printf("Entity: %d\n", ent);

		transform* comp = iso_entity_add_component(ecs, ent, transform, 10, 20);
		tag* tcomp = iso_entity_add_component(ecs, ent, tag, "helo");
		printf("Transform: %d %d\n", comp->x, comp->y);
		printf("Tag: %s\n", tcomp->t);

		comp->x = 100;
		tcomp->t = "World";

		comp = iso_entity_get_component(ecs, ent, transform);
		tcomp = iso_entity_get_component(ecs, ent, tag);

		printf("New Transform: %d %d\n", comp->x, comp->y);
		printf("New Tag: %s\n", tcomp->t);

		iso_entity_delete(ecs, ent);
	}

	// Entity remove component
	{
		iso_entity ent = iso_entity_new(ecs);
		printf("Entity: %d\n", ent);

		transform* comp = iso_entity_add_component(ecs, ent, transform, 10, 20);
		printf("Transform: %d %d\n", comp->x, comp->y);

		iso_entity_remove_component(ecs, ent, transform);
		printf("Transform removed\n");

		iso_entity_delete(ecs, ent);
	}

	iso_ecs_delete(ecs);

	iso_memory_alert();
}
