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

#include "scene.h"

void main_init( void );
void main_init_config( void );
void ExitProgram(void);

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
	gameflag[121] = 50;	
	gameflag[100] = 1;	
}
