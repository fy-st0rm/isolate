#include "game.h"

game_t* game_new(iso_app* app) {
	printf("%p\n", memory);
	game_t* game = iso_alloc(sizeof(game_t));
	game->app = app;
	return game;
}

void game_delete(game_t *game) {
	iso_free(game);
}
