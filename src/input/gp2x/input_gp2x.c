#include "../../define.h"
#include "../../input/input.h"
#include "../../general.h"
#include "../../extern.h"
#include "../../function.h"
#include <SDL.h>
#ifdef SDL_JOYSTICK
#include <SDL_joystick.h>
#endif

static int key_eventPress[GP2X_BUTTON_MAX];
static int key_eventPress_old[GP2X_BUTTON_MAX];
static int key_eventPush[GP2X_BUTTON_MAX];
static int pad_type;
#ifdef SDL_JOYSTICK
static int pads;
static int trgs;
static int reps;
static SDL_Joystick *joys;
#endif
static Uint8 *keys;
static SDL_Event sdl_event;

#ifdef SDL_JOYSTICK
#define JOYSTICK_AXIS 8000
#endif

void Input_Init(void)
{
	int i;
	
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
		key_eventPress_old[i] = 0;
		key_eventPush[i] = 0;
	}

#ifdef SDL_JOYSTICK
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0){
		return;
    }

	if(SDL_NumJoysticks() > 0)
	{
		joys = SDL_JoystickOpen(0);
		SDL_JoystickEventState(SDL_ENABLE);
	}
	else
	{
		joys = NULL;
	}

	pad_type = 0;
	trgs = 0;
	reps = 0;
#endif
}

void Input_Shutdown(void)
{
#ifdef SDL_JOYSTICK
	if(SDL_JoystickOpened(0))
		SDL_JoystickClose(joys);
#endif
}

int Input_PollEvent(void)
{
	return SDL_PollEvent(&sdl_event);
}

void Input_Update(void)
{
	int i;
	int pad = 0;

	#ifdef SDL_JOYSTICK
	int	btn1 = 0, btn2 = 0, btn3 = 0, btn4 = 0, btn5 = 0, btn6 = 0, btn7 = 0, btn8 = 0, btn9 = 0, btnA = 0;
	#endif

#ifdef SDL_JOYSTICK
	int x, y;
	if(joys){
		x = SDL_JoystickGetAxis(joys, 0);
		y = SDL_JoystickGetAxis(joys, 1);
	}
#endif

	keys = SDL_GetKeyState(NULL);

	if(pad_type == 0)
	{
		if(keys[SDLK_RIGHT] == SDL_PRESSED || keys[SDLK_KP6] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		 || x > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_RIGHT;
		}
		if(keys[SDLK_LEFT] == SDL_PRESSED || keys[SDLK_KP4] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| x < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_LEFT;
		}
		if(keys[SDLK_DOWN] == SDL_PRESSED || keys[SDLK_KP2] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| y > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_DOWN;
		}
		if(keys[SDLK_UP] == SDL_PRESSED || keys[SDLK_KP8] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| y < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_UP;
		}
	}
	else if(pad_type == 1)
	{
		if(keys[SDLK_d] == SDL_PRESSED || keys[SDLK_KP6] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| x > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_RIGHT;
		}
		if(keys[SDLK_a] == SDL_PRESSED || keys[SDLK_KP4] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| x < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_LEFT;
		}
		if(keys[SDLK_s] == SDL_PRESSED || keys[SDLK_KP2] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| y > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_DOWN;
		}
		if(keys[SDLK_w] == SDL_PRESSED || keys[SDLK_KP8] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| y < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_UP;
		}
	}

#ifdef SDL_JOYSTICK
	if(joys){
		btn1 = SDL_JoystickGetButton(joys, 0);
		btn2 = SDL_JoystickGetButton(joys, 1);
		btn3 = SDL_JoystickGetButton(joys, 2);
		btn4 = SDL_JoystickGetButton(joys, 3);
		btn5 = SDL_JoystickGetButton(joys, 4);
		btn6 = SDL_JoystickGetButton(joys, 5);
		btn7 = SDL_JoystickGetButton(joys, 6);
		btn8 = SDL_JoystickGetButton(joys, 7);
		btn9 = SDL_JoystickGetButton(joys, 8);
		btnA = SDL_JoystickGetButton(joys, 9);
	}
#endif	
	if(pad_type == 0)
	{
		if(keys[SDLK_LCTRL] == SDL_PRESSED
		|| keys[SDLK_x] == SDL_PRESSED
#ifdef SDL_JOYSTICK
		 || btn1
#endif
		 ){
			pad |= PAD_BUTTON1;
		}
		if(keys[SDLK_LALT] == SDL_PRESSED
		|| keys[SDLK_SPACE] == SDL_PRESSED
		|| keys[SDLK_c] == SDL_PRESSED
#ifdef SDL_JOYSTICK
		 || btn2
#endif
		){
			pad |= PAD_BUTTON2;
		}
		if(keys[SDLK_RETURN] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| btn3
		#endif
		){
			pad |= PAD_BUTTON3;
		}
	}
	else if(pad_type == 1)
	{
		if(keys[SDLK_BACKSLASH] == SDL_PRESSED
#ifdef SDL_JOYSTICK
		 || btn1
#endif
		){
			pad |= PAD_BUTTON1;
		}
		if(keys[SDLK_RSHIFT] == SDL_PRESSED
#ifdef SDL_JOYSTICK
		 || btn2
#endif
		){
			pad |= PAD_BUTTON2;
		}
		if(keys[SDLK_p] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| btn3
		#endif
		){
			pad |= PAD_BUTTON3;
		}
	}

	if(keys[SDLK_F1] == SDL_PRESSED
		#ifdef SDL_JOYSTICK
		|| btn7
		#endif
	){
		pad |= PAD_BUTTON7;
	}
	if(keys[SDLK_F2] == SDL_PRESSED
#ifdef SDL_JOYSTICK
		 || btn8
#endif
	){
		pad |= PAD_BUTTON8;
	}
	
#ifdef SDL_JOYSTICK
	if(btn4){
		pad |= PAD_BUTTON4;
	}
	if(btn5){
		pad |= PAD_BUTTON5;
	}
	if(btn6){
		pad |= PAD_BUTTON6;
	}
	if(btn9){
		pad |= PAD_BUTTON9;
	}
	if(btnA){
		pad |= PAD_BUTTONA;
	}
#endif

	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
	}
	
	if ( pad & PAD_UP )
	{
		key_eventPress[GP2X_BUTTON_UP] = 1;
	}
	if ( pad & PAD_DOWN )
	{
		key_eventPress[GP2X_BUTTON_DOWN] = 1;
	}
	if ( pad & PAD_LEFT )
	{
		key_eventPress[GP2X_BUTTON_LEFT] = 1;
	}
	if ( pad & PAD_RIGHT )
	{
		key_eventPress[GP2X_BUTTON_RIGHT] = 1;
	}
	if ( pad & PAD_BUTTON1 )
	{
		key_eventPress[GP2X_BUTTON_A] = 1;
	}
	if ( pad & PAD_BUTTON2 )
	{
		key_eventPress[GP2X_BUTTON_X] = 1;
	}
	if ( pad & PAD_BUTTON3 )
	{
		key_eventPress[GP2X_BUTTON_Y] = 1;
	}
	if ( pad & PAD_BUTTON4 )
	{
		key_eventPress[GP2X_BUTTON_B] = 1;
	}
	if ( pad & PAD_BUTTON5 )
	{
		key_eventPress[GP2X_BUTTON_R] = 1;
	}
	if ( pad & PAD_BUTTON6 )
	{
		key_eventPress[GP2X_BUTTON_L] = 1;
	}
	if ( pad & PAD_BUTTON7 )
	{
		key_eventPress[GP2X_BUTTON_VOLDOWN] = 1;
	}
	if ( pad & PAD_BUTTON8 )
	{
		key_eventPress[GP2X_BUTTON_VOLUP] = 1;
	}
	if ( pad & PAD_BUTTON9 )
	{
		key_eventPress[GP2X_BUTTON_SELECT] = 1;
	}
	if ( pad & PAD_BUTTONA )
	{
		key_eventPress[GP2X_BUTTON_START] = 1;
	}
	if ( pad & PAD_BUTTONB )
	{
		key_eventPress[GP2X_BUTTON_CLICK] = 1;
	}
	
	if ( keys[SDLK_ESCAPE] )
	{
		key_eventPress[GP2X_BUTTON_EXIT] = 1;
	}
	
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		if ( ( key_eventPress_old[i] == 0 ) && ( key_eventPress[i] != 0 ) )
		{
			key_eventPush[i] = 1;
		}
		else 
		{
			key_eventPush[i] = 0;
		}
		key_eventPress_old[i] = key_eventPress[i];
	}
}

int Input_IsPush( int keycode )
{
	int rc = 0;
	
	if ( key_eventPush[keycode] == 1 )
	{
		rc = 1;
	}

	return( rc );
}

int Input_IsPress( int keycode )
{
	int rc = 0;
	
	if ( key_eventPress[keycode] == 1 )
	{
		rc = 1;
	}

	return( rc );
}

int Input_IsPushOK( void )
{
	int rc = 0;
	if ( key_eventPush[GP2X_BUTTON_A] == 1 )
	{
		rc = 1;
	}
	return( rc );
}

int Input_IsPushCancel( void )
{
	int rc = 0;
	if ( key_eventPush[GP2X_BUTTON_X] == 1 )
	{
		rc = 1;
	}
	return( rc );
}

int Input_SystemKeys(void)
{
	int rc;
	
	rc = 1;
	if ( ( Input_IsPress( GP2X_BUTTON_START ) ) && ( Input_IsPress( GP2X_BUTTON_L ) ) && ( Input_IsPress( GP2X_BUTTON_R ) ) )
	{
		rc = 0;
		g_scene = EN_SN_EXIT;
	}
	if ( Input_IsPush( GP2X_BUTTON_VOLUP ) )
	{
		gameflag[60] = gameflag[60] + 10;
		if ( gameflag[60] > VOL_MAX )
		{
			gameflag[60] = VOL_MAX;
		}
		Set_Volume( gameflag[60] );
	}
	if ( Input_IsPush( GP2X_BUTTON_VOLDOWN ) )
	{
		gameflag[60] = gameflag[60] - 10;
		if ( gameflag[60] < 0 )
		{
			gameflag[60] = 0;
		}
		Set_Volume( gameflag[60] );
	}

	return rc;
}
