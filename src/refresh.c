#ifdef SCALING
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_rotozoom.h"
#include "refresh.h"

#include "get_resolution.h"

void RefreshScreen(SDL_Surface* tmp)
{
	SDL_Surface* doble;
	doble = zoomSurface(tmp,screen_scale.w_scale,screen_scale.h_scale,0);
	SDL_BlitSurface(doble,NULL,real_screen,&screen_position);
	SDL_Flip(real_screen);
	SDL_FreeSurface(doble);
}

#endif
