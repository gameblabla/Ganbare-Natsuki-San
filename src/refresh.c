#ifdef SCALING
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "refresh.h"
#include "get_resolution.h"

extern SDL_Surface *g_screen;

void RefreshScreen(SDL_Surface* tmp)
{
	SDL_SoftStretch(g_screen,NULL,real_screen,NULL);
	SDL_Flip(real_screen);
}

#endif
