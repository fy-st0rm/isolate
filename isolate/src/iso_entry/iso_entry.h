#ifndef __ISO_ENTRY_H__
#define __ISO_ENTRY_H__

#include "iso_util/iso_memory.h"
#include "iso_app/iso_app.h"

typedef struct {
    iso_app_def (*iso_init ) ();
    void        (*iso_start) (iso_app* app);
    void        (*iso_exit ) (iso_app* app);
} iso_entry_def;

/*
 * @brief Program entry point of the program
 * @param argc = Number of command line arguments.
 * @param argv = List of command line arguments.
 */

ISO_API int iso_run(int argc, char** argv, iso_entry_def def);

#endif //__ISO_ENTRY_H__
