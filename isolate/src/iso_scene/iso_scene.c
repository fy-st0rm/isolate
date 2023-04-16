#include "iso_scene.h"

iso_scene_manager* iso_scene_manager_new() {
	iso_log_info("Constructing scene manager...\n");

	iso_scene_manager* manager = iso_alloc(sizeof(iso_scene_manager));
	manager->current_scene = iso_str_new("");
	manager->scenes = NULL;

	iso_log_sucess("Created scene manager.\n");
	return manager;
}

void iso_scene_manager_delete(iso_scene_manager* manager) {
	iso_log_info("Deleting scene manager.\n");

	iso_hmap_delete(manager->scenes);
	iso_str_delete(manager->current_scene);
	iso_free(manager);

	iso_log_sucess("Deleted scene manager.\n");
}

void iso_scene_manager_reset(iso_scene_manager* manager) {
	iso_log_info("Reseting current scene...\n");

	iso_str_clear(manager->current_scene);

	iso_log_sucess("Reseted current scene.\n");
}

void iso_scene_new(iso_scene_manager* manager, iso_scene_def def) {
	iso_str tmp_name = iso_str_new(def.name);

	iso_assert(iso_str_len(tmp_name) > 0, "Name of the scene isnt provided.\n");

	iso_log_info("Constructing scene: `%s` ...\n", tmp_name);

	iso_scene* scene = iso_alloc(sizeof(iso_scene));

	scene->name = iso_str_new(tmp_name);
	iso_str_delete(tmp_name);

	iso_log_info("Loading functions..\n");
	scene->scene_data = def.scene_data;
	scene->new        = def.new;
	scene->delete     = def.delete;
	scene->on_entry   = def.on_entry;
	scene->on_exit    = def.on_exit;
	scene->on_update  = def.on_update;
	scene->on_event   = def.on_event;

	// Calling the constructor
	scene->new(scene);

	iso_hmap_add(manager->scenes, scene->name, scene);

	if (iso_str_len(manager->current_scene) == 0) {
		iso_scene_switch(manager, scene->name);
	}

	iso_log_sucess("Created scene: `%s`\n", scene->name);
}

void iso_scene_delete(iso_scene_manager* manager, iso_str name) {
	iso_str tmp_name = iso_str_new(name);

	iso_log_info("Deleting scene: `%s` ...\n", tmp_name);

	b8 res = false;
	iso_hmap_exists(manager->scenes, tmp_name, res);

	iso_assert(res, "Scene to delete doesnt exists: `%s`\n", tmp_name);

	// Cannot delete the currently running scene
	if (iso_str_len(manager->current_scene) != 0) {
		iso_assert(iso_str_cmp(manager->current_scene, tmp_name),  "Cannot delete currently active scene: `%s`\n", manager->current_scene);
	}

	// Calling destructors
	iso_scene* scene;
	iso_hmap_get(manager->scenes, tmp_name, scene);
	scene->on_exit(scene);
	scene->delete(scene);

	// Deleting from memory
	iso_hmap_remove(manager->scenes, scene->name);

	iso_log_sucess("Deleted scene: `%s`\n", tmp_name);

	iso_str_delete(scene->name);
	iso_str_delete(tmp_name);
	iso_free(scene);

}

void iso_scene_switch(iso_scene_manager* manager, iso_str name) {
	iso_str tmp_name = iso_str_new(name);

	b8 res = false;
	iso_hmap_exists(manager->scenes, tmp_name, res);

	iso_assert(res, "Scene to switch doesnt exists: `%s`\n", tmp_name);

	// Exiting the previous scene
	if (iso_str_len(manager->current_scene) != 0) {
		iso_scene* scene;
		iso_hmap_get(manager->scenes, manager->current_scene, scene);

		scene->on_exit(scene);
	}

	iso_str_delete(manager->current_scene);
	manager->current_scene = iso_str_new(tmp_name);
	iso_str_delete(tmp_name);

	iso_scene* scene;
	iso_hmap_get(manager->scenes, manager->current_scene, scene);

	scene->on_entry(scene);

}
