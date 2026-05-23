#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define.h"
#include "function.h"
#include "audio/audio.h"
#include "extern.h"
#include "title.h" 
#include "renderer/renderer.h"
#include "filesystem/filesystem.h"
#include "platform.h"

#include "refresh.h"

void title_main( void );
void title_init( void );
void title_relese( void );
void title_keys( void );
void title_drow( void );
void title_init_save_data( void );
void title_kane_set( int x, int y );
void title_kane_disp( void );
void title_k_jmp( int i );
int replay_file_find( void );
int replay_file_find2( void );

enum
{
	EN_TITLE_NONE = 0,
	EN_TITLE_image1,
	EN_TITLE_image2,
	EN_TITLE_image3,
	EN_TITLE_image4,
	EN_TITLE_image5,
	EN_TITLE_image6,
};

static int scene_exit;

static int mode;	
static int a[2] = {0,0};
static int b[2] = {0,0};
static int kane[200];	
static int uracount = 0;

static int title_no = 0;
/* Size was 1024, reduce it to 29 as that's the minimum here. - Gameblabla */
static char string[29];

#ifndef GNS_FRAME_STEPPED
void title_main( void )
{
	int exit_code;

	title_init( );		
	
	while( scene_exit )
	{
		title_keys( );		
		title_drow( );
		
		RefreshScreen(NULL);
		
		
		FPSWait( );	
		exit_code = system_keys( ); 
		if ( exit_code == 0 )
		{
			scene_exit = 0;
		}
	}
	
	title_relese( );
}

#else
int title_step( void )
{
	static int active = 0;
	int exit_code;

	if ( ! active )
	{
		title_init( );
		active = 1;
	}

	if ( scene_exit )
	{
		title_keys( );
		title_drow( );
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
		title_relese( );
		active = 0;
		return 0;
	}

	return 1;
}
#endif

void title_init( void )
{
	Stop_Music();
	scene_exit = 1;

	title_no = 0;
	mode = 0;		
	memset( kane, 0, sizeof( kane ) );
	uracount = 0;
	title_no = 0;

	LoadBitmap(TITLE_IMAGE_PATH "title_natuki.bmp",1,true);

	if ( gameflag[100] == 1 )
	{
		LoadBitmap(TITLE_IMAGE_PATH "title2_2.bmp",2,true);
	}
	else 
	{
		LoadBitmap(TITLE_IMAGE_PATH "title2.bmp",2,true);
	}
	LoadBitmap(TITLE_IMAGE_PATH "scl.bmp",3,true);
	LoadBitmap(BAK_IMAGE_PATH "1.bmp",5, false );

	if ( gameflag[126] == 0 )
	{
		LoadBitmap(TITLE_IMAGE_PATH "title5.bmp",7,true);
	}
	else 
	{
		LoadBitmap(TITLE_IMAGE_PATH "title5_ura.bmp",7,true);
	}

	LoadBitmap(TITLE_IMAGE_PATH "title3.bmp",10,true);
	LoadBitmap(SYS_IMAGE_PATH "fonts2.bmp",18,true);
	/*LoadBitmap(SYS_IMAGE_PATH "waku.bmp",109,true);*/

	a[0] = 100;
	a[1] = 200;
	title_kane_set( 0, 300 );
	
	if ( gameflag[123] != -1 )
	{
		gameflag[120] = gameflag[123];
	}
	
	if ( gameflag[120] > 50 )
	{
		gameflag[120] = 50;
	}
          
	
	soundPlayBgm( EN_BGM_GAME01 );

}

void title_relese( void )
{
	int i;
	
	for ( i = 0; i < BMPBUFF_MAX; i++ )
	{
		ReleaseBitmap( i );
	}
	soundStopBgm(EN_BGM_GAME01);

}


void title_keys( void )
{
	char path_item[96];
	char path_work[96];

	Filesystem_GetWorkSavePath(path_work, sizeof(path_work));
	Filesystem_GetItemSavePath(path_item, sizeof(path_item));

	if ( IsPushKey( gameflag[0] ) )
	{
		soundPlaySe( EN_SE_SELECT );
		mode--;
		if ( gameflag[100] == 1 )
		{
			if ( mode == -2 )
			{
				mode = 4;
			}
		}
		else 
		{
			if ( mode == -1 )
			{
				mode = 4;
			}
		}
	}
	else if ( IsPushKey( gameflag[1] ) )
	{
		soundPlaySe( EN_SE_SELECT );
		mode++;
		if ( gameflag[100] == 1 )
		{
			if ( mode == 5 )
			{
				{
					mode = -1;
				}
			}
		}
		else 
		{
			if ( mode == 5 )
			{
				{
					mode = 0;
				}
			}
		}
	}
	if ( IsPushKey( gameflag[2] ) )
	{
		if ( mode == 0 )
		{
			soundPlaySe( EN_SE_SELECT );
			gameflag[120]--;
			if ( gameflag[120] < 1 )
			{
				gameflag[120] = gameflag[121];
				if ( gameflag[120] > 50 )
				{
					gameflag[120] = 50;
				}
			}
		}
		if ( mode == 1 )
		{
			 
			soundPlaySe( EN_SE_SELECT );
			gameflag[124]--;
			if ( gameflag[124] < 0 )
			{
				gameflag[124] = 1;
			}
		}
	}
	else if ( IsPushKey( gameflag[3] ) )
	{
		if ( mode == 0 )
		{
			soundPlaySe( EN_SE_SELECT );
			gameflag[120]++;
			if ( ( gameflag[120] > gameflag[121] ) || ( gameflag[120] > 50 ) )
			{
				gameflag[120] = 1;
			}
		}
		if ( mode == 1 )
		{
			 
			soundPlaySe( EN_SE_SELECT );
			gameflag[124]++;
			if ( gameflag[124] > 1 )
			{
				gameflag[124] = 0;
			}
		}
	}
	
	if ( IsPressKey( gameflag[2] ) )
	{
		b[0]++;
		if ( mode == 0 )
		{
			if ( b[0] >= 12 )
			{
				b[0] = 0;
				soundPlaySe( EN_SE_SELECT );
				gameflag[120]--;
				if ( gameflag[120] < 1 )
				{
					gameflag[120] = gameflag[121];
					if ( gameflag[120] > 50 )
					{
						gameflag[120] = 50;
					}
				}
			}
		}
	}
	else if ( IsPressKey( gameflag[3] ) )
	{
		b[0]++;
		if ( mode == 0 )
		{
			if ( b[0] >= 12 )
			{
				b[0] = 0;
				soundPlaySe( EN_SE_SELECT );
				gameflag[120]++;
				if ( ( gameflag[120] > gameflag[121] ) || ( gameflag[120] > 50 ) )
				{
					gameflag[120] = 1;
				}
			}
		}
	}
	else 
	{
		b[0] = 0;
	}

	if ( IsPushOKKey( ) )
	{
		if ( mode == 4 )	/* Exit */
		{
			gameflag[123] = -1;
			gameflag[40] = 10;
			g_scene = EN_SN_EXIT;
			scene_exit=0;
			Platform_SaveCustom(path_work, "gan_work.sav", "WORK");
			Platform_SaveCustom(path_item, "gan_item.sav", "ITEM");
		}
		else if ( mode == 3 )	/* option */
		{
			gameflag[123] = -1;
			gameflag[40] = 3;
			g_scene = EN_SN_OPTION;
			scene_exit=0;
		}
		else if ( mode == 2 )	/* demo */
		{
			ResetGameFlag2( );
			
			title_init_save_data( );
			
			gameflag[125] = 0;	/* replay nomal */
			gameflag[123] = gameflag[120];
			gameflag[132] =  1;		
			gameflag2[3] = 1;
			gameflag2[2] = 0;
			gameflag[70] = 1;
			SaveGameFlag2(path_work);
			ResetGameFlag2( );
			SaveGameFlag2(path_item);

			gameflag[40] = 4;
			g_scene = EN_SN_ACT;
			scene_exit=0;
		}
		else if ( mode == 1 )	/* replay */
		{
			if ( replay_file_find( ) == 1 )
			{
				ResetGameFlag2( );
				
				title_init_save_data( );
				
				gameflag[127] = 0;
				gameflag[125] = 0;
				gameflag[123] = gameflag[120];
				gameflag[132] =  1;	
				gameflag[70] = 1;
				SaveGameFlag2(path_work);
				ResetGameFlag2( );
				SaveGameFlag2(path_item);

				gameflag[40] = 4;
				g_scene = EN_SN_ACT;
				scene_exit=0;
				return;
			}
			else 
			{
				soundPlaySe( EN_SE_MSG );
			}
		}
		else if ( mode == 0 )	/* new */
		{
			ResetGameFlag2( );
			
			title_init_save_data( );
			
			gameflag[127] = 0;
			gameflag[123] = -1;	
			gameflag[132] =  0;
			gameflag[70] = 1;
			SaveGameFlag2(path_work);
			ResetGameFlag2( );
			SaveGameFlag2(path_item);

			gameflag[40] = 4;
			g_scene = EN_SN_ACT;
			scene_exit=0;
			return;
		}
		else if ( mode == -1 )
		{
			ResetGameFlag2( );
			
			title_init_save_data( );
			
			gameflag[135] = 1000000;
			gameflag[136] = 0;			
			gameflag2[2] = 1;	
			gameflag2[3] = 1;	
			gameflag[123] = gameflag[120];	
			gameflag[127] = 1;	
			gameflag[123] = -1;	
			gameflag[132] =  0;
			gameflag[70] = 1;
			SaveGameFlag2(path_work);
			ResetGameFlag2( );
			SaveGameFlag2(path_item);

			gameflag[40] = 4;
			g_scene = EN_SN_ACT;
			scene_exit=0;
		}
	}

	if ( IsPushCancelKey( ) )
	{
		if ( mode == 0 )
		{
			if ( gameflag[100] == 1 )
			{
				uracount++;
				if ( uracount >= 16 )
				{
					if ( gameflag[126] == 0 )
					{
						gameflag[126] = 1;
						LoadBitmap(TITLE_IMAGE_PATH "title5_ura.bmp",7,true);
					}
					else 
					{
						gameflag[126] = 0;
						LoadBitmap(TITLE_IMAGE_PATH "title5.bmp",7,true);				
					}
					uracount = 0;
				}
			}
		}
		if ( mode == 1 )	/* replay jamp */
		{
			if ( replay_file_find2( ) == 1 )
			{
				ResetGameFlag2( );
				
				title_init_save_data( );
				
				gameflag[127] = 0;	
				gameflag[125] = 1;	
				gameflag[123] = gameflag[120];	
				gameflag[132] =  1;	
				gameflag[70] = 1;
				SaveGameFlag2(path_work);
				ResetGameFlag2( );
				SaveGameFlag2(path_item);

				gameflag[40] = 4;
				g_scene = EN_SN_ACT;
				scene_exit=0;
			}
			else 
			{
				soundPlaySe( EN_SE_MSG );
			}
		}
	}
	if ( IsPushKey( gameflag[6] ) )
	{
		gameflag[122]++;
		if ( gameflag[100] == 1 )
		{
			if ( gameflag[122] > 4 )
			{
				gameflag[122] = 0;
			}
		}
		else 
		{
			if ( gameflag[122] > 2 )
			{
				gameflag[122] = 0;
			}
		}
	}

}

void title_drow( void )
{
	int stage_hosei;
	int wk;
	
	stage_hosei = 0;
	if ( gameflag[126] == 1 )
	{
		stage_hosei = 50;
	}

	ClearSecondary();
	
	if ( title_no == 0 )
	{
		Blt( 5, 0, 0 );
		
		Blt( 1, 100 - a[0], 0 + a[1] );
		
		a[0] = a[0] - 10;
		if ( a[0] < 0 )
		{
			a[0] = 0;
		}
		a[1] = a[1] - 20;
		if ( a[1] < 0 )
		{
			a[1] = 0;
		}
		
		Blt( 2, 0, 0 );
		title_kane_disp(  );
		
		BltRect( 3, 96, 128 + ( mode * 16 ), 0, gameflag[122] * 32 , 32 , 32 );
		BltNumericImage2( gameflag[120], 2, 262, 148, 18, 0, 0, 10, 8 );
		BltRect( 18, 262, 164, 0, 56 + ( gameflag[124] * 8 ), 100, 8 );
		
		BltRect( 18, 5, 230 , 0, 24, 100, 8 );	/*  */
		BltRect( 18, 50, 230 , 0, 8, 100, 8 );	/*  */

		wk = get2keta( gameflag[200 + gameflag[120] + stage_hosei], 1 );
		BltNumericImage( wk, 2, 110, 230, 18, 0, 0, 10, 8 );	/*  */
		wk = get2keta( gameflag[200 + gameflag[120] + stage_hosei], 100 );
		BltNumericImage( wk, 2, 80, 230, 18, 0, 0, 10, 8 );	/*  */
		wk = get2keta( gameflag[200 + gameflag[120] + stage_hosei], 10000 );
		BltNumericImage( wk, 2, 50, 230, 18, 0, 0, 10, 8 );	/*  */
			
		BltRect( 18, 150, 230, 0, 80, 100, 8 );	/*  */
		BltNumericImage2( gameflag[300 + gameflag[120] + stage_hosei], 3, 185, 230, 18, 0, 0, 10, 8 );
	}

	/*if ( gameflag[61] == 0 )
	{
		Blt( 109 , -160, -120 );
	}*/
	
	KeyInput();				

}


void title_init_save_data( void )
{
	gameflag2[0]	= 8 * 32;	
	gameflag2[1]	= ( 2 * 32 ) - 16;	
	if ( gameflag[126] == 1 )
	{
		gameflag2[2]	= 2;
	}
	else 
	{
		gameflag2[2]	= 1;
	}
	gameflag2[3]	= gameflag[120];
	gameflag2[4]	= 0;	/* ���� */
	gameflag2[5]	= 3;	/* ���݂g�o */
	gameflag2[6]	= 3;	/* �ő�g�o */
	gameflag2[7]	= 0;	/* �n�[�g�̂����珊���� */
	gameflag2[8]	= 0;	/* ���ݐݒ�X�L�� */
	gameflag2[9]	= 0;	/* �e���|�[�^�g�p�s�A�X�N���[���s�t���O */
	gameflag2[10]	= 0;	/* �e���|�[�^�g�p�s�t���O */

	gameflag2[20]	= 0; 	/* �� */
	gameflag2[21]	= 0; 	/* �� */
	gameflag2[22]	= 0; 	/* �b */
	
	gameflag2[30]	= 0;	/* �e���|�[�^�[�g�p */
	gameflag2[31]	= 0;	/* �e���|�[�^�[�X�e�[�W */
	gameflag2[32]	= 0;	/* �e���|�[�^�[��ʂm�� */
	gameflag2[33]	= 0;	/* �e���|�[�^�[�w */
	gameflag2[34]	= 0;	/* �e���|�[�^�[�x */

	gameflag2[40]	= 0;	/* �擾�X�L���P */
	gameflag2[41]	= 0;	/* �擾�X�L���Q */
	gameflag2[42]	= 0;	/* �擾�X�L���R */
	gameflag2[43]	= 0;	/* �擾�X�L���S */
}

/***************************************************************************/
// NAME      = kane_set
// FUNCTION  = �^�C�g�������̐���
// NOTES     = 
// DATE      = 
// AUTHER    = koizumi
// HISTORY   =
// PARAMETER = x�F�����ʒu
//             y�F�����ʒu
// RETURN    = �Ȃ�
/***************************************************************************/
void title_kane_set( int x, int y )
{
	int i;
	
	for ( i = 0; i < 20; i++ )
	{
		if ( kane[0 + ( i * 10 )] == 0 )
		{
			kane[0 + ( i * 10 )] = 1;
			kane[1 + ( i * 10 )] = x;
			kane[2 + ( i * 10 )] = y;
			kane[4 + ( i * 10 )] = 0;
			kane[3 + ( i * 10 )] = 0;
			kane[5 + ( i * 10 )] = 0;
			kane[6 + ( i * 10 )] = 0;
			kane[7 + ( i * 10 )] = 0;
			kane[8 + ( i * 10 )] = 0;
			kane[9 + ( i * 10 )] = 6;
			break;
		}
	}
}


/***************************************************************************/
// NAME      = kane_disp
// FUNCTION  = �^�C�g�������̕\��
// NOTES     = 
// DATE      = 
// AUTHER    = koizumi
// HISTORY   =
// PARAMETER = �Ȃ�
// RETURN    = �Ȃ�
/***************************************************************************/
void title_kane_disp( void )
{
	int i;
	
	for ( i = 0; i < 20; i++ )
	{
		if ( kane[0 + ( i * 10 )] == 1 )
		{
			BltRect( 7, kane[1 + ( i * 10 )], ( 0 - kane[2 + ( i * 10 )] ) + DISPLY_HEIGHT - 96, 0, 64, 192, 96 );

			title_k_jmp( i );
		}
	}
}


/***************************************************************************/
// NAME      = k_jmp
// FUNCTION  = �^�C�g��������Y�\���ʒu�v�Z
// NOTES     = 
// DATE      = 
// AUTHER    = koizumi
// HISTORY   =
// PARAMETER = i�F�o�b�t�@�ԍ�
// RETURN    = �Ȃ�
/***************************************************************************/
void title_k_jmp( int i )
{
	int y1;



	if ( kane[6 + ( i * 10 )] == 10 ) 
	{
		if ( kane[9 + ( i * 10 )] > 0 )
		{ 
			kane[6 + ( i * 10 )] = kane[9 + ( i * 10 )];
			kane[9 + ( i * 10 )] = kane[9 + ( i * 10 )] - 1;
		}
		return;
	}

	kane[5 + ( i * 10 )]++;
	if ( kane[5 + ( i * 10 )] >= 10 )
	{
		kane[5 + ( i * 10 )] = 0;
	} 
	
	if ( kane[5 + ( i * 10 )] == 0 )
	{
		kane[6 + ( i * 10 )] = kane[6 + ( i * 10 )] - 3;
		if ( ( kane[6 + ( i * 10 )] <= 2 ) && ( kane[6 + ( i * 10 )] > -2 ) )
		{
			kane[6 + ( i * 10 )] = -3;
		}
		if ( kane[6 + ( i * 10 )] < -8 )
		{
			kane[6 + ( i * 10 )] = -8;
		}
		/* �n�ʔ��� */
	}
	
	/* ����̈ʒu */
	y1 = ( ( 0 - kane[6 + ( i * 10 )] ) * ( 0 - kane[6 + ( i * 10 )] ) * ( 0 - kane[6 + ( i * 10 )] ) );
	kane[2 + ( i * 10 )] = kane[2 + ( i * 10 )] - ( y1 / 25 );

	if ( kane[2 + ( i * 10 )] < 140 ) 
	{
		kane[6 + ( i * 10 )] = 10;
	}
}



/***************************************************************************/
// NAME      = replay_file_find
// FUNCTION  = ���v���C�t�@�C���̌���
// NOTES     = 
// DATE      = 
// AUTHER    = koizumi
// HISTORY   =
// PARAMETER = �Ȃ�
// RETURN    = �t�@�C���̗L��
/***************************************************************************/
int replay_file_find( void )
{
	Sint32 i;
	int file_j;
	int stage;
	
	stage = 1;
	if ( gameflag[126] == 1 )
	{
		stage = 2;	/* ���X�e�[�W */
	}
	else 
	{
	}
	
	file_j = 1;
	
	sprintf( string, "replay/%d/replay_data_%d.dat", ( int )stage, ( int )gameflag[120] );
	if ( LoadFile( string, &i, 1 ) )
	{
		file_j = 0;
	}
	
	return( file_j );
} 


/***************************************************************************/
// NAME      = replay_file_find2
// FUNCTION  = �ŒZ�W�����v���v���C�t�@�C���̌���
// NOTES     = 
// DATE      = 
// AUTHER    = koizumi
// HISTORY   =
// PARAMETER = �Ȃ�
// RETURN    = �t�@�C���̗L��
/***************************************************************************/
int replay_file_find2( void )
{
	Sint32 i;
	int file_j;
	int stage;
	
	stage = 1;
	if ( gameflag[126] == 1 )
	{
		stage = 2;	/* ���X�e�[�W */
	}
	else 
	{
	}
	
	file_j = 1;
	sprintf( string, "replay/%d/replay_data_j%d.dat", ( int )stage, ( int )gameflag[120] );
	if ( LoadFile( string, &i, 1 ) )
	{
		file_j = 0;
	}
	
	return( file_j );
} 




