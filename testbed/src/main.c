#include "isolate.h"

int main(int argc, char** argv) {
	iso_memory_init();

	for (i32 i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	iso_memory_alert();
	return 0;
}
