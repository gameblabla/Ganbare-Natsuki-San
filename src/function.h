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
extern int SaveFile( char *fn, int32_t *buff, int32_t size );
extern int LoadFile( char *fn, int32_t *buff, int32_t size );
extern int32_t GetConfig( char* fn, char* cParam );
extern int32_t LogFileWrite( char* fn, char* cParam );

extern int32_t LoadBitmap( char *fname, int bmpindex, int flag );
extern void ReleaseBitmap( int bmpindex );
extern void Blt( int bmpindex, int dstX, int dstY );
extern void BltRect( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height);
extern void ClearSecondary( void );
extern void BltNumericImage( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height );
extern void BltNumericImage2( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height );
extern void SetGscreenPalette( SDL_Surface *surface );
extern void SetPalette(int getbmpindex, int setbmpindex);
extern void CreateSurface( int bmpindex, int size_x, int size_y  );
extern void SwapToSecondary( int bmpindex );
extern void SaveBmp( int bmpindex, char *fn );
extern void drawGRPline(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Uint32 color);

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

extern int32_t funcSin( int32_t rdo );
extern int32_t funcCos( int32_t rdo );
extern int32_t funcTan2( int32_t posX, int32_t posY );
extern int32_t get2keta( int32_t val, int32_t st );

#endif
