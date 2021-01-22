#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <unistd.h>

#include "define.h"
#include "extern.h"
#include "function.h"
#include "util_snd.h"

#include "scene.h"

#ifdef SCALING
SDL_Surface* real_screen;
#endif

#ifdef GP2X
#include <unistd.h>
#endif

#ifdef PSPUMODE
#include <pspkernel.h>
#endif 

#ifndef SDL_TRIPLEBUF
#define SDL_TRIPLEBUF SDL_DOUBLEBUF
#endif

void main_init( void );
void main_init_config( void );
void ExitProgram(void);

///----------------------------------------------------------------------------
/// PSP Related Stuff
///----------------------------------------------------------------------------
#ifdef PSPUMODE
PSP_MODULE_INFO("Gannatsu Portable", 0, 1, 1);
PSP_HEAP_SIZE_KB(21*1024);
int iDoRun;
#endif

int main(int argc, char *argv[])
{
	Uint32 flags;
	SDL_Surface* tmp;
	
	#ifdef MacOS
	char AppPath[1024];
	char AppPathw[1024];
	#endif
	
#ifdef PSPUMODE
	// PSP things
	//psp_setup_callbacks();
	scePowerSetClockFrequency(333, 333, 166);
#endif 

	/* 初期化 */
	#ifdef MacOS
	memset( &AppPath[0], '\0', sizeof( AppPath ) );
	memset( &AppPathw[0], '\0', sizeof( AppPath ) );
	#endif
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

#ifdef GP2X
#else
	#ifdef MacOS
		if ( argc > 0 )
		{
			strcpy( &AppPath[0], argv[0] );
			char_work = strrchr( &AppPath[0], '/' );
			strlen = char_work - &AppPath[0];
			strncpy( &AppPathw[0], &AppPath[0], strlen );
			chdir( &AppPathw[0] );
		}
	#else
	#endif
#endif	
	
#ifdef GP2X
	flags = SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWPALETTE;
#elif defined(PSPUMODE)
	flags = SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWPALETTE;
#elif defined(GCW0)
	flags = SDL_HWSURFACE | SDL_TRIPLEBUF;
#elif defined(DREAMCAST)
	flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
#elif defined(CLASSICMAC)
	flags = SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWPALETTE;
#else
	flags = SDL_SWSURFACE;
#endif

	SDL_ShowCursor(SDL_DISABLE);

#ifdef SCALING
	real_screen = SDL_SetVideoMode(FINAL_RESOLUTION_WIDTH, FINAL_RESOLUTION_HEIGHT, DEPTH, flags | SDL_NOFRAME);
	g_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, 0,0,0,0);
#else
	g_screen = SDL_SetVideoMode(DISPLY_WIDTH, DISPLY_HEIGHT, DEPTH, flags);
#endif

#if DEPTH != 8
	if (g_screen->format->BitsPerPixel == 8)
#endif
	{
		tmp = SDL_LoadBMP(IMAGE_PATH "color.bmp");	
		SetGscreenPalette( tmp );
		SDL_FreeSurface(tmp);
	}
	
	if (!g_screen)
	{
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_WM_SetCaption("Ganbare-Natsuki-san", "image/ico.bmp");


	FunctionInit( );
	main_init( );

	scenemanager( );
	
	ExitProgram( );
	return 0;
}


void main_init( void )
{
	g_scene = EN_SN_LOGO;

	main_init_config( );	

#ifndef NOSOUND

#ifdef MIDI_MUSIC
	soundLoadBuffer(EN_BGM_GAME01, (Uint8 *)BGM_SOUND_PATH "01.mid", -1);
	soundLoadBuffer(EN_BGM_GAME02, (Uint8 *)BGM_SOUND_PATH "02.mid", -1);
	soundLoadBuffer(EN_BGM_GAME03, (Uint8 *)BGM_SOUND_PATH "03.mid", -1);
	soundLoadBuffer(EN_BGM_GAME04, (Uint8 *)BGM_SOUND_PATH "04.mid", -1);
	soundLoadBuffer(EN_BGM_GAME05, (Uint8 *)BGM_SOUND_PATH "05.mid", -1);
	soundLoadBuffer(EN_BGM_GAME06, (Uint8 *)BGM_SOUND_PATH "06.mid", -1);
	soundLoadBuffer(EN_BGM_GAME07, (Uint8 *)BGM_SOUND_PATH "07.mid", -1);
#else
	soundLoadBuffer(EN_BGM_GAME01, (Uint8 *)BGM_SOUND_PATH "01.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME02, (Uint8 *)BGM_SOUND_PATH "02.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME03, (Uint8 *)BGM_SOUND_PATH "03.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME04, (Uint8 *)BGM_SOUND_PATH "04.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME05, (Uint8 *)BGM_SOUND_PATH "05.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME06, (Uint8 *)BGM_SOUND_PATH "06.ogg", -1);
	soundLoadBuffer(EN_BGM_GAME07, (Uint8 *)BGM_SOUND_PATH "07.ogg", -1);
#endif
	
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

void ExitProgram(void)
{
	char path_config[96];
#ifndef RELATIVE_PATH
	char path_folder[96];
#endif

#ifdef MINGW
	sprintf(path_config, "save/config");
	sprintf(path_folder, "save");
#elif defined(_TINSPIRE)
	sprintf(path_config, "./save/config.tns");
	sprintf(path_folder, "./save");
#elif defined(RELATIVE_PATH)
	sprintf(path_config, "config.dat");
#else		
	sprintf(path_config, "%s/.ganbare/config", getenv("HOME"));
	sprintf(path_folder, "%s/.ganbare", getenv("HOME"));
#endif
	
	soundStopBgm(0);
	soundStopSeAll();
	soundRelease();

	printf("SaveGameFlag(path_config); (ExitProgram)\n");
	SaveGameFlag(path_config);
	
	closePAD();
	SDL_Quit();

#ifdef GP2X
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif
#ifdef PSPUMODE
	sceDisplayWaitVblankStart();
	sceKernelExitGame(); 
#endif
}


void main_init_config( void )
{
	int i;
	char path_config[96];
#ifndef RELATIVE_PATH
	char path_folder[96];
#endif
	
#ifdef MINGW
	sprintf(path_config, "save/config");
	sprintf(path_folder, "save");
#elif defined(_TINSPIRE)
	sprintf(path_config, "./save/config.tns");
	sprintf(path_folder, "./save");
#elif defined(DREAMCAST)
	sprintf(path_config, "/cd/.ganbare/config");
	sprintf(path_folder, "/cd/.ganbare");
#elif defined(RELATIVE_PATH)
	sprintf(path_config, "config.dat");
#else		
	sprintf(path_config, "%s/.ganbare/config", getenv("HOME"));
	sprintf(path_folder, "%s/.ganbare", getenv("HOME"));
#endif

#ifdef MINGW
	mkdir(path_folder);
#elif defined(_TINSPIRE)
	mkdir(path_folder, 0755);
#elif defined(RELATIVE_PATH)

#else	
	mkdir(path_folder, 0755);
#endif
	
	if ( ! ( LoadGameFlag(path_config) == 0 ) )
	{
		ResetGameFlag();
#ifdef GP2X
		gameflag[0]=GP2X_BUTTON_UP;		//上	Up
		gameflag[1]=GP2X_BUTTON_DOWN;	//下	Down
		gameflag[2]=GP2X_BUTTON_LEFT;	//左	Left
		gameflag[3]=GP2X_BUTTON_RIGHT;	//右	Right
		gameflag[4]=GP2X_BUTTON_X;	//Ａ	Z
		gameflag[5]=GP2X_BUTTON_B;	//Ｂ	X
		gameflag[6]=GP2X_BUTTON_Y;	//Ｃ	C
		gameflag[7]=GP2X_BUTTON_A;	//Ｄ	S
		gameflag[8]=GP2X_BUTTON_L;	//Ｄ	S
		gameflag[9]=GP2X_BUTTON_R;	//Ｄ	S
		gameflag[10]=GP2X_BUTTON_START;	//Ｄ	S
		gameflag[11]=GP2X_BUTTON_SELECT;	//Ｄ	S
#else 
		gameflag[0]=GP2X_BUTTON_UP;		//上	Up
		gameflag[1]=GP2X_BUTTON_DOWN;	//下	Down
		gameflag[2]=GP2X_BUTTON_LEFT;	//左	Left
		gameflag[3]=GP2X_BUTTON_RIGHT;	//右	Right
		gameflag[4]=GP2X_BUTTON_A;	//Ａ	Z
		gameflag[5]=GP2X_BUTTON_X;	//Ｂ	X
		gameflag[6]=GP2X_BUTTON_Y;	//Ｃ	C
		gameflag[7]=GP2X_BUTTON_B;	//Ｄ	S
		gameflag[8]=GP2X_BUTTON_L;	//Ｄ	S
		gameflag[9]=GP2X_BUTTON_R;	//Ｄ	S
		gameflag[10]=GP2X_BUTTON_START;	//Ｄ	S
		gameflag[11]=GP2X_BUTTON_SELECT;	//Ｄ	S
#endif
		
		gameflag[60] = 128;	//音量

		gameflag[70] = 1;	//タイトル画面から来た
		gameflag[71] = 1;	//リトライを選択した


		gameflag[100] = 0;	//クリアフラグ
		
		gameflag[107] = 0;	//play time s
		gameflag[108] = 0;	//play time s
		gameflag[109] = 0;	//play time m
		gameflag[110] = 0;	//play time h
		gameflag[111] = 59;	//CLEAR time s
		gameflag[112] = 59;	//CLEAR time m
		gameflag[113] = 99;	//CLEAR time h
		
		
		gameflag[120] = 1;	//ステージセレクト
		gameflag[121] = 1;	//到達ステージ
		gameflag[122] = 0;	//キャラセレクト
		gameflag[123] = -1;	//リプレイ選択時のステージ
		gameflag[124] = 0;	//リプレイＡＬＬ、ＯＮＥ
		gameflag[125] = 0;	//リプレイjamp
		gameflag[126] = 0;	//裏面
		gameflag[127] = 0;	//トータルアタック
		
		gameflag[132] = 0;	//リプレイ
		gameflag[133] = 0;	//アンカータイプ

		gameflag[135] = 0;	//トータルアタック 時間					[130]>[135]
		gameflag[136] = 0;	//トータルアタック ミス				[131]>[136]
		gameflag[137] = 1595959;	//トータルアタック クリア時間	[132]>[137]
		gameflag[138] = 999;	//トータルアタック クリアミス	[133]>[138]
		
		for ( i = 0; i <= 99; i++ )
		{
			gameflag[200 + i] = 1105959;
		}
		for ( i = 0; i <= 99; i++ )
		{
			gameflag[300 + i] = 100;
		}
	}
	gameflag[121] = 50;	//到達ステージ
	gameflag[100] = 1;	//到達ステージ
}




