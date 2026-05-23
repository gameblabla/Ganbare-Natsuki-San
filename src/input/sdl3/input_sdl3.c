#include "../../define.h"
#include "../../input/input.h"
#include "../../general.h"
#include "../../extern.h"
#include "../../function.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Opaque SDL3 input backend. Gameplay code sees only input.h. */
typedef struct SDL3InputBackend {
	int key_eventPress[GP2X_BUTTON_MAX];
	int key_eventPress_old[GP2X_BUTTON_MAX];
	int key_eventPush[GP2X_BUTTON_MAX];
	int quit_requested;
	int pad_type;
} SDL3InputBackend;

static SDL3InputBackend *sdl3_input;

static int key_down(const bool *keys, SDL_Scancode scancode)
{
	return keys && keys[scancode] ? 1 : 0;
}

void Input_Init(void)
{
	if (sdl3_input) {
		return;
	}

	sdl3_input = (SDL3InputBackend*)calloc(1, sizeof(*sdl3_input));
}

void Input_Shutdown(void)
{
	free(sdl3_input);
	sdl3_input = NULL;
}

int Input_PollEvent(void)
{
	SDL_Event event;
	int saw_event = 0;

	if (!sdl3_input) {
		return 0;
	}

	while (SDL_PollEvent(&event)) {
		saw_event = 1;
		if (event.type == SDL_EVENT_QUIT) {
			sdl3_input->quit_requested = 1;
		}
	}

	return saw_event;
}

void Input_Update(void)
{
	SDL3InputBackend *in = sdl3_input;
	const bool *keys;
	int pad = 0;
	int i;

	if (!in) {
		return;
	}

	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(NULL);

	if (in->pad_type == 0) {
		if (key_down(keys, SDL_SCANCODE_RIGHT) || key_down(keys, SDL_SCANCODE_KP_6)) pad |= PAD_RIGHT;
		if (key_down(keys, SDL_SCANCODE_LEFT)  || key_down(keys, SDL_SCANCODE_KP_4)) pad |= PAD_LEFT;
		if (key_down(keys, SDL_SCANCODE_DOWN)  || key_down(keys, SDL_SCANCODE_KP_2)) pad |= PAD_DOWN;
		if (key_down(keys, SDL_SCANCODE_UP)    || key_down(keys, SDL_SCANCODE_KP_8)) pad |= PAD_UP;

		if (key_down(keys, SDL_SCANCODE_LCTRL) || key_down(keys, SDL_SCANCODE_X)) pad |= PAD_BUTTON1;
		if (key_down(keys, SDL_SCANCODE_LALT) || key_down(keys, SDL_SCANCODE_SPACE) || key_down(keys, SDL_SCANCODE_C)) pad |= PAD_BUTTON2;
		if (key_down(keys, SDL_SCANCODE_RETURN)) pad |= PAD_BUTTON3;
	} else {
		if (key_down(keys, SDL_SCANCODE_D) || key_down(keys, SDL_SCANCODE_KP_6)) pad |= PAD_RIGHT;
		if (key_down(keys, SDL_SCANCODE_A) || key_down(keys, SDL_SCANCODE_KP_4)) pad |= PAD_LEFT;
		if (key_down(keys, SDL_SCANCODE_S) || key_down(keys, SDL_SCANCODE_KP_2)) pad |= PAD_DOWN;
		if (key_down(keys, SDL_SCANCODE_W) || key_down(keys, SDL_SCANCODE_KP_8)) pad |= PAD_UP;

		if (key_down(keys, SDL_SCANCODE_BACKSLASH)) pad |= PAD_BUTTON1;
		if (key_down(keys, SDL_SCANCODE_RSHIFT)) pad |= PAD_BUTTON2;
		if (key_down(keys, SDL_SCANCODE_P)) pad |= PAD_BUTTON3;
	}

	if (key_down(keys, SDL_SCANCODE_F1)) pad |= PAD_BUTTON7;
	if (key_down(keys, SDL_SCANCODE_F2)) pad |= PAD_BUTTON8;

	for (i = 0; i < GP2X_BUTTON_MAX; i++) {
		in->key_eventPress[i] = 0;
	}

	if (pad & PAD_UP) in->key_eventPress[GP2X_BUTTON_UP] = 1;
	if (pad & PAD_DOWN) in->key_eventPress[GP2X_BUTTON_DOWN] = 1;
	if (pad & PAD_LEFT) in->key_eventPress[GP2X_BUTTON_LEFT] = 1;
	if (pad & PAD_RIGHT) in->key_eventPress[GP2X_BUTTON_RIGHT] = 1;
	if (pad & PAD_BUTTON1) in->key_eventPress[GP2X_BUTTON_A] = 1;
	if (pad & PAD_BUTTON2) in->key_eventPress[GP2X_BUTTON_X] = 1;
	if (pad & PAD_BUTTON3) in->key_eventPress[GP2X_BUTTON_Y] = 1;
	if (pad & PAD_BUTTON4) in->key_eventPress[GP2X_BUTTON_B] = 1;
	if (pad & PAD_BUTTON5) in->key_eventPress[GP2X_BUTTON_R] = 1;
	if (pad & PAD_BUTTON6) in->key_eventPress[GP2X_BUTTON_L] = 1;
	if (pad & PAD_BUTTON7) in->key_eventPress[GP2X_BUTTON_VOLDOWN] = 1;
	if (pad & PAD_BUTTON8) in->key_eventPress[GP2X_BUTTON_VOLUP] = 1;
	if (pad & PAD_BUTTON9) in->key_eventPress[GP2X_BUTTON_SELECT] = 1;
	if (pad & PAD_BUTTONA) in->key_eventPress[GP2X_BUTTON_START] = 1;
	if (pad & PAD_BUTTONB) in->key_eventPress[GP2X_BUTTON_CLICK] = 1;

	if (key_down(keys, SDL_SCANCODE_ESCAPE) || in->quit_requested) {
		in->key_eventPress[GP2X_BUTTON_EXIT] = 1;
	}

	for (i = 0; i < GP2X_BUTTON_MAX; i++) {
		if ((in->key_eventPress_old[i] == 0) && (in->key_eventPress[i] != 0)) {
			in->key_eventPush[i] = 1;
		} else {
			in->key_eventPush[i] = 0;
		}
		in->key_eventPress_old[i] = in->key_eventPress[i];
	}
}

int Input_IsPush(int keycode)
{
	if (!sdl3_input || keycode < 0 || keycode >= GP2X_BUTTON_MAX) {
		return 0;
	}
	return sdl3_input->key_eventPush[keycode] == 1;
}

int Input_IsPress(int keycode)
{
	if (!sdl3_input || keycode < 0 || keycode >= GP2X_BUTTON_MAX) {
		return 0;
	}
	return sdl3_input->key_eventPress[keycode] == 1;
}

int Input_IsPushOK(void)
{
	return Input_IsPush(GP2X_BUTTON_A);
}

int Input_IsPushCancel(void)
{
	return Input_IsPush(GP2X_BUTTON_X);
}

int Input_SystemKeys(void)
{
	if (Input_IsPress(GP2X_BUTTON_EXIT)) {
		g_scene = EN_SN_EXIT;
		return 0;
	}

	if (Input_IsPush(GP2X_BUTTON_VOLUP)) {
		gameflag[60] += 10;
		if (gameflag[60] > VOL_MAX) gameflag[60] = VOL_MAX;
		Set_Volume(gameflag[60]);
	}

	if (Input_IsPush(GP2X_BUTTON_VOLDOWN)) {
		gameflag[60] -= 10;
		if (gameflag[60] < 0) gameflag[60] = 0;
		Set_Volume(gameflag[60]);
	}

	return 1;
}
