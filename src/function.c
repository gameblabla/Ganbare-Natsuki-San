#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <SDL.h>
#ifndef NOSOUND
#include <SDL_mixer.h>
#endif
#include "define.h"
#include "function.h"
#include "extern.h"
#include "util_snd.h"
#include "general.h"
#include "dconv.h"

enum Joystick_SDL SDLJoyBtn;

void FunctionInit( void );
void ResetGameFlag( void );
void ResetGameFlag2( void );
int LoadGameFlag( char *fn );
int SaveGameFlag( char *fn );
int LoadGameFlag2( char *fn );
int SaveGameFlag2( char *fn );
int SaveFile( char *fn, int32_t *buff, int32_t size );
int LoadFile( char *fn, int32_t *buff, int32_t size );
int32_t GetConfig( char* fn, char* cParam );
int32_t LogFileWrite( char* fn, char* cParam );

int32_t LoadBitmap( char *fname , int bmpindex, int flag );
void ReleaseBitmap( int bmpindex );
void BltRect(int bmpindex, int srcX, int srcY, int dstX, int dstY, int width, int height);
void Blt( int bmpindex, int dstX, int dstY );
void ClearSecondary( void );
void BltNumericImage( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height );
void BltNumericImage2( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height );
int32_t get2keta( int32_t val, int32_t st );
void SetGscreenPalette( SDL_Surface *surface );
void SetPalette(int getbmpindex, int setbmpindex);
void CreateSurface( int bmpindex, int size_x, int size_y  );
void SwapToSecondary( int bmpindex );
void SaveBmp( int bmpindex, char *fn );
void drawGRPline(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Uint32 color);
void pointSDLsurface( int32_t px, int32_t py, Uint32 color);
void putSDLpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int IsPushKey( int keycode );
int IsPressKey( int keycode );
void KeyInit( void );
void KeyInput( void );
int initPAD(void);
void closePAD(void);
int getPAD(void);
int IsPushOKKey( void );
int IsPushCancelKey( void );

void FPSWait( void );
int system_keys( void );
int Set_Volume( int vol );

void soundInitBuffer(void);
void soundRelease(void);
void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop);

int32_t funcSin( int32_t rdo );
int32_t funcCos( int32_t rdo );
int32_t funcTan2( int32_t posX, int32_t posY );

// キー取得用
static int key_eventPress[GP2X_BUTTON_MAX];
static int key_eventPress_old[GP2X_BUTTON_MAX];
static int key_eventPush[GP2X_BUTTON_MAX];
int	pad_type;
int	pads;
int	trgs;
int	reps;

#ifdef SDL_JOYSTICK
SDL_Joystick *joys;
#endif
Uint8 *keys;

//static int pads_old;
// 画像表示用
static SDL_Surface* bitmap[BMPBUFF_MAX];
static SDL_Surface* g_surface_bakup;
// 定時処理用
static int32_t prvTickCount;
static int32_t nowTick;
//static int frame;

#ifdef GP2X
static const int INTERVAL_BASE = 16;
#else
static const int INTERVAL_BASE = 16;
#endif

SDL_Event event;

// 音楽再生
static int32_t sound_vol;

void FunctionInit( void )
{
	int i;
	
	prvTickCount = 0;
	
	for ( i = 0; i < BMPBUFF_MAX; i++ )
	{
		bitmap[i] = NULL;
	}
	KeyInit( );
	soundInitBuffer();

}

void ResetGameFlag( void )
{
	memset( &gameflag[0], 0, sizeof( gameflag ) );
}
int LoadGameFlag( char *fn )
{
	int rc = 0;
	FILE *fp;

#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "rb" ) ) == NULL )
#elif defined(DREAMCAST)
	char buf[192];
	snprintf(buf, sizeof(buf), "/cd/%s", fn);
	if ( ( fp = fopen( buf, "rb" ) ) == NULL )
#else	
	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
#endif
	{
		printf("file open error!!\n");
		rc = -1;	
	}
	else 
	{
		fread( &gameflag[0], 1, sizeof( gameflag ), fp );
		fclose( fp );
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
int SaveGameFlag( char *fn )
{
	int rc = 0;
	FILE *fp;

#ifdef _TINSPIRE
	char buf[255];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	
	if (strcmp(fn,"save/config")==0)
	{
		if ( ( fp = fopen( "save/config.tns", "wb" ) ) == NULL )
		{
			printf("file open error!!\n");
			rc = -1;
		}
		else 
		{
			fwrite( &gameflag[0], 1, sizeof( gameflag ), fp );
			fclose( fp );
		}
	}
	else
	{
		if ( ( fp = fopen( buf, "wb" ) ) == NULL )
		{
			printf("file open error!!\n");
			rc = -1;
		}
		else 
		{
			fwrite( &gameflag[0], 1, sizeof( gameflag ), fp );
			fclose( fp );
		}
	}
	
#elif defined(DREAMCAST)


	char buf[255];
	snprintf(buf, sizeof(buf), "/cd/%s", fn);
	
	if (strcmp(fn,"/ram/config")==0)
	{
		if ( ( fp = fopen( "/ram/config", "wb" ) ) == NULL )
		{
			printf("file open error!!\n");
			rc = -1;
		}
		else 
		{
			fwrite( &gameflag[0], 1, sizeof( gameflag ), fp );
			fclose( fp );
		}
	}
	else
	{
		if ( ( fp = fopen( buf, "wb" ) ) == NULL )
		{
			printf("buffer % s \n", buf);
			printf("file open error!!\n");
			rc = -1;
		}
		else 
		{
			fwrite( &gameflag[0], 1, sizeof( gameflag ), fp );
			fclose( fp );
		}
	}
	
	
#else
	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
	{
		printf("file open error!!\n");
		rc = -1;
	}
	else 
	{
		fwrite( &gameflag[0], 1, sizeof( gameflag ), fp );
		fclose( fp );
#ifdef GP2X
		sync( );
#endif
	}
	
#endif
	
	return ( rc );
}

void ResetGameFlag2( void )
{
	memset( &gameflag2[0], 0, sizeof( gameflag ) );
}

int LoadGameFlag2( char *fn )
{
	int rc = 0;
	FILE *fp;	
	
	printf("Fopen : %s\n", fn);
	
#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "rb" ) ) == NULL )
#elif defined(DREAMCAST)
	char buf[192];
	snprintf(buf, sizeof(buf), "/cd/%s", fn);
	if ( ( fp = fopen( buf, "rb" ) ) == NULL )
#else
	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
#endif
	{
		printf("file open error!!\n");
		rc = -1;	
	}
	else 
	{
		printf("Fopen %s was a sucess\n", fn);
		fread( &gameflag2[0], 1, sizeof( gameflag ), fp ); 
		fclose( fp );	
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}

int SaveGameFlag2( char *fn )
{
	int rc = 0;
	FILE *fp;	

#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "wb" ) ) == NULL )
#elif defined(DREAMCAST)
	char buf[192];
	snprintf(buf, sizeof(buf), "/cd/%s", fn);
	if ( ( fp = fopen( buf, "wb" ) ) == NULL )
#else
	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
#endif
	{
		printf("file open error!!\n");
		rc = -1;
	}
	else 
	{
		fwrite( &gameflag2[0], 1, sizeof( gameflag ), fp ); 
		fclose( fp );
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
int SaveFile( char *fn, int32_t *buff, int32_t size )
{
	int rc = 0;
	FILE *fp;	

#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "wb" ) ) == NULL )
#elif defined(DREAMCAST)
	char buf[192];
	snprintf(buf, sizeof(buf), "/cd/%s", fn);
	if ( ( fp = fopen( buf, "wb" ) ) == NULL )
#else
	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
#endif
	{
		printf("file open error!!\n");
		rc = -1;
	}
	else 
	{
		fwrite( buff, 1, size, fp ); 
		fclose( fp );
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
int LoadFile( char *fn, int32_t *buff, int32_t size )
{
	int rc = 0;
	FILE *fp;	
	
#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "rb" ) ) == NULL )
#elif defined(DREAMCAST)
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "rb" ) ) == NULL )
#else
	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
#endif
	{
		printf("file open error!!\n");
		rc = -1;
	}
	else 
	{
		fread( buff, 1, size, fp ); 
		fclose( fp );
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}

int32_t GetConfig( char* fn, char* cParam )
{
	FILE *fp;	
	char *sp;
	char s[256];
	char s2[256];
	memset( s, '\0', sizeof( s ) );
	int32_t rc;
	
	rc = 0;

#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "r" ) ) == NULL )
#elif defined(DREAMCAST)
	char buf[192];
	snprintf(buf, sizeof(buf), "/cd/%s", fn);
	if ( ( fp = fopen( buf, "r" ) ) == NULL )
#else
	if ( ( fp = fopen( fn, "r" ) ) == NULL )
#endif
	{
		rc = 0;
	}
	else 
	{
		while ( fgets( s, 256, fp ) != NULL) 
		{
			if ( strstr( s, cParam ) != NULL )
			{
				sp = NULL;
				sp = strstr( s, "=" );
				if ( sp != NULL )
				{
					sp++;
					memset( s2, '\0', sizeof( s ) );
					if ( ! ( ( *sp >= '0' && *sp <= '9' ) || ( *sp =='-' ) ) )
					{
						return 0;	
					}
					while ( *sp >= '0' && *sp <= '9' )
					{
						rc = rc * 10 + ( *sp - '0' );
						sp++;
					}
				}
				break;
			}
			memset( s, '\0', sizeof( s ) );
		}
#ifdef GP2X
		sync( );
#endif
	}
	return( rc );
}

int32_t LogFileWrite( char* fn, char* cParam )
{
	FILE *fp;	
	int32_t rc;
	
	rc = 0;

#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "w" ) ) == NULL )
#elif defined(DREAMCAST)
	char buf[192];
	snprintf(buf, sizeof(buf), "/cd/%s", fn);
	if ( ( fp = fopen( buf, "w" ) ) == NULL )
#else
	if ( ( fp = fopen( fn, "w" ) ) == NULL )
#endif
	{
		rc = 0;	
	}
	else 
	{
		fputs( cParam, fp);
#ifdef GP2X
		sync( );
#endif
	}
	return( rc );
}


int32_t LoadBitmap( char *fname , int bmpindex, int flag )
{
	int32_t rc;
	SDL_Surface* tmp;
	char filename[1024];

	memset( &filename[0], '\0', sizeof( filename ) );
	rc = 0;
	
	ReleaseBitmap( bmpindex );

#ifdef _TINSPIRE
	char buf[128];
	snprintf(buf, sizeof(buf), "%s.tns", fname);
	tmp = SDL_LoadBMP( buf );
#elif defined(DREAMCAST)
	char buf[128];
	snprintf(buf, sizeof(buf), "/cd/%s", fname);
	tmp = SDL_LoadBMP( buf );
#else
	tmp = SDL_LoadBMP( fname );
#endif
	
	if (tmp)
	{
		if (flag != 0) SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(tmp->format,16,99,62) );
		bitmap[bmpindex] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}
	else 
	{
		rc = -1;
	}


	return ( rc );
}

void ReleaseBitmap( int bmpindex )
{
	if ( bitmap[bmpindex] != NULL )
	{
		SDL_FreeSurface(bitmap[bmpindex]);
		bitmap[bmpindex] = NULL;
	}
}

void BltRect( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height)
{
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width;
	dstRect.h = height;

	if(bitmap[bmpindex])
	{
		SDL_BlitSurface(bitmap[bmpindex], &srcRect, g_screen, &dstRect);
	}
}

void Blt( int bmpindex, int dstX, int dstY )
{
	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = 0;
	dstRect.h = 0;

	if(bitmap[bmpindex])
	{
		SDL_BlitSurface(bitmap[bmpindex], NULL, g_screen, &dstRect);
	}
}
void SetGscreenPalette( SDL_Surface *surface )
{
    Uint8 bpp;
	SDL_Palette *pal;

    if(surface)
    {
	    bpp = surface->format->BytesPerPixel;
		if(bpp <= 1)
		{
			pal = surface->format->palette;
			if(pal){
				#ifdef SCALING
				extern SDL_Surface* real_screen;
				SDL_SetPalette(real_screen, SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
				SDL_SetPalette(g_screen, SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
				#else
				SDL_SetPalette(g_screen, SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
				#endif
			}
		}
	}
}
void SetPalette(int getbmpindex, int setbmpindex)
{
	SDL_Surface *surface;
    Uint8 bpp;
	SDL_Palette *pal;

	surface = bitmap[getbmpindex];
    if(surface)
    {
	    bpp = surface->format->BytesPerPixel;
		if(bpp <= 8)
		{
			pal = surface->format->palette;
			if(pal)
			{
				SDL_SetPalette(bitmap[setbmpindex], SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
			}
		}
	}
}
void SaveBmp( int bmpindex, char *fn )
{
	if ( bmpindex >= 0 )
	{
		SDL_SaveBMP(bitmap[bmpindex], fn);
	}
	else 
	{
		SDL_SaveBMP(g_screen, fn);
	}
}
void CreateSurface( int bmpindex, int size_x, int size_y  )
{
	SDL_Surface* tmp;
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

	ReleaseBitmap( bmpindex );
	tmp = SDL_CreateRGBSurface( SDL_SWSURFACE, size_x, size_y, 0, rmask, gmask, bmask, amask );
	if(tmp)
	{
		bitmap[bmpindex] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}
}

void SwapToSecondary( int bmpindex )
{
	if ( g_surface_bakup != NULL )
	{
		g_screen = g_surface_bakup;
		g_surface_bakup = NULL;
	}
	else 
	{
		g_surface_bakup = g_screen;
		g_screen = bitmap[bmpindex];
	}
}


void ClearSecondary( void )
{
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = DISPLY_WIDTH;
	rect.h = DISPLY_HEIGHT;
	
	
	SDL_FillRect(g_screen, &rect, SDL_MapRGBA(g_screen->format,0,0,0,255));
}

void KeyInit( void )
{
	int i;
	
	sound_vol = 128;
	 
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
		key_eventPress_old[i] = 0;
		key_eventPush[i] = 0;
	}

	initPAD( );
}

int initPAD(void)
{
#ifdef SDL_JOYSTICK
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0){
		return 0;
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
	return 1;
}

void closePAD(void)
{
#ifdef SDL_JOYSTICK
	if(SDL_JoystickOpened(0))
		SDL_JoystickClose(joys);
#endif
}

void KeyInput( void )
{
	int i;
#ifdef PSPUMODE
	int pad = 0;

	keys = SDL_GetKeyState(NULL);

	if(joys){
		int x = 0, y = 0;
		x = SDL_JoystickGetAxis(joys, 0);
		y = SDL_JoystickGetAxis(joys, 1);

		if(SDL_JoystickGetButton(joys,  sdljbUp) || y < -JOYSTICK_AXIS) pad |= PAD_UP;
		if(SDL_JoystickGetButton(joys,  sdljbLeft) || x < -JOYSTICK_AXIS) pad |= PAD_LEFT;
		if(SDL_JoystickGetButton(joys,  sdljbDown) || y > JOYSTICK_AXIS) pad |= PAD_DOWN;
		if(SDL_JoystickGetButton(joys,  sdljbRight) || x > JOYSTICK_AXIS) pad |= PAD_RIGHT;
		if(SDL_JoystickGetButton(joys, sdljbCross)) pad |= PAD_BUTTON1;
		if(SDL_JoystickGetButton(joys, sdljbCircle)) pad |= PAD_BUTTON2;
		if(SDL_JoystickGetButton(joys, sdljbTriangle)) pad |= PAD_BUTTON3;
		if(SDL_JoystickGetButton(joys, sdljbSquare)) pad |= PAD_BUTTON5;
	}
#elif defined(DREAMCAST)

	keys = SDL_GetKeyState(NULL);
	if(joys){
		int x = 0, y = 0;
		x = SDL_JoystickGetAxis(joys, 0);
		y = SDL_JoystickGetAxis(joys, 1);
		if(SDL_JoystickGetButton(joys,  sdljbUp) || y < -JOYSTICK_AXIS) pad |= PAD_UP;
		if(SDL_JoystickGetButton(joys,  sdljbLeft) || x < -JOYSTICK_AXIS) pad |= PAD_LEFT;
		if(SDL_JoystickGetButton(joys,  sdljbDown) || y > JOYSTICK_AXIS) pad |= PAD_DOWN;
		if(SDL_JoystickGetButton(joys,  sdljbRight) || x > JOYSTICK_AXIS) pad |= PAD_RIGHT;
		if(SDL_JoystickGetButton(joys, sdljbCross)) pad |= PAD_BUTTON1;
		if(SDL_JoystickGetButton(joys, sdljbCircle)) pad |= PAD_BUTTON2;
		if(SDL_JoystickGetButton(joys, sdljbTriangle)) pad |= PAD_BUTTON3;
		if(SDL_JoystickGetButton(joys, sdljbSquare)) pad |= PAD_BUTTON5;
	}	

#else
	int x = 0, y = 0;
	int pad = 0;

	keys = SDL_GetKeyState(NULL);

#ifdef SDL_JOYSTICK
	if(joys){
		x = SDL_JoystickGetAxis(joys, 0);
		y = SDL_JoystickGetAxis(joys, 1);
	}
#endif

	if(pad_type == 0)
	{
		if(keys[SDLK_RIGHT] == SDL_PRESSED || keys[SDLK_KP6] == SDL_PRESSED || x > JOYSTICK_AXIS){
			pad |= PAD_RIGHT;
		}
		if(keys[SDLK_LEFT] == SDL_PRESSED || keys[SDLK_KP4] == SDL_PRESSED || x < -JOYSTICK_AXIS){
			pad |= PAD_LEFT;
		}
		if(keys[SDLK_DOWN] == SDL_PRESSED || keys[SDLK_KP2] == SDL_PRESSED || y > JOYSTICK_AXIS){
			pad |= PAD_DOWN;
		}
		if(keys[SDLK_UP] == SDL_PRESSED || keys[SDLK_KP8] == SDL_PRESSED || y < -JOYSTICK_AXIS){
			pad |= PAD_UP;
		}
	}
	else if(pad_type == 1)
	{
		if(keys[SDLK_d] == SDL_PRESSED || keys[SDLK_KP6] == SDL_PRESSED || x > JOYSTICK_AXIS){
			pad |= PAD_RIGHT;
		}
		if(keys[SDLK_a] == SDL_PRESSED || keys[SDLK_KP4] == SDL_PRESSED || x < -JOYSTICK_AXIS){
			pad |= PAD_LEFT;
		}
		if(keys[SDLK_s] == SDL_PRESSED || keys[SDLK_KP2] == SDL_PRESSED || y > JOYSTICK_AXIS){
			pad |= PAD_DOWN;
		}
		if(keys[SDLK_w] == SDL_PRESSED || keys[SDLK_KP8] == SDL_PRESSED || y < -JOYSTICK_AXIS){
			pad |= PAD_UP;
		}
	}

	int	btn1 = 0, btn2 = 0, btn3 = 0, btn4 = 0, btn5 = 0, btn6 = 0, btn7 = 0, btn8 = 0, btn9 = 0, btnA = 0;

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
		if(keys[SDLK_LCTRL] == SDL_PRESSED || btn1){
			pad |= PAD_BUTTON1;
		}
#ifdef _TINSPIRE
		if(keys[SDLK_LSHIFT] == SDL_PRESSED || btn2){
#else
		if(keys[SDLK_LALT] == SDL_PRESSED || btn2){
#endif
			pad |= PAD_BUTTON2;
		}
		if(keys[SDLK_RETURN] == SDL_PRESSED || btn3){
			pad |= PAD_BUTTON3;
		}
	}
	else if(pad_type == 1)
	{
		if(keys[SDLK_BACKSLASH] == SDL_PRESSED || btn1){
			pad |= PAD_BUTTON1;
		}
		if(keys[SDLK_RSHIFT] == SDL_PRESSED || btn2){
			pad |= PAD_BUTTON2;
		}
		if(keys[SDLK_p] == SDL_PRESSED || btn3){
			pad |= PAD_BUTTON3;
		}
	}

	if(keys[SDLK_F1] == SDL_PRESSED || btn7){
		pad |= PAD_BUTTON7;
	}
	if(keys[SDLK_F2] == SDL_PRESSED || btn8){
		pad |= PAD_BUTTON8;
	}
	
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
#ifndef GP2X
	if ( keys[SDLK_ESCAPE] )	// 終了
	{
		key_eventPress[GP2X_BUTTON_EXIT] = 1;
	}
#endif
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

int IsPushKey( int keycode )
{
	int rc = 0;
	
	if ( key_eventPush[keycode] == 1 )
	{
		rc = 1;
	}

	return( rc );
}

int IsPressKey( int keycode )
{
	int rc = 0;
	
	if ( key_eventPress[keycode] == 1 )
	{
		rc = 1;
	}

	return( rc );
}

int IsPushOKKey( void )
{
	int rc = 0;
#ifdef GP2X
	if ( key_eventPush[GP2X_BUTTON_B] == 1 )
#else
	if ( key_eventPush[GP2X_BUTTON_A] == 1 )
#endif
	{
		rc = 1;
	}

	return( rc );
}
int IsPushCancelKey( void )
{
	int rc = 0;
	
#ifdef GP2X
	if ( key_eventPush[GP2X_BUTTON_X] == 1 )
#else
	if ( key_eventPush[GP2X_BUTTON_X] == 1 )
#endif
	{
		rc = 1;
	}

	return( rc );
}

void FPSWait( void )
{
	int interval = INTERVAL_BASE;
	Uint32 leftTick;

	//サウンドの制御
	soundPlayCtrl( );

	SDL_PollEvent(&event);
/*
	nowTick = SDL_GetTicks();
	frame = (nowTick - prvTickCount) / interval;
	if(frame <= 0){
#ifdef GP2X
		wait(prvTickCount + interval - nowTick);
#else
		SDL_Delay(prvTickCount + interval - nowTick);
#endif
	}
	prvTickCount = SDL_GetTicks();
*/

	if(prvTickCount == 0) prvTickCount = SDL_GetTicks();
	
	while (true)
	{
	 	nowTick = SDL_GetTicks();
 		leftTick = prvTickCount + interval - nowTick;
 		if(leftTick < 1 || leftTick > 9999)
 		{
			break;
		}
#ifdef GP2X
 	 	wait(1);
#else
		SDL_Delay(1);
#endif
	}
	prvTickCount = nowTick;

	gameflag[107] = gameflag[107] + 1;
	if ( gameflag[107] >= 60 )
	{
		gameflag[107] = 0;
		gameflag[108] = gameflag[108] + 1;
		if ( gameflag[108] >= 60 )
		{
			gameflag[108] = 0;
			gameflag[109] = gameflag[109] + 1;
			if ( gameflag[109] >= 60 )
			{
				gameflag[109] = 0;
				gameflag[110] = gameflag[110] + 1;
				if ( gameflag[110] >= 999 )
				{
					gameflag[110] = 999;
				}
			}
		}
	}

}

int system_keys( void )
{
	int rc;
	
	rc = 1;
#ifdef GP2X
	// 終了
//	if ( ( IsPressKey( GP2X_BUTTON_START ) ) && ( IsPressKey( GP2X_BUTTON_SELECT ) ) )
	if ( ( IsPressKey( GP2X_BUTTON_START ) ) && ( IsPressKey( GP2X_BUTTON_L ) ) && ( IsPressKey( GP2X_BUTTON_R ) ) )
#else
	if ( ( event.type == SDL_QUIT ) || ( IsPressKey( GP2X_BUTTON_EXIT ) ) )
#endif
	{
		rc = 0;
		g_scene = EN_SN_EXIT;
	}
	// 音量調整
	if ( IsPushKey( GP2X_BUTTON_VOLUP ) )
	{
		gameflag[60] = gameflag[60] + 10;
		if ( gameflag[60] > VOL_MAX )
		{
			gameflag[60] = VOL_MAX;
		}
		Set_Volume( gameflag[60] );
	}
	if ( IsPushKey( GP2X_BUTTON_VOLDOWN ) )
	{
		gameflag[60] = gameflag[60] - 10;
		if ( gameflag[60] < 0 )
		{
			gameflag[60] = 0;
		}
		Set_Volume( gameflag[60] );
	}
	
	return( rc );
}
int Set_Volume( int vol )
{
	int rc = 0;
	
	soundSetVolumeBgm( vol, 0 );
	soundSetVolumeAll( vol );
	
	return( rc );
}

/*[ BltNumericImage ]************************************************/
/*	数値画像表示関数												*/
/*-[引数]-----------------------------------------------------------*/
/*	value			(i )	画像表示する数値						*/
/*	length			(i )	表示する桁数（桁数以上は表示されない）	*/
/*	x				(i )	画像を表示する位置ｘ座標				*/
/*	y				(i )	画像を表示する位置ｙ座標				*/
/*	plane			(i )	数値画像が読み込まれているプレーン番号	*/
/*	num_stpos_x		(i )	プレーン内での数値画像の開始位置ｘ座標	*/
/*	num_stpos_y		(i )	プレーン内での数値画像の開始位置ｙ座標	*/
/*	num_width		(i )	数値画像１文字の幅（ドット数）			*/
/*	num_height		(i )	数値画像１文字の高さ（ドット数）		*/
/*-[戻り値]---------------------------------------------------------*/
/*	無し															*/

void BltNumericImage( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height )
{
	int32_t blt_num;	// １桁の数値を格納する
	int32_t i;			// 桁数分のforループで使用
	int32_t dv;		// 割り算で使用する値

	// value が負の値の場合、正の値に置き換える
	if ( value < 0 )
	{
		value = value * -1;
	}

	// 最初の割り算で使用する値を求める
	dv = 1;
	for( i = 1; i < length; i++ )
	{
		dv = dv * 10;
	}

	// 指定された桁数分の数字画像を転送する
	for( i = 0; i < length; i++ )
	{
		// 表示する数字を求める
		blt_num = value / dv;
		value = value - blt_num * dv;
		if ( blt_num > 9 )
		{	// 表示したい１桁の数値にならなければ、１桁にする。
			blt_num = blt_num % 10;
		}
		// 数字画像転送
		BltRect( plane, x + (num_width * i), y, num_stpos_x + (num_width * blt_num), num_stpos_y, num_width, num_height );
		// 割り算で使用する値を10で割る
		dv = dv / 10;
	}

	return;
}

/*[ BltNumericImage2 ]************************************************/
/*	数値画像表示関数（右詰め）										*/
/*-[引数]-----------------------------------------------------------*/
/*	value			(i )	画像表示する数値						*/
/*	length			(i )	表示する桁数（桁数以上は表示されない）	*/
/*	x				(i )	画像を表示する位置ｘ座標				*/
/*	y				(i )	画像を表示する位置ｙ座標				*/
/*	plane			(i )	数値画像が読み込まれているプレーン番号	*/
/*	num_stpos_x		(i )	プレーン内での数値画像の開始位置ｘ座標	*/
/*	num_stpos_y		(i )	プレーン内での数値画像の開始位置ｙ座標	*/
/*	num_width		(i )	数値画像１文字の幅（ドット数）			*/
/*	num_height		(i )	数値画像１文字の高さ（ドット数）		*/
/*-[戻り値]---------------------------------------------------------*/
/*	無し															*/
/********************************************************************/
void BltNumericImage2( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height )
{
	int32_t blt_num;	// １桁の数値を格納する
	int32_t i;			// 桁数分のforループで使用
	int32_t dv;		// 割り算で使用する値
	int32_t x_hosei;	//右詰め補正値
	
	// value が負の値の場合、正の値に置き換える
	if ( value < 0 )
	{
		value = value * -1;
	}
	
	/* 2002.10.21 D.K start */
	int buf = value;
	int t = 1;
	while(true)
	{
		t++;
		buf = buf / 10;
		if ( 0 <= buf )
		{
			break;
		}
	}
	x_hosei = t;
//	scanf(string[0],"%d",value);
//	x_hosei = StrLen( string[0] );
	if ( x_hosei == 0 )
	{
		x_hosei = 1;
	}
	x_hosei = length - x_hosei;
	x_hosei = x_hosei * num_width;
//	length = StrLen( string[0] );
	x = x + x_hosei;
	/* 2002.10.21 D.K end */
	
	// 最初の割り算で使用する値を求める
	dv = 1;
	for( i = 1; i < length; i++ )
	{
		dv = dv * 10;
	}

	// 指定された桁数分の数字画像を転送する
	for( i = 0; i < length; i++ )
	{
		// 表示する数字を求める
		blt_num = value / dv;
		value = value - blt_num * dv;
		if ( blt_num > 9 )
		{	// 表示したい１桁の数値にならなければ、１桁にする。
			blt_num = blt_num % 10;
		}
		// 数字画像転送
		BltRect( plane, x + (num_width * i), y, num_stpos_x + (num_width * blt_num), num_stpos_y, num_width, num_height );
		// 割り算で使用する値を10で割る
		dv = dv / 10;
	}

	return;
}

int32_t funcSin( int32_t rdo )
{
	int32_t ang = 0;
	int32_t rc = 0;
	
	if ( ( rdo >= 0 ) && ( rdo < 180 ) )
	{
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTsin( ang );
	}
	if ( ( rdo >= 180 ) && ( rdo < 360 ) )
	{
		rdo = rdo - 180;
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTsin( ang );
		rc = rc * -1;
	}
	
	return( rc );
}
int32_t funcCos( int32_t rdo )
{
	int32_t ang = 0;
	int32_t rc = 0;
	
	if ( ( rdo >= 0 ) && ( rdo < 180 ) )
	{
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTcos( ang );
	}
	if ( ( rdo >= 180 ) && ( rdo < 360 ) )
	{
		rdo = rdo - 180;
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTcos( ang );
		rc = rc * -1;
	}
	
	return( rc );
}


int32_t funcTan2( int32_t posX, int32_t posY )
{
	int32_t rc = 0;
	
	rc = MOTatan( ( posX ) * 256, ( posY * -1 ) * 256 );
	
	return( rc );
}

int32_t get2keta( int32_t val, int32_t st )
{
	int32_t rc = 0;
	
	val = val / st;
	rc = val % 100;
	
	return( rc );
}

