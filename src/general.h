#ifndef	_GENERAL_
#define _GENERAL_

#include <SDL.h>

enum
{
	PAD_DOWN = 0x01,
	PAD_LEFT = 0x02,
	PAD_RIGHT = 0x04,
	PAD_UP = 0x08,
	PAD_BUTTON1 = 0x10,
	PAD_BUTTON2 = 0x20,
	PAD_BUTTON3 = 0x40,
	PAD_BUTTON4 = 0x80,
	PAD_BUTTON5 = 0x100,
	PAD_BUTTON6 = 0x200,
	PAD_BUTTON7 = 0x400,
	PAD_BUTTON8 = 0x800,
	PAD_BUTTON9 = 0x1000,
	PAD_BUTTONA = 0x2000,
	PAD_BUTTONB = 0x4000,

	PAD_DIR = PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT,
	PAD_BUTTON = PAD_BUTTON1 | PAD_BUTTON2 | PAD_BUTTON3 | PAD_BUTTON4 | PAD_BUTTON5 | PAD_BUTTON6 | PAD_BUTTON7 | PAD_BUTTON8 | PAD_BUTTON9 | PAD_BUTTONA | PAD_BUTTONB,
	PAD_ALL = PAD_DIR | PAD_BUTTON,

	JOYSTICK_AXIS = 16384,

	REP_MIN = 2,
	REP_MAX = 30
};

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NONE
#define NONE 0
#endif

#ifndef ABS
#define ABS(a) ((a) < 0 ? -(a) : (a))
#endif

typedef union REG32{
	struct{
		Uint16 low;
		Uint16 high;
	}half;
	Uint32 full;
}REG32;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(PSPUMODE) 
#if !defined(DREAMCAST)
#include <memory.h>
#endif
#endif

#endif /* _GENERAL_ */
