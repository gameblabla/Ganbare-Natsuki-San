#include <stdio.h>
#include <stdlib.h>

#include "define.h"
#include "extern.h"
#include "scene.h"

#include "title.h"
#include "act.h"
#include "option.h"
#include "ending.h"
#include "logo.h"

#ifndef GNS_FRAME_STEPPED
void scenemanager( void );

void scenemanager( void )
{
	int exit;
	
	exit = true;
	while( exit )
	{
		switch( g_scene )
		{
		case EN_SN_TITLE:
			title_main( );
			break;
		case EN_SN_ACT:
			act_main( );
			break;
		case EN_SN_OPTION:
			option_main( );
			break;
		case EN_SN_ENDING:
			ending_main( );
			break;
		case EN_SN_LOGO:
			logo_main( );
			break;
		default:
			exit = false;
			break;
		}
	}
}
#else
int title_step( void );
int act_step( void );
int option_step( void );
int ending_step( void );
int logo_step( void );

void scenemanager( void )
{
	while ( scenemanager_frame( ) )
	{
	}
}

int scenemanager_frame( void )
{
	switch( g_scene )
	{
	case EN_SN_TITLE:
		title_step( );
		return 1;
	case EN_SN_ACT:
		act_step( );
		return 1;
	case EN_SN_OPTION:
		option_step( );
		return 1;
	case EN_SN_ENDING:
		ending_step( );
		return 1;
	case EN_SN_LOGO:
		logo_step( );
		return 1;
	default:
		return 0;
	}
}
#endif
