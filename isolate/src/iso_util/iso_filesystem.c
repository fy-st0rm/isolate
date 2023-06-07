#include "iso_filesystem.h"

b8 iso_fs_path_exists(iso_str path) {
	struct stat sb;
	
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		return true;
	}
	return false;
}

void iso_fs_get_cwd(iso_str* out) {
	char cwd[500];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		*out = iso_str_new(cwd);
	} else {
		iso_assert(false, "iso_fs_get_cwd failed.\n");
	}
}

void iso_fs_mkdir(iso_str path) {
	iso_assert(!iso_fs_path_exists(path), "`%s` directory already exists\n", path);

#ifdef ISO_PLAT_WIN32
	mkdir(path);
#elif defined(ISO_PLAT_LINUX)
	mkdir(path, 0700);
#endif
}
