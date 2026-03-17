#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void Renderer_Init(void);
void Renderer_Shutdown(void);
void Renderer_InitVideo(int argc, char *argv[]);
void Renderer_ShutdownVideo(void);

void Renderer_SetWindowCaption(const char *title, const char *icon);
void Renderer_Flip(void);
void Renderer_RefreshScreen(void);

int LoadBitmap(char *fname, int bmpindex, int flag);
void ReleaseBitmap(int bmpindex);
void Blt(int bmpindex, int dstX, int dstY);
void BltRect(int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height);
void ClearSecondary(void);
void CreateSurface(int bmpindex, int size_x, int size_y);
void SwapToSecondary(int bmpindex);
void SaveBmp(int bmpindex, char *fn);

void Renderer_SetPalette(void* surface);

void SetGscreenPalette(void* surface);

uint32_t Renderer_GetTicks(void);
void Renderer_Delay(uint32_t ms);
int Renderer_PollEvent(void* event);

void* Renderer_GetScreen(void);

void* Renderer_LoadSurface(const char* path);
void Renderer_FreeSurface(void* surface);

#ifdef __cplusplus
}
#endif

#endif
