#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define.h"
#include "function.h"
#include "audio/audio.h"
#include "extern.h"
#include "logo.h" 

#include "refresh.h"

void logo_main( void );
void logo_init( void );
void logo_relese( void );
void logo_keys( void );
void logo_drow( void );
void logo_init_save_data( void );

static int scene_exit;
static int demo;


#ifndef GNS_FRAME_STEPPED
void logo_main( void )
{
	int exit_code;
	
	logo_init( );	
	
	while( scene_exit )
	{
		logo_keys( );	
		logo_drow( );		
		
		RefreshScreen(NULL);
		FPSWait( );	

		exit_code = system_keys( ); 
		if ( exit_code == 0 )
		{
			scene_exit = 0;
		}
	}
	
	logo_relese( );	
}

#else
int logo_step( void )
{
	static int active = 0;
	int exit_code;

	if ( ! active )
	{
		logo_init( );
		active = 1;
	}

	if ( scene_exit )
	{
		logo_keys( );
		logo_drow( );
		RefreshScreen(NULL);
		FPSWait( );

		exit_code = system_keys( );
		if ( exit_code == 0 )
		{
			scene_exit = 0;
		}
	}

	if ( ! scene_exit )
	{
		logo_relese( );
		active = 0;
		return 0;
	}

	return 1;
}
#endif

void logo_init( void )
{
	scene_exit = 1;
	demo = 0;
	
	LoadBitmap(IMAGE_PATH "a_logo.bmp",2,true);	

	soundStopBgm(EN_BGM_GAME01);
}

void logo_relese( void )
{
	int i;
	
	for ( i = 0; i < BMPBUFF_MAX; i++ )
	{
		ReleaseBitmap( i );
	}
	soundStopBgm(EN_BGM_GAME01);

}


void logo_keys( void )
{
	if ( ( IsPushKey(gameflag[4]) ) || ( IsPushKey(gameflag[5]) ) || ( demo > ( 60 * 3 ) ) )
	{
		gameflag[40] = 0;	
		g_scene = EN_SN_TITLE;
		scene_exit=0;
		return;
	}
}

void logo_drow( void )
{
	ClearSecondary();
	demo++;
	
#ifdef WIDESCREEN
	/* The logo/intro artwork is still the original 320x240 image.  Center it
	 * inside the 400x240 widescreen framebuffer instead of pinning it to the
	 * left edge.  This is intentionally local to the intro/logo screen so the
	 * rest of the widescreen layout remains unchanged. */
	Blt( 2, ( DISPLY_WIDTH - 320 ) / 2, 0 );
#else
	Blt( 2, 0, 0 );		/* 320 * 240 */
#endif
	KeyInput();				
}


