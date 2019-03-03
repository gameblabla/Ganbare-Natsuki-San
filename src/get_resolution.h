#pragma once

#ifdef SCALING
#include <SDL/SDL.h>
SDL_Surface *real_screen;
SDL_Rect screen_position;
struct scaling
{
	unsigned short w_display;
	unsigned short h_display;
} screen_scale;
#endif

void Get_Resolution(void);
