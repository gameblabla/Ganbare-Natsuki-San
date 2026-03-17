#include "../../define.h"
#include "../../renderer/renderer.h"
#include "../../general.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static uint32_t tick_count = 0;

void Renderer_Init(void)
{
}

void Renderer_InitVideo(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
}

void Renderer_ShutdownVideo(void)
{
}

void Renderer_SetWindowCaption(const char *title, const char *icon)
{
	(void)title;
	(void)icon;
}

void Renderer_Shutdown(void)
{
}

void Renderer_Flip(void)
{
}

void Renderer_RefreshScreen(void)
{
}

int LoadBitmap(char *fname, int bmpindex, int flag)
{
	(void)fname;
	(void)bmpindex;
	(void)flag;
	return 0;
}

void ReleaseBitmap(int bmpindex)
{
	(void)bmpindex;
}

void Blt(int bmpindex, int dstX, int dstY)
{
	(void)bmpindex;
	(void)dstX;
	(void)dstY;
}

void BltRect(int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height)
{
	(void)bmpindex;
	(void)dstX;
	(void)dstY;
	(void)srcX;
	(void)srcY;
	(void)width;
	(void)height;
}

void ClearSecondary(void)
{
}

void CreateSurface(int bmpindex, int size_x, int size_y)
{
	(void)bmpindex;
	(void)size_x;
	(void)size_y;
}

void SwapToSecondary(int bmpindex)
{
	(void)bmpindex;
}

void SaveBmp(int bmpindex, char *fn)
{
	(void)bmpindex;
	(void)fn;
}

void Renderer_SetPalette(void* surface)
{
	(void)surface;
}

uint32_t Renderer_GetTicks(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint32_t)((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000));
}

void Renderer_Delay(uint32_t ms)
{
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

int Renderer_PollEvent(void* event)
{
	(void)event;
	return 0;
}

void* Renderer_GetScreen(void)
{
	return (void*)1;
}

void* Renderer_LoadSurface(const char* path)
{
	(void)path;
	return NULL;
}

void Renderer_FreeSurface(void* surface)
{
	(void)surface;
}

void SetGscreenPalette(void* surface)
{
	(void)surface;
}
