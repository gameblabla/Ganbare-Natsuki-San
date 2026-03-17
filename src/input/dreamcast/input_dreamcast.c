#include "../../define.h"
#include "../../input/input.h"
#include "../../general.h"
#include <SDL.h>
#include <kos.h>

static int key_eventPress[GP2X_BUTTON_MAX];
static int key_eventPress_old[GP2X_BUTTON_MAX];
static int key_eventPush[GP2X_BUTTON_MAX];

static maple_device_t *cont;
static maple_device_t *kbd;
static kbd_state_t* first_kbd_state; 

void Input_Init(void)
{
	int i;
	
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
		key_eventPress_old[i] = 0;
		key_eventPush[i] = 0;
	}

	cont = NULL;
	kbd = NULL;
}

void Input_Shutdown(void)
{
}

void Input_Update(void)
{
	int i;
	int pad = 0;
	cont_state_t *state;

	if (!cont)
	{
		for(i=0;i<4;i++)
		{
			cont = maple_enum_type(i, MAPLE_FUNC_CONTROLLER);
			if (cont) break;
		}
	}
	if (!kbd)
	{
		for(i=0;i<4;i++)
		{
			kbd = maple_enum_type(i, MAPLE_FUNC_KEYBOARD);
			if (kbd) break;
		}
	}
	
	if(cont)
	{
		state = (cont_state_t *)maple_dev_status(cont);
		if (state->buttons & CONT_START)
            pad |= PAD_BUTTON3;
		if (state->buttons & CONT_Y)
			pad |= PAD_BUTTON1;
		if (state->buttons & CONT_X)
			pad |= PAD_BUTTON2;
		if (state->buttons & CONT_A) 
            pad |= PAD_BUTTON1;
		if (state->buttons & CONT_B) 
			pad |= PAD_BUTTON2;
           
		if (state->buttons & CONT_DPAD_UP || state->joyy < -64) 
			pad |= PAD_UP;
		else if (state->buttons & CONT_DPAD_DOWN || state->joyy > 64) 
           pad |= PAD_DOWN;
		if (state->buttons & CONT_DPAD_LEFT || state->joyx < -64) 
           pad |= PAD_LEFT;
		else if (state->buttons & CONT_DPAD_RIGHT || state->joyx > 64) 
           pad |= PAD_RIGHT;
	}
	
	if (kbd)
	{
		first_kbd_state = (kbd_state_t *) maple_dev_status(kbd);
	
		if (first_kbd_state->matrix[KBD_KEY_UP]) pad |= PAD_UP;
		else if (first_kbd_state->matrix[KBD_KEY_DOWN]) pad |= PAD_DOWN;
		
		if (first_kbd_state->matrix[KBD_KEY_LEFT]) pad |= PAD_LEFT;
		else if (first_kbd_state->matrix[KBD_KEY_RIGHT]) pad |= PAD_RIGHT;
		
		if (first_kbd_state->matrix[KBD_KEY_SPACE] || first_kbd_state->matrix[KBD_KEY_X]) pad |= PAD_BUTTON1;
		if (first_kbd_state->matrix[KBD_KEY_Z] || first_kbd_state->matrix[KBD_KEY_C]) pad |= PAD_BUTTON2;
		
		if (first_kbd_state->matrix[KBD_KEY_V] || first_kbd_state->matrix[KBD_KEY_ESCAPE]) pad |= PAD_BUTTON3;
	}
	
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
