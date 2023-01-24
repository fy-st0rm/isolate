#include "iso_util/iso_hash_map.h"
#include <stddef.h>

void int_map() {
	iso_hmap_def(int, int, 5) map = NULL;
	iso_hmap_add(map, 1, 10);
	iso_hmap_add(map, 2, 10);
	iso_hmap_add(map, 3, 10);
	iso_hmap_add(map, 4, 11);
	iso_hmap_add(map, 5, 10);
	iso_hmap_dump(map, "%d", "%d");
	int i = iso_hmap_get(map, 1);
	printf("GOT: %d\n\n", i);
	iso_hmap_delete(map);
}

void char_map() {
	iso_hmap_def(char*, char*, 3) map = NULL;
	iso_hmap_add(map, "name_1", "Helo_1");
	iso_hmap_add(map, "name_2", "Helo_2");
	iso_hmap_add(map, "name_3", "Helo_3");
	iso_hmap_add(map, "name_4", "Helo_4");
	iso_hmap_add(map, "name_5", "Helo_5");
	iso_hmap_dump(map, "%s", "%s");
	char* i = iso_hmap_get(map, "name_5");
	printf("GOT: %s\n\n", i);
	iso_hmap_delete(map);
}

void main() {
	iso_memory_init();
	int_map();
	char_map();
	iso_memory_alert();
}

