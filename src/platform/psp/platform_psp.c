#include "../../define.h"
#include "../../platform.h"
#include "../../renderer/renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspkernel.h>

PSP_MODULE_INFO("Gannatsu Portable", 0, 1, 1);
PSP_HEAP_SIZE_KB(21*1024);
int iDoRun;

void Platform_InitEarly(void)
{
}

void Platform_Init(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	scePowerSetClockFrequency(333, 333, 166);
	Renderer_Init();
	Renderer_InitVideo(argc, argv);
}

void Platform_InitVideoPost(void)
{
}

void Platform_Shutdown(void)
{
	sceDisplayWaitVblankStart();
	sceKernelExitGame();
}

void Platform_ExitToLauncher(void)
{
}

void Platform_InitMain(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
}

void Platform_Exit(int code)
{
	(void)code;
	sceDisplayWaitVblankStart();
	sceKernelExitGame();
}
