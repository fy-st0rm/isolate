#include "iso_camera.h"

iso_camera_manager* iso_camera_manager_new() {
    iso_log_info("Constructing iso_camera_manager.\n");
    iso_camera_manager* cm = iso_alloc(sizeof(iso_camera_manager));
    iso_log_sucess("Created iso_camera_manager.\n");
    return cm;
}

void iso_camera_manager_delete(iso_camera_manager* cm) {
	iso_log_info("Deleting iso_camera_manager\n");
	iso_hmap_delete(cm->cameras);
	iso_free(cm);
	iso_log_sucess("Deleted iso_camera_manager\n");
}

iso_camera* iso_camera_manager_get(iso_camera_manager* man, iso_str name) {
	iso_camera* cam;
	iso_hmap_get(man->cameras, name, cam);
	return cam;
}


iso_camera* iso_ortho_camera_new(iso_camera_manager* cm, iso_ortho_camera_def def) {
	iso_log_info("Constructing iso_ortho_camera\n");

	iso_camera* cam = iso_alloc(sizeof(iso_camera));

	// Initializing camera
    iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of camera is not given.\n");

	cam->name = iso_str_new(tmp);
	iso_str_delete(tmp);

	cam->type = ISO_ORTHOGRAPHIC_CAMERA;

	// Constructing camera
	cam->rot = def.rot;
	cam->pos = def.pos;

	iso_camera_ortho_viewport_def view = def.viewport;
	cam->proj = iso_ortho_projection(view.left, view.right, view.top, view.bottom, view.near, view.far);

	// Saving the camera in memory
	iso_hmap_add(cm->cameras, cam->name, cam);

	iso_log_sucess("Created iso_ortho_camera: (Name: `%s`, Viewport: %f-%f-%f-%f-%f-%f)\n", cam->name, view.left, view.right, view.top, view.bottom, view.near, view.far);
	return cam;
}

void iso_ortho_camera_update(iso_camera_manager* cm, iso_str name) {
	iso_camera* cam;
	iso_hmap_get(cm->cameras, name, cam);

	iso_assert((cam->type == ISO_ORTHOGRAPHIC_CAMERA), "Cannot update non-orthographic camera using iso_ortho_camera_update.\n");

	iso_mat4 transform = iso_mat4_identity();
	transform = iso_mat4_translate(transform, cam->pos);
	transform = iso_mat4_mul(transform, iso_rotate(cam->rot));

	iso_mat4 view_mat = iso_mat4_inverse(transform);
	cam->mvp = iso_mat4_mul(cam->proj, view_mat);
}

void iso_ortho_camera_delete(iso_camera_manager* cm, iso_str name) {
	iso_log_info("Deleting iso_ortho_camera: `%s`...\n", name);
	iso_camera* cam;
	iso_hmap_get(cm->cameras, name, cam);

	iso_assert((cam->type == ISO_ORTHOGRAPHIC_CAMERA), "Cannot delete non-orthographic camera using iso_ortho_camera_update.\n");

	iso_str_delete(cam->name);
	iso_free(cam);
	iso_log_info("Deleted iso_ortho_camera: `%s`\n", name);
}

iso_camera* iso_persp_camera_new(iso_camera_manager* cm, iso_persp_camera_def def) {
	iso_log_info("Constructing iso_perspective_camera\n");

	iso_camera* cam = iso_alloc(sizeof(iso_camera));

	// Initializing camera
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of camera is not given.\n");

	cam->name = iso_str_new(tmp);
	iso_str_delete(tmp);

	cam->type = ISO_PERSPECTIVE_CAMERA;

	// Constructing camera
	cam->rot = def.rot;
	cam->pos = def.pos;

	iso_camera_persp_viewport_def view = def.viewport;
	cam->proj = iso_persp_projection(view.aspect_ratio, view.fov, view.near, view.far);

	// Saving the camera in memory
	iso_hmap_add(cm->cameras, cam->name, cam);
	iso_log_sucess("Created iso_perspective_camera: (Name: `%s`, Viewport: %f-%f-%f-%f)\n", cam->name, view.aspect_ratio, view.fov, view.near, view.far);

	return cam;
}

void iso_persp_camera_update(iso_camera_manager* cm, iso_str name) {
	iso_camera* cam;
	iso_hmap_get(cm->cameras, name, cam);

	iso_assert((cam->type == ISO_PERSPECTIVE_CAMERA), "Cannot update non-perspective camera using iso_persp_camera_update.\n");

	iso_mat4 transform = iso_mat4_identity();
	transform = iso_mat4_translate(transform, cam->pos);

	transform = iso_mat4_mul(transform, iso_rotate(cam->rot));
	iso_mat4 view_mat = iso_mat4_inverse(transform);
	cam->mvp = iso_mat4_mul(cam->proj, view_mat);
}

void iso_persp_camera_delete(iso_camera_manager* cm, iso_str name) {
	iso_log_info("Deleting iso_persp_camera: `%s`...\n", name);
	iso_camera* cam;
	iso_hmap_get(cm->cameras, name, cam);

	iso_assert((cam->type == ISO_PERSPECTIVE_CAMERA), "Cannot delete non-perspective camera using iso_persp_camera_delete.\n");

	iso_str_delete(cam->name);
	iso_free(cam);
	iso_log_info("Deleted iso_persp_camera: `%s`\n", name);
}
