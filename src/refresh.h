#ifdef SCALING
	extern void RefreshScreen(SDL_Surface* tmp);
#else
	#define RefreshScreen SDL_Flip
#endif
