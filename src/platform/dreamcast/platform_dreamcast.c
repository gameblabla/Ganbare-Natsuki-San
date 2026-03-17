#include "../../define.h"
#include "../../platform.h"
#include "../../renderer/renderer.h"
#include "../../filesystem/dreamcast/vmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void Platform_InitEarly(void)
{
}

void Platform_Init(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	Renderer_Init();
	Renderer_InitVideo(argc, argv);
}

void Platform_InitVideoPost(void)
{
}

void Platform_Shutdown(void)
{
	Renderer_ShutdownVideo();
	Renderer_Shutdown();
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
}

void Platform_SaveCustom(const char *filename, const char *vmu_name, const char *description)
{
	DC_SaveVMU((char*)filename, (char*)vmu_name, (char*)description);
}

void Platform_LoadCustom(const char *filename, const char *vmu_name)
{
	DC_LoadVMU((char*)filename, (char*)vmu_name);
}
