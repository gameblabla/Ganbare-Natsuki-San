#pragma once

#ifdef SCALING
#include <SDL/SDL.h>
extern SDL_Surface *real_screen;
extern SDL_Rect screen_position;
struct scaling
{
	unsigned short w_display;
	unsigned short h_display;
};
extern struct scaling screen_scale;
#endif

void Get_Resolution(void);
