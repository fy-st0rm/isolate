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

	int search = 5;
	int res;
	iso_hmap_exists(map, search, res);
	printf("Searching for %d: %d\n", search, res);

	int val;
	iso_hmap_get(map, search, val);
	printf("Value for key %d: %d\n\n", search, val);

	printf("Removing: %d\n", search);
	iso_hmap_remove(map, search);
	iso_hmap_dump(map, "%d", "%d");

	iso_hmap_delete(map);
}

void char_map() {
	iso_hmap_def(char*, char*, 5) map = NULL;
	iso_hmap_add(map, "name_1", "Helo_1");
	iso_hmap_add(map, "name_2", "Helo_2");
	iso_hmap_add(map, "name_3", "Helo_3");
	iso_hmap_add(map, "name_4", "Helo_4");
	iso_hmap_add(map, "name_5", "Helo_5");
	iso_hmap_dump(map, "%s", "%s");

	char* search = "name_3";
	int res;
	iso_hmap_exists(map, search, res);
	printf("Searching for %s: %d\n", search, res);

	char* val;
	iso_hmap_get(map, search, val);
	printf("Value for key %s: %s\n\n", search, val);

	printf("Removing: %s\n", search);
	iso_hmap_remove(map, search);
	iso_hmap_dump(map, "%s", "%s");

	iso_hmap_delete(map);
}

void main() {
	iso_memory_init();
	int_map();
	char_map();
	iso_memory_alert();
}

