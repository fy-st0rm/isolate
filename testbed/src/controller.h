#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "isolate.h"
#include "globals.h"

typedef enum {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
} direction;

typedef struct {
	iso_vec3 dir, up, right;
	iso_vec3 position;
	iso_mat4 look_at;

	f32 pitch, yaw;

	// Mouse
	i32 mx, my;
	b8 first_click;
	b8 mouse_enable;

	// Keyboard
	b8 movement[6];
} controller;

controller* controller_new(iso_vec3 position, iso_vec3 dir);
void controller_delete(controller* con);
void controller_update(controller* con, iso_app* app, f32 dt);
void controller_update_mouse(controller* con, iso_app* app);
void controller_event(controller* con, SDL_Event event, f32 dt);

#endif //__CONTROLLER_H__
