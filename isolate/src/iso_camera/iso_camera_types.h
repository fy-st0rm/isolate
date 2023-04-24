#ifndef __ISO_CAMERA_TYPES_H__
#define __ISO_CAMERA_TYPES_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_str.h"
#include "iso_math/iso_math.h"

/*
 * @brief Enum of types of camera supported by the system.
 */

typedef enum {
	ISO_ORTHOGRAPHIC_CAMERA,
	ISO_PERSPECTIVE_CAMERA
} iso_camera_type;


/*=====================
 * Orthographic Camera
 *=====================*/

/*
 * @brief Struct that defines the orthographic camera viewport
 * @mem left, right, top, bottom, near, far = Bounds of the camera view
 */

typedef struct {
	f32 left, right, top, bottom, near, far;
} iso_camera_ortho_viewport_def;

/*
 * @brief Struct that holds the orthographic camera definition
 * @mem name     = Name of the camera
 * @mem pos      = Position of the camera
 * @mem rot      = Rotation of the camera
 * @mem ortho_viewport = Orthogonal Viewport of the camera
 */

typedef struct {
	iso_str      name;
	iso_vec3     pos;
	iso_rotation rot;
	iso_camera_ortho_viewport_def viewport;
} iso_ortho_camera_def;


/*=====================
 * Perspective Camera
 *=====================*/

/*
 * @brief Struct that defines the perspective camera viewport
 * @mem aspect_ratio = Width / height of the window
 * @mem fov          = Field of view of the camera
 * @mem near, far    = Near and far render distance
 */

typedef struct {
	f32 aspect_ratio, fov, near, far;
} iso_camera_persp_viewport_def;

/*
 * @brief Struct that holds the perspective camera definition
 * @mem name     = Name of the camera
 * @mem pos      = Position of the camera
 * @mem rot      = Rotation of the camera
 * @mem persp_viewport = Perspective Viewport of the camera
 */

typedef struct {
	iso_str      name;
	iso_vec3     pos;
	iso_rotation rot;
	iso_camera_persp_viewport_def viewport;
} iso_persp_camera_def;

#endif // __ISO_CAMERA_TYPES_H__
