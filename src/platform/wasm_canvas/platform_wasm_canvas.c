#include "../../define.h"
#include "../../platform.h"
#include "../../renderer/renderer.h"
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void Platform_InitEarly(void)
{
}

void Platform_InitMain(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
}

void Platform_Init(int argc, char *argv[])
{
    Renderer_Init();
    Renderer_InitVideo(argc, argv);
}

void Platform_InitVideoPost(void)
{
    Renderer_SetWindowCaption("Ganbare-Natsuki-san", "image/ico.bmp");
}

void Platform_Shutdown(void)
{
    Renderer_ShutdownVideo();
    Renderer_Shutdown();
}

void Platform_ExitToLauncher(void)
{
}

void Platform_Exit(int code)
{
#ifdef __EMSCRIPTEN__
    emscripten_force_exit(code);
#else
    exit(code);
#endif
}

void Platform_SaveCustom(const char *filename, const char *vmu_name, const char *description)
{
    (void)filename;
    (void)vmu_name;
    (void)description;
}

void Platform_LoadCustom(const char *filename, const char *vmu_name)
{
    (void)filename;
    (void)vmu_name;
}
