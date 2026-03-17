#include "../../define.h"
#include "../../renderer/renderer.h"
#include "../../general.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef OPENGL_SCALING
#include <GL/gl.h>
#endif

SDL_Surface* g_screen;

#if defined(SCALING) || defined(OPENGL_SCALING)
SDL_Surface* real_screen;
#endif

void Renderer_Init(void)
{
	SDL_Init(SDL_INIT_VIDEO);
}

void Renderer_InitVideo(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
#if defined(SCALING) || defined(OPENGL_SCALING)
	Uint32 flags;
	#ifdef OPENGL_SCALING
	flags = SDL_OPENGL;
	#else
	flags = 0;
	#endif

	real_screen = SDL_SetVideoMode(FINAL_RESOLUTION_WIDTH, FINAL_RESOLUTION_HEIGHT,
	#ifdef OPENGL_SCALING
	0,
	#else
	DEPTH,
	#endif
	flags | SDL_NOFRAME);

	#ifdef OPENGL_SCALING
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, real_screen->w, real_screen->h);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)real_screen->w, (GLdouble)real_screen->h, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4d(1.0f, 1.0f, 1.0f, 1.0f);
	#endif

	g_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, DISPLY_WIDTH, DISPLY_HEIGHT,
	#ifdef OPENGL_SCALING
	16,
	#else
	DEPTH,
	#endif
	0,0,0,0);
#else
	#ifdef GP2X
	g_screen = SDL_SetVideoMode(DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWPALETTE);
	#elif defined(GCW0)
	g_screen = SDL_SetVideoMode(DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, SDL_HWSURFACE | SDL_TRIPLEBUF);
	#elif defined(RS90)
	g_screen = SDL_SetVideoMode(DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, SDL_HWSURFACE | SDL_TRIPLEBUF | SDL_YUV444);
	#elif defined(DREAMCAST)
	g_screen = SDL_SetVideoMode(DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, SDL_HWSURFACE | SDL_DOUBLEBUF);
	#elif defined(MINGW)
	g_screen = SDL_SetVideoMode(DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWPALETTE | SDL_FULLSCREEN);
	#else
	g_screen = SDL_SetVideoMode(DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, SDL_SWSURFACE);
	#endif
#endif
}

void Renderer_ShutdownVideo(void)
{
#ifdef OPENGL_SCALING
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
#endif
}

void Renderer_Shutdown(void)
{
	SDL_Quit();
}

void Renderer_SetWindowCaption(const char *title, const char *icon)
{
	SDL_WM_SetCaption(title, icon);
}

void Renderer_Flip(void)
{
}

void Renderer_RefreshScreen(void)
{
	SDL_Flip(g_screen);
}

void Renderer_FlipReal(void* screen)
{
#ifdef OPENGL_SCALING
	const float s_fDrawRate = 1.0f;
	GLuint texture;
	int dx, dy, dw, dh, sx, sy, sw, sh;
	int x, y, w, h;
	float texMinX, texMaxX, texMinY, texMaxY;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGB,
		     screen->w, screen->h,
		     0,
		     GL_RGB,
		     GL_UNSIGNED_SHORT,
		     screen->pixels);
		     
	dx = 0;
	dy = 0;
	dw = real_screen->w;
	dh = real_screen->h;
	sx = 0;
	sy = 0;
	sw = screen->w;
	sh = screen->h;
	
	x = (int)((float)dx * s_fDrawRate);
	y = (int)((float)dy * s_fDrawRate);
	w = (int)((float)(dw + dx) * s_fDrawRate) - x;
	h = (int)((float)(dh + dy) * s_fDrawRate) - y;

	texMinX = (float) sx       / (float)screen->w;
	texMaxX = (float)(sx + sw) / (float)screen->w;
	texMinY = (float) sy       / (float)screen->h;
	texMaxY = (float)(sy + sh) / (float)screen->h;

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y  );
	glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y  );
	glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
	glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
	glEnd();
	
	SDL_GL_SwapBuffers( );
#elif defined(SCALING)
	if (real_screen->w == DISPLAY_WIDTH*2 && real_screen->h == DISPLAY_HEIGHT*2)
	{
		Simple2x((unsigned char*)screen->pixels, screen->pitch, (unsigned char*)real_screen->pixels, real_screen->pitch, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}
	else
	{
		SDL_SoftStretch(screen,NULL,real_screen,NULL);
	}
	SDL_Flip(screen);
#endif
}

static SDL_Surface* bitmap[BMPBUFF_MAX];

int LoadBitmap(char *fname, int bmpindex, int flag)
{
	SDL_Surface* tmp;
	int rc = 0;

	ReleaseBitmap(bmpindex);

	tmp = SDL_LoadBMP(fname);
	
	if (tmp)
	{
		if (flag != 0) SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(tmp->format,16,99,62));
		bitmap[bmpindex] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}
	else
	{
		rc = -1;
	}

	return rc;
}

void ReleaseBitmap(int bmpindex)
{
	if (bitmap[bmpindex] != NULL)
	{
		SDL_FreeSurface(bitmap[bmpindex]);
		bitmap[bmpindex] = NULL;
	}
}

void BltRect(int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height)
{
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width;
	dstRect.h = height;

	SDL_BlitSurface(bitmap[bmpindex], &srcRect, g_screen, &dstRect);
}

void Blt(int bmpindex, int dstX, int dstY)
{
	SDL_Rect dstRect;

	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = 0;
	dstRect.h = 0;

	SDL_BlitSurface(bitmap[bmpindex], NULL, g_screen, &dstRect);
}

void ClearSecondary(void)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = DISPLY_WIDTH;
	rect.h = DISPLY_HEIGHT;
	SDL_FillRect(g_screen, &rect, SDL_MapRGBA(g_screen->format,0,0,0,255));
}

void CreateSurface(int bmpindex, int size_x, int size_y)
{
	SDL_Surface* tmp;
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	ReleaseBitmap(bmpindex);
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, size_x, size_y, 0, rmask, gmask, bmask, amask);
	if(tmp)
	{
		bitmap[bmpindex] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}
}

void SwapToSecondary(int bmpindex)
{
}

void SaveBmp(int bmpindex, char *fn)
{
	SDL_SaveBMP(bitmap[bmpindex], fn);
}

void SetGscreenPalette(void* surface)
{
	SDL_Surface* s = (SDL_Surface*)surface;
	if(s && s->format->palette)
	{
		SDL_SetColors(g_screen, s->format->palette->colors, 0, s->format->palette->ncolors);
	}
}

void Renderer_SetPalette(void* surface)
{
	(void)surface;
}

uint32_t Renderer_GetTicks(void)
{
	return SDL_GetTicks();
}

void Renderer_Delay(uint32_t ms)
{
	SDL_Delay(ms);
}

int Renderer_PollEvent(void* event)
{
	return SDL_PollEvent((SDL_Event*)event);
}

void* Renderer_GetScreen(void)
{
	return g_screen;
}

void* Renderer_LoadSurface(const char* path)
{
	return SDL_LoadBMP(path);
}

void Renderer_FreeSurface(void* surface)
{
	SDL_FreeSurface((SDL_Surface*)surface);
}
