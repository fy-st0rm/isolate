#ifndef __ISO_CAMERA_H__
#define __ISO_CAMERA_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_hash_map.h"
#include "iso_util/iso_str.h"
#include "iso_math/iso_math.h"
#include "iso_camera_types.h"

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
	iso_str         name;
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

#define ISO_CAMERA_MEM_SIZE 99
typedef struct {
	iso_hmap(char*, iso_camera*, ISO_CAMERA_MEM_SIZE) cameras;
} iso_camera_manager;


/*
 * @brief Function create new iso_camera_manager
 * @return Returns pointer to iso_camera_manager
 */

ISO_API iso_camera_manager* iso_camera_manager_new();

/*
 * @brief Function to delete iso_camera_manager
 * @param cm = Pointer to the iso_camera_manager
 */

ISO_API void iso_camera_manager_delete(iso_camera_manager* cm);

/*
 * @brief Function to get camera from manager
 * @param cm   = Pointer to the iso_camera_manager
 * @param name = Name of the camera
 * @return Returns iso_camera pointer
 */

ISO_API iso_camera* iso_camera_manager_get(iso_camera_manager* cm, iso_str name);


/*
 * @brief Function to create new orthographic camera
 * @param cm  = Pointer to the iso_camera_manager
 * @param def = Definition of iso_ortho_camera
 * @return Returns iso_camera pointer
 */

ISO_API iso_camera* iso_ortho_camera_new(iso_camera_manager* cm, iso_ortho_camera_def def);

/*
 * @brief Function to update orthographic camera
 * @param cm   = Pointer to the iso_camera_manager
 * @param name = Name of the camera
 */

ISO_API void iso_ortho_camera_update(iso_camera_manager* cm, iso_str name);

/*
 * @brief Function to delete orthographic camera
 * @param cm   = Pointer to the iso_camera_manager
 * @param name = Name of the camera
 */

ISO_API void iso_ortho_camera_delete(iso_camera_manager* cm, iso_str name);


/*
 * @brief Function to create new perspective camera
 * @param cm  = Pointer to the iso_camera_manager
 * @param def = Definition of iso_persp_camera
 * @return Returns iso_camera pointer
 */

ISO_API iso_camera* iso_persp_camera_new(iso_camera_manager* cm, iso_persp_camera_def def);

/*
 * @brief Function to update perspective camera
 * @param cm   = Pointer to the iso_camera_manager
 * @param name = Name of the camera
 */

ISO_API void iso_persp_camera_update(iso_camera_manager* cm, iso_str name);

/*
 * @brief Function to delete perspective camera
 * @param cm   = Pointer to the iso_camera_manager
 * @param name = Name of the camera
 */

ISO_API void iso_persp_camera_delete(iso_camera_manager* cm, iso_str name);

#endif // __ISO_CAMERA_H__
