#ifndef __ISO_ENTRY_H__
#define __ISO_ENTRY_H__

#include "iso_util/iso_memory.h"
#include "iso_app/iso_app.h"

/*
 * @brief Function that will initialize the app
 */

ISO_API iso_app_def iso_init();

/*
 * @brief Function that will be called at the start of the app
 * @param app = Pointer to the iso_app
 */

ISO_API void iso_start(iso_app* app);

/*
 * @brief Function that will be called at the end of the app
 */

ISO_API void iso_exit(iso_app* app);

/*
 * @brief Program entry point of the program
 * @param argc = Number of command line arguments.
 * @param argv = List of command line arguments.
 */

ISO_API int iso_run(int argc, char** argv);

#endif //__ISO_ENTRY_H__
