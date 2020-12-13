#include <SDL/SDL.h>
#ifdef _cplusplus
extern "C" {
#endif
#include "get_resolution.h"
#include "define.h"
#ifdef _cplusplus
}
#endif

#ifdef SYLLABLE
#include <gui/desktop.h>
#endif

struct scaling screen_scale;
SDL_Surface *real_screen;

void Get_Resolution(void)
{
#ifdef SCALING
	#ifdef SYLLABLE
		os::Desktop cDesktop;
		os::IPoint point = cDesktop.GetResolution();
		screen_scale.w_display = point.x;
		screen_scale.h_display = point.y;
	#else
		const SDL_VideoInfo* info = SDL_GetVideoInfo();
		screen_scale.w_display = info->current_w;
		screen_scale.h_display = info->current_h; 
	#endif
#endif
}
