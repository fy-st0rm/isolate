#ifndef __ISO_CAMERA_H__
#define __ISO_CAMERA_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_hash_map.h"
#include "iso_math.h"

/*
 * @brief Enum of types of camera supported by the system.
 */

typedef enum {
	ISO_ORTHOGRAPHIC_CAMERA,
	ISO_PERSPECTIVE_CAMERA
} iso_camera_type;

/*
 * @brief Struct that defines the camera viewport
 * @mem left, right, top, bottom, near, far = Bounds of the camera view
 */

typedef struct {
	f32 left, right, top, bottom, near, far;
} iso_camera_viewport_def;

/*
 * @brief Struct that holds the camera definition
 * @mem name     = Name of the camera
 * @mem type     = Type of camera to construct
 * @mem pos      = Position of the camera
 * @mem rot      = Rotation of the camera
 * @mem viewport = Viewport of the camera
 */

typedef struct {
	char*                   name;
	iso_camera_type         type;
	iso_vec3                pos;
	iso_rotation            rot;
	iso_camera_viewport_def viewport;
} iso_camera_def;

/*
 * @brief Struct that holds the camera
 * @mem name = Name of the camera
 * @mem rot  = Rotation of the camera
 * @mem pos  = Position of the camera
 * @mem proj = Projection matrix of the camera
 * @mem mvp  = Model View Projection Matrix of the camera
 * @mem type = Type of camera constructed
 */

typedef struct {
	char*           name;
	iso_rotation    rot;
	iso_vec3        pos;
	iso_mat4        proj;
	iso_mat4        mvp;
	iso_camera_type type;
} iso_camera;

/*
 * @brief Struct that deals with managing and storing different cameras.
 * @mem cameras = Hashmap to store all the cameras.
 * @mem api     = Api to manage the camera
 */

#define ISO_CAMERA_MEM_SIZE 100
typedef struct iso_camera_manager iso_camera_manager;
struct iso_camera_manager {
	iso_hmap_def(char*, iso_camera*, ISO_CAMERA_MEM_SIZE) cameras;

	struct {
		iso_camera* (*camera_new)    (iso_camera_manager* man, iso_camera_def def);    // Function to create new camera
		void        (*camera_delete) (iso_camera_manager* man, char* name);            // Function to delete the camera
		void        (*camera_update) (iso_camera_manager* man, char* name);            // Function to update the camera
	} api;

	struct {
		iso_camera* (*get_camera)    (iso_camera_manager* man, char* name);
	} memory;
};

/*====================
 * Orthographic Camera
 *==================*/

/*
 * @brief Function to construct new orthographic camera
 * @param man = Pointer to the iso_camera_manager
 * @param def = Camera definition struct
 * @return Returns pointer to the camera
 */

static iso_camera* __iso_ortho_camera_new(iso_camera_manager* man, iso_camera_def def) {
	iso_camera* cam = iso_alloc(sizeof(iso_camera));

	// Initializing camera
	iso_assert(strlen(def.name), "Name of camera is not given.\n");
	cam->name = iso_alloc(strlen(def.name));
	strcpy(cam->name, def.name);
	cam->type = def.type;

	// Constructing camera
	cam->rot = def.rot;
	cam->pos = def.pos;

	iso_camera_viewport_def view = def.viewport;
	cam->proj = iso_ortho_projection(view.left, view.right, view.top, view.bottom, view.near, view.far);

	// Saving the camera in memory
	iso_hmap_add(man->cameras, cam->name, cam);

	return cam;
}

/*
 * @brief Function to calculate model view matrix for orthographic camera.
 * @param cam = Pointer to iso_camera
 */

static void __iso_ortho_camera_update(iso_camera* cam) {
	iso_mat4 transform = iso_mat4_identity();
	iso_mat4_translate(&transform, cam->pos);

	transform = iso_mat4_mul(transform, iso_rotate(cam->rot));

	iso_mat4 view_mat = iso_mat4_inverse(transform);
	cam->mvp = iso_mat4_mul(cam->proj, view_mat);
}

/*====================
 * Perspective Camera
 *==================*/

/*
 * @brief Function to construct new perspective camera
 * @param man = Pointer to the iso_camera_manager
 * @param def = Camera definition struct
 * @return Returns the pointer to the camera
 */

static iso_camera* __iso_persp_camera_new(iso_camera_manager* man, iso_camera_def def) {
	iso_assert(false, "PERSPECTIVE_CAMERA has not been implemented yet!\n");
}

/*
 * @brief Function to calculate model view matrix for perspective camera.
 * @param cam = Pointer to iso_camera
 */

static void __iso_persp_camera_update(iso_camera* cam) {
	iso_assert(false, "PERSPECTIVE_CAMERA has not been implemented yet!\n");
}

/*
 * @brief Function to create a iso_camera
 * @param man = Pointer to the iso_camera_manager
 * @param def = Definition of the camera
 * @return Returns pointer to the iso_camera
 */

static iso_camera* __iso_camera_new(iso_camera_manager* man, iso_camera_def def) {
	switch (def.type) {
		case ISO_ORTHOGRAPHIC_CAMERA:
			return __iso_ortho_camera_new(man, def);
		case ISO_PERSPECTIVE_CAMERA:
			return __iso_persp_camera_new(man, def);
		default:
			iso_assert(false, "Unknown ISO_CAMERA_TYPE: %d\n", def.type);
	}
}

/*
 * @brief Function to delete the iso_camera
 * @param man  = Pointer to the iso_camera_manager
 * @param name = Name of the camera
 */

static void __iso_camera_delete(iso_camera_manager* man, char* name) {
	iso_camera* cam;
	iso_hmap_get(man->cameras, name, cam);
	iso_free(cam->name);
	iso_free(cam);
}

/*
 * @brief Function to update the camera
 * @param man  = Pointer to the iso_camera_manager
 * @param name = Name of the camera
 */

static void __iso_camera_update(iso_camera_manager* man, char* name) {
	iso_camera* cam;
	iso_hmap_get(man->cameras, name, cam);

	switch (cam->type) {
		case ISO_ORTHOGRAPHIC_CAMERA:
			__iso_ortho_camera_update(cam);
			break;
		case ISO_PERSPECTIVE_CAMERA:
			__iso_persp_camera_update(cam);
			break;
		default:
			iso_assert(false, "Unknown ISO_CAMERA_TYPE: %d\n", cam->type);
	}
}

/*
 * @brief Function to get the camera from memory
 * @param man  = Pointer to the iso_camera_manager
 * @param name = Name of the camera to be searched
 * @return Returns pointer to the iso_camera
 */

static iso_camera* __iso_get_camera(iso_camera_manager* man, char* name) {
	iso_camera* cam;
	iso_hmap_get(man->cameras, name, cam);
	return cam;
}

/*
 * @brief Function to contruct iso_camera_manager
 * @return Returns the pointer to the iso_camera_manager
 */

static iso_camera_manager* iso_camera_manager_new() {
	iso_camera_manager* man = iso_alloc(sizeof(iso_camera_manager));

	// Loading the functions
	man->api.camera_new    = __iso_camera_new;
	man->api.camera_delete = __iso_camera_delete;
	man->api.camera_update = __iso_camera_update;
	man->memory.get_camera = __iso_get_camera;

	return man;
}

/*
 * @brief Function to delete iso_camera_manager
 * @param man = Pointer to the iso_camera_manager
 */

static void iso_camera_manager_delete(iso_camera_manager* man) {
	iso_hmap_delete(man->cameras);
	iso_free(man);
}

#endif // __ISO_CAMERA_H__
