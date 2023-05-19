#include "controller.h"
#include "SDL2/SDL_mouse.h"
#include "iso_math/iso_mat/iso_mat.h"

controller* controller_new(iso_vec3 position, iso_vec3 dir) {
	controller* con = iso_alloc(sizeof(controller));

	con->position = position;
	con->dir   = iso_vec3_normalize(dir);
	con->up    = (iso_vec3) { 0.0f, 1.0f, 0.0f };

	con->mouse_enable = false;
	con->pitch = 0.0f;
	con->yaw = -90.0f;
	SDL_GetMouseState(&con->mx, &con->my);

	return con;
}

void controller_delete(controller* con) {
	iso_free(con);
}

void controller_update(controller* con, iso_app* app, f32 dt) {
	controller_update_mouse(con, app);

	if (con->movement[FORWARD]) {
		con->position = iso_vec3_add(con->position, iso_vec3_mul_scalar(con->dir, SPEED * dt));
	}
	if (con->movement[BACKWARD]) {
		con->position = iso_vec3_sub(con->position, iso_vec3_mul_scalar(con->dir, SPEED * dt));
	}
	if (con->movement[LEFT]) {
		con->position = iso_vec3_sub(con->position, iso_vec3_mul_scalar(con->right, SPEED * dt));
	}
	if (con->movement[RIGHT]) {
		con->position = iso_vec3_add(con->position, iso_vec3_mul_scalar(con->right, SPEED * dt));
	}

	iso_mat4 camera_mat = {
		.m = {
			{ con->right.x, con->right.y, con->right.z, 0.0f },
			{ con->up.x   , con->up.y   , con->up.z   , 0.0f },
			{ con->dir.x  , con->dir.y  , con->dir.z  , 0.0f },
			{         0.0f,         0.0f,         0.0f, 1.0f }
		}
	};

	iso_mat4 camera_trans = iso_mat4_translate(iso_mat4_identity(), (iso_vec3) { -con->position.x, -con->position.y, -con->position.z });

	con->look_at = iso_mat4_mul(camera_mat, camera_trans);
}

void controller_update_mouse(controller* con, iso_app* app) {
	if (con->mouse_enable) {
		SDL_CaptureMouse(SDL_TRUE);
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WarpMouseInWindow(app->window->sdl_window, WIDTH / 2, HEIGHT / 2);

		i32 x, y;
		SDL_GetMouseState(&x, &y);

		if (con->first_click) {
			con->first_click = false;
			con->mx = WIDTH / 2;
			con->my = HEIGHT / 2;
			return;
		}

		i32 dx = con->mx - x;
		i32 dy = con->my - y;

		f32 offset_x = dx * SENSITIVITY;
		f32 offset_y = dy * SENSITIVITY;

		con->yaw -= offset_x;
		con->pitch += offset_y;

		if (con->pitch > 89.0f) {
			con->pitch = 89.0f;
		} else if (con->pitch < -89.0f) {
			con->pitch = -89.0f;
		}
	} else {
		SDL_CaptureMouse(SDL_FALSE);
		SDL_ShowCursor(SDL_ENABLE);
	}

	iso_vec3 front;
	front.x = cos(iso_radians(con->yaw)) * cos(iso_radians(con->pitch));
	front.y = sin(iso_radians(con->pitch));
	front.z = sin(iso_radians(con->yaw)) * cos(iso_radians(con->pitch));
	con->dir   = iso_vec3_normalize(front);
	con->right = iso_vec3_normalize(iso_vec3_cross(con->dir, (iso_vec3) { 0.0f, 1.0f, 0.0f }));
	con->up    = iso_vec3_normalize(iso_vec3_cross(con->right, con->dir));
}

void controller_event(controller* con, SDL_Event event, f32 dt) {
	f32 speed = 0.01f;

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				con->movement[FORWARD] = true;
				break;
			case SDLK_s:
				con->movement[BACKWARD] = true;
				break;
			case SDLK_a:
				con->movement[LEFT] = true;
				break;
			case SDLK_d:
				con->movement[RIGHT] = true;
				break;
			case SDLK_q:
				con->position.y += SPEED * dt;
				break;
			case SDLK_ESCAPE:
				con->mouse_enable = con->mouse_enable ? false : true;
				if (con->mouse_enable) con->first_click = true;
				break;
		}
	}
	else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				con->movement[FORWARD] = false;
				break;
			case SDLK_s:
				con->movement[BACKWARD] = false;
				break;
			case SDLK_a:
				con->movement[LEFT] = false;
				break;
			case SDLK_d:
				con->movement[RIGHT] = false;
				break;
		}
	}
}
