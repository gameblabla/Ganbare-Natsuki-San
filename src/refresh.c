#include "define.h"
#include "renderer/renderer.h"

void RefreshScreen(void* tmp)
{
	(void)tmp;
	Renderer_RefreshScreen();
}
