#include "../../define.h"
#include "../../input/input.h"
#include "../../general.h"
#include <stddef.h>

static int key_eventPress[GP2X_BUTTON_MAX];
static int key_eventPress_old[GP2X_BUTTON_MAX];
static int key_eventPush[GP2X_BUTTON_MAX];

void Input_Init(void)
{
	int i;
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
		key_eventPress_old[i] = 0;
		key_eventPush[i] = 0;
	}
}

void Input_Shutdown(void)
{
}

int Input_PollEvent(void)
{
	return 0;
}

void Input_Update(void)
{
}

int Input_IsPush(int keycode)
{
	if (keycode >= 0 && keycode < GP2X_BUTTON_MAX)
	{
		return key_eventPush[keycode];
	}
	return 0;
}

int Input_IsPress(int keycode)
{
	if (keycode >= 0 && keycode < GP2X_BUTTON_MAX)
	{
		return key_eventPress[keycode];
	}
	return 0;
}

int Input_IsPushOK(void)
{
	return 0;
}

int Input_IsPushCancel(void)
{
	return 0;
}

int Input_SystemKeys(void)
{
	return 1;
}
