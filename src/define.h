#pragma once
#ifndef DEFINE_H
#define DEFINE_H

#include "paths.h"

#define GAMEFLAG_SIZE 513

#define WP 65536
#define WWP (WP*2)

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define CONFIGFILE_NAME "debug.txt"

#ifdef WIDESCREEN
#define DISPLY_WIDTH (400)
#define DISPLY_HEIGHT (240)
#else
#define DISPLY_WIDTH (320)
#define DISPLY_HEIGHT (240)
#endif

#define DISPLY_WIDTH_PLUS_HALF (DISPLY_WIDTH + DISPLY_WIDTH/2)
#define DISPLY_HEIGHT_PLUS_HALF (DISPLY_HEIGHT + DISPLY_HEIGHT/2)

#define DISPLY_WIDTH_DOUBLE (DISPLY_WIDTH*2)
#define DISPLY_HEIGHT_DOUBLE (DISPLY_HEIGHT*2)

#define DISPLY_WIDTH_HALF (DISPLY_WIDTH/2)
#define DISPLY_HEIGHT_HALF (DISPLY_HEIGHT/2)

#define DISPLY_WIDTH_WP (DISPLY_WIDTH*WP)
#define DISPLY_HEIGHT_WP (DISPLY_HEIGHT*WP)

#if defined(DREAMCAST)
	#define DEPTH 16
	#define FINAL_RESOLUTION_WIDTH 320
	#define FINAL_RESOLUTION_HEIGHT 240
#elif defined(GCW0)
	#define DEPTH 16
	#define FINAL_RESOLUTION_WIDTH 320
	#define FINAL_RESOLUTION_HEIGHT 240
#elif defined(PSPUMODE)
	#define DEPTH 16
#elif defined(GP2X)
	#define DEPTH 16
#elif defined(MINGW)
	#define DEPTH 8
	#define SCALING
	#define MIDI_MUSIC
#elif defined(TEST_8BPP)
	#define DEPTH 8
#elif defined(RS90)
	#define DEPTH 8
	#define FINAL_RESOLUTION_WIDTH 320
	#define FINAL_RESOLUTION_HEIGHT 240
#elif defined(CLASSICMAC)
	#define DEPTH 8
	#define RELATIVE_PATH
	#define SCALING
	#define MIDI_MUSIC
	#define SDLFILE
	
	#define FINAL_RESOLUTION_WIDTH 640
	#define FINAL_RESOLUTION_HEIGHT 480
#else
	/* Zero means it will use whatever is available */
	#define DEPTH 0
	// OpenGL scaling
	//#define OPENGL_SCALING
#endif

#ifndef FINAL_RESOLUTION_WIDTH
#define FINAL_RESOLUTION_WIDTH 0
#endif

#ifndef FINAL_RESOLUTION_HEIGHT
#define FINAL_RESOLUTION_HEIGHT 0
#endif

#ifndef RELATIVE_PATH
#include <sys/stat.h>
#endif


#define BMPBUFF_MAX (128)

#define GP2X_BUTTON_UP              (0)
#define GP2X_BUTTON_DOWN            (4)
#define GP2X_BUTTON_LEFT            (2)
#define GP2X_BUTTON_RIGHT           (6)
#define GP2X_BUTTON_UPLEFT          (1)
#define GP2X_BUTTON_UPRIGHT         (7)
#define GP2X_BUTTON_DOWNLEFT        (3)
#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_CLICK           (18)
#define GP2X_BUTTON_A               (12)
#define GP2X_BUTTON_B               (13)
#define GP2X_BUTTON_Y               (14)
#define GP2X_BUTTON_X               (15)
#define GP2X_BUTTON_L               (10)
#define GP2X_BUTTON_R               (11)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_VOLUP           (16)
#define GP2X_BUTTON_VOLDOWN         (17)
#define GP2X_BUTTON_EXIT            (19)
#define GP2X_BUTTON_MAX             (20)


#define VOL_MAX (128)

enum  
{
	EN_SN_EXIT = 0,
	EN_SN_TITLE,
	EN_SN_ACT,
	EN_SN_OPTION,
	EN_SN_ENDING,
	EN_SN_LOGO,
};

enum {
	EN_BGM_GAME01 = 0,
	EN_BGM_GAME02,
	EN_BGM_GAME03,
	EN_BGM_GAME04,
	EN_BGM_GAME05,
	EN_BGM_GAME06,
	EN_BGM_GAME07,
};
enum {
	EN_SE_ATK1   = 0,
	EN_SE_DAMEGE = 1,
	EN_SE_L1     = 2,
	EN_SE_LANDING= 3,
	EN_SE_MSG    = 4,
	EN_SE_SELECT = 5,
	EN_SE_SW     = 6,
	EN_SE_JUMP   = 7,
	EN_SE_PAWA   = 8,
};

#endif


