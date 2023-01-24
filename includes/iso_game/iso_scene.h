#ifndef __ISO_SCENE_H__
#define __ISO_SCENE_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"

typedef struct {
	u32*** world;
	u32 cell_width, cell_height;
} iso_scene;

#endif //__ISO_SCENE_H__

