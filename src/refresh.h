#ifndef REFRESH_H
#define REFRESH_H

#include <SDL.h>
#if defined(SCALING) || defined(OPENGL_SCALING)
	extern void RefreshScreen(SDL_Surface* tmp);
	extern SDL_Surface* real_screen;
#else
	#define RefreshScreen SDL_Flip
#endif
#endif
