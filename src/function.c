#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "define.h"
#include "function.h"
#include "extern.h"
#include "audio/audio.h"
#include "dconv.h"
#include "filesystem/filesystem.h"
#include "renderer/renderer.h"
#include "input/input.h"

void FunctionInit( void );
void ResetGameFlag( void );
void ResetGameFlag2( void );

/* Unused - Gameblabla */
/*
Sint32 GetConfig( char* fn, char* cParam );
Sint32 LogFileWrite( char* fn, char* cParam );
*/

void FPSWait( void );
int system_keys( void );
int Set_Volume( int vol );

void soundInitBuffer(void);
void soundRelease(void);
void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop);

Sint32 funcSin( Sint32 rdo );
Sint32 funcCos( Sint32 rdo );
Sint32 funcTan2( Sint32 posX, Sint32 posY );

// 待ち時間用
static Uint32 prvTickCount;
static Uint32 nowTick;
//static int frame;

#define INTERVAL_BASE 16


void FunctionInit( void )
{
	int i;
	
	prvTickCount = 0;
	
	initPAD( );
	soundInitBuffer();

}

void ResetGameFlag( void )
{
	memset( &gameflag[0], 0, sizeof( gameflag ) );
}

void ResetGameFlag2( void )
{
	memset( &gameflag2[0], 0, sizeof( gameflag ) );
}

/* These two are unused */

/*Sint32 GetConfig( char* fn, char* cParam )
{
	FILE *fp;	
	char *sp;
	char s[256];
	char s2[256];
	memset( s, '\0', sizeof( s ) );
	Sint32 rc;
	
	rc = 0;

#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
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
*/

/*Sint32 LogFileWrite( char* fn, char* cParam )
{
	FILE *fp;	
	Sint32 rc;
	
	rc = 0;

#ifdef _TINSPIRE
	char buf[192];
	snprintf(buf, sizeof(buf), "%s.tns", fn);
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
	fclose(fp);
	return( rc );
}*/



void FPSWait( void )
{
	Uint32 leftTick;

	//サウンドの再生
	soundPlayCtrl( );
	Input_PollEvent();
	Input_Update();
/*
	nowTick = Renderer_GetTicks();
	frame = (nowTick - prvTickCount) / INTERVAL_BASE;
	if(frame <= 0){
#ifdef GP2X
		wait(prvTickCount + interval - nowTick);
#else
		Renderer_Delay(prvTickCount + INTERVAL_BASE - nowTick);
#endif
	}
	prvTickCount = Renderer_GetTicks();
*/

	if(prvTickCount == 0) prvTickCount = Renderer_GetTicks();
	
	while (true)
	{
	 	nowTick = Renderer_GetTicks();
   		leftTick = prvTickCount + INTERVAL_BASE - nowTick;
   		if(leftTick < 1 || leftTick > 9999)
   		{
 			break;
 		}
 		Renderer_Delay(1);
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
	return Input_SystemKeys();
}

int Set_Volume( int vol )
{
	int rc = 0;
	
	soundSetVolumeBgm( vol, 0 );
	soundSetVolumeAll( vol );
	
	return( rc );
}

/*[ BltNumericImage ]************************************************/
/*	���l�摜�\���֐�												*/
/*-[����]-----------------------------------------------------------*/
/*	value			(i )	�摜�\�����鐔�l						*/
/*	length			(i )	�\�����錅���i�����ȏ�͕\������Ȃ��j	*/
/*	x				(i )	�摜��\������ʒu�����W				*/
/*	y				(i )	�摜��\������ʒu�����W				*/
/*	plane			(i )	���l�摜���ǂݍ��܂�Ă���v���[���ԍ�	*/
/*	num_stpos_x		(i )	�v���[�����ł̐��l�摜�̊J�n�ʒu�����W	*/
/*	num_stpos_y		(i )	�v���[�����ł̐��l�摜�̊J�n�ʒu�����W	*/
/*	num_width		(i )	���l�摜�P�����̕��i�h�b�g���j			*/
/*	num_height		(i )	���l�摜�P�����̍����i�h�b�g���j		*/
/*-[�߂�l]---------------------------------------------------------*/
/*	����															*/

void BltNumericImage( Sint32 value, Sint32 length, Sint32 x, Sint32 y, Sint32 plane, Sint32 num_stpos_x, Sint32 num_stpos_y, Sint32 num_width, Sint32 num_height )
{
	Sint32 blt_num;	// �P���̐��l���i�[����
	Sint32 i;			// ��������for���[�v�Ŏg�p
	Sint32 dv;		// ����Z�Ŏg�p����l

	// value �����̒l�̏ꍇ�A���̒l�ɒu��������
	if ( value < 0 )
	{
		value = value * -1;
	}

	// �ŏ��̊���Z�Ŏg�p����l�����߂�
	dv = 1;
	for( i = 1; i < length; i++ )
	{
		dv = dv * 10;
	}

	// �w�肳�ꂽ�������̐����摜��]������
	for( i = 0; i < length; i++ )
	{
		// �\�����鐔�������߂�
		blt_num = value / dv;
		value = value - blt_num * dv;
		if ( blt_num > 9 )
		{	// �\���������P���̐��l�ɂȂ�Ȃ���΁A�P���ɂ���B
			blt_num = blt_num % 10;
		}
		// �����摜�]��
		BltRect( plane, x + (num_width * i), y, num_stpos_x + (num_width * blt_num), num_stpos_y, num_width, num_height );
		// ����Z�Ŏg�p����l��10�Ŋ���
		dv = dv / 10;
	}

	return;
}

/*[ BltNumericImage2 ]************************************************/
/*	���l�摜�\���֐��i�E�l�߁j										*/
/*-[����]-----------------------------------------------------------*/
/*	value			(i )	�摜�\�����鐔�l						*/
/*	length			(i )	�\�����錅���i�����ȏ�͕\������Ȃ��j	*/
/*	x				(i )	�摜��\������ʒu�����W				*/
/*	y				(i )	�摜��\������ʒu�����W				*/
/*	plane			(i )	���l�摜���ǂݍ��܂�Ă���v���[���ԍ�	*/
/*	num_stpos_x		(i )	�v���[�����ł̐��l�摜�̊J�n�ʒu�����W	*/
/*	num_stpos_y		(i )	�v���[�����ł̐��l�摜�̊J�n�ʒu�����W	*/
/*	num_width		(i )	���l�摜�P�����̕��i�h�b�g���j			*/
/*	num_height		(i )	���l�摜�P�����̍����i�h�b�g���j		*/
/*-[�߂�l]---------------------------------------------------------*/
/*	����															*/
/********************************************************************/
void BltNumericImage2( Sint32 value, Sint32 length, Sint32 x, Sint32 y, Sint32 plane, Sint32 num_stpos_x, Sint32 num_stpos_y, Sint32 num_width, Sint32 num_height )
{
	Sint32 blt_num;	// �P���̐��l���i�[����
	Sint32 i;			// ��������for���[�v�Ŏg�p
	Sint32 dv;		// ����Z�Ŏg�p����l
	Sint32 x_hosei;	//�E�l�ߕ␳�l
	int buf;
	int t = 1;
	
	// value �����̒l�̏ꍇ�A���̒l�ɒu��������
	if ( value < 0 )
	{
		value = value * -1;
	}
	
	/* 2002.10.21 D.K start */
	buf = value;
	
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
	
	// �ŏ��̊���Z�Ŏg�p����l�����߂�
	dv = 1;
	for( i = 1; i < length; i++ )
	{
		dv = dv * 10;
	}

	// �w�肳�ꂽ�������̐����摜��]������
	for( i = 0; i < length; i++ )
	{
		// �\�����鐔�������߂�
		blt_num = value / dv;
		value = value - blt_num * dv;
		if ( blt_num > 9 )
		{	// �\���������P���̐��l�ɂȂ�Ȃ���΁A�P���ɂ���B
			blt_num = blt_num % 10;
		}
		// �����摜�]��
		BltRect( plane, x + (num_width * i), y, num_stpos_x + (num_width * blt_num), num_stpos_y, num_width, num_height );
		// ����Z�Ŏg�p����l��10�Ŋ���
		dv = dv / 10;
	}

	return;
}

Sint32 funcSin( Sint32 rdo )
{
	Sint32 ang;
	Sint32 rc = 0;
	
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
Sint32 funcCos( Sint32 rdo )
{
	Sint32 ang;
	Sint32 rc = 0;
	
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


Sint32 funcTan2( Sint32 posX, Sint32 posY )
{
	Sint32 rc = 0;
	
	rc = MOTatan( ( posX ) * 256, ( posY * -1 ) * 256 );
	
	return( rc );
}

Sint32 get2keta( Sint32 val, Sint32 st )
{
	Sint32 rc = 0;
	
	val = val / st;
	rc = val % 100;
	
	return( rc );
}

