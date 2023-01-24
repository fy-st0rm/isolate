#ifndef __ISO_ENTRY_H__
#define __ISO_ENTRY_H__

#include "iso_game/iso_game_inst.h"
#include "iso_util/iso_memory.h"

/*
 * @brief Initialize function which will be called by user.
 */

b8 iso_init(iso_game_inst* game_inst);

/*
 * @brief Update function which will be called by user. It will be called every frame.
 */

void iso_update(iso_game_inst* game_inst, f32 dt);

/*
 * @brief Exit function which will be called by user. Cleans up the memory and stuff.
 */

void iso_exit();

/*
 * @brief Program entry point of the program
 * @param argc = Number of command line arguments.
 * @param argv = List of command line arguments.
 */

int main(int argc, char** argv) {
	// Initialize the engine memory
	iso_memory_init();
	iso_game_inst* game_inst = iso_alloc(sizeof(iso_game_inst));
	strcpy(game_inst->title, "Window");
	game_inst->win_width  = 800;
	game_inst->win_height = 600;
	game_inst->running    = iso_init(game_inst);

	// Main loop
	while (game_inst->running) {
		iso_update(game_inst, 0.0f);
	}

	// Exit of the engine
	iso_exit();

	iso_free(game_inst);
	iso_memory_alert();
}

#endif //__ISO_ENTRY_H__
