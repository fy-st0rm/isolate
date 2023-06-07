#ifndef __ISO_FILESYSTEM_H__
#define __ISO_FILESYSTEM_H__

#include "iso_defines.h"
#include "iso_str.h"
#include "iso_log.h"
#include "iso_list.h"

ISO_API b8 iso_fs_path_exists(iso_str path);
ISO_API void iso_fs_get_cwd(iso_str* out);
ISO_API void iso_fs_mkdir(iso_str path);
//ISO_API void iso_fs_lsdir(iso_str path, iso_list(iso_str)* out);

#endif //__ISO_FILESYSTEM_H__
