#ifndef __ISO_WINDOW_H__
#define __ISO_WINDOW_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"
#include "iso_util/iso_str.h"
#include "iso_math/iso_math.h"

/*
 * @brief Enum that renamed SDL Window Flags to Iso Window Flags
 */

typedef enum {
	ISO_WINDOW_FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
	ISO_WINDOW_FULLSCREEN         = SDL_WINDOW_FULLSCREEN,
	ISO_WINDOW_SHOWN              = SDL_WINDOW_SHOWN,
	ISO_WINDOW_HIDDEN             = SDL_WINDOW_HIDDEN,
	ISO_WINDOW_BORDERLESS         = SDL_WINDOW_BORDERLESS,
	ISO_WINDOW_RESIZABLE          = SDL_WINDOW_RESIZABLE,
	ISO_WINDOW_MINIMIZED          = SDL_WINDOW_MINIMIZED,
	ISO_WINDOW_MAXIMIZED          = SDL_WINDOW_MAXIMIZED,
	ISO_WINDOW_INPUT_GRABBED      = SDL_WINDOW_INPUT_GRABBED,
	ISO_WINDOW_INPUT_FOCUS        = SDL_WINDOW_INPUT_FOCUS,
	ISO_WINDOW_MOUSE_FOCUS        = SDL_WINDOW_MOUSE_FOCUS,
	ISO_WINDOW_FOREIGN            = SDL_WINDOW_FOREIGN,
	ISO_WINDOW_ALLOW_HIGHDPI      = SDL_WINDOW_ALLOW_HIGHDPI,
	ISO_WINDOW_MOUSE_CAPTURE      = SDL_WINDOW_MOUSE_CAPTURE,
	ISO_WINDOW_ALWAYS_ON_TOP      = SDL_WINDOW_ALWAYS_ON_TOP,
	ISO_WINDOW_SKIP_TASKBAR       = SDL_WINDOW_SKIP_TASKBAR,
	ISO_WINDOW_UTILITY            = SDL_WINDOW_UTILITY,
	ISO_WINDOW_TOOLTIP            = SDL_WINDOW_TOOLTIP,
	ISO_WINDOW_POPUP_MENU         = SDL_WINDOW_POPUP_MENU,
} iso_window_flags;

/*
 * @brief Window defination structure. Defines on how the window should be.
 * @mem title  = Title of the window.
 * @mem width  = Width of the window.
 * @mem height = Height of the window.
 */

typedef struct {
	iso_str title;
	i32 width, height;
	iso_window_flags flags;
} iso_window_def;

/*
 * @brief Structure of the iso_window
 * @mem title        = Title of the window
 * @mem width        = Width of the window
 * @mem height       = Height of the window
 * @mem sdl_window   = Pointer to the SDL_Window
 * @mem sdl_renderer = Pointer to the SDL_Renderer
 */

typedef struct {
	iso_str title;
	i32 width, height;
	SDL_Window* sdl_window;
	SDL_Renderer* sdl_renderer;
} iso_window;

/*
 * @brief Function to contruct new window using the window defination.
 * @param window_def = window defination structure
 * @return Returns the pointer to the iso_window
 */

ISO_API iso_window* iso_window_new(iso_window_def window_def);

/*
 * @brief Function to delete the window.
 * @param window = Pointer to the iso_window
 */

ISO_API void iso_window_delete(iso_window* window);


/*
 * @brief Function to clear window
 * @param window = Pointer to iso_window
 * @param color  = Color of the background
 */

ISO_API void iso_window_clear(iso_window* window, iso_vec4 color);

#endif // __ISO_WINDOW_H__
