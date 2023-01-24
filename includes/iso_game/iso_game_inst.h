#ifndef __ISO_GAME_INST_H__
#define __ISO_GAME_INST_H__

#include "iso_util/iso_defines.h"
#include "iso_scene.h"

typedef struct {
	char title[256];
	u32 win_width, win_height;
	b8 running;
} iso_game_inst;


/*
 * @brief Function to create a iso scene
 * @param x_size = Total cell number in X-axis
 * @param y_size = Total cell number in Y-axis
 * @param z_size = Total cell number in Z-axis
 * @param cell_width = Width of each cell
 * @param cell_height = Height of each cell
 */

static iso_scene* iso_scene_create(
	iso_game_inst* game_inst, char* name,
	u32 x_size, u32 y_size, u32 z_size,
	u32 cell_width, u32 cell_height
) {
	//TODO: Implement this
	iso_scene* scene = iso_alloc(sizeof(iso_scene));
	return scene;
}

#endif // __ISO_GAME_INST_H__
