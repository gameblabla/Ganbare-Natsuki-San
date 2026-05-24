#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "define.h"
#include "extern.h"
#include "function.h"
#include "audio/audio.h"
#include "platform.h"
#include "renderer/renderer.h"
#include "filesystem/filesystem.h"
#include "wasm/achievements.h"

#include "scene.h"

void main_init( void );
void main_init_config( void );
void ExitProgram(void);

#ifndef GNS_FRAME_STEPPED
int main(int argc, char *argv[])
{
	void* tmp;
	
	Platform_InitEarly();
	Platform_InitMain(argc, argv);
	Platform_Init(argc, argv);
	Platform_InitVideoPost();

#if DEPTH != 8
	if (Renderer_GetScreen() != NULL)
#endif
	{
		tmp = Renderer_LoadSurface(IMAGE_PATH "color.bmp");	
		if (tmp)
		{
			SetGscreenPalette( tmp );
			Renderer_FreeSurface(tmp);
		}
	}
	
	if (!Renderer_GetScreen())
	{
		fprintf(stderr, "Couldn't set video mode\n");
		return 1;
	}

	FunctionInit( );
	main_init( );

	scenemanager( );
	
	ExitProgram( );
	return 0;
}

#else
static int gns_started = 0;

__attribute__((export_name("gns_start")))
int gns_start(void)
{
	void* tmp;
	int argc = 0;
	char **argv = NULL;

	if (gns_started) return 0;
	gns_started = 1;

	Platform_InitEarly();
	Platform_InitMain(argc, argv);
	Platform_Init(argc, argv);
	Platform_InitVideoPost();

#if DEPTH != 8
	if (Renderer_GetScreen() != NULL)
#endif
	{
		tmp = Renderer_LoadSurface(IMAGE_PATH "color.bmp");	
		if (tmp)
		{
			SetGscreenPalette( tmp );
			Renderer_FreeSurface(tmp);
		}
	}
	
	if (!Renderer_GetScreen())
	{
		fprintf(stderr, "Couldn't set video mode\n");
		return 1;
	}

	FunctionInit( );
	main_init( );
	GNS_Achievement(GNS_ACH_STARTUP, 0);
	return 0;
}

__attribute__((export_name("gns_frame")))
int gns_frame(void)
{
	if (!gns_started)
	{
		if (gns_start() != 0) return 0;
	}

	/* In frame-stepped/browser builds the host owns the event loop.
	   Snapshot browser input before scene logic so menu/button pushes are
	   visible in the same exported frame. */
	Input_Update( );

	if (!scenemanager_frame( ))
	{
		ExitProgram( );
		return 0;
	}
	return 1;
}


#ifdef GNS_WASM_RAW
extern void gns_wasm_debug_start_level(int level);
extern void title_step_force_inactive( void );
extern void act_step_force_inactive( void );
extern void gns_input_clear(void);


__attribute__((export_name("gns_debug_unlock_levels")))
void gns_debug_unlock_levels(void)
{
	/* Browser-only debug helper.  This is intentionally separate from normal
	   save/config loading so ?debug=1 can test any level without requiring an
	   already-completed save file. */
	gameflag[100] = 1;
	gameflag[121] = 50;
	if (gameflag[120] < 1) gameflag[120] = 1;
	if (gameflag[120] > 50) gameflag[120] = 50;
	/* Debug access must never make the Newgrounds Time Attack board eligible. */
	gameflag[139] = 2;
}

__attribute__((export_name("gns_debug_start_level")))
int gns_debug_start_level(int level)
{
	if (level < 1) level = 1;
	if (level > 50) level = 50;

	if (!gns_started)
	{
		if (gns_start() != 0) return 0;
	}

	if (g_scene == EN_SN_TITLE)
	{
		title_step_force_inactive();
	}
	else if (g_scene == EN_SN_ACT)
	{
		act_step_force_inactive();
	}

	gns_input_clear();
	gns_wasm_debug_start_level(level);

	gameflag[120] = level;
	gameflag[121] = 50;
	gameflag[123] = -1;
	gameflag[124] = 0;
	gameflag[125] = 0;
	gameflag[127] = 0;
	gameflag[132] = 0;
	gameflag[139] = 2;
	gameflag[140] = 0;
	gameflag[70] = 1;
	gameflag[71] = 1;
	gameflag[40] = 4;

	g_scene = EN_SN_ACT;
	return level;
}
#endif

__attribute__((export_name("gns_shutdown")))
void gns_shutdown(void)
{
	if (!gns_started) return;
	soundStopBgm(0);
	soundStopSeAll();
	soundRelease();
	SaveGameFlag("config");
	closePAD();
	Platform_Shutdown();
	gns_started = 0;
}
#endif

void ExitProgram(void)
{
	char path_config[96];
	char path_folder[96];

	Filesystem_GetConfigPath(path_config, sizeof(path_config));
	Filesystem_GetSaveFolderPath(path_folder, sizeof(path_folder));
	
	soundStopBgm(0);
	soundStopSeAll();
	soundRelease();

	printf("SaveGameFlag(path_config); (ExitProgram)\n");
	SaveGameFlag(path_config);
	
	closePAD();
	Platform_Shutdown();

	Platform_Exit(0);
}

void main_init( void )
{
	g_scene = EN_SN_LOGO;

	main_init_config( );	

#ifndef NOSOUND
	soundLoadBuffer(EN_BGM_GAME01, (Uint8 *)BGM_SOUND_PATH "01.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME02, (Uint8 *)BGM_SOUND_PATH "02.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME03, (Uint8 *)BGM_SOUND_PATH "03.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME04, (Uint8 *)BGM_SOUND_PATH "04.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME05, (Uint8 *)BGM_SOUND_PATH "05.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME06, (Uint8 *)BGM_SOUND_PATH "06.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME07, (Uint8 *)BGM_SOUND_PATH "07.ogg", -1);
	
	soundLoadBufferSE(EN_SE_ATK1   , (Uint8 *)SE_SOUND_PATH "atk1.wav" );
	soundLoadBufferSE(EN_SE_DAMEGE , (Uint8 *)SE_SOUND_PATH "damage.wav" );
	soundLoadBufferSE(EN_SE_L1     , (Uint8 *)SE_SOUND_PATH "l1.wav" );
	soundLoadBufferSE(EN_SE_LANDING, (Uint8 *)SE_SOUND_PATH "landing.wav" );
	soundLoadBufferSE(EN_SE_MSG    , (Uint8 *)SE_SOUND_PATH "msg.wav" );
	soundLoadBufferSE(EN_SE_SELECT , (Uint8 *)SE_SOUND_PATH "select.wav" );
	soundLoadBufferSE(EN_SE_SW     , (Uint8 *)SE_SOUND_PATH "sw.wav" );
	soundLoadBufferSE(EN_SE_JUMP   , (Uint8 *)SE_SOUND_PATH "jump.wav" );
	soundLoadBufferSE(EN_SE_PAWA   , (Uint8 *)SE_SOUND_PATH "puwa.wav" );
#endif

	Set_Volume( gameflag[60] );
}

void main_init_config( void )
{
	int i;
	char path_config[96];
	char path_folder[96];
	
	Filesystem_GetConfigPath(path_config, sizeof(path_config));
	Filesystem_GetSaveFolderPath(path_folder, sizeof(path_folder));
	Filesystem_CreateSaveFolder(path_folder);
	
	if ( ! ( LoadGameFlag(path_config) == 0 ) )
	{
		ResetGameFlag();
		gameflag[0]=GP2X_BUTTON_UP;		
		gameflag[1]=GP2X_BUTTON_DOWN;	
		gameflag[2]=GP2X_BUTTON_LEFT;	
		gameflag[3]=GP2X_BUTTON_RIGHT;	
		gameflag[4]=GP2X_BUTTON_A;	
		gameflag[5]=GP2X_BUTTON_X;	
		gameflag[6]=GP2X_BUTTON_Y;	
		gameflag[7]=GP2X_BUTTON_B;	
		gameflag[8]=GP2X_BUTTON_L;	
		gameflag[9]=GP2X_BUTTON_R;	
		gameflag[10]=GP2X_BUTTON_START;	
		gameflag[11]=GP2X_BUTTON_SELECT;	
		
		gameflag[60] = 128;	
		gameflag[70] = 1;	
		gameflag[71] = 1;	
		gameflag[100] = 0;	
		
		gameflag[107] = 0;	
		gameflag[108] = 0;	
		gameflag[109] = 0;	
		gameflag[110] = 0;	
		gameflag[111] = 59;	
		gameflag[112] = 59;	
		gameflag[113] = 99;	
			
		gameflag[120] = 1;	
		gameflag[121] = 1;	
		gameflag[122] = 0;	
		gameflag[123] = -1;	
		gameflag[124] = 0;	
		gameflag[125] = 0;	
		gameflag[126] = 0;	
		gameflag[127] = 0;	
		
		gameflag[132] = 0;	
		gameflag[133] = 0;	
		gameflag[135] = 0;	
		gameflag[136] = 0;	
		gameflag[137] = 1595959;	
		gameflag[138] = 999;	
		
		for ( i = 0; i <= 99; i++ )
		{
			gameflag[200 + i] = 1105959;
		}
		for ( i = 0; i <= 99; i++ )
		{
			gameflag[300 + i] = 100;
		}
	}

#ifdef GNS_WASM_RAW
	/* The browser host exposes a fixed virtual GP2X-style pad.  Keep the
	   logical game bindings canonical even when an older localStorage config
	   was written by a previous experimental WASM build. */
	gameflag[0]=GP2X_BUTTON_UP;
	gameflag[1]=GP2X_BUTTON_DOWN;
	gameflag[2]=GP2X_BUTTON_LEFT;
	gameflag[3]=GP2X_BUTTON_RIGHT;
	gameflag[4]=GP2X_BUTTON_A;
	gameflag[5]=GP2X_BUTTON_X;
	gameflag[6]=GP2X_BUTTON_Y;
	gameflag[7]=GP2X_BUTTON_B;
	gameflag[8]=GP2X_BUTTON_L;
	gameflag[9]=GP2X_BUTTON_R;
	gameflag[10]=GP2X_BUTTON_START;
	gameflag[11]=GP2X_BUTTON_SELECT;

	/* Do not let stale browser config imply that stage 50 is available when
	   no valid work save is present.  The WASM filesystem returns a synthetic
	   stage-1 save but returns -1 for the missing/corrupt case, so this detects
	   a genuinely persisted work save without breaking the ACT fallback. */
	{
		Sint32 saved_gameflag2[GAMEFLAG_SIZE];
		char path_work_wasm[96];
		int valid_work_wasm;

		memcpy(saved_gameflag2, gameflag2, sizeof(gameflag2));
		Filesystem_GetWorkSavePath(path_work_wasm, sizeof(path_work_wasm));
		valid_work_wasm = (LoadGameFlag2(path_work_wasm) == 0);
		memcpy(gameflag2, saved_gameflag2, sizeof(gameflag2));

		if (!valid_work_wasm)
		{
			gameflag[120] = 1;
			gameflag[121] = 1;
			gameflag[123] = -1;
		}
		else
		{
			if (gameflag[120] < 1 || gameflag[120] > 50) gameflag[120] = 1;
			if (gameflag[121] < 1 || gameflag[121] > 50) gameflag[121] = gameflag[120];
		}
	}
#else
	gameflag[121] = 50;
#endif
	gameflag[100] = 1;	
}
