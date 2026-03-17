#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <stddef.h>

void Platform_Init(int argc, char *argv[]);
void Platform_Shutdown(void);
void Platform_ExitToLauncher(void);
void Platform_InitEarly(void);
void Platform_InitVideoPost(void);
void Platform_SetWindowCaption(void);
void Platform_InitMain(int argc, char *argv[]);
void Platform_Exit(int code);

void Platform_SaveCustom(const char *filename, const char *vmu_name, const char *description);
void Platform_LoadCustom(const char *filename, const char *vmu_name);

#endif
