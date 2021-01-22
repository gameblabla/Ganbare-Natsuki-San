#ifndef __FUNCTION
#define __FUNCTION

#include "general.h"

enum Joystick_SDL { sdljbTriangle = 0, sdljbCircle, sdljbCross, sdljbSquare, sdljbLTrig, sdljbRTrig, sdljbDown, sdljbLeft, sdljbUp, sdljbRight, sdljbSelect, sdljbStart, sdljbHome };

extern void ResetGameFlag( void );
extern void ResetGameFlag2( void );
extern int LoadGameFlag( char *fn );
extern int SaveGameFlag( char *fn );
extern int LoadGameFlag2( char *fn );
extern int SaveGameFlag2( char *fn );
extern int SaveFile( char *fn, Sint32 *buff, Sint32 size );
extern int LoadFile( char *fn, Sint32 *buff, Sint32 size );
extern Sint32 GetConfig( char* fn, char* cParam );
extern Sint32 LogFileWrite( char* fn, char* cParam );

extern Sint32 LoadBitmap( char *fname, int bmpindex, int flag );
extern void ReleaseBitmap( int bmpindex );
extern void Blt( int bmpindex, int dstX, int dstY );
extern void BltRect( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height);
extern void ClearSecondary( void );
extern void BltNumericImage( Sint32 value, Sint32 length, Sint32 x, Sint32 y, Sint32 plane, Sint32 num_stpos_x, Sint32 num_stpos_y, Sint32 num_width, Sint32 num_height );
extern void BltNumericImage2( Sint32 value, Sint32 length, Sint32 x, Sint32 y, Sint32 plane, Sint32 num_stpos_x, Sint32 num_stpos_y, Sint32 num_width, Sint32 num_height );
extern void SetGscreenPalette( SDL_Surface *surface );
//extern void SetPalette(int getbmpindex, int setbmpindex);
extern void CreateSurface( int bmpindex, int size_x, int size_y  );
extern void SwapToSecondary( int bmpindex );
extern void SaveBmp( int bmpindex, char *fn );
extern void drawGRPline(Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2, Uint32 color);

extern int IsPushKey( int keycode );
extern int IsPressKey( int keycode );
extern void KeyInput( void );
extern int initPAD(void);
extern void closePAD(void);
extern int getPAD(void);
extern int IsPushOKKey( void );
extern int IsPushCancelKey( void );

extern void FunctionInit( void );
extern void FPSWait( void );
extern int system_keys( void );
extern int Set_Volume( int vol );

extern Sint32 funcSin( Sint32 rdo );
extern Sint32 funcCos( Sint32 rdo );
extern Sint32 funcTan2( Sint32 posX, Sint32 posY );
extern Sint32 get2keta( Sint32 val, Sint32 st );

#endif
