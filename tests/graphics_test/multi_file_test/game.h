#ifndef __GAME_H__
#define __GAME_H__

#include "isolate.h"

typedef struct {
	iso_app* app;
} game_t;

game_t* game_new(iso_app* app);
void game_delete(game_t* game);

#endif
