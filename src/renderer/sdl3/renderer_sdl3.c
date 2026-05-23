#include "../../define.h"
#include "../../renderer/renderer.h"
#include "../../general.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * SDL3 renderer backend.
 *
 * The game still draws to a software framebuffer using the old LoadBitmap/Blt
 * API.  SDL3 owns an opaque backend state and presents that framebuffer through
 * a streaming texture with aspect-correct scaling.  This keeps gameplay code
 * free of SDL3 types and preserves the existing 320x240 / 400x240 widescreen
 * coordinate systems.
 */
typedef struct SDL3RendererBackend {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Surface *screen;
	SDL_Surface *bitmap[BMPBUFF_MAX];
	int window_w;
	int window_h;
	int initialised;
} SDL3RendererBackend;

static SDL3RendererBackend *sdl3_backend;

static SDL3RendererBackend *backend(void)
{
	return sdl3_backend;
}

static SDL_FRect present_rect(SDL3RendererBackend *b)
{
	SDL_FRect rect;
	float scale_x;
	float scale_y;
	float scale;
	int ww = 0;
	int wh = 0;

	SDL_GetWindowSize(b->window, &ww, &wh);
	if (ww <= 0 || wh <= 0) {
		ww = b->window_w;
		wh = b->window_h;
	}

	scale_x = (float)ww / (float)DISPLY_WIDTH;
	scale_y = (float)wh / (float)DISPLY_HEIGHT;
	scale = scale_x < scale_y ? scale_x : scale_y;

	if (scale <= 0.0f) {
		scale = 1.0f;
	}

	rect.w = (float)DISPLY_WIDTH * scale;
	rect.h = (float)DISPLY_HEIGHT * scale;
	rect.x = ((float)ww - rect.w) * 0.5f;
	rect.y = ((float)wh - rect.h) * 0.5f;
	return rect;
}

void Renderer_Init(void)
{
	if (sdl3_backend) {
		return;
	}

	sdl3_backend = (SDL3RendererBackend*)calloc(1, sizeof(*sdl3_backend));
	if (!sdl3_backend) {
		return;
	}

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL3 video init failed: %s\n", SDL_GetError());
		free(sdl3_backend);
		sdl3_backend = NULL;
		return;
	}

	sdl3_backend->initialised = 1;
}

void Renderer_InitVideo(int argc, char *argv[])
{
	SDL3RendererBackend *b = backend();
	SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
	const int default_scale = 2;
	(void)argc;
	(void)argv;

	if (!b) {
		return;
	}

	b->window_w = FINAL_RESOLUTION_WIDTH > 0 ? FINAL_RESOLUTION_WIDTH : DISPLY_WIDTH * default_scale;
	b->window_h = FINAL_RESOLUTION_HEIGHT > 0 ? FINAL_RESOLUTION_HEIGHT : DISPLY_HEIGHT * default_scale;

	if (!SDL_CreateWindowAndRenderer("Ganbare Natsuki-san", b->window_w, b->window_h, flags, &b->window, &b->renderer)) {
		fprintf(stderr, "SDL3 window/renderer creation failed: %s\n", SDL_GetError());
		return;
	}

	b->screen = SDL_CreateSurface(DISPLY_WIDTH, DISPLY_HEIGHT, SDL_PIXELFORMAT_ARGB8888);
	b->texture = SDL_CreateTexture(b->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, DISPLY_WIDTH, DISPLY_HEIGHT);
	if (!b->screen || !b->texture) {
		fprintf(stderr, "SDL3 framebuffer creation failed: %s\n", SDL_GetError());
		return;
	}

	SDL_SetTextureScaleMode(b->texture, SDL_SCALEMODE_NEAREST);
}

void Renderer_ShutdownVideo(void)
{
	SDL3RendererBackend *b = backend();
	int i;

	if (!b) {
		return;
	}

	for (i = 0; i < BMPBUFF_MAX; i++) {
		if (b->bitmap[i]) {
			SDL_DestroySurface(b->bitmap[i]);
			b->bitmap[i] = NULL;
		}
	}

	if (b->texture) {
		SDL_DestroyTexture(b->texture);
		b->texture = NULL;
	}
	if (b->screen) {
		SDL_DestroySurface(b->screen);
		b->screen = NULL;
	}
	if (b->renderer) {
		SDL_DestroyRenderer(b->renderer);
		b->renderer = NULL;
	}
	if (b->window) {
		SDL_DestroyWindow(b->window);
		b->window = NULL;
	}
}

void Renderer_Shutdown(void)
{
	SDL3RendererBackend *b = backend();

	if (!b) {
		return;
	}

	if (b->initialised) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
	free(b);
	sdl3_backend = NULL;
}

void Renderer_SetWindowCaption(const char *title, const char *icon)
{
	SDL3RendererBackend *b = backend();
	(void)icon;

	if (b && b->window && title) {
		SDL_SetWindowTitle(b->window, title);
	}
}

void Renderer_Flip(void)
{
}

void Renderer_RefreshScreen(void)
{
	SDL3RendererBackend *b = backend();
	SDL_FRect dst;

	if (!b || !b->renderer || !b->texture || !b->screen) {
		return;
	}

	SDL_UpdateTexture(b->texture, NULL, b->screen->pixels, b->screen->pitch);
	dst = present_rect(b);

	SDL_SetRenderDrawColor(b->renderer, 0, 0, 0, 255);
	SDL_RenderClear(b->renderer);
	SDL_RenderTexture(b->renderer, b->texture, NULL, &dst);
	SDL_RenderPresent(b->renderer);
}

int LoadBitmap(char *fname, int bmpindex, int flag)
{
	SDL3RendererBackend *b = backend();
	SDL_Surface *tmp;
	SDL_Surface *converted;
	Uint32 color_key;

	if (!b || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) {
		return -1;
	}

	ReleaseBitmap(bmpindex);
	tmp = SDL_LoadBMP(fname);
	if (!tmp) {
		fprintf(stderr, "SDL3 LoadBMP failed for %s: %s\n", fname, SDL_GetError());
		return -1;
	}

	if (flag != 0) {
		color_key = SDL_MapSurfaceRGB(tmp, 16, 99, 62);
		SDL_SetSurfaceColorKey(tmp, true, color_key);
	}

	converted = SDL_ConvertSurface(tmp, SDL_PIXELFORMAT_ARGB8888);
	if (converted) {
		SDL_DestroySurface(tmp);
		b->bitmap[bmpindex] = converted;
	} else {
		b->bitmap[bmpindex] = tmp;
	}

	return 0;
}

void ReleaseBitmap(int bmpindex)
{
	SDL3RendererBackend *b = backend();

	if (!b || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) {
		return;
	}

	if (b->bitmap[bmpindex]) {
		SDL_DestroySurface(b->bitmap[bmpindex]);
		b->bitmap[bmpindex] = NULL;
	}
}

void BltRect(int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height)
{
	SDL3RendererBackend *b = backend();
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	if (!b || !b->screen || bmpindex < 0 || bmpindex >= BMPBUFF_MAX || !b->bitmap[bmpindex]) {
		return;
	}

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width;
	dstRect.h = height;

	SDL_BlitSurface(b->bitmap[bmpindex], &srcRect, b->screen, &dstRect);
}

void Blt(int bmpindex, int dstX, int dstY)
{
	SDL3RendererBackend *b = backend();
	SDL_Rect dstRect;

	if (!b || !b->screen || bmpindex < 0 || bmpindex >= BMPBUFF_MAX || !b->bitmap[bmpindex]) {
		return;
	}

	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = 0;
	dstRect.h = 0;

	SDL_BlitSurface(b->bitmap[bmpindex], NULL, b->screen, &dstRect);
}

void ClearSecondary(void)
{
	SDL3RendererBackend *b = backend();
	SDL_Rect rect;

	if (!b || !b->screen) {
		return;
	}

	rect.x = 0;
	rect.y = 0;
	rect.w = DISPLY_WIDTH;
	rect.h = DISPLY_HEIGHT;
	SDL_FillSurfaceRect(b->screen, &rect, SDL_MapSurfaceRGBA(b->screen, 0, 0, 0, 255));
}

void CreateSurface(int bmpindex, int size_x, int size_y)
{
	SDL3RendererBackend *b = backend();

	if (!b || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) {
		return;
	}

	ReleaseBitmap(bmpindex);
	b->bitmap[bmpindex] = SDL_CreateSurface(size_x, size_y, SDL_PIXELFORMAT_ARGB8888);
}

void SwapToSecondary(int bmpindex)
{
	(void)bmpindex;
}

void SaveBmp(int bmpindex, char *fn)
{
	SDL3RendererBackend *b = backend();

	if (!b || bmpindex < 0 || bmpindex >= BMPBUFF_MAX || !b->bitmap[bmpindex]) {
		return;
	}

	SDL_SaveBMP(b->bitmap[bmpindex], fn);
}

void Renderer_SetPalette(void* surface)
{
	(void)surface;
}

void SetGscreenPalette(void* surface)
{
	/* SDL3 desktop builds render through ARGB8888 surfaces; palette hardware is not used. */
	(void)surface;
}

uint32_t Renderer_GetTicks(void)
{
	return (uint32_t)SDL_GetTicks();
}

void Renderer_Delay(uint32_t ms)
{
	SDL_Delay(ms);
}

int Renderer_PollEvent(void* event)
{
	return SDL_PollEvent((SDL_Event*)event) ? 1 : 0;
}

void* Renderer_GetScreen(void)
{
	SDL3RendererBackend *b = backend();
	return b ? b->screen : NULL;
}

void* Renderer_LoadSurface(const char* path)
{
	return SDL_LoadBMP(path);
}

void Renderer_FreeSurface(void* surface)
{
	if (surface) {
		SDL_DestroySurface((SDL_Surface*)surface);
	}
}
