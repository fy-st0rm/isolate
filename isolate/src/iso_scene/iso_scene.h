#ifndef __ISO_SCENE_H__
#define __ISO_SCENE_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_hash_map.h"
#include "iso_util/iso_log.h"
#include "iso_util/iso_str.h"


/*
 * @brief Structure of the iso_scene
 * @mem name       = Name of the scene
 * @mem scene_data = Excess data that user can use
 */

typedef struct iso_scene iso_scene;
struct iso_scene {
	iso_str name;
	void* scene_data;
	void (*new)       (iso_scene* scene);                           // Function that will be called during scene construction
	void (*delete)    (iso_scene* scene);                           // Function that will be called during destruction of scene
	void (*on_entry)  (iso_scene* scene);                           // Function that will be called during the entry of the scene
	void (*on_exit)   (iso_scene* scene);                           // Function that will be called during the exit of the scene
	void (*on_update) (iso_scene* scene, f32 dt);                   // Function that will be called for every frame
	void (*on_event)  (iso_scene* scene, SDL_Event event, f32 dt);  // Function that will be called during events
};


/*
 * @brief Scene definition struct
 * @mem name       = Name of the scene
 * @mem scene_data = Excess data that user can use
 */

typedef struct {
	char* name;
	void* scene_data;
	void (*new)       (iso_scene* scene);
	void (*delete)    (iso_scene* scene);
	void (*on_entry)  (iso_scene* scene);
	void (*on_exit)   (iso_scene* scene);
	void (*on_update) (iso_scene* scene, f32 dt);
	void (*on_event)  (iso_scene* scene, SDL_Event event, f32 dt);
} iso_scene_def;


/*
 * @brief Scene manager struct
 * @mem scenes        = Hash map to hold all scenes
 * @mem current_scene = Name of the currently active scene
 */

#define ISO_SCENE_MANAGER_MEM_SZ 100
typedef struct {
	iso_hmap_def(char*, iso_scene*, ISO_SCENE_MANAGER_MEM_SZ) scenes;
	iso_str current_scene;
} iso_scene_manager;


/*
 * @brief Function to create a new scene manager
 * @return Returns pointer to iso_scene_manager
 */

ISO_API iso_scene_manager* iso_scene_manager_new();

/*
 * @brief Function to delete the scene manager
 * @param manager = Pointer to the iso_scene_manager
 */

ISO_API void iso_scene_manager_delete(iso_scene_manager* manager);

/*
 * @brief Function to reset the current scenes
 * @param manager = Pointer to the iso_scene_manager
 */

ISO_API void iso_scene_manager_reset(iso_scene_manager* manager);

/*
 * @brief Function to create new scene
 * @param manager = Pointer to the iso_scene_manager
 * @param def     = iso_scene_def struct
 */

ISO_API void iso_scene_new(iso_scene_manager* manager, iso_scene_def def);

/*
 * @brief Function to delete scene
 * @param manager = Pointer to the iso_scene_manager
 * @param name    = Name of the scene to delete
 */

ISO_API void iso_scene_delete(iso_scene_manager* manager, char* name);

/*
 * @brief Function to change the scene
 * @param manager = Pointer to the iso_scene_manager
 * @param name    = Name of the scene to change
 */

ISO_API void iso_scene_switch(iso_scene_manager* manager, char* name);

#endif // __ISO_SCENE_H__
