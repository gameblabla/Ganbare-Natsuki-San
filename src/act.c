#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "function.h"
#include "audio/audio.h"
#include "extern.h"
#include "act.h" 
#include "dconv.h" 
#include "general.h" 
#include "renderer/renderer.h"
#include "filesystem/filesystem.h"
#include "platform.h"

#include "refresh.h"

void act_main( void );
void act_init( void );
void act_relese( void );
void act_keys( void );
void fram_set( );
void jmp_st( );
void g_jmp( );
void idou_l( Sint32 ankr_flag );
void idou_r( Sint32 ankr_flag );
void cleaer_ankr( Sint32 ankr_flag );
void act_drow( void );
void save_file_w( );
void save_file( );
void clear_save_file( );
void playerdisp( );
void bakdisp1();
void bakdisp2( );
void bak_make( );
void wait_scl( Sint32 type, Sint32 d_num );
void wait_scl2( Sint32 d_num ,Sint32 init_x, Sint32 init_y );
void wait_scl3( );
void snack_counter_disp();
void minimap_disp( void );
void mapdisp();
void bgm_midi( Sint32 BgmWavNo );
void stage_start( );
void stage_endj( );
void stage_claer( );
void stage_end( );
void p_shot_set( Sint32 type , Sint32 muki, Sint32 houkou );
void p_shot_disp( );
void p_shot_f_set( Sint32 x, Sint32 y , Sint32 type );
void p_shot_f_disp( );
void p_shot_f2_set( Sint32 x, Sint32 y , Sint32 muki );
void p_shot_f2_disp( );
void item_set( Sint32 type, Sint32 x, Sint32 y, Sint32 iti, Sint32 g_num );
Sint32 item_sonzai( Sint32 map1, Sint32 map2 );
void item_disp( );
Sint32 item_up( );
Sint32 item_up2( );
Sint32 item_event( );
void init_event_set_enm( Sint32 event_no , Sint32 b_num );
void init_event_set( Sint32 event_no , Sint32 b_num );
void event_set( Sint32 event_no , Sint32 b_num );
void event_set_enm( Sint32 event_no , Sint32 b_num );
void event_disp( );
void event_save( );
void load_save_file( );
void event_kaihuku( );
void event_return( );
void event_warp( );
void enm_set( Sint32 type, Sint32 x, Sint32 y );
void enm_set2( Sint32 type, Sint32 x, Sint32 y ,Sint32 muki );
Sint32 e_hp( Sint32 type );
void enm_disp();
void e_move( Sint32 i );
void boss_dead( Sint32 x, Sint32 y );
void e_shot_set( Sint32 type , Sint32 i , Sint32 x, Sint32 y );
void e_shot_disp( );
void enmjmp( Sint32 i );
Sint32 map1_item_j( Sint32 x, Sint32 y );
void kane_set( Sint32 x, Sint32 y );
void kane_disp(  );
void k_jmp( Sint32 i );
void p_jump_f_set( Sint32 x, Sint32 y );
void p_jump_f_disp( );
void screen_BackupPlane( );
void window_disp( );
void window_keys( );
Sint32 labs_gnp( Sint32 para );
void p_mgc_set( Sint32 muki , Sint32 p3 );
void p_mgc_disp( );
void BlendExBltRectW( Sint32 plane_no, Sint32 x, Sint32 y, Sint32 sx, Sint32 sy, Sint32 hx, Sint32 hy, Sint32 func, Sint32 blend );
void BlendExBltW( Sint32 plane_no, Sint32 x, Sint32 y, Sint32 func, Sint32 blend );
void BlendExBltFastW( Sint32 plane_no, Sint32 x, Sint32 y, Sint32 func, Sint32 blend );
void replay_input_save( void );
void replay_input_load( void );
void replay_file_save_w( void );
void replay_file_save( Sint32 type );
Sint32 replay_file_load( void );
Sint32 GetKyori( Sint32 i1, Sint32 i2 );
void debugdisp( void );

/* Size was 1024, reduce it to 30 as that's the minimum required here. - Gameblabla */
static char string[30];

static Sint32 scene_exit;
static Sint32 sn = 0;									/* ��ʗp�ϐ������ς��邱�Ƃɂ���ĕ`��֐����̏�����ς���		*/

static Sint32 enm1_parameter[10 * 20] = 
{
/*   HP */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 0���g�p */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 1 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 2 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 3 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 4 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 5 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 6 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 7 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 8 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 9 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 10 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 11 */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 12 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0 	/* 20���g�p */
};

static Sint32 enm2_parameter[10 * 20] = 
{
/*   HP */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 0���g�p */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 1 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 2 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 3 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 4 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 5 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 6 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 7 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 8 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 9 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0 	/* 20���g�p */
};

static Sint32 enm3_parameter[10 * 20] = 
{
/*   HP   �� �U�� ���� ���� */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 0���g�p */
	  1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 1 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 2 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 3 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 4 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 5 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 6 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 7 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 8 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 9 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0 	/* 20���g�p */
};

static Sint32 enm4_parameter[10 * 20] = 
{
/*   HP */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 0���g�p */
	  7,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 1�{�X */
	  7,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 2 */
	 30,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 3 */
	 27,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 4 */
     42,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 5 */
     30,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 6 */
      1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 7 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 8 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 	/* 9 */
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0 	/* 20���g�p */
};

static Sint32 item_data[3] = { 32, 500, 10 };			/* �A�C�e���̏��@�T�C�Y�A�ő�z�u���A1�f�[�^�o�b�t�@�� */
static Sint32 enm_data[3] = { 0, 500, 20 };			/* �G�̏��@�\���A�ő�z�u���A1�f�[�^�o�b�t�@�� */
static Sint32 MapInfo[10] = 
{
	20 * 1									/* 0:�}�b�v��x�T�C�Y */
,	15 * 1									/* 1:�}�b�v��y�T�C�Y */
,	( 20 * 1 ) * 32							/* 2:�}�b�v��x�\���T�C�Y */
,	( 15 * 1 ) * 32							/* 3:�}�b�v��y�\���T�C�Y */
,   20										/* 4:�w�b�_�f�[�^���̃f�[�^�� */
};

static Sint32 player[50];									/* �v���C���[�p�o�b�t�@ */
											/* player[0] X */
											/* player[1] Y */
											/* player[2] ��� */
											/* player[3] ���� */
											/* player[4] BMP���̕\���ʒuX */
											/* player[5] BMP���̕\���ʒuY */
											/* player[6] �͂ݒ��̈ړ��␳X */
											/* player[7] �͂ݒ��̈ړ��␳Y */
											/* player[8] ����HP */
											/* player[9] �G��͂�ł��� */
											/* player[10] �A�j���[�V�����t���[���J�E���^ */
											/* player[11] �A�j���[�V�����J�E���^ */
											/* player[12] �A�j���[�V�����t���[���w�� */
											/* player[13] �W�����v�t���[���J�E���^ */
											/* player[14] �W�����v�J�E���^ */
											/* player[15] �~�X�̏ꍇ��X */
											/* player[16] �~�X�̏ꍇ��Y */
											/* player[17] ���G���� */
											/* player[18] ���n���[�V�����p�J�E���^ */
											/* player[19] �͂񂾎��_�ł̋��� */
											/* player[20] �r�̕��� */
											/* player[21] �r�̋��� */
											/* player[22] ��]���� */
											/* player[23] �͂ރt���O */
											/* player[24] �A���J�[��Ԃ���W�����v���� */
											/* player[25] �{�^���������ăW�����v�����t���O */
											/* player[26] �����ԗ��������t���O�i���n�t�F�N�g�p�j */
											/* player[27] �͂񂾂Ƃ��ɓ��͂���Ă����L�[ */
											/* player[28] �ړ������[�V���� */
											/* player[29] ���΂ː�����ђ� */
											/* player[30] �A���J�[X */
											/* player[31] �A���J�[Y */
											/* player[32] �A���J�[�݈͂ʒuX */
											/* player[33] �A���J�[�݈͂ʒuY */
											/* player[34] �A���J�[�������X */
											/* player[35] �A���J�[�������Y */
											/* player[36] �A���J�[������ѕ��� */
											/* player[37] �A�C�e����͂�ł��� */
											/* player[38] �A���J�[��L�΂���(���ʉ��p) */
static Sint32 dp_x;									/* �v���C���[�̕\���ʒu�i��ʓ��̑��΍��W�j */
static Sint32 dp_y;									/* �v���C���[�̕\���ʒu�i��ʓ��̑��΍��W�j */
static Sint32 demo[10];
static Sint32 map1[320];								/* �w�i�p�o�b�t�@ ���C���[1 */
static Sint32 map2[320];								/* �w�i�p�o�b�t�@ ���C���[2 */
static Sint32 map_data[20];
static Sint32 bak_cnt = 0;							/* �w�i�A�j���[�V�����p�J�E���^ */
static Sint32 d_x = 0;								/* �w�i�\���ʒu */
static Sint32 d_y = 0;								/* �w�i�\���ʒu */
//static Sint32 disp_data[60];							/* �Z�[�u�ꗗ */
static Sint32 next_px = 0;							/* ��ʐ؂�ւ����̃L�����ړ��΍� */
static Sint32 next_py = 0;

static Sint32 atk_rr1 =22;						/* �r�̉�]���x */
static Sint32 atk_rr2 = 1;						/* �r�̉�]���x */
//Sint32 atk_rr = 360 / 32;						/* �r�̉�]���x */

static Sint32 stage = 0;								/* �X�e�[�W�ԍ� */

static Sint32 pshot[10 * 10];							/* �v���C���[�U���p��� */
static Sint32 pshotf[10 * 40];						/* �v���C���[�U���p��� */
static Sint32 pshotf2[10 * 80];						/* �v���C���[�U���p��� */
static Sint32 pmshot[20 * 40];						/* �v���C���[�U���p��� */
static Sint32 pjumpf[100];							/* �v���C���[�U���p��� */
static Sint32 item[10 * 50];							/* �A�C�e���p�o�b�t�@ */
static Sint32 item_wk[5*100];							/* �n�� ���̑��̃}�b�v��� */
static Sint32 enemy[40 * 20];								/* �G���															*/
static Sint32 mgc_e[10 * 20] ;						/* MGC�\�����														*/
static Sint32 p_ef[1000] ;							/* �ėp�o�b�t�@�i���Փ��j														*/
//static Sint32 life_a[5];								/* HP�A�j���[�V�����p */
static Sint32 kane[200];								/* �A�C�e���p�o�b�t�@ */

static Sint32 Jump_counts = 0;
static Sint32 save_data[400];							/* �ۑ���� */
static Sint32 test[5];
static Sint32 rayer[5];								/* �w�i�X�N���[���� */
static Sint32 play_time[5];
/* ���v���C�Q�O�� */

#define MAX_REPLAY_SIZE 60 * 60 * 10
#define SUPER_MAX_REPLAY_SIZE 60 * 60 * 20

static Sint32 replay[MAX_REPLAY_SIZE];	/* �t���[�����b���� */
static Sint32 replay_time = 1;
static Sint32 replay_time_MAX = MAX_REPLAY_SIZE;	/* �ő厞�� */
static Sint32 replay_load_key[10];
static Sint32 replay_save_flag = 0;				/* �ۑ����s���� 1:���Ȃ� */
static Sint32 rrkey[5];								/* ���̓L�[���� */

//static Sint32 event_buff[100];
/* event_buff[0]	�����p����޳�\�� */
/* event_buff[1]	��\�� */
/* event_buff[2]	�����͑҂� */
/* event_buff[3]	wait */
/* event_buff[4]	���s�C�x���g�o�b�t�@�|�C���^ */
/* event_buff[5]	��\�� */
/* event_buff[7]	i */
/* event_buff[8]	player[2] */
/* event_buff[9]	player[3] */
/* event_buff[10]	��ʍX�V */
/* event_buff[11]	��ʉ��o */

/* TextLayer( 10 )�Ͳ���ĂŎg�p�i�Œ�j */
/* �v���[��80�`90�Ͳ���ĂŎg�p */
/* wav30�Ͳ���ĂŎg�p */
//static Sint32 file_handle = 0	;
static Sint32 one_event = 0;							/* �d�Ȃ��Ĕ�������C�x���g�̑O��d�Ȃ��Ă������̃t���O */
//static Sint32 start_plyer_disp = 1;					/* �J�n���ɕ\�����邩�t���O */
//static Sint32 start_effct_disp = 0;					/* �J�n���Ɍ��ʂ�\�����邩�t���O */

#define DEBUG_DISP (0)

//----------------------------------------------------------------------
// ���C���֐�
#ifndef GNS_FRAME_STEPPED
void act_main( void )
{
	Sint32 exit_code;
	
	act_init( );		// ������
	
	while( scene_exit )
	{
//		KeyInput( );	// �L�[����
		
//		act_keys( );		// �L�[����
		act_drow( );		// �`��
		
		FPSWait( );		// �҂�

		exit_code = system_keys( );  //
		if ( exit_code == 0 )
		{
			scene_exit = 0;
		}
	}
	
	act_relese( );		// �I��
}

#else
int act_step( void )
{
	static int active = 0;
	Sint32 exit_code;

	if ( ! active )
	{
		act_init( );
		active = 1;
	}

	if ( scene_exit )
	{
		act_drow( );
		FPSWait( );

		exit_code = system_keys( );
		if ( exit_code == 0 )
		{
			scene_exit = 0;
		}
	}

	if ( ! scene_exit )
	{
		act_relese( );
		active = 0;
		return 0;
	}

	return 1;
}
#endif

#ifdef DREAMCAST
/* Yes; this mess is really required to avoid issues with music playback */
static int32_t old = 0;
#endif

void act_init( void )
{
	Sint32 i;
	Sint32 x;
	Sint32 y;
	Sint32 wk;
	Sint32 d_num;
	
	char path_item[96];
	char path_work[96];

	Filesystem_GetWorkSavePath(path_work, sizeof(path_work));
	Filesystem_GetWorkSavePath(path_work, sizeof(path_work));
	Filesystem_GetItemSavePath(path_item, sizeof(path_item));

	scene_exit = 1;
	sn = 0;
	bak_cnt = 0;							/* �w�i�A�j���[�V�����p�J�E���^ */
	d_x = 0;								/* �w�i�\���ʒu */
	d_y = 0;								/* �w�i�\���ʒu */
	next_px = 0;							/* ��ʐ؂�ւ����̃L�����ړ��΍� */
	next_py = 0;
	atk_rr1 =22;						/* �r�̉�]���x */
	atk_rr2 = 1;						/* �r�̉�]���x */
	stage = 0;								/* �X�e�[�W�ԍ� */
	Jump_counts = 0;
	replay_time = 1;
	replay_time_MAX = SUPER_MAX_REPLAY_SIZE;	/* �ő厞�� */
	replay_save_flag = 0;				/* �ۑ����s���� 1:���Ȃ� */
	one_event = 0;							/* �d�Ȃ��Ĕ�������C�x���g�̑O��d�Ȃ��Ă������̃t���O */

//#if false
	memset( player, 0, sizeof( player ) );									/* �v���C���[�p�o�b�t�@ */
	memset( demo, 0, sizeof( demo ) );
	memset( map1, 0, sizeof( map1 ) );								/* �w�i�p�o�b�t�@ ���C���[1 */
	memset( map2, 0, sizeof( map2 ) );								/* �w�i�p�o�b�t�@ ���C���[2 */
	memset( map_data, 0, sizeof( map_data ) );
	memset( pshot, 0, sizeof( pshot ) );							/* �v���C���[�U���p��� */
	memset( pshotf, 0, sizeof( pshotf ) );						/* �v���C���[�U���p��� */
	memset( pshotf2, 0, sizeof( pshotf2 ) );						/* �v���C���[�U���p��� */
	memset( pmshot, 0, sizeof( pmshot ) );						/* �v���C���[�U���p��� */
	memset( pjumpf, 0, sizeof( pjumpf ) );							/* �v���C���[�U���p��� */
	memset( item, 0, sizeof( item ) );							/* �A�C�e���p�o�b�t�@ */
	memset( item_wk, 0, sizeof( item_wk ) );							/* �n�� ���̑��̃}�b�v��� */
	memset( enemy, 0, sizeof( enemy ) );								/* �G���															*/
	memset( mgc_e, 0, sizeof( mgc_e ) ) ;						/* MGC�\�����														*/
	memset( p_ef, 0, sizeof( p_ef ) ) ;							/* �ėp�o�b�t�@�i���Փ��j														*/
	memset( kane, 0, sizeof( kane ) );								/* �A�C�e���p�o�b�t�@ */
	memset( save_data, 0, sizeof( save_data ) );							/* �ۑ���� */
	memset( test, 0, sizeof( test ) );
	memset( rayer, 0, sizeof( rayer ) );								/* �w�i�X�N���[���� */
	memset( play_time, 0, sizeof( play_time ) );
	memset( replay, 0, sizeof( replay ) );	/* �t���[�����b���� */
	memset( replay_load_key, 0, sizeof( replay_load_key ) );
	memset( rrkey, 0, sizeof( rrkey ) );								/* ���̓L�[���� */
	memset( string, 0, sizeof( string ) );								/* ���̓L�[���� */

	ResetGameFlag2();				/* �t�@�C���Ȃ� */
	for ( i = 0; i < 500; i++ )
	{
		item_wk[i] = gameflag2[i];
	}
	
	ResetGameFlag2( );

	Platform_LoadCustom(path_work, "gan_work.sav");
	Platform_LoadCustom(path_item, "gan_item.sav");

	LoadGameFlag2(path_work);
	
	for ( i = 0; i < 400; i++ )
	{
		save_data[i] = gameflag2[i];
	}
	player[8] = 1;	/* HP */
	stage = save_data[2];
	d_num = save_data[3]; 	/* ��� */
	gameflag[120] = d_num;


	player[20] = 0;
	player[21] = 32;
	player[22] = 1;

	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", ( int )stage , ( int )1, ( int )d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( i = 0; i <= 319; i++ )
		{
			map1[i] = gameflag2[i];
		}
	}
	else 
	{
		if ( gameflag[132] == 1 )
		{
			g_scene = EN_SN_TITLE;
		}
		else 
		{
			g_scene = EN_SN_ENDING;	
		}
		sn = 10;
	}
/* Yes, this ugly mess is to stop the Dreamcast (or rather the ADX driver) from crashing */
#ifdef DREAMCAST
	if ( gameflag[70] == 1 )
	{
		if (
		     ( ( save_data[3] >  0 ) && ( save_data[3] <=  5 ) ) 
		  || ( ( save_data[3] > 20 ) && ( save_data[3] <= 25 ) )
		  || ( ( save_data[3] > 40 ) && ( save_data[3] <= 45 ) )
		   )
		{
			Stop_Music();
		}
		else if (
		     ( ( save_data[3] > 45 ) && ( save_data[3] <= 50 ) )
		   )
		{
			Stop_Music();
		}
		if (
		     ( ( save_data[3] >  5 ) && ( save_data[3] <= 10 ) ) 
		  || ( ( save_data[3] > 25 ) && ( save_data[3] <= 30 ) )
		  || ( ( save_data[3] > 35 ) && ( save_data[3] <= 40 ) )
		   )
		{
			Stop_Music();
		}
		else if (
		     ( ( save_data[3] > 10 ) && ( save_data[3] <= 15 ) ) 
		  || ( ( save_data[3] > 30 ) && ( save_data[3] <= 35 ) )
		   )
		{
			Stop_Music();
		}
		else if (
		     ( ( save_data[3] > 15 ) && ( save_data[3] <= 20 ) ) 
		  || ( ( save_data[3] > 35 ) && ( save_data[3] <= 40 ) )
		   )
		{
			Stop_Music();
		}
		else
		{
			Pause_Music();
		}
	}
	else 
	{
		if ( gameflag[71] == 0 )
		{
			if ( (save_data[3] ==  1) || (save_data[3] == 21) || (save_data[3] == 41 ))
			{
				Stop_Music();
			}
			else if (save_data[3] == 46)
			{
				Stop_Music();
			}
			else if (
			     ( ( save_data[3] ==  6 ) ) 
			  || ( ( save_data[3] == 26 ) )
			   )
			{
				Stop_Music();
			}
			else if (
			     ( ( save_data[3] == 11 ) ) 
			  || ( ( save_data[3] == 31 ) )
			   )
			{
				Stop_Music();
			}
			else if (
			     ( ( save_data[3] == 16 ) ) 
			  || ( ( save_data[3] == 36 ) )
			   )
			{
				Stop_Music();
			}
			else
			{
				Pause_Music();
			}
		}
	}
	old = save_data[3];
#endif
	
	for ( i = 0 ; i < 20 ; i++ )
	{
		map_data[i] = gameflag2[i + 320];
	}
	
	for ( i = 0 ; i < 5 ; i++ )
	{
		if ( map_data[11 + i] != 0 )
		{
			sprintf(string,BAK_IMAGE_PATH "%d.bmp", ( int )map_data[11 + i] );
			if ( LoadBitmap(string,110 + i , 0 ) != 0 )	
			{
				ReleaseBitmap(110 + i);
			}
		}
	}

	player[0] = map_data[9] * 32;
	player[1] = ( map_data[10] * 32 ) - 16;
	
	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", ( int )stage , ( int )2, ( int )d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( i = 0; i <= 319; i++ )
		{
			map2[i] = gameflag2[i];
		}
	}

	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "i%d_%d.map", ( int )stage , ( int )1, ( int )d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( x = 0; x <= 19 ; x++ )
		{
			for ( y = 0; y <= 16 ; y++ )
			{
				if ( gameflag2[x + ( y * 20 )] != 0 )
				{
					item_set( gameflag2[x + ( y * 20 )], x * 32, ( ( 14 - y ) * 32 ) + 16, x + ( y * 20 ), d_num );
				}
			}
		}
	}


	
	ResetGameFlag2();				/* �t�@�C���Ȃ� */
	sprintf( string,MAP_DATA_PATH "e%d_%d.map", ( int )stage , ( int )1, ( int )d_num );
	if ( LoadGameFlag2( string ) == 0 )	/* �G�z�u */
	{
		/* �G�z�u */
		for ( x = 0; x <= 19 ; x++ )
		{
			for ( y = 0; y <= 16 ; y++ )
			{
				if ( gameflag2[x + ( y * 20 )] != 0 )
				{
					enm_set( gameflag2[x + ( y * 20 )], x * 32, ( ( 14 - y ) * 32 ) + 16 );
				}
			}
		}
	}
	

	player[14] = 10;
	player[15] = player[0];
	player[16] = player[1];
	player[2] = 0;

	if ( gameflag[132] == 1 )
	{
		if ( 1 != replay_file_load( ) )
		{
			g_scene = EN_SN_TITLE;	/* �^�C�g���� */
			sn = 10;
			return;
		}
	}
	else 
	{
	}

	if ( gameflag[127] == 1 )	//�g�[�^���A�^�b�N
	{
		wk = get2keta( gameflag[135] , 1 );
		play_time[0] = play_time[0] + wk;
		if ( play_time[0] >= 60 )
		{
			play_time[0] = play_time[0] - 60;
			play_time[1]++; 
		}
		wk = get2keta( gameflag[135] , 100 );
		play_time[1] = play_time[1] + wk;
		if ( play_time[1] >= 60 )
		{
			play_time[1] = play_time[1] - 60;
			play_time[2]++; 
		}
		
		wk = get2keta( gameflag[135] , 10000 );
		play_time[2] = play_time[2] + wk;
		if ( play_time[2] >= 999 )
		{
			play_time[2] = 999;
			play_time[1] = 59;
			play_time[0] = 59;
		}
		
		if ( gameflag[40] == 5 )
		{
			if ( gameflag[137] > gameflag[135] )
			{
				gameflag[137] = gameflag[135];
			}
			if ( gameflag[138] > gameflag[136] )
			{
				gameflag[138] = gameflag[136];
			}
		}
	}

	sprintf(string,PLAYER_IMAGE_PATH "ri%d.bmp", ( int )gameflag[122] );
	LoadBitmap(string,1,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(PLAYER_IMAGE_PATH "mgc1.bmp",2,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(PLAYER_IMAGE_PATH "atk1.bmp",3,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(PLAYER_IMAGE_PATH "atk2.bmp",4,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���

	LoadBitmap(PLAYER_IMAGE_PATH "atkf.bmp",5,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���

	sprintf(string,MAP_IMAGE_PATH "map%d_1.bmp", ( int )stage );
	
	printf("Map = %s \n", string);
	
	LoadBitmap(string,10,true);				
	sprintf(string,MAP_IMAGE_PATH "map%d_2.bmp", ( int )stage );
	LoadBitmap(string,11,true);
	sprintf(string,MAP_IMAGE_PATH "item%d.bmp", ( int )stage );
	LoadBitmap(string,12,true);


	LoadBitmap(SYS_IMAGE_PATH "map_f0.bmp",15,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "map_f1.bmp",16,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "map_f2.bmp",17,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "fonts2.bmp",18,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "sc.bmp",19,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���

	LoadBitmap(SYS_IMAGE_PATH "p_data.bmp",20,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "skill.bmp",21,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "hp.bmp",22,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "hp2.bmp",23,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	
	LoadBitmap(SYS_IMAGE_PATH "pause.bmp",24,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	
	LoadBitmap(ENM_IMAGE_PATH "e32.bmp",40,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(ENM_IMAGE_PATH "e48.bmp",41,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(ENM_IMAGE_PATH "e64.bmp",42,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	
	LoadBitmap(ENM_IMAGE_PATH "e_shot.bmp",50,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���

	LoadBitmap(PLAYER_IMAGE_PATH "font_d.bmp",51,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(PLAYER_IMAGE_PATH "hp.bmp",52,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	

	LoadBitmap(SYS_IMAGE_PATH "menu.bmp",60,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(TITLE_IMAGE_PATH "scl.bmp",61,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "window_item.bmp",62,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���

	LoadBitmap(PLAYER_IMAGE_PATH "arm.bmp",65,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(PLAYER_IMAGE_PATH "arm2.bmp",66,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "input.bmp",67,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "new.bmp",68,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "new2.bmp",69,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(SYS_IMAGE_PATH "mini_map.bmp",70,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
	LoadBitmap(BAK_IMAGE_PATH "0.bmp",72,true);					//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���

	/*LoadBitmap(SYS_IMAGE_PATH "waku.bmp",109,true);*/				
	LoadBitmap(IMAGE_PATH "color.bmp",127,false);

	if ( gameflag[70] == 1 )
	{
		if (
		     ( ( save_data[3] >  0 ) && ( save_data[3] <=  5 ) ) 
		  || ( ( save_data[3] > 20 ) && ( save_data[3] <= 25 ) )
		  || ( ( save_data[3] > 40 ) && ( save_data[3] <= 45 ) )
		   )
		{
			soundPlayBgm( EN_BGM_GAME02 );
		}
		else if (
		     ( ( save_data[3] > 45 ) && ( save_data[3] <= 50 ) )
		   )
		{
			if ( stage == 2 )
			{
				soundPlayBgm( EN_BGM_GAME07 );
			}
			else 
			{
				soundPlayBgm( EN_BGM_GAME06 );
			}
		}
		if (
		     ( ( save_data[3] >  5 ) && ( save_data[3] <= 10 ) ) 
		  || ( ( save_data[3] > 25 ) && ( save_data[3] <= 30 ) )
		  || ( ( save_data[3] > 35 ) && ( save_data[3] <= 40 ) )
		   )
		{
			soundPlayBgm( EN_BGM_GAME03 );
		}
		else if (
		     ( ( save_data[3] > 10 ) && ( save_data[3] <= 15 ) ) 
		  || ( ( save_data[3] > 30 ) && ( save_data[3] <= 35 ) )
		   )
		{
			soundPlayBgm( EN_BGM_GAME04 );
		}
		else if (
		     ( ( save_data[3] > 15 ) && ( save_data[3] <= 20 ) ) 
		  || ( ( save_data[3] > 35 ) && ( save_data[3] <= 40 ) )
		   )
		{
			soundPlayBgm( EN_BGM_GAME05 );
		}
	}
	else 
	{
		if ( gameflag[71] == 0 )
		{
			if ( (save_data[3] ==  1) || (save_data[3] == 21) || (save_data[3] == 41 ))
			{
				soundPlayBgm( EN_BGM_GAME02 );
			}
			else if (save_data[3] == 46)
			{
				if ( stage == 2 )
				{
					soundPlayBgm( EN_BGM_GAME07 );
				}
				else 
				{
					soundPlayBgm( EN_BGM_GAME06 );
				}
			}
			else if (
			     ( ( save_data[3] ==  6 ) ) 
			  || ( ( save_data[3] == 26 ) )
			   )
			{
				soundPlayBgm( EN_BGM_GAME03 );
			}
			else if (
			     ( ( save_data[3] == 11 ) ) 
			  || ( ( save_data[3] == 31 ) )
			   )
			{
				soundPlayBgm( EN_BGM_GAME04 );
			}
			else if (
			     ( ( save_data[3] == 16 ) ) 
			  || ( ( save_data[3] == 36 ) )
			   )
			{
				soundPlayBgm( EN_BGM_GAME05 );
			}
		}
	}
	gameflag[71] = 0;
	gameflag[70] = 0;
#ifdef DREAMCAST
	Resume_Music();
#endif
}

void act_relese( void )
{
	Sint32 i;
	
	save_file_w( );

	for ( i = 0; i < BMPBUFF_MAX; i++ )
	{
		ReleaseBitmap( i );
	}
//	soundStopBgm(EN_BGM_GAME01);

}


void act_keys( void )
{
	Sint32 i;
	Sint32 old_x;
	Sint32 muki;
	Sint32 muki2;
	Sint32 px;
	Sint32 py;
	Sint32 kaiten;
	Sint32 rr;
	Sint32 rg;
	Sint32 ii;
	
	if ( sn == 1 )
	{
		play_time[0] = play_time[0] + 1;
		if ( play_time[0] >= 60 )
		{
			play_time[0] = 0;
			play_time[1] = play_time[1] + 1;
			if ( play_time[1] >= 60 )
			{
				play_time[1] = 0;
				play_time[2] = play_time[2] + 1;
				if ( play_time[2] >= 60 )
				{
					/* Always true due to previous condition - Gameblabla */
					//if ( play_time[2] >= 10 )
					{
						play_time[2] = 10;
					}
				}
			}
		}
	}
	if ( gameflag[132] == 0 )
	{
		replay_input_save( );		/* ���v���C�f�[�^�ۑ� */
	}
	else 
	{
		replay_input_load( );		/* ���v���C�f�[�^�Đ� */
	}
	replay_time++;
	if ( replay_time > replay_time_MAX )
	{
		replay_time = replay_time_MAX;	/* �ő厞�Ԃ��z�����A�ۑ����Ȃ� */
		replay_save_flag = 1;			/* �ۑ����Ȃ� */
	}

	for ( i = 0; i < 5; i++ )
	{
		rrkey[i] = 0;						/* ���̓L�[���� */
	}


	player[17]--;			/* ���G���� */
	if ( player[17] <= 0 )
	{
		player[17] = 0;
	}
	if ( player[9] == 0 )
	{
		g_jmp( );
	}
	
	
	if ( ( item_up( ) != -1 ) && ( player[14] == 10 ) )	/* �A�C�e���̏�ɏ���Ă��� */
	{
		player[1] = item_up( ); 
	}
	

	if ( player[2] == 31 )
	{
		if ( player[3] == 1 ) 
		{
//			player[0] = player[0] + 3;
//			idou_r( );
		}
		else 
		{
//			player[0] = player[0] - 3;
//			idou_l( );
		}
	}
	else 
	{
		old_x = player[0];
		
		if ( player[9] == 1 )
		{			
			muki = -1;
			if ( ( ( IsPressKey( gameflag[0] ) ) && ( gameflag[132] == 0 ) )		// ��
		  || ( ( replay_load_key[2] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				muki = 180;
			}
			else if ( ( ( IsPressKey( gameflag[1] ) ) && ( gameflag[132] == 0 ) )	// ��
		  || ( ( replay_load_key[3] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				muki = 0;
			}
			
			if ( ( ( IsPressKey( gameflag[2] ) ) && ( gameflag[132] == 0 ) )		// ��
		  || ( ( replay_load_key[0] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				if ( muki == 0 )
				{
					muki = 45;
				}
				else if ( muki == 180 )
				{
					muki = 135;
				}
				else 
				{
					muki = 90;
				}
			}
			else if ( ( ( IsPressKey( gameflag[3] ) ) && ( gameflag[132] == 0 ) )	// �E
		  || ( ( replay_load_key[1] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				if ( muki == 0 )
				{
					muki = 315;
				}
				else if ( muki == 180 )
				{
					muki = 225;
				}
				else 
				{
					muki = 270;
				}
			}
			
			if ( player[27] == muki )	/* �͂񂾂Ƃ��̓��͕����������Ă��� */
			{
				muki = -1;	/* �ړ����Ȃ� */
			}
			else 
			{
				player[27] = -1;
			}
			
			player[0] = player[32] - ( ( funcSin( player[20] ) * ( player[21] ) ) >> 16 );
			player[1] = player[33] - ( ( funcCos( player[20] ) * ( player[21] ) ) >> 16 );
		}
		else
		{
			if ( ( ( IsPressKey( gameflag[2] ) ) && ( gameflag[132] == 0 ) )		// ��
		  || ( ( replay_load_key[0] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				
				player[3] = 1;	/* ���� */
				if ( player[2] == 0 )
				{
					player[10] = 0;	/* �A�j���[�V�����Ԋu�t���[���ݒ� */
					player[11] = 0;	/* �A�j���[�V�����J�E���^ */
					player[2] = 1;	/* ���� */
				}
				else if ( player[2] == 1 )
				{
					player[0] = player[0] - 2;	/* �L�����N�^�[�ړ� */
				}
				else if ( player[2] == 21 )
				{
					player[0] = player[0] - 2;	/* �L�����N�^�[�ړ� */
				}

			}
			else if ( ( ( IsPressKey( gameflag[3] ) ) && ( gameflag[132] == 0 ) )	// �E
		  || ( ( replay_load_key[1] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				player[3] = 0;
				if ( player[2] == 0 )
				{
					player[10] = 0;
					player[11] = 0;
					player[2] = 1;
				}
				else if ( player[2] == 1 )
				{
					player[0] = player[0] + 2;
				}
				else if ( player[2] == 21 )
				{
					player[0] = player[0] + 2;
				}
			}
			else 
			{
				if ( player[2] == 1 )
				{
					player[10] = 0;
					player[11] = 0;
					player[2] = 0;
				}
			}
			
			muki = -1;
			if ( ( ( IsPressKey( gameflag[0] ) ) && ( gameflag[132] == 0 ) )		// ��
		  || ( ( replay_load_key[2] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				muki = 0;
			}
			else if ( ( ( IsPressKey( gameflag[1] ) ) && ( gameflag[132] == 0 ) )	// ��
		  || ( ( replay_load_key[3] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				muki = 180;
			}
			
			if ( ( ( IsPressKey( gameflag[2] ) ) && ( gameflag[132] == 0 ) )		// ��
		  || ( ( replay_load_key[0] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				if ( muki == 0 )
				{
					muki = 315;
				}
				else if ( muki == 180 )
				{
					muki = 225;
				}
				else 
				{
					muki = 270;
				}
			}
			else if ( ( ( IsPressKey( gameflag[3] ) ) && ( gameflag[132] == 0 ) )	// �E
		  || ( ( replay_load_key[1] == 1 ) && ( gameflag[132] == 1 ) ) )
			{
				if ( muki == 0 )
				{
					muki = 45;
				}
				else if ( muki == 180 )
				{
					muki = 135;
				}
				else 
				{
					muki = 90;
				}
			}
			if ( old_x > player[0] )
			{
				idou_l( 0 );
			}
			else if ( old_x < player[0] )
			{
				idou_r( 0 );
			}
		}
		
		muki2 = muki;
		
		if ( muki != -1 )	/* �L�[�����͂���Ă��� */
		{
			muki = muki - player[20];
			if ( muki < 0 )
			{
				muki = muki + 360;
			}
			
			if ( muki > 180 )
			{
				player[22] = -1;
			}
			else if ( ( muki < 180 ) && ( muki > 0 ) )
			{
				player[22] = 1;
			}
			
			
			
			if ( muki != 0 )
			{
				if ( player[9] == 0 )
				{
					for ( i = 0; i <= atk_rr1; i++ )
					{
						if ( player[20] == muki2 )
						{
							break;
						}
						player[20] = player[20] + ( player[22] * 1 );	/* �A���J�[�̈ړ� */
						if ( player[20] >= 360 )
						{
							player[20] = player[20] - 360;
						}
						if ( player[20] < 0 )
						{
							player[20] = player[20] + 360;
						}
					}
				}
				else
				{
					for ( i = 0; i <= atk_rr2; i++ )
					{
						if ( player[20] == muki2 )
						{
							break;
						}
						kaiten = player[20] + ( player[22] * 1 );	/* �A���J�[�̈ړ� */
						if ( kaiten >= 360 )
						{
							kaiten = kaiten - 360;
						}
						if ( kaiten < 0 )
						{
							kaiten = kaiten + 360;
						}
						px = ( ( funcSin( kaiten ) * ( player[21] * 1 ) ) >> 16 );	/* �A���J�[�ʒu�w */
						py = ( ( funcCos( kaiten ) * ( player[21] * 1 ) ) >> 16 );	/* �A���J�[�ʒu�x */
						if (
						   ( ( ( map1_item_j( player[0] + px + 16, player[1] + py + 16  ) >= 100 )
						    && ( map1_item_j( player[0] + px + 16, player[1] + py + 16  ) <= 119 ) )
						  || ( ( map1_item_j( player[0] + px + 16, player[1] + py + 16  ) >= 200 )
						    && ( map1_item_j( player[0] + px + 16, player[1] + py + 16  ) <= 219 ) ) )
						   )
						{
//							break;
						}
						player[20] = player[20] + ( player[22] * 1 );	/* �A���J�[�̈ړ� */
						if ( player[20] >= 360 )
						{
							player[20] = player[20] - 360;
						}
						if ( player[20] < 0 )
						{
							player[20] = player[20] + 360;
						}
					}
				}
				
	//			if ( ( player[22] == -1 ) && ( player[20] < muki2 ) )
	//			{
	//				player[20] = muki2;
	//			}
	//			else if ( ( player[22] == 0 ) && ( player[20] > muki2 ) )
	//			{
	//				player[20] = muki2;
	//			}
				
			}
		}
		
		player[30] = ( ( funcSin( player[20] ) * ( player[21] ) ) >> 16 );	/* �A���J�[�ʒu�w */
		player[31] = ( ( funcCos( player[20] ) * ( player[21] ) ) >> 16 );	/* �A���J�[�ʒu�x */
		

		
		
		if ( player[28] == 1 )	/* �ړ��� */
		{
			player[0] = player[0] + 2;
		}
		else if ( player[28] == 2 )
		{
			player[0] = player[0] - 2;
		}
		
		if ( player[29] > 0 )	/* �ړ��� */
		{
			player[0] = player[0] + ( player[29] * 1 );
			player[29] = player[29] - 1;
		}
		else if ( player[29] < 0 )
		{
			player[0] = player[0] + ( player[29] * 1 );
			player[29] = player[29] + 1;
		}
		/* �A���J�[������� */
/*		
		if ( player[34] > 0 )
		{
			player[0] = player[0] + ( ( funcSin( player[36] ) * ( player[34] ) ) >> 16 );
			player[34] = player[34] - 8;
			if ( player[34] < 0 )
			{
				player[34] = 0;
			}
		}
		if ( player[35] > 0 )
		{
			player[1] = player[1] + ( ( funcCos( ( 32768 / 360 ) * player[36] ) * ( player[35] ) ) >> 16 );
			player[35] = player[35] - 8;
			if ( player[35] < 0 )
			{
				player[35] = 0;
			}
		}
*/		
		
		
		if ( ( ( IsPressKey( gameflag[0] ) ) && ( gameflag[132] == 0 ) )
		  || ( ( replay_load_key[2] == 1 ) && ( gameflag[132] == 1 ) ) )
		{
			/* �����A���� */
			if ( ( player[2] == 0 ) || ( player[2] == 1 ) )
			{
				item_event( );
			}
		}
		else if ( ( ( IsPressKey( gameflag[1] ) ) && ( gameflag[132] == 0 ) )
		  || ( ( replay_load_key[3] == 1 ) && ( gameflag[132] == 1 ) ) )
		{
		}
		
		//	�X�y�[�X�L�[���������
		if ( ( ( IsPressKey( gameflag[4] ) )  && ( gameflag[132] == 0 ) ) 		/* �ړ����x�ቺ */
		  || ( ( replay_load_key[4] == 1 ) && ( gameflag[132] == 1 ) ) )
		{
			if ( player[24] == 1 )	/* �A���J�[��Ԃ���W�����v���� or �n�� */
			{
			}
			else 
			{
				if ( player[38] == 0 )
				{
					soundPlaySe( EN_SE_PAWA );
				}
				player[38] = 1;
				
				for ( i = 0; i < 20; i++ )
				{
					player[21] = player [21] + 1;
					player[23] = 1;
					if ( player[21] > 100 )
					{
						player[21] = 100;
						player[23] = 0;
					}
					if ( player[9] == 1 )
					{
						if ( player[21] > player[19] )
						{
							player[21] = player[19];
						}
					}

					player[30] = ( ( funcSin( player[20] ) * ( player[21] ) ) >> 16 );	/* �A���J�[�ʒu�w */
					player[31] = ( ( funcCos( player[20] ) * ( player[21] ) ) >> 16 );	/* �A���J�[�ʒu�x */
/*
					if (
					   ( ( ( map1_item_j( player[0] + player[30] + 0, player[1] + player[31] + 0  ) >= 100 )
					    && ( map1_item_j( player[0] + player[30] + 0, player[1] + player[31] + 0  ) <= 119 ) )
					  || ( ( map1_item_j( player[0] + player[30] + 0, player[1] + player[31] + 0  ) >= 200 )
					    && ( map1_item_j( player[0] + player[30] + 0, player[1] + player[31] + 0  ) <= 219 ) ) )
					|| ( ( ( map1_item_j( player[0] + player[30] + 32, player[1] + player[31] + 32  ) >= 100 )
					    && ( map1_item_j( player[0] + player[30] + 32, player[1] + player[31] + 32  ) <= 119 ) )
					  || ( ( map1_item_j( player[0] + player[30] + 32, player[1] + player[31] + 32  ) >= 200 )
					    && ( map1_item_j( player[0] + player[30] + 32, player[1] + player[31] + 32  ) <= 219 ) ) )
					   )
*/
					if (
					     ( ( player[0] + player[30] + 10 ) < -15 ) 
					  || ( ( player[0] + player[30] + 22 ) > 640 )
//					  || ( ( player[1] + player[31] + 0 ) < 0 )
//					  || ( ( player[1] + player[31] + 32 ) > 480 )
					   )
					{
					}
					else 
					{
						if (
						   ( ( ( map1_item_j( player[0] + player[30] + 16, player[1] + player[31] + 16  ) >= 100 )
						    && ( map1_item_j( player[0] + player[30] + 16, player[1] + player[31] + 16  ) <= 118 ) )
						  || ( ( map1_item_j( player[0] + player[30] + 16, player[1] + player[31] + 16  ) >= 200 )
						    && ( map1_item_j( player[0] + player[30] + 16, player[1] + player[31] + 16  ) <= 218 ) ) )
						   )
						{
							if   ( player[23] == 1 )	/* �L�΂��Ă��� */
							{
								if ( player[9] == 0 )
								{
									player[10] = 0;
									player[11] = 0;
									player[2] = 21;		/* �� */
									player[13] = 0;
									player[14] = -1;
									px = ( player[0] + player[30] + 16 ) / 32;
									py = ( ( ( ( player[1] + player[31] + 16 ) + 16 ) / 32 ) );
	//								player[32] = player[0] + player[30];
	//								player[33] = player[1] + player[31];
									player[32] = px * 32;
									player[33] = ( py * 32 ) - 16;
									
									px = player[0] - player[32];
									py = player[1] - player[33];
									rg = funcTan2( px, py ) ;
									rr = ( rg * 360 ) / 65535 ;
									player[20] = rr + 180;
									
									
									rr = GetKyori(
													  labs_gnp( player[0] - player[32] ) ,
													  labs_gnp( player[1] - player[33] )
													); 
									player[21] = rr;
									if ( player[21] < 37 )
									{
										player[21] = 37;
									}
									player[19] = player[21];
									
									
									player[27] = -1;

									if ( ( ( IsPressKey( gameflag[0] ) ) && ( gameflag[132] == 0 ) )
								  || ( ( replay_load_key[2] == 1 ) && ( gameflag[132] == 1 ) ) )
									{
										player[27] = 180;
									}
									else if ( ( ( IsPressKey( gameflag[1] ) ) && ( gameflag[132] == 0 ) )
								  || ( ( replay_load_key[3] == 1 ) && ( gameflag[132] == 1 ) ) )
									{
										player[27] = 0;
									}
									
									if ( ( ( IsPressKey( gameflag[2] ) ) && ( gameflag[132] == 0 ) )
								  || ( ( replay_load_key[0] == 1 ) && ( gameflag[132] == 1 ) ) )
									{
										if ( player[27] == 0 )
										{
											player[27] = 45;
										}
										else if ( player[27] == 180 )
										{
											player[27] = 135;
										}
										else 
										{
											player[27] = 90;
										}
									}
									else if ( ( ( IsPressKey( gameflag[3] ) ) && ( gameflag[132] == 0 ) )
								  || ( ( replay_load_key[1] == 1 ) && ( gameflag[132] == 1 ) ) )
									{
										if ( player[27] == 0 )
										{
											player[27] = 315;
										}
										else if ( player[27] == 180 )
										{
											player[27] = 225;
										}
										else 
										{
											player[27] = 270;
										}
									}

//									muki2 = player[27];
									soundPlaySe( EN_SE_ATK1 );
									p_shot_f2_set( player[32]  + 14, player[33]  + 14, 0 );
								}
								player[9] = 1;	/* �͂�ł��� */
								break;
							}
						}
						else 
						{
							for ( ii = 0; ii < 50; ii++ )
							{
								if ( item[( ii * 10 ) + 0 ] == 1 )
								{
									/* �v���C���[�Ƃ̓����蔻�� */
									if ( ( ( (item[1 + ( ii * 10 )] + 2 ) < ( player[0] + player[30] + 16 ) )	/* ���@�Ƃ̂����蔻�� */
										&& ( ( player[0] + player[30] + 16 ) < (item[1 + ( ii * 10 )] + 30 ) )
										&& ( (item[2 + ( ii * 10 )]  + 2 ) < ( player[1] + player[31] + 16 ) )
										&& ( ( player[1] + player[31] + 16 ) < ( item[2 + ( ii * 10 )] + 30 ) ) )
										&& ( sn == 1 ) )
									{
										if ( 
										     ( item[( ii * 10 ) + 3 ] == 703 )
										  || ( item[( ii * 10 ) + 3 ] == 704 )
										   )
										{
											if   ( player[23] == 1 )	/* �L�΂��Ă��� */
											{
												if ( player[9] == 0 )
												{
													player[37] = ii;		/* �A�C�e����͂�ł��� */
													player[10] = 0;
													player[11] = 0;
													player[2] = 21;		/* �� */
													player[13] = 0;
													player[14] = -1;
													
													/* It gets set and then reassigned right after. This code makes no sense - Gameblabla */
													//px = ( player[0] + player[30] + 16 ) / 32;
													//py = ( ( ( ( player[1] + player[31] + 16 ) + 16 ) / 32 ) );
													
					//								player[32] = player[0] + player[30];
					//								player[33] = player[1] + player[31];
													player[32] = item[1 + ( ii * 10 )];
													player[33] = item[2 + ( ii * 10 )];
													
													px = player[0] - player[32];
													py = player[1] - player[33];
													rg = funcTan2( px, py ) ;
													rr = ( rg * 360 ) / 65535 ;
													player[20] = rr + 180;
													
													
													rr = GetKyori(
																	  labs_gnp( player[0] - player[32] ) ,
																	  labs_gnp( player[1] - player[33] )
																	); 
													player[21] = rr;
													if ( player[21] < 37 )
													{
														player[21] = 37;
													}
													player[19] = player[21];
													
													
													player[27] = -1;
													if ( ( ( IsPressKey( gameflag[0] ) ) && ( gameflag[132] == 0 ) )
												  || ( ( replay_load_key[2] == 1 ) && ( gameflag[132] == 1 ) ) )
													{
														player[27] = 180;
													}
													else if ( ( ( IsPressKey( gameflag[1] ) ) && ( gameflag[132] == 0 ) )
												  || ( ( replay_load_key[3] == 1 ) && ( gameflag[132] == 1 ) ) )
													{
														player[27] = 0;
													}
													
													if ( ( ( IsPressKey( gameflag[2] ) ) && ( gameflag[132] == 0 ) )
												  || ( ( replay_load_key[0] == 1 ) && ( gameflag[132] == 1 ) ) )
													{
														if ( player[27] == 0 )
														{
															player[27] = 45;
														}
														else if ( player[27] == 180 )
														{
															player[27] = 135;
														}
														else 
														{
															player[27] = 90;
														}
													}
													else if ( ( ( IsPressKey( gameflag[3] ) ) && ( gameflag[132] == 0 ) )
												  || ( ( replay_load_key[1] == 1 ) && ( gameflag[132] == 1 ) ) )
													{
														if ( player[27] == 0 )
														{
															player[27] = 315;
														}
														else if ( muki == 180 )
														{
															player[27] = 225;
														}
														else 
														{
															player[27] = 270;
														}
													}
													
												soundPlaySe( EN_SE_ATK1 );
													p_shot_f2_set( player[32] + 14, player[33] + 14, 0 );
												}
												player[9] = 1;	/* �͂�ł��� */
												break;
											}
										}
									}
								}
							}
						}
	//						else 
	//						{
	//				//				player[9] = 0;	/* �͂�ł��� */
	//						}
					}

				}
				if ( player[9] == 1 )
				{
					idou_r( 1 );
					idou_l( 1 );
				}
			}
		}
		else 
		{
			player[21] = player [21] - 10;
			if ( player[21] < 32 )
			{
				player[21] = 32;
			}
			player[23] = 0;
			
			if ( player[9] == 1 )	/* �G��͂�ł���ꍇ */
			{
				player[14] = -1;	/*  */
				player[13] = 0;
//				player[36] = player[20];
//				player[34] = player[21] - 32; /* �A���J�[�������X */
//				player[35] = player[21] - 32; /* �A���J�[�������Y */
			}
			player[9] = 0;	/*  */
			player[37] = -1;
			player[24] = 0;
			player[38] = 0;
		}

		
		
		//B���������
		if ( ( ( ( IsPushKey( gameflag[5] ) ) || IsPushKey( gameflag[5] ) ) && ( gameflag[132] == 0 ) )
		  || ( ( replay_load_key[6] == 1 ) && ( gameflag[132] == 1 ) ) )
		{
			if ( ( player[2] != 11 ) && ( player[2] != 31 ) && ( player[14] == 10 ) )	/* �n��ōU���A�_���[�W����Ȃ� */
			{
				Jump_counts++;
				soundPlaySe( EN_SE_JUMP );
				/* �A�C�e���ɏ���Ă��Ȃ��ꍇ�͂��������g���C�ʒu */
				if ( item_up( ) == -1 )
				{
					player[15] = player[0];
					player[16] = player[1];
				}
				player[10] = 0;
				player[11] = 0;
				player[2] = 21;	/* �� */
				player[14] = 5;
				player[13] = 0;
				player[25] = 1;
				jmp_st( );
				player[24] = 0;
			}
			if ( player[9] == 1 )
			{
				Jump_counts++;
				soundPlaySe( EN_SE_JUMP );
				/* �A�C�e���ɏ���Ă��Ȃ��ꍇ�͂��������g���C�ʒu */
				if ( item_up( ) == -1 )
				{
					player[15] = player[0];
					player[16] = player[1];
				}
				player[10] = 0;
				player[11] = 0;
				player[2] = 21;	/* �� */
				player[14] = 5;
				player[13] = 0;
				player[25] = 1;
				jmp_st( );
				player[9] = 0;
				player[21] = 32;
				player[23] = 0;
				player[24] = 1;
			}
		}
		if ( ( ( IsPressKey( gameflag[5] ) ) && ( gameflag[132] == 0 ) )
		  || ( ( replay_load_key[5] == 1 ) && ( gameflag[132] == 1 ) ) )
		{
		}
		else 
		{
			if ( player[14] == 10 )
			{
			}
			else 
			{
				if ( ( player[14] > 0 ) && ( player[25] == 1 ) )	/* �r���ŗ��� */
				{
//					player[14] = -1;
				}
			}
			player[25] = 0;
		}
		
		
	}
	if ( Jump_counts > 99 )
	{
		Jump_counts = 99;
	}
	
	/* 12�{�^���@���@�b�������ꂽ */
	if ( IsPushKey( gameflag[6] ) )
	{
		sn = 4;
		demo[0] = 0;
	}
	
	/* �X�L���V���[�g�J�b�g�E�B���h�E */
	if ( IsPushKey( gameflag[7] ) )
	{
	}



	if ( player[0] < 0 )
	{
		player[0] = 0;
//		cleaer_ankr( );
		if ( player[9] == 1 ) 
		{
//			player[32] = px * 32;
//			player[33] = ( py * 32 ) - 16;
//			
//			px = player[0] - player[32];
//			py = player[1] - player[33];
//			rg = funcTan2( px, py ) ;
//			rr = ( rg * 360 ) / 65535 ;
//			player[20] = rr + 179;
			
			
			rr = GetKyori(
							  labs_gnp( player[0] - player[32] ) ,
							  labs_gnp( player[1] - player[33] )
							); 
			player[21] = rr;
			if ( player[21] < 37 )
			{
				player[21] = 37;
			}
			player[19] = player[21];
		}
		if ( save_data[9] == 1 )	/* �e���|�[�^�g�p�s�� */
		{
		}
		else if ( ( item_up( ) != -1 ) )
		{
			/* �A�C�e���ɏ���Ă���ꍇ�̓X�N���[�����Ȃ� */
		}
		else 
		{
			if ( player[1] <= 460 )
			{
				if ( map_data[4] == 1 )
				{
					wait_scl( 2, map_data[5] );
				}
			}
		}
	}
	if ( player[0] > ( 640 - 32 ) )
	{
		player[0] = 640 - 32;
//		cleaer_ankr( );
		if ( player[9] == 1 ) 
		{
//			player[32] = px * 32;
//			player[33] = ( py * 32 ) - 16;
//			
//			px = player[0] - player[32];
//			py = player[1] - player[33];
//			rg = funcTan2( px, py ) ;
//			rr = ( rg * 360 ) / 65535 ;
//			player[20] = rr + 179;
			
			
			rr = GetKyori(
							  labs_gnp( player[0] - player[32] ) ,
							  labs_gnp( player[1] - player[33] )
							); 
			player[21] = rr;
			if ( player[21] < 37 )
			{
				player[21] = 37;
			}
			player[19] = player[21];
		}
		if ( save_data[9] == 1 )	/* �e���|�[�^�g�p�s�� */
		{
		}
		else if ( ( item_up( ) != -1 ) )
		{
		}
		else 
		{
			if ( player[1] <= 460 )
			{
				if ( map_data[6] == 1 )
				{
					wait_scl( 3, map_data[7] );
				}
			}
		}
	}
	if ( player[1] < -30 )	/* ��ʉ� */
	{
		cleaer_ankr( 1 );	/*  */
	}
	if ( player[1] <= 0 )	/* ��ʉ� */
	{
		if ( save_data[9] == 1 )	/* �e���|�[�^�g�p�s�� */
		{
		}
		else 
		{
			if ( map_data[2] == 1 )
			{
				wait_scl( 1, map_data[3] );
			}
		}
	}
	if ( player[1] >= 450 )	/* ��ʏ� */
	{
		if ( save_data[9] == 1 )	/* �e���|�[�^�g�p�s�� */
		{
		}
		else 
		{
			if ( map_data[0] == 1 )
			{
				wait_scl( 0 , map_data[1] );
			}
		}
	}

}

void fram_set( )
{
	if ( player[2] == 0 )	/* �j���[�g���� */
	{
		player[12] = 20;
	}
	if ( player[2] == 1 )	/* ���� */
	{
		player[12] = 5;
	}
	if ( player[2] == 11 )	/* �U�� */
	{
		player[12] = 2;
	}
	if ( player[2] == 21 )	/* �� */
	{
		player[12] = 5;
	}
	if ( player[2] == 31 )	/* ���� */
	{
		player[12] = 3;
	}
	if ( player[2] == 33 )	/* �N���A */
	{
		player[12] = 10;
	}
	if ( player[2] == 61 )	/* ����� */
	{
		player[12] = 10;
	}
}


/* �������̏ꍇ�̓W�����v�͑��� */
void jmp_st( )
{
	Sint32 px1 ;
	Sint32 px2 ;
	Sint32 py2 ;

	px1 = ( player[0] + 10 ) / 32;
	px2 = ( player[0] + 22 ) / 32;
	py2 = ( 16 - ( ( player[1] + 32 + 28 ) / 32 ) );

	if ( ( map1[ px1 + ( ( py2 ) * 20 ) ] == 319 ) 
	  || ( map1[ px2 + ( ( py2 ) * 20 ) ] == 419 )  
	  || ( map1[ px1 + ( ( py2 ) * 20 ) ] == 318 )  
	  || ( map1[ px2 + ( ( py2 ) * 20 ) ] == 418 )  
	  || ( map1[ px1 + ( ( py2 ) * 20 ) ] == 719 )
	  || ( map1[ px1 + ( ( py2 ) * 20 ) ] == 718 ) )
	{
		player[14] = player[14] + 1; 		/* �W�����v�t���O */
	}
}

/* �W�����v�x�ʒu�v�Z */
/* Renamed from jmp() because that function name could be problematic - Gameblabla */
void g_jmp( )
{
	Sint32 y1;
	Sint32 px1 ;
	Sint32 px2 ;
	Sint32 py ;
	Sint32 py3 ;

	player[13]++;
	if ( player[13] >= 11 )
	{
		player[13] = 0;
	}
	
	if ( player[14] == 10 ) 
	{
		/* ����̈ʒu */
		px1 = ( player[0] + 10 ) / 32;
		px2 = ( player[0] + 21 ) / 32;
		py = ( 16 - ( ( player[1] + 28 ) / 32 ) );
		/* 100 �` 200�͒n�� */
		if (
		   ( ( !( ( map1[ px1 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 119 ) ) )
		  && ( !( ( map1[ px2 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 119 ) ) ) 
		  && ( !( ( map1[ px1 + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 219 ) ) ) 
		  && ( !( ( map1[ px2 + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 219 ) ) )
		   )
		  || ( ( ( player[1] - 16 ) % 32 ) != 0 ) )
			{
			if ( item_up( ) != -1 )	/* �A�C�e���̏�ɏ���Ă��� */
			{
				player[1] = item_up( ); 
				return;
			}
			
			if ( ( player[2] == 0 )
			  || ( player[2] == 1 ) )
			{
				player[10] = 0;
				player[11] = 0;
				player[2] = 21;		/* �� */
			}
			player[14] = -1;
			player[13] = 0;
			
			if ( player[2] == 31 )	/* �_���[�W�� */
			{
			}
		}
		else
		{
			return;
		}
	}

	if ( player[13] == 0 )
	{
		player[14] = player[14] - 2;
//		if ( player[14] == 0 )
//		{
//			player[14] = -3;
//		}

		if ( ( player[14] <= 1 ) && ( player[14] > -2 ) )
		{
			player[14] = -1;
		}

		if ( player[14] < -4 )
		{
			player[14] = -4;
			player[26]++;		/* ���n�t�F�N�g�p */
		}
		else 
		{
			player[26] = 0;
		}
		
		/* �n�ʔ��� */
	}
	
	/* ����̈ʒu */
	y1 = ( ( 0 - player[14] ) * ( 0 - player[14] ) * ( 0 - player[14] ) );
//	player[1] = player[1] - ( y1 / 30 );

	px1 = ( player[0] + 10 ) / 32;
	px2 = ( player[0] + 21 ) / 32;
	py = ( 16 - ( ( player[1] + 28 ) / 32 ) );
//	py = ( 16 - ( ( player[1] ) / 32 ) );
		/* �������� */
	if ( ( map1[ px1 + ( ( py ) * 20 ) ] == 319 ) 
	  || ( map1[ px2 + ( ( py ) * 20 ) ] == 419 )  
	  || ( map1[ px1 + ( ( py ) * 20 ) ] == 318 )  
	  || ( map1[ px2 + ( ( py ) * 20 ) ] == 418 )  
	  || ( map1[ px1 + ( ( py ) * 20 ) ] == 719 )
	  || ( map1[ px1 + ( ( py ) * 20 ) ] == 718 ) )
	{
		if ( player[14] > 0 ) /* �㏸�� */
		{
/*			player[1] = player[1] - ( y1 / 6 ); */
			player[1] = player[1] - ( y1 / 18 );
		}
		else 
		{
			player[1] = player[1] - ( y1 / 40 );
		}
	}
	else 
	{
		player[1] = player[1] - ( y1 / 20 );
	}

	if ( player[1] > 480 ) 
	{
		return;
	}
	py3 = ( 16 - ( ( player[1] + 32 + 32 + 16 ) / 32 ) );

	if ( player[14] <= 0 )													/* ��蒆�͔��肵�Ȃ� */
	{
		/* �n�ʔ��� */
		if ( ( ( map1[ px1 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 119 ) )
		  || ( ( map1[ px1 + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 219 ) ) 
		  || ( ( map1[ px2 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 119 ) ) 
		  || ( ( map1[ px2 + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 219 ) ) )
		{
			if ( player[8] > 0 )
			{ 
				soundPlaySe( EN_SE_LANDING );
			}
			if ( ( player[2] != 31 ) && ( player[2] != 32 ) )	/* �_���[�W������Ȃ� */
			{
				player[2] = 0;												/* ���n */
				player[11] = 0;												/* �d������ */
				player[18] = 10;
			}
			player[1] = ( ( 16 - py ) * 32 ) - 16; 
			/* ���n���̉��𐶐� */
			if ( player[26] >= 2 )
			{
				p_jump_f_set( player[0], player[1] );
			}
			player[14] = 10;
		}
		else 
		{
			if ( item_up( ) != -1 )	/* �A�C�e���̏�ɏ���Ă��� */
			{
				soundPlaySe( EN_SE_LANDING );
				if ( ( player[2] != 31 ) && ( player[2] != 32 ) )	/* �_���[�W������Ȃ� */
				{
					player[2] = 0;												/* ���n */
					player[11] = 0;												/* �d������ */
				}
				player[1] = item_up( ); 
				player[14] = 10;
				player[18] = 10;
			}
		}
	}
	else		/* �㏸�� */
	{
		/* �n�ʔ��� */
		if ( ( ( map1[ px1 + ( ( py3 ) * 20 ) ] >= 100 ) && ( map1[ px1 + ( ( py3 ) * 20 ) ] <= 119 ) )
		  || ( ( map1[ px1 + ( ( py3 ) * 20 ) ] >= 200 ) && ( map1[ px1 + ( ( py3 ) * 20 ) ] <= 219 ) )
		  || ( ( map1[ px2 + ( ( py3 ) * 20 ) ] >= 100 ) && ( map1[ px2 + ( ( py3 ) * 20 ) ] <= 119 ) )
		  || ( ( map1[ px2 + ( ( py3 ) * 20 ) ] >= 200 ) && ( map1[ px2 + ( ( py3 ) * 20 ) ] <= 219 ) ) )
		{
			player[13] = 0;
			player[14] = -3;
			player[26] = 0;
		}
		else 
		{
			if ( item_up2( ) != -1 )	/* �A�C�e���ɓ����Ԃ��� */
			{
				player[1] = item_up2( ); 
				player[13] = 0;
				player[14] = -3;
				player[26] = 0;
			}
		}
	}
	if ( player[1] <= -32 )
	{
		if ( map_data[2] == 0 )	/* �X�N���[�����Ȃ��ꍇ�̓_���[�W */
		{
			if ( player[8] > 0 )
			{ 
				if ( player[17] == 0 )
				{
					player[8] = player[8] - 1;
				}
				if ( player[17] == 0 )
				{
					soundPlaySe( EN_SE_DAMEGE );
				}
				if ( player[8] > 0 )
				{ 
					player[0] = player[15];
					player[1] = player[16];
				}
				else
				{
					
					cleaer_ankr( 1 );
					player[1] = -32 ;
				} 
			}
		}
	}

}

void idou_l( Sint32 ankr_flag )
{
	Sint32 px ;
	Sint32 py1 ;
	Sint32 py2 ;

	Sint32 i;


	/* ����̈ʒu */
	px = ( player[0] + 10 + player[29] ) / 32;
	py1 = ( 16 - ( ( player[1] + 49 + 16  ) / 32 ) );
	py2 = ( 16 - ( ( player[1] + 52 ) / 32 ) );
	if ( py2 <= 0 )	/* ��ʊO�͈�ԏ�Ŕ��� */
	{
		py2 = 1;
	}

	/* 502�͂�����@501�͂����� */
	if ( ( ( map1[ px + ( ( py1 ) * 20 ) ] >= 100 ) && ( map1[ px + ( ( py1 ) * 20 ) ] <= 299 ) )
	  || ( ( map1[ px + ( ( py2 ) * 20 ) ] >= 100 ) && ( map1[ px + ( ( py2 ) * 20 ) ] <= 299 ) ) )
	{
		cleaer_ankr( ankr_flag );
		
		px = ( player[0] + 16 ) / 32;
		player[0] = px * 32 - 10;
		player[29] = 0;

	}
	else 
	{
	
	}
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 1 )
		{
			/* �v���C���[�Ƃ̓����蔻�� */
			if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( (player[0] + 10 + player[29] ) < (item[1 + ( i * 10 )] + 32 ) )
				&& ( (item[2 + ( i * 10 )] ) < (player[1] + 32 ) )
				&& ( (player[1] + 5 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
			{
				if ( ( ( item[( i * 10 ) + 3 ] >= 500 ) && ( item[( i * 10 ) + 3 ] <= 620 ) ) /* �ړ��s�A�C�e�� */
				    || ( item[( i * 10 ) + 3 ] == 7 ) )
				{
					cleaer_ankr( ankr_flag );
		
					px = ( player[0] + 16 ) / 32;
					player[0] = px * 32 - 10;
					player[29] = 0;
				}
			}
		}
	}

}
/* �����̍��E�Ɉړ��\���H */
void idou_r( Sint32 ankr_flag )
{
	Sint32 px ;
	Sint32 py1 ;
	Sint32 py2 ;

	Sint32 i;

	/* ����̈ʒu */
	px = ( player[0] + 21 + player[29]  ) / 32;
	py1 = ( 16 - ( ( player[1] + 49 + 16 ) / 32 ) );
	py2 = ( 16 - ( ( player[1] + 52 ) / 32 ) );
	if ( py2 <= 0 )	/* ��ʊO�͈�ԏ�Ŕ��� */
	{
		py2 = 1;
	}

	/* 502�͂�����@501�͂����� */
	if ( ( ( map1[ px + ( ( py1 ) * 20 ) ] >= 100 ) && ( map1[ px + ( ( py1 ) * 20 ) ] <= 299 ) )
	  || ( ( map1[ px + ( ( py2 ) * 20 ) ] >= 100 ) && ( map1[ px + ( ( py2 ) * 20 ) ] <= 299 ) ) )
	{
		cleaer_ankr( ankr_flag );
		
		px = ( player[0] + 16 ) / 32;
		player[0] = px * 32 + 10;
		player[29] = 0;
	}
	else 
	{
	}
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 1 )
		{
			/* �v���C���[�Ƃ̓����蔻�� */
			if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 + player[29] ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
				&& ( (item[2 + ( i * 10 )] ) < (player[1] + 32 ) )
				&& ( (player[1] + 5 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
			{
				if ( ( ( item[( i * 10 ) + 3 ] >= 500 ) && ( item[( i * 10 ) + 3 ] <= 620 ) ) /* �ړ��s�A�C�e�� */
				    || ( item[( i * 10 ) + 3 ] == 7 ) )
				{
					cleaer_ankr( ankr_flag );
			
					px = ( player[0] + 16 ) / 32;
					player[0] = px * 32 + 10;
					player[29] = 0;
				}
			}
		}

	}

}

void cleaer_ankr( Sint32 ankr_flag )
{
	if ( ankr_flag != 0 )
	{
		player[24] = 1;
	}
	player[9] = 0;
	player[21] = 32;
	player[23] = 0;
	player[37] = -1;
}

void act_drow( void )
{
	//-------------------------------------------------------------
	//�ϐ��錾
	//-------------------------------------------------------------

	//�w�i�N���A
	ClearSecondary();

	switch( sn )							/* sn�ɂ���ĕ`�悷����̂�ς��� */
	{
	case 0:		//�J�n�f��
		bakdisp1( );
		playerdisp( );
		enm_disp( );
		bakdisp2( );
		stage_start( );
		debugdisp( );
		//-----------------------------------------------------------------------------------
		break;
	case 1 :		//�A�N�V����
		act_keys( );
		bakdisp1( );
		item_disp( );
		kane_disp( );
		enm_disp( );
		e_shot_disp( );
		playerdisp( );
		p_jump_f_disp( );
		p_shot_disp( );
		p_mgc_disp( );
		bakdisp2( );
		p_shot_f2_disp( );
		p_shot_f_disp( );
		minimap_disp( );
		snack_counter_disp( ); // ����J�E���^
		mapdisp( );
		debugdisp( );		/* �f�o�b�O��� */
		stage_endj( );
		//-----------------------------------------------------------------------------------
		break;
	case 2 :		//�N���A�f��
		bakdisp1( );
		item_disp( );
//			enm_disp( );
//			e_shot_disp( );
//			g_jmp( );
		playerdisp( );
//			p_shot_disp( );
//			p_mgc_disp( );
		bakdisp2( );
		p_shot_f2_disp( );
		stage_claer( );
		minimap_disp( );
		snack_counter_disp( ); // ����J�E���^
		mapdisp( );
		debugdisp( );
		//-----------------------------------------------------------------------------------
		break;
	case 3 :		//�Q�[���I�[�o�[
		bakdisp1( );
		item_disp( );
		enm_disp( );
		e_shot_disp( );
		g_jmp( );
		playerdisp( );
		p_shot_disp( );
		p_mgc_disp( );
		bakdisp2( );
		p_shot_f2_disp( );
		minimap_disp( );
		snack_counter_disp( ); // ����J�E���^
		mapdisp( );
		stage_end( );
		debugdisp( );
		//-----------------------------------------------------------------------------------
		break;
	case 4 :		//�A�C�e��
		window_keys( );
		bakdisp1( );
		item_disp( );
		kane_disp( );
		enm_disp( );
		e_shot_disp( );
		playerdisp( );
		p_jump_f_disp( );
		p_shot_disp( );
		p_mgc_disp( );
		bakdisp2( );
		p_shot_f2_disp( );
		p_shot_f_disp( );
		minimap_disp( );
		snack_counter_disp( ); // ����J�E���^
		mapdisp( );
		window_disp( );
		debugdisp( );
		//-----------------------------------------------------------------------------------
		break;
	case 5 :		//�C�x���g
		//-----------------------------------------------------------------------------------
		break;
	default :	//	�ǂ̏����ɂ������Ȃ��ꍇ
		
		/* ��ʐ؂�ւ����̕\���΍� */
		player[0] = next_px;
		player[1] = next_py;
		
		scene_exit = 0;
		break;
	}

	RefreshScreen( NULL );	
	KeyInput();				
}

/* �ꎞ�t�@�C���ۑ� */
void save_file_w( )
{
	Sint32 i;
	char path_config[96];
	char path_item[96];
	char path_work[96];
	char path_folder[96];
	
	Filesystem_GetConfigPath(path_config, sizeof(path_config));
	Filesystem_GetSaveFolderPath(path_folder, sizeof(path_folder));
	Filesystem_GetWorkSavePath(path_work, sizeof(path_work));
	Filesystem_GetItemSavePath(path_item, sizeof(path_item));
	
	Filesystem_CreateSaveFolder(path_folder);
	
	printf("SaveGameFlag(path_config); save_file_w\n");
	SaveGameFlag(path_config);
	
	ResetGameFlag2();
	save_data[5] = player[8];
	save_data[2] = stage;
	
	save_data[0] = player[0];	
	save_data[1] = player[1];
	save_data[4] = player[3];
	for ( i = 0; i < 400; i++ )
	{
		gameflag2[i] = save_data[i];
	}
	SaveGameFlag2(path_work);
	Platform_SaveCustom(path_work, "gan_work.sav", "WORK");
	ResetGameFlag2();				
	for ( i = 0; i < 500; i++ )
	{
		gameflag2[i] = item_wk[i];
	}
	SaveGameFlag2(path_item);
	Platform_SaveCustom(path_item, "gan_item.sav", "ITEM");
}


/**********************************/
/* �v���C���[�̏�Ԍ��菈��       */
/**********************************/

/* Player sprite - Displayed according to the screen scrolling - Gameblabla */
void playerdisp( )
{
	Sint32 i;
	Sint32 arm_muki;
	
	arm_muki = 0;
	
	player[10]++;
	if ( player[10] >= player[12] )
	{
		player[10] = 0;
		
		player[11]++;
		if ( player[11] >= 8 )
		{
			player[11] = 0;
			if ( player[2] == 11 )	/* �U���I�� */
			{
				player[2] = 0;
				if ( player[14] != 10 )	/* �� */
				{
					player[2] = 21;
				}
			}
			if ( player[2] == 31 )	/* �_���[�W */
			{
				player[2] = 0;
				if ( player[14] != 10 )	/* �� */
				{
					player[2] = 21;
				}
			}
			if ( player[2] == 32 )	/* �_�E���Q�[���I�[�o�[ */
			{
				player[2] = 32;
				player[11] = 7;
			}
			if ( player[2] == 33 )	/* �N���A */
			{
				player[2] = 33;
				player[11] = 7;
			}
		}
	}
	fram_set( );
	
	if ( player[0] < DISPLY_WIDTH_HALF )
	{
		dp_x = player[0];
	}
	else if ( player[0] >= DISPLY_WIDTH_PLUS_HALF )
	{
		dp_x = player[0] - DISPLY_WIDTH;
	}
	else 
	{
		dp_x = DISPLY_WIDTH_HALF;
	}
	
	if ( player[1] < DISPLY_HEIGHT_HALF )
	{
		dp_y = player[1];
	}
	else if ( player[1] >= DISPLY_HEIGHT + DISPLY_HEIGHT_HALF - 16 )
	{
		dp_y = player[1] - DISPLY_HEIGHT + 16;
	}
	else 
	{
		dp_y = DISPLY_HEIGHT_HALF;
	}
	
	
	if ( player[2] == 0 )
	{
		player[4] = 0;
		player[5] = 0;
		if ( player[18] > 0 )
		{
			player[4] = 0;
			player[5] = 7;
			player[11] = 8;
			if ( player[28] == 1 )	/* �ړ��� */
			{
				player[3] = 0;
				player[4] = 1;
				player[5] = 2;
				player[11] = 0;
			}
			if ( player[28] == 2 )	/* �ړ��� */
			{
				player[3] = 1;
				player[4] = 1;
				player[5] = 2;
				player[11] = 0;
			}
		}
		else 
		{
			if ( player[11] == 8 )
			{
				player[11] = 0;
			}
		}
		if ( player[28] == 1 )	/* �ړ��� */
		{
			player[3] = 0;
			player[4] = 1;
			player[5] = 2;
		}
		if ( player[28] == 2 )	/* �ړ��� */
		{
			player[3] = 1;
			player[4] = 1;
			player[5] = 2;
		}
	}
	if ( player[18] > 0 )	/* ���n���[�V�����\�� */
	{
		player[18]--;
	}
	if ( player[2] != 0 )
	{
		player[18] = 0;
	}
	else 
	{
	}

	
	if ( player[2] == 1 )		/* ���� */
	{
		player[4] = 0;
		player[5] = 1;
		if ( player[28] == 1 )	/* �ړ��� */
		{
			player[3] = 0;
			player[4] = 1;
			player[5] = 2;
		}
		if ( player[28] == 2 )	/* �ړ��� */
		{
			player[3] = 1;
			player[4] = 1;
			player[5] = 2;
		}
	}
	if ( player[2] == 11 )
	{
		player[4] = 0;
		player[5] = 3;
	}
	if ( player[2] == 21 )
	{
		player[4] = 0;
		player[5] = 2;
		if ( player[14] < 0 )	/* �����Ă� */
		{
			player[5] = 7;
		}
	}
	if ( player[2] == 31 )	/* �_���[�W */
	{
		player[4] = 0;
		player[5] = 4;

	}
	if ( player[2] == 32 )	/* �Q�[���I�[�o�[ */
	{
		player[4] = 1;
		player[5] = 0;

	}
	if ( player[2] == 33 )	/* �N���A */
	{
		player[4] = 0;
		player[5] = 8;

	}
	if ( player[2] == 61 )	/* ����� */
	{
		player[4] = 0;
		player[5] = 0;
		player[11] = 8;
		player[10] = 0;
	}

	if ( player[29] != 0 )
	{
		player[4] = 0;
		player[5] = 2;
	}
	
	/* �A���J�[ */
/*
	player[20]
	player[21]
	player[22]
*/
//	csl_x[i] = ( dp_x + ( char_size[0] / 2 ) ) + ( ( funcSin( ( 32768 / 360 ) * ( player[36] + ( 90 * i ) ) ) * find_dot ) >> 16 );
//	csl_y[i] = ( dp_y - ( char_size[1] / 2 ) ) + ( ( funcCos( ( 32768 / 360 ) * ( player[36] + ( 90 * i ) ) ) * find_dot ) >> 16 );

//	BltRect( 4,
//	     ( dp_x ) + ( ( funcSin( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ),
//	     ( 240 - dp_y - 32 ) + ( ( funcCos( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ) ,
//	     0,
//	     0,
//	     32,
//	     32 );
	/* �p�x����\���p�^�[�����擾 */
	arm_muki = ( ( player[20] + 11 ) * 100 ) / ( ( 360 * 100 ) / 16 ) ;
	if ( arm_muki >= 16 )
	{
		arm_muki = 0;
	} 

	for ( i = 1; i < 5; i++ )
	{ 
		if ( ( player[21] == 32 ) && ( i > 1 ) ) 
		{
		}
		else 
		{
			BltRect( 4,
			     ( dp_x ) + ( ( player[30] / 5 ) * i ),
			     ( DISPLY_HEIGHT - ( dp_y + ( ( player[31] / 5 ) * i ) ) ) - 32,
			     arm_muki * 32,
			     96 + ( gameflag[133] * 32 ),
			     32,
			     32 );
		}
	}
//		BltTrans( 65,
//		     ( dp_x ) + ( ( ( ( funcSin( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ) / 5 ) * i ),
//		     ( 240 - dp_y - 32 ) + ( ( ( ( funcCos( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ) / 5 ) * i ) ,
//		     125,
//		     player[20] );

		arm_muki = ( ( player[20] + 11 ) * 100 ) / ( ( 360 * 100 ) / 16 ) ;
		if ( arm_muki >= 16 )
		{
			arm_muki = 0;
		} 
		BltRect( 4,
		     ( dp_x ) + player[30],
		     ( DISPLY_HEIGHT - ( dp_y + player[31] ) ) - 32,
		     arm_muki * 32,
		     32 + ( player[23] * 32 ),
		     32,
		     32 );
/*
		if ( player[23] == 0 )
		{
			BltTrans( 65,
			     ( dp_x ) + ( ( ( ( funcSin( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ) / 5 ) * i ),
			     ( 240 - dp_y - 32 ) + ( ( ( ( funcCos( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ) / 5 ) * i ) ,
			     149,
			     arm_muki );
		}
		else 
		{
			BltTrans( 66,
			     ( dp_x ) + ( ( ( ( funcSin( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ) / 5 ) * i ),
			     ( 240 - dp_y - 32 ) + ( ( ( ( funcCos( ( 32768 / 360 ) * player[20] ) * player[21] ) >> 16 ) / 5 ) * i ) ,
			     149,
			     arm_muki );
		}
*/	
	
	if ( ( player[17] % 2 ) == 0 )
	{
		BltRect( 1, dp_x, DISPLY_HEIGHT - dp_y - 32 , ( player[11] * 32 ) + ( player[4] * 320 ), ( player[5] * 64 ) + ( player[3] * 32 ), 32, 32 );
	}
	
}
//-------------------------------------------------------------
//�w�i�\��

/* Scrolling map display - Gameblabla */
void bakdisp1()
{
	int x;
	int y;
	int b_x;
	int b_y;
	int i;

	x = 0;
	i = 0;
	
	if ( gameflag[67] == 1 )
	{
		if ( stage == 11 )
		{
			rayer[0] = rayer[0] + 1;
			if ( rayer[0] > 480 )
			{
				rayer[0] = rayer[0] - 480;
			}
			rayer[1] = rayer[1] + 2;
			if ( rayer[1] > 480 )
			{
				rayer[1] = rayer[1] - 480;
			}
			rayer[2] = rayer[2] + 4;
			if ( rayer[2] > 480 )
			{
				rayer[2] = rayer[2] - 480;
			}
		}
		else 
		{
		
			rayer[0] = rayer[0] + 1;
			if ( rayer[0] > 640 )
			{
				rayer[0] = rayer[0] - 640;
			}
			rayer[1] = rayer[1] + 2;
			if ( rayer[1] > 640 )
			{
				rayer[1] = rayer[1] - 640;
			}
			rayer[2] = rayer[2] + 4;
			if ( rayer[2] > 640 )
			{
				rayer[2] = rayer[2] - 640;
			}
		}
	}
	
	//��ʔw�i����ʑS�̂ɓ]��
	bak_cnt = bak_cnt + 1;
	if ( bak_cnt >= 40 )
	{
		bak_cnt = 0;
	}

	if ( player[0] < DISPLY_WIDTH_HALF )
	{
		d_x = 0;
	}
	else if ( player[0] >= DISPLY_WIDTH_PLUS_HALF )
	{
		d_x = DISPLY_WIDTH;
	}
	else 
	{
		d_x = player[0] - DISPLY_WIDTH_HALF;
	}

	if ( player[1] < ( DISPLY_HEIGHT_HALF ) )
	{
		d_y = DISPLY_HEIGHT;
	}
	else if ( player[1] >= ( DISPLY_HEIGHT + DISPLY_HEIGHT_HALF - 16 ) )
	{
		d_y = 16;
	}
	else 
	{
		d_y = DISPLY_HEIGHT + DISPLY_HEIGHT_HALF - player[1];
	}

	Blt( 113, 0, 0 );						/* �w�i�X�N���[�����o */

	i = bak_cnt / 10;
	for ( x = 0; x < 20; x++ )
	{
		for ( y = 0; y <= 16; y++ )
		{
			if ( ( ( d_x - 32 ) <= ( x * 32 ) ) 
			  && ( ( d_y - 32 ) <= ( y * 32 ) )
			  && ( ( d_x + DISPLY_WIDTH + 32 ) >= ( x * 32 ) )
			  && ( ( d_y + DISPLY_HEIGHT + 32 ) >= ( y * 32 ) ) )
			{
				b_y = map1[ x + ( y * 20 ) ] / 100;
				b_x = map1[ x + ( y * 20 ) ] % 100;
				if ( ( map1[ x + ( y * 20 ) ] >= 100 ) && ( map1[ x + ( y * 20 ) ] <= 219 ) )
				{
					BltRect( 10, ( x * 32 ) - d_x , ( y * 32 ) - d_y - 32 + 16 , 0 * 32 , 0 * 32 ,32, 32 );
				}
				else 
				{	

					if ( b_y == 7 )			/* �w�i�A�j���[�V���� */
					{
						BltRect( 10, ( x * 32 ) - d_x, ( y * 32 ) - d_y - 32 + 16 , b_x * 32 , ( b_y + i ) * 32 ,32, 32 );
					}
					else 
					{
						BltRect( 10, ( x * 32 )  - d_x , ( y * 32 )  - d_y - 32 + 16 , b_x * 32 , b_y * 32 ,32, 32 );
					}
				}
			}
		}
	}

}
void bakdisp2( )
{
	int i;
	int x;
	int y;
	int b_x;
	int b_y;
	
	i = bak_cnt / 10;
	
	for ( x = 0; x < 20; x++ )
	{
		for ( y = 0; y <= 16; y++ )
		{
			if ( ( ( d_x - 32 ) <= ( x * 32 ) ) 
			  && ( ( d_y - 32 ) <= ( y * 32 ) )
			  && ( ( d_x + DISPLY_WIDTH + 32 ) >= ( x * 32 ) )
			  && ( ( d_y + DISPLY_HEIGHT + 32 ) >= ( y * 32 ) ) )
			{
				if ( ( map1[ x + ( y * 20 ) ] >= 100 ) && ( map1[ x + ( y * 20 ) ] <= 219 ) )
				{
					b_y = map1[ x + ( y * 20 ) ] / 100;
					b_x = map1[ x + ( y * 20 ) ] % 100;
					
					if ( b_y == 7 )				/* �w�i�A�j���[�V���� */
					{
						BltRect( 10, ( x * 32 ) - d_x , ( y * 32 ) - d_y - 32 + 16, b_x * 32 , ( b_y + i ) * 32 ,32, 32 );
					}
					else 
					{
						BltRect( 10, ( x * 32 ) - d_x , ( y * 32 ) - d_y - 32 + 16 , b_x * 32 , b_y * 32 ,32, 32 );
					}
					
					b_y = map2[ x + ( y * 20 ) ] / 100;
					b_x = map2[ x + ( y * 20 ) ] % 100;
					if ( b_y == 7 )				/* �w�i�A�j���[�V���� */
					{
						BltRect( 11, ( x * 32 ) - d_x , ( y * 32 ) - d_y - 32 + 16 , b_x * 32 , ( b_y + i ) * 32 ,32, 32 );
					}
					else 
					{
						BltRect( 11, ( x * 32 ) - d_x , ( y * 32 ) - d_y - 32 + 16 , b_x * 32 , b_y * 32 ,32, 32 );
					}
				}
				else  
				{
					b_y = map2[ x + ( y * 20 ) ] / 100;
					b_x = map2[ x + ( y * 20 ) ] % 100;
					if ( b_y == 7 )				/* �w�i�A�j���[�V���� */
					{
						BltRect( 11, ( x * 32 ) - d_x , ( y * 32 ) - d_y - 32 + 16 , b_x * 32 , ( b_y + i ) * 32 ,32, 32 );
					}
					else 
					{
	//					BltRect( 11, ( x * 32 ) - d_x , ( y * 32 ) - d_y - 32 + 16 , b_x * 32 , b_y * 32 ,32, 32 );
					}
				}
			}
		}
	}
}

void bak_make( )
{

}



/* �v�`�h�s���X�N���[�� */
void wait_scl( Sint32 type, Sint32 d_num )
{
/*  
	0:��	1:��	2:��	3:�E
	��݂��ރt�@�C���m�n
*/
	Sint32 i;
	Sint32 x;
	Sint32 y;

	screen_BackupPlane( );

	/* �o�b�t�@�N���A */
	for ( i = 0; i < 100; i++ )
	{
		p_ef[i * 5] = 0;						/* ���� */
	}
	for ( i = 0; i < 40; i++ )
	{
		pmshot[i * 20 + 2] = 0;						/* ���� */
	}
	for ( i = 0; i < 10; i++ )
	{
		pshot[i * 10 + 0] = 0 ;	
	}
	for ( i = 0; i < 40; i++ )
	{
		pshotf[i * 10 + 0] = 0 ;	
	}
	/* �o�b�t�@�N���A */
	for ( i = 0; i < 50; i++ )
	{
		item[i * 10 + 0] = 0 ;	
	}
	for ( i = 0; i < 20; i++ )
	{
		enemy[i * 20 + 0] = 0 ;					/* ���݂��ĂȂ� */
	}
	for ( i = 0; i < 20; i++ )
	{
		mgc_e[i * 10 + 0] = 0 ;					/* ���݂��ĂȂ� */
	}
	for ( i = 0; i < 20; i++ )
	{
		kane[i * 10 + 0] = 0 ;					/* ���݂��ĂȂ� */
	}

	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", (int)stage , (int)1, (int)d_num );
	LoadGameFlag2(string);
	{
		for ( i = 0; i <= 319; i++ )
		{
			map1[i] = gameflag2[i];
		}
	}
	for ( i = 0 ; i < 20 ; i++ )
	{
		map_data[i] = gameflag2[i + 320];
	}
	
	for ( i = 0 ; i < 5 ; i++ )
	{
		sprintf(string,BAK_IMAGE_PATH "%d.bmp", (int)map_data[11 + i] );
		
		if ( ( i == 0 ) || ( map_data[11 + i] != 0 ) )
		{
			LoadBitmap(string,110 + i , 0 );
		}
		else 
		{
			ReleaseBitmap(110 + i);
		}
	}

	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", (int)stage , (int)2, (int)d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( i = 0; i <= 319; i++ )
		{
			map2[i] = gameflag2[i];
		}
	}
	bak_make();


	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "i%d_%d.map", (int)stage , (int)1, (int)d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( x = 0; x <= 19 ; x++ )
		{
			for ( y = 0; y <= 16 ; y++ )
			{
				if ( gameflag2[x + ( y * 20 )] != 0 )
				{
					item_set( gameflag2[x + ( y * 20 )], x * 32, ( ( 14 - y ) * 32 ) + 16, x + ( y * 20 ), d_num );
				}
			}
		}
	}

	ResetGameFlag2();
	sprintf( string,MAP_DATA_PATH "e%d_%d.map", (int)stage , (int)1, (int)d_num );
	LoadGameFlag2( string );
	for ( x = 0; x <= 19 ; x++ )
	{
		for ( y = 0; y <= 16 ; y++ )
		{
			if ( gameflag2[x + ( y * 20 )] != 0 )
			{
				enm_set( gameflag2[x + ( y * 20 )], x * 32, ( ( 14 - y ) * 32 ) + 16 );
			}
		}
	}
	
	save_data[3] = d_num;
	
	if ( type == 0 )
	{
		player[1] = 0;
	}
	else if ( type == 1 )
	{
		player[1] = ( 480 - 32 - 16 );
	}
	else if ( type == 2 )
	{
		player[0] = ( 640 - 32 );
	}
	else if ( type == 3 )
	{
		player[0] = 0;
	}

	player[15] = player[0];
	player[16] = player[1];

	player[9] = 0;	
}


void wait_scl2( Sint32 d_num ,Sint32 init_x, Sint32 init_y )
{
	Sint32 i;
	Sint32 x;
	Sint32 y;

	for ( i = 0; i < 100; i++ )
	{
		p_ef[i * 5] = 0;						/* ���� */
	}
	for ( i = 0; i < 40; i++ )
	{
		pmshot[i * 20 + 2] = 0;						/* ���� */
	}
	for ( i = 0; i < 10; i++ )
	{
		pshot[i * 10 + 0] = 0 ;	
	}
	for ( i = 0; i < 40; i++ )
	{
		pshotf[i * 10 + 0] = 0 ;	
	}

	for ( i = 0; i < 50; i++ )
	{
		item[i * 10 + 0] = 0 ;	
	}
	for ( i = 0; i < 20; i++ )
	{
		enemy[i * 20 + 0] = 0 ;					/* ���݂��ĂȂ� */
	}
	for ( i = 0; i < 20; i++ )
	{
		mgc_e[i * 10 + 0] = 0 ;
	}
	for ( i = 0; i < 20; i++ )
	{
		kane[i * 10 + 0] = 0 ;				
	}

	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", (int)stage , (int)1, (int)d_num );
	LoadGameFlag2(string);
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( i = 0; i <= 319; i++ )
		{
			map1[i] = gameflag2[i];
		}
	}
	for ( i = 0 ; i < 20 ; i++ )
	{
		map_data[i] = gameflag2[i + 320];
	}
	
	for ( i = 0 ; i < 5 ; i++ )
	{
		sprintf(string,BAK_IMAGE_PATH "%d.bmp", (int)map_data[11 + i] );
		
		if ( ( i == 0 ) || ( map_data[11 + i] != 0 ) )
		{
			LoadBitmap(string,110 + i , 0 );
		}
		else 
		{
			ReleaseBitmap(110 + i);
		}
	}
	
	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", (int)stage , (int)2, (int)d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( i = 0; i <= 319; i++ )
		{
			map2[i] = gameflag2[i];
		}
	}
	bak_make();


	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "i%d_%d.map", (int)stage , (int)1, (int)d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( x = 0; x <= 19 ; x++ )
		{
			for ( y = 0; y <= 16 ; y++ )
			{
				if ( gameflag2[x + ( y * 20 )] != 0 )
				{
					item_set( gameflag2[x + ( y * 20 )], x * 32, ( ( 14 - y ) * 32 ) + 16, x + ( y * 20 ), d_num );
				}
			}
		}
	}

	ResetGameFlag2();
	sprintf( string,MAP_DATA_PATH "e%d_%d.map", (int)stage , (int)1, (int)d_num );
	LoadGameFlag2( string );

	for ( x = 0; x <= 19 ; x++ )
	{
		for ( y = 0; y <= 16 ; y++ )
		{
			if ( gameflag2[x + ( y * 20 )] != 0 )
			{
				enm_set( gameflag2[x + ( y * 20 )], x * 32, ( ( 14 - y ) * 32 ) + 16 );
			}
		}
	}
	
	save_data[3] = d_num;
	
	player[0] = init_x;
	player[1] = init_y;


	player[15] = player[0];
	player[16] = player[1];
}


void wait_scl3( )
{
	Sint32 i;
	Sint32 x;
	Sint32 y;
	Sint32 d_num;
	
	d_num = save_data[3];

	for ( i = 0; i < 100; i++ )
	{
		p_ef[i * 10] = 0;						/* ���� */
	}
	for ( i = 0; i < 40; i++ )
	{
		pmshot[i * 20 + 2] = 0;						/* ���� */
	}
	for ( i = 0; i < 10; i++ )
	{
		pshot[i * 10 + 0] = 0 ;	
	}
	for ( i = 0; i < 40; i++ )
	{
		pshotf[i * 10 + 0] = 0 ;	
	}

	for ( i = 0; i < 50; i++ )
	{
		item[i * 10 + 0] = 0 ;	
	}
	for ( i = 0; i < 20; i++ )
	{
		if ( enemy[0 + ( i * 20 )] == 1 )				
		{
			enemy[1 + ( i * 20 )] = enemy[14 + ( i * 20 )] ;
			enemy[2 + ( i * 20 )] = enemy[15 + ( i * 20 )] ;
		}
	}
	for ( i = 0; i < 20; i++ )
	{
		mgc_e[i * 10 + 0] = 0 ;		
	}

	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", (int)stage , (int)1, (int)d_num );
	LoadGameFlag2(string);
	{
		for ( i = 0; i <= 319; i++ )
		{
			map1[i] = gameflag2[i];
		}
	}
	for ( i = 0 ; i < 20 ; i++ )
	{
		map_data[i] = gameflag2[i + 320];
	}
	
	for ( i = 0 ; i < 5 ; i++ )
	{
		sprintf(string,BAK_IMAGE_PATH "%d.bmp", (int)map_data[11 + i] );
		
		if ( ( i == 0 ) || ( map_data[11 + i] != 0 ) )
		{
			LoadBitmap(string,110 + i , 0 );
		}
		else 
		{
			ReleaseBitmap(110 + i);
		}
	}
	
	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "%d_%d.map", (int)stage , (int)2, (int)d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( i = 0; i <= 319; i++ )
		{
			map2[i] = gameflag2[i];
		}
	}
	bak_make();

	ResetGameFlag2( );
	sprintf(string,MAP_DATA_PATH "i%d_%d.map", (int)stage , (int)1, (int)d_num );
	if ( LoadGameFlag2(string) == 0 )
	{
		for ( x = 0; x <= 19 ; x++ )
		{
			for ( y = 0; y <= 16 ; y++ )
			{
				if ( gameflag2[x + ( y * 20 )] != 0 )
				{
					item_set( gameflag2[x + ( y * 20 )], x * 32, ( ( 14 - y ) * 32 ) + 16, x + ( y * 20 ), d_num );
				}
			}
		}
	}

	ResetGameFlag2();
	sprintf( string,MAP_DATA_PATH "e%d_%d.map", (int)stage , (int)1, (int)d_num );
	LoadGameFlag2( string );
	
	player[15] = player[0];
	player[16] = player[1];


}

void snack_counter_disp()
{
	Sint32 count;
	Sint32 i;

	count = 0;
	for(i = 0; i < 20; i++)
	{
		if ( enemy[0 +( i * 20 )] == 1 )
		{
			count++;
		}
	}
	
	BltRect( 18, 100, 2, 0, 72, 100, 8 );	/* replay */
	
	BltNumericImage2( count, 2, 140, 2, 18, 0, 0, 10, 8 );
}


void minimap_disp( void )
{
	Sint32 px;
	Sint32 py;
	Sint32 size;
	Sint32 height;
	Sint32 i;
	Sint32 x;
	Sint32 y;

	px   = 2;
	py   = 12;
	size = 3;
	height = size*15;

	if ( gameflag[132] == 0 )
	{
		return;
	}

	Blt( 70, px, py );
	x = px + 0      + ((player[0] / 32 + 0) * size);
	y = py + height - ((player[1] / 32 + 1) * size);
	BltRect( 127, x, y, 127, 191, size, size );

	for( i = 0; i < 20; i++)
	{
		if ( enemy[0 +( i * 20 )] == 1 )
		{
			x = px + 0      + ((enemy[1 +( i * 20)] / 32 + 0) * size);
			y = py + height - ((enemy[2 +( i * 20)] / 32 + 1) * size);
			BltRect( 127, x, y, 127, 127, size, size );
		}

	}
}


void mapdisp()
{
	int stage_hosei;
	Sint32 wk;
	
	stage_hosei = 0;
	if ( stage == 2 )
	{
		stage_hosei = 50;
	}

	if ( gameflag[132] == 1 )
	{
		if ( stage != 0 )
		{
			BltRect( 18, 260, 2, 0, 32, 100, 8 );	/* replay */
		}

		if ( replay_load_key[0] == 1 )  /* l */
		{
			if ( replay_load_key[2] == 1 )  /* up */
			{
				BltRect( 67, 10, 150, 256, 0, 64, 64 );	/* l */
			}
			else if ( replay_load_key[3] == 1 )  /* down */
			{
				BltRect( 67, 10, 150, 192, 64, 64, 64 );	/* l */
			}
			else 
			{
				BltRect( 67, 10, 150, 128, 64, 64, 64 );	/* l */
			}
		}
		else if ( replay_load_key[1] == 1 )  /* r */
		{
			if ( replay_load_key[2] == 1 )  /* up */
			{
				BltRect( 67, 10, 150, 192, 0, 64, 64 );	/* r */
			}
			else if ( replay_load_key[3] == 1 )  /* down */
			{
				BltRect( 67, 10, 150, 256, 64, 64, 64 );	/* r */
			}
			else 
			{
				BltRect( 67, 10, 150, 64, 64, 64, 64 );	/* r */
			}
		}
		else 
		{
			if ( replay_load_key[2] == 1 )  /* up */
			{
				BltRect( 67, 10, 150, 64, 0, 64, 64 );	/* r */
			}
			else if ( replay_load_key[3] == 1 )  /* down */
			{
				BltRect( 67, 10, 150, 128, 0, 64, 64 );	/* r */
			}
			else 
			{
				BltRect( 67, 10, 150, 0, 0, 64, 64 );	/* r */
			}
		}
		if ( replay_load_key[4] == 1 )  /* 1 */
		{
			BltRect( 67, 90, 190, 32, 128, 32, 32 );	/* off */
		}
		else 
		{
			BltRect( 67, 90, 190, 0, 128, 32, 32 );	/* on */
		}
		if ( replay_load_key[5] == 1 )  /* 2 */
		{
			BltRect( 67, 130, 190, 32, 128, 32, 32 );	/* off */
		}
		else 
		{
			BltRect( 67, 130, 190, 0, 128, 32, 32 );	/* on */
		}
	}

	/* Game HUD in-game - Gameblabla */
	if ( stage != 0 )
	{	
		if ( gameflag[127] == 1 )	//�g�[�^���A�^�b�N
		{
			BltRect( 18, 5, DISPLY_HEIGHT - 20, 0, 128, 100, 8 );	/* rank */
		}
		else if ( stage == 2 )
		{
			BltRect( 18, 5, DISPLY_HEIGHT - 20, 0, 112, 100, 8 );	/* rank */
		}
		else 
		{
			if ( ( save_data[3] >= 1 ) && ( save_data[3] <= 15 ) )
			{
				BltRect( 18, 5, DISPLY_HEIGHT - 20, 0, 88, 100, 8 );	/* rank */
			}
			if ( ( save_data[3] >= 16 ) && ( save_data[3] <= 35 ) )
			{
				BltRect( 18, 5, DISPLY_HEIGHT - 20, 0, 96, 100, 8 );	/* rank */
			}
			if ( ( save_data[3] >= 36 ) && ( save_data[3] <= 50 ) )
			{
				BltRect( 18, 5, DISPLY_HEIGHT - 20, 0, 104, 100, 8 );	/* rank */
			}
		}

		BltRect( 18, 10, 2, 0, 40, 100, 8 );	/*  */
		BltNumericImage2( save_data[3], 2, 60, 2, 18, 0, 0, 10, 8 );	/* d_num */

		if ( gameflag[127] == 1 )	
		{
			BltRect( 18, 180, 2, 0, 120, 100, 8 );	/*  */
			BltNumericImage2( gameflag[136], 3, 220, 2, 18, 0, 0, 10, 8 );	/* miss count */
		}
		else 
		{
			BltRect( 18, 180, 2, 0, 80, 100, 8 );	/*  */
			BltNumericImage2( Jump_counts, 2, 230, 2, 18, 0, 0, 10, 8 );	/* Jump_counts */
		}
		
		BltRect( 18, 50, DISPLY_HEIGHT - 10, 0, 8, 100, 8 );	/*  */
		BltRect( 18, 5, DISPLY_HEIGHT - 10, 0, 16, 100, 8 );	/*  */
		
		BltRect( 18, 200, DISPLY_HEIGHT - 10 , 0, 8, 100, 8 );	/*  */
		BltRect( 18, 155, DISPLY_HEIGHT - 10 , 0, 24, 100, 8 );	/*  */

		BltNumericImage( play_time[0], 2, 110, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	/* ���� */
		BltNumericImage( play_time[1], 2, 80, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	/* ���� */
		BltNumericImage( play_time[2], 2, 50, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	/* ���� */

		if ( gameflag[127] == 1 )	
		{
			wk = get2keta( gameflag[137] , 1 );
			BltNumericImage( wk, 2, 260, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	
			wk = get2keta( gameflag[137] , 100 );
			BltNumericImage( wk, 2, 230, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	
			wk = get2keta( gameflag[137] , 10000 );
			BltNumericImage( wk, 2, 200, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	
		}
		else 
		{
			wk = get2keta( gameflag[200 + save_data[3] + stage_hosei] , 1 );
			BltNumericImage( wk, 2, 260, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	
			wk = get2keta( gameflag[200 + save_data[3] + stage_hosei] , 100 );
			BltNumericImage( wk, 2, 230, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	
			wk = get2keta( gameflag[200 + save_data[3] + stage_hosei] , 10000 );
			BltNumericImage( wk, 2, 200, DISPLY_HEIGHT - 10, 18, 0, 0, 10, 8 );	
		}
	}
	else 
	{
		BltRect( 18, 180, 2, 0, 80, 100, 8 );	/*  */
		BltNumericImage2( Jump_counts, 3, DISPLY_HEIGHT - 30, 2, 18, 0, 0, 10, 8 );	/* Jump_counts */
	}

}


void stage_start( )
{
	int x = 0;
	int y;
	int rec = 0;
	int rec2 = 0;

	KeyInput( );
	demo[0]++;
	rec = 32 - ( demo[0] * 2 );
	if ( rec < 0 )
	{
		rec = 0;
	}
	rec2 = demo[0];
	if ( rec2 > 16 )
	{
		rec2 = 16;
	}
	
	for ( x = 0; x < SCREEN_TILE_WIDTH; x++ )
	{
		for ( y = 0; y < SCREEN_TILE_HEIGHT; y++ )
		{
			BltRect( 72, ( x * 32 ) + rec2, ( y * 32 ) + rec2, 0, 0, rec, rec );
		}
	}

	if ( demo[0] >= 16 )
	{
		sn = 1;
	}

}

void stage_endj( )
{
	if ( player[8] <= 0 )
	{
		player[2] = 32;
		player[10] = 0;
		player[11] = 0;
		sn = 3;
		demo[0] = 0;
	}
}

void stage_claer( )
{
	int stage_hosei;
	int rec = 0;
	int rec2 = 0;
	int fead = 0;
	int x = 0;
	int y;
	
	stage_hosei = 0;
	if ( stage == 2 )
	{
		stage_hosei = 50;
	}
	
	demo[0]++;
	
	play_time[4] = 1000000 + ( play_time[2] * 10000 ) + ( play_time[1] * 100 ) + ( play_time[0] );

	if ( gameflag[127] == 0 )
	{
		if ( ( gameflag[300 + save_data[3] + stage_hosei] > Jump_counts ) )
		{
			if ( gameflag[132] == 0 )
			{
				Blt( 69, 0, 0 );
			}
		}
		if ( ( gameflag[200 + save_data[3] + stage_hosei] > play_time[4] ) )
		{
			if ( gameflag[132] == 0 )
			{
				Blt( 68, 0, 0 );
			}
		}
	}

	fead = demo[0] - 140;
	if ( fead < 0 )
	{
		fead = 0;
	}
	rec = ( fead * 2 );
	if ( rec > 32 )
	{
		rec = 32;
	}
	rec2 = 16 - fead;
	if ( rec2 < 0 )
	{
		rec2 = 0;
	}
	
	for ( x = 0; x < SCREEN_TILE_WIDTH; x++ )
	{
		for ( y = 0; y < SCREEN_TILE_HEIGHT; y++ )
		{
			BltRect( 72, ( x * 32 ) + rec2, ( y * 32 ) + rec2, 0, 0, rec, rec );
		}
	}

	if ( demo[0] > 180 )
	{
		if ( gameflag[132] == 0 )
		{
			if ( gameflag[127] == 1 )	//�g�[�^���A�^�b�N
			{
				gameflag[135] = play_time[4];
			}
			else 
			{
				replay_file_save_w( );
				if ( gameflag[200 + save_data[3] + stage_hosei] > play_time[4] )
				{
					gameflag[200 + save_data[3] + stage_hosei] = play_time[4];	/* �X�R�A�ۑ� */
				
					replay_file_save( 0 );
				}

				if ( gameflag[300 + save_data[3] + stage_hosei] > Jump_counts )
				{
					gameflag[300 + save_data[3] + stage_hosei] = Jump_counts;	/* �X�R�A�ۑ� */
				
					replay_file_save( 1 );
				}
			}
				
				save_data[3]++;
				if ( gameflag[121] < save_data[3] )
				{
					gameflag[121] = save_data[3];
				}
		}
		else 
		{
			replay_file_save_w( );
			save_data[3]++;
			if ( ( gameflag[124] == 1 ) || ( stage == 0 ) )	/* ���v���C�n�m�d */
			{
				gameflag[40] = 1;
				g_scene = EN_SN_TITLE;
			}
		}
		sn = 10;
	}

		if ( player[2] != 33 )
		{
			player[10] = 0 ;
			player[11] = 0 ;
			player[2] = 33 ;
		}
	
}

void stage_end( )
{
	int rec = 0;
	int rec2 = 0;
	int fead = 0;
	int x = 0;
	int y;

	demo[0]++;

	fead = demo[0] - 140;
	if ( fead < 0 )
	{
		fead = 0;
	}
	rec = ( fead * 2 );
	if ( rec > 32 )
	{
		rec = 32;
	}
	rec2 = 16 - fead;
	if ( rec2 < 0 )
	{
		rec2 = 0;
	}
	
	for ( x = 0; x < SCREEN_TILE_WIDTH; x++ )
	{
		for ( y = 0; y < SCREEN_TILE_HEIGHT; y++ )
		{
			BltRect( 72, ( x * 32 ) + rec2, ( y * 32 ) + rec2, 0, 0, rec, rec );
		}
	}

	if ( demo[0] > 180 )
	{
		if ( player[8] <= 0 )		/* ���C�t�O */
		{
			gameflag[136]++;
			if ( gameflag[136] > 999 )	/* �~�X�J�E���g */
			{
				gameflag[136] = 999;
			}
		}
		if ( gameflag[127] == 1 )	//�g�[�^���A�^�b�N
		{
			play_time[4] = 1000000 + ( play_time[2] * 10000 ) + ( play_time[1] * 100 ) + ( play_time[0] );
			gameflag[135] = play_time[4];
		}
		gameflag[71] = 1;
		sn = 10;
	}
}


void p_shot_set( Sint32 type , Sint32 muki, Sint32 houkou )
{
	Sint32 i;
		
	for ( i = 0; i < 10; i++ )
	{
		if ( pshot[( i * 10 ) + 0 ] == 0 )
		{
			pshot[( i * 10 ) + 0 ] = 1;
			pshot[( i * 10 ) + 1 ] = player[0];
			pshot[( i * 10 ) + 2 ] = player[1];
			pshot[( i * 10 ) + 3 ] = houkou;
			pshot[( i * 10 ) + 4 ] = muki;
			pshot[( i * 10 ) + 5 ] = 0;
			pshot[( i * 10 ) + 6 ] = 0;
			pshot[( i * 10 ) + 7 ] = type;
			pshot[( i * 10 ) + 8 ] = 0;		
			pshot[( i * 10 ) + 9 ] = 0;
			
			break;
		}
	}
}

void p_shot_disp( )
{
	Sint32 i;
	
	for ( i = 0; i < 10; i++ )
	{
		if ( pshot[( i * 10 ) + 0 ] == 1 )
		{
			pshot[( i * 10 ) + 1 ] = player[0];
			pshot[( i * 10 ) + 2 ] = player[1];
			
			if ( pshot[( i * 10 ) + 3 ] == 0 )
			{
				if ( pshot[( i * 10 ) + 4 ] == 0 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] - 16;
					pshot[( i * 10 ) + 2 ] = player[1] + 16;
					pshot[( i * 10 ) + 8 ] = 0;	
					pshot[( i * 10 ) + 9 ] = 0;	
				}
				if ( pshot[( i * 10 ) + 4 ] == 1 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] + 5;
					pshot[( i * 10 ) + 2 ] = player[1] + 10;
					pshot[( i * 10 ) + 8 ] = 0;		/* �����蔻����WX */
					pshot[( i * 10 ) + 9 ] = 0;		/* �����蔻����WX */
				}
				if ( pshot[( i * 10 ) + 4 ] == 2 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] + 5;
					pshot[( i * 10 ) + 2 ] = player[1] - 16;
					pshot[( i * 10 ) + 8 ] = 0;		/* �����蔻����WX */
					pshot[( i * 10 ) + 9 ] = 0;		/* �����蔻����WX */
				}
				if ( pshot[( i * 10 ) + 4 ] == 3 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] + 5;
					pshot[( i * 10 ) + 2 ] = player[1] - 43;
					pshot[( i * 10 ) + 8 ] = 0;		/* �����蔻����WX */
					pshot[( i * 10 ) + 9 ] = 0;		/* �����蔻����WX */
				}
				if ( pshot[( i * 10 ) + 4 ] == 4 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] - 16;
					pshot[( i * 10 ) + 2 ] = player[1] - 43;
					pshot[( i * 10 ) + 8 ] = 0;		/* �����蔻����WX */
					pshot[( i * 10 ) + 9 ] = 0;		/* �����蔻����WX */
				}
			}
			else if ( pshot[( i * 10 ) + 3 ] == 1 )
			{
				if ( pshot[( i * 10 ) + 4 ] == 0 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] - 16;
					pshot[( i * 10 ) + 2 ] = player[1] + 16;
				}
				if ( pshot[( i * 10 ) + 4 ] == 1 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] - 37;
					pshot[( i * 10 ) + 2 ] = player[1] + 10;
				}
				if ( pshot[( i * 10 ) + 4 ] == 2 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] - 37;
					pshot[( i * 10 ) + 2 ] = player[1] - 16;
				}
				if ( pshot[( i * 10 ) + 4 ] == 3 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] - 37;
					pshot[( i * 10 ) + 2 ] = player[1] - 43;
				}
				if ( pshot[( i * 10 ) + 4 ] == 4 )
				{
					pshot[( i * 10 ) + 1 ] = player[0] - 16;
					pshot[( i * 10 ) + 2 ] = player[1] - 43;
				}
			}


			BltRect( 3, pshot[( i * 10 ) + 1 ] - d_x, 480 - pshot[( i * 10 ) + 2 ] - 64 - d_y , pshot[( i * 10 ) + 6 ] * 64, pshot[( i * 10 ) + 3 ] * 256, 64, 64 );
			
			pshot[( i * 10 ) + 5 ]++;
			if ( pshot[( i * 10 ) + 5 ] >= 4 )
			{
				pshot[( i * 10 ) + 5 ] = 0;
				pshot[( i * 10 ) + 6 ]++;
				if ( pshot[( i * 10 ) + 6 ] >= 4 )
				{
					pshot[( i * 10 ) + 6 ] = 0;
					pshot[( i * 10 ) + 0 ] = 0;
				}
			}
		}
	}
}

void p_shot_f_set( Sint32 x, Sint32 y , Sint32 type )
{
	Sint32 i;
	
	for ( i = 0; i < 40; i++ )
	{
		if ( pshotf[( i * 10 ) + 0 ] == 0 )
		{
			pshotf[( i * 10 ) + 0 ] = 1;
			pshotf[( i * 10 ) + 1 ] = x;
			pshotf[( i * 10 ) + 2 ] = y;
			pshotf[( i * 10 ) + 5 ] = 0;
			pshotf[( i * 10 ) + 6 ] = 0;
			pshotf[( i * 10 ) + 7 ] = type;
			break;
		}
	}
}
void p_shot_f_disp( )
{
	Sint32 i;
	
	for ( i = 0; i < 40; i++ )
	{
		if ( pshotf[( i * 10 ) + 0 ] == 1 )
		{
			BltRect( 5, pshotf[( i * 10 ) + 1 ] - d_x, 480 - pshotf[( i * 10 ) + 2 ] - 32 - d_y , pshotf[( i * 10 ) + 6 ] * 32, pshotf[( i * 10 ) + 7 ] * 32, 32, 32 );

			pshotf[( i * 10 ) + 5 ]++;
			if ( pshotf[( i * 10 ) + 5 ] >= 5 )
			{
				pshotf[( i * 10 ) + 5 ] = 0;
				pshotf[( i * 10 ) + 6 ]++;
				if ( pshotf[( i * 10 ) + 6 ] >= 4 )
				{
					pshotf[( i * 10 ) + 0 ] = 0;
					pshotf[( i * 10 ) + 6 ] = 0;
				}
			}
		}
	}
}
void p_shot_f2_set( Sint32 x, Sint32 y , Sint32 muki )
{
	Sint32 i;
	Sint32 k; 
	Sint32 ii;
	Sint32 rg;
	Sint32 max;
	

	max = 10;
	
	for ( i = 0; i < max; i++ )
	{
		rg = ( ( rand( ) % 360 ) );
		
		k = ( rand( ) % 5 ) + 1;

		for ( ii = 0; ii < 80; ii++ )
		{
			if ( pshotf2[ii * 10 + 2] == 0 )
			{
				pshotf2[ii * 10 + 0] = funcSin( rg ) * k;		/* �e��x */
				pshotf2[ii * 10 + 1] = funcCos( rg ) * k;		/* �e��y */
				pshotf2[ii * 10 + 2] = 1;				/* ���� */									
				pshotf2[ii * 10 + 3] = 1;				/* ���� */
				pshotf2[ii * 10 + 4] = 0 ;			/* ������܂ł̃t���[���� */
				pshotf2[ii * 10 + 5] = ( x ) << 16 ;	/* �����ʒu�̐ݒ� */
				pshotf2[ii * 10 + 6] = ( y ) << 16 ;
				pshotf2[ii * 10 + 7] = 0 ;			/* ������܂ł̃t���[���� */
				pshotf2[ii * 10 + 8] = 2 ; 			/* ������܂ł̃t���[���� */
				break;
			}
		}
	}
}
void p_shot_f2_disp( )
{
	Sint32 ii;

	for ( ii = 0; ii < 80; ii++ )
	{
		if ( pshotf2[ii * 10 + 2] == 1 )
		{
			BltRect( 5, ( pshotf2[ii * 10 + 5] >> 16 ) - d_x, 480 - ( pshotf2[ii * 10 + 6] >> 16 ) - 4 - d_y, pshotf2[ii * 10 + 4] * 4, 32, 4, 4 );
			pshotf2[ii * 10 + 5] = pshotf2[ii * 10 + 0] + pshotf2[ii * 10 + 5];		/* �e��x */
			pshotf2[ii * 10 + 6] = pshotf2[ii * 10 + 1] + pshotf2[ii * 10 + 6];		/* �e��x */
			pshotf2[ii * 10 + 3] = 1;				/* ���� */
			pshotf2[ii * 10 + 7]++ ;			/* ������܂ł̃t���[���� */
			if ( pshotf2[ii * 10 + 7] >= pshotf2[ii * 10 + 8] )
			{
				pshotf2[ii * 10 + 7] = 0;
				pshotf2[ii * 10 + 4] = pshotf2[ii * 10 + 4] + 1 ;			/* ������܂ł̃t���[���� */
				if ( pshotf2[ii * 10 + 4] >= 10 )
				{
					pshotf2[ii * 10 + 2] = 0;				/* ���ݖ��� */									
				}
			}
		}
	}

}


void item_set( Sint32 type, Sint32 x, Sint32 y, Sint32 iti, Sint32 g_num )
{
	Sint32 i;
	
	if ( iti != 0 )
	{
		if ( 0 == item_sonzai( iti, g_num ) )
		{
			return;
		} 
	}	
	if ( iti < 0 )
	{
		return;
	}
		
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 0 )
		{
			item[( i * 10 ) + 0 ] = 1;
			item[( i * 10 ) + 1 ] = x;
			item[( i * 10 ) + 2 ] = y;
			item[( i * 10 ) + 3 ] = type;
			item[( i * 10 ) + 4 ] = iti;
			item[( i * 10 ) + 5 ] = g_num;
			item[( i * 10 ) + 6 ] = stage;	/* �X�e�[�W�� */
			item[( i * 10 ) + 7 ] = 0;	/* �C�x���g�����t���O�i1:�\���̂݁j */
			
			/* ���[�v�̃O���t�B�b�N�ύX */
			if ( ( save_data[30] == 1 ) && ( type == 5 ) )	/* ���[�v�\ */
			{
				item[( i * 10 ) + 3 ] = 6;					/* ���[�v�\�A�C�e���ɕύX */
			}
			
			/* 620�܂ł͔z�u���C�x���g���s�� */
			if ( ( type >= 100 ) && ( type <= 620 ) )
			{
//				init_event_set( item[( i * 10 ) + 3 ] , i );	/* �z�u���� */
			}
			break;
		}
	}
}
Sint32 item_sonzai( Sint32 map1, Sint32 map2 )
{
	Sint32 i;
	Sint32 rc;

	rc = 1;
	for ( i = 0; i < 100; i++ )
	{
		if ( item_wk[0 + ( i * 5 )] == 1 )
		{
			if ( ( item_wk[1 + ( i * 5 )] == map1 )
			  && ( item_wk[2 + ( i * 5 )] == map2 )
			  && ( item_wk[3 + ( i * 5 )] == stage ) )
			{
				rc = 0;
				break;
			}
		}
	}
	return( rc );

}


void item_disp( )
{
	Sint32 i;
	Sint32 k;
	Sint32 bx;
	Sint32 by;
	Sint32 k_f;
	Sint32 disp_x_j;
	Sint32 disp_y_j;

	k_f = 0;
	
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 1 )
		{
			bx = item[3 + ( i * 10 )] % 100 ;
			by = item[3 + ( i * 10 )] / 100 ;
			
			disp_x_j = item[1 + ( i * item_data[2] )];
			disp_y_j = ( 0 - item[2 + ( i * item_data[2] )] ) + MapInfo[3];
			if ( ( ( d_x - item_data[0] ) <= ( disp_x_j ) ) 
			  && ( ( d_y - item_data[0] ) <= ( disp_y_j  ) )
			  && ( ( d_x + DISPLY_WIDTH + item_data[0] ) >= ( disp_x_j ) )
			  && ( ( d_y + DISPLY_HEIGHT + item_data[0] ) >= ( disp_y_j ) ) )
			{

				BltRect( 12 , item[1 + ( i * 10 )] - d_x, ( 0 - item[2 + ( i * 10 )] ) + 480 - 32 - d_y, 32 * bx, by * 32, 32, 32 );
			}
			/* �v���C���[�Ƃ̓����蔻�� */
			if ( ( ( (item[1 + ( i * 10 )] + 2 ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 30 ) )
				&& ( (item[2 + ( i * 10 )]  + 2 ) < (player[1] + 22 ) )
				&& ( (player[1] + 5 ) < ( item[2 + ( i * 10 )] + 30 ) ) )
				&& ( sn == 1 ) )
			{
				if ( player[14] == 10 )
				{
					if ( ( item[( i * 10 ) + 3 ] >= 300 ) && ( item[( i * 10 ) + 3 ] <= 420 ) ) /* �d�Ȃ��Ĕ�������C�x���g */
					{
						if ( item[( i * 10 ) + 7 ] == 0 )
						{
							k_f = 1;
							if ( one_event == 0 )
							{
								one_event = 1;
//								event_set( item[( i * 10 ) + 3 ] , i );
							}
						}
					}
				}
				if ( 
				     ( item[( i * 10 ) + 3 ] == 1 ) /* �_���[�W��*/
				  || ( item[( i * 10 ) + 3 ] == 2 ) /* �_���[�W�� */
				  || ( item[( i * 10 ) + 3 ] == 3 ) /* �_���[�W�� */
				  || ( item[( i * 10 ) + 3 ] == 4 ) /* �_���[�W�� */
				  || ( item[( i * 10 ) + 3 ] == 703 ) /* �_���[�W�� */
				  || ( item[( i * 10 ) + 3 ] == 704 ) /* �_���[�W�� */
				   )
				{
					player[8] = player[8] - 1;
					soundPlaySe( EN_SE_DAMEGE );
				}
			}

			if ( sn == 1 )
			{
				if ( item[( i * 10 ) + 3 ] == 701 ) 	/* �ړ����A�c */
				{
					if ( item[8 + ( i * 10 )] == 0 )
					{
						if ( item[2 + ( i * 10 )] > 400 )
						{
							item[8 + ( i * 10 )] = 1;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] + 63 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] + 63 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 1;
							}
							else 
							{
								item[2 + ( i * 10 )]++;
							}
						}
					}
					else 
					{
						if ( item[2 + ( i * 10 )] < 16 )
						{
							item[8 + ( i * 10 )] = 0;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] - 0 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] - 0 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 0;
							}
							else 
							{
								item[2 + ( i * 10 )]--;
							}
						}
					}
				}
				if ( item[( i * 10 ) + 3 ] == 702 ) 	/* �ړ����A�� */
				{
					if ( item[8 + ( i * 10 )] == 0 )
					{
						if ( item[1 + ( i * 10 )] > 602 )
						{
							item[8 + ( i * 10 )] = 1;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] + 32 , item[2 + ( i * 10 )] + 16 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] + 32 , item[2 + ( i * 10 )] + 16 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 1;
							}
							else 
							{
								item[1 + ( i * 10 )]++;
								if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
									&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
									&& ( (item[2 + ( i * 10 )] + 27 ) < (player[1] + 10 ) )
									&& ( (player[1] - 1 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
								{
									player[0]++;
									idou_r( 1 );
								}
							}
						}
					}
					else 
					{
						if ( item[1 + ( i * 10 )] < 0 )
						{
							item[8 + ( i * 10 )] = 0;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] , item[2 + ( i * 10 )] + 16 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] , item[2 + ( i * 10 )] + 16 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 0;
							}
							else 
							{
								item[1 + ( i * 10 )]--;
								if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
									&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
									&& ( (item[2 + ( i * 10 )] + 27 ) < (player[1] + 10 ) )
									&& ( (player[1] - 1 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
								{
									player[0]--;
									idou_l( 1 );
								}
							}
						}
					}
				}
				if ( item[( i * 10 ) + 3 ] == 703 ) 	/* �ړ����A�c */
				{
					if ( item[8 + ( i * 10 )] == 0 )
					{
						if ( item[2 + ( i * 10 )] > 432 )
						{
							item[8 + ( i * 10 )] = 1;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] + 31 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] + 31 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 1;
							}
							else 
							{
								item[2 + ( i * 10 )]++;
								if ( player[37] == i )
								{
									player[33]++;
									idou_l( 1 );
									idou_r( 1 );
								}
							}
						}
					}
					else 
					{
						if ( item[2 + ( i * 10 )] < 16 )
						{
							item[8 + ( i * 10 )] = 0;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] - 1 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] + 16 , item[2 + ( i * 10 )] - 1 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 0;
							}
							else 
							{
								item[2 + ( i * 10 )]--;
								if ( player[37] == i )
								{
									player[33]--;
									idou_l( 1 );
									idou_r( 1 );
								}
							}
						}
					}
				}
				if ( item[( i * 10 ) + 3 ] == 704 ) 	/* �ړ����A�� */
				{
					if ( item[8 + ( i * 10 )] == 0 )
					{
						if ( item[1 + ( i * 10 )] > 602 )
						{
							item[8 + ( i * 10 )] = 1;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] + 32 , item[2 + ( i * 10 )] + 16 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] + 32 , item[2 + ( i * 10 )] + 16 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 1;
							}
							else 
							{
								item[1 + ( i * 10 )]++;
								if ( player[37] == i )
								{
									player[32]++;
								}
							}
						}
					}
					else 
					{
						if ( item[1 + ( i * 10 )] < 0 )
						{
							item[8 + ( i * 10 )] = 0;
						}
						else 
						{
							if ( ( map1_item_j( item[1 + ( i * 10 )] , item[2 + ( i * 10 )] + 16 ) >= 100 )
							  && ( map1_item_j( item[1 + ( i * 10 )] , item[2 + ( i * 10 )] + 16 ) <= 219 ) )
							{
								item[8 + ( i * 10 )] = 0;
							}
							else 
							{
								item[1 + ( i * 10 )]--;
								if ( player[37] == i )
								{
									player[32]--;
								}
							}
						}
					}
				}

				
				for ( k = 0; k < 40 ; k++ )
				{
					/* �v���C���[�̒e�Ƃ̓����蔻�� */
					if ( ( ( ( item[1 + i * 10] ) < ( ( pmshot[5 + k * 20] >> 16 ) + 32 ) )
						&& ( ( ( pmshot[5 + k * 20] >> 16 ) ) < ( item[1 + i * 10] + 32 ) )
						&& ( ( item[2 + i * 10] ) < ( ( pmshot[6 + k * 20] >> 16 ) + 32 ) )
						&& ( ( ( pmshot[6 + k * 20] >> 16 ) ) < ( item[2 + i * 10] + 32 ) ) ) 
						&& ( pmshot[2 + k * 20] == 1 )
						&& ( sn == 1 ) )
					{
	//					if ( item[( i * 10 ) + 3 ] == 1 ) /* �C�x���g */
	//					{
	//					}
					}
				}
			}
		}
	}
	if ( ( k_f == 0 ) && ( sn == 1 ) )
	{
		one_event = 0;
	}
}

Sint32 item_up( )
{
	Sint32 i;
	Sint32 bx;
	Sint32 by;
	
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 1 )
		{
			bx = item[3 + ( i * 10 )] % 100 ;
			by = item[3 + ( i * 10 )] / 100 ;
			
			if ( ( by == 5 ) 
			  || ( by == 6 ) /* �� */
			  || ( ( by == 0 ) && ( bx == 7 ) ) )
			{
				if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
					&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
					&& ( (item[2 + ( i * 10 )] + 27 ) < (player[1] + 10 ) )
					&& ( (player[1] - 1 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
				{
					return( item[2 + ( i * 10 )] + 32 );
				}
			}
			if ( ( by == 7 ) && ( bx == 1 ) )	/* 	�ړ����A�c */
			{
				if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
					&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
					&& ( (item[2 + ( i * 10 )] + 27 ) < (player[1] + 10 ) )
					&& ( (player[1] - 1 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
				{
					return( item[2 + ( i * 10 )] + 31 );
				}
			}
			if ( ( by == 7 ) && ( bx == 2 ) )	/* �ړ����A�� */
			{
				if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
					&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
					&& ( (item[2 + ( i * 10 )] + 27 ) < (player[1] + 10 ) )
					&& ( (player[1] - 1 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
				{
					return( item[2 + ( i * 10 )] + 31 );
				}
			}
		}
	}
	return( -1 );
}
Sint32 item_up2( )
{
	Sint32 i;
	Sint32 bx;
	Sint32 by;
	
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 1 )
		{
			bx = item[3 + ( i * 10 )] % 100 ;
			by = item[3 + ( i * 10 )] / 100 ;
			
			if ( ( by == 5 ) 
			  || ( by == 6 ) /* �� */
			  || ( ( by == 0 ) && ( bx == 7 ) ) )
			{
				if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
					&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
					&& ( (item[2 + ( i * 10 )] + 0 ) < (player[1] + 33 ) )
					&& ( (player[1] + 20 ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
				{
					return( item[2 + ( i * 10 )] - 32 );
				}
			}
		}
	}
	return( -1 );
}

/* �A�C�e���ɏd�Ȃ��ďオ�����ꂽ���̃C�x���g */
Sint32 item_event( )
{
	Sint32 i;
	Sint32 rc;
	Sint32 r_f;
	
	rc = 0;
	
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 1 )
		{
			r_f = 0;
			
			/* �v���C���[�Ƃ̓����蔻�� */
			if ( ( ( (item[1 + ( i * 10 )]  ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( (player[0] + 10 ) < (item[1 + ( i * 10 )] + 32 ) )
				&& ( (item[2 + ( i * 10 )] ) < (player[1] + 32 ) )
				&& ( (player[1] + 5 ) < ( item[2 + ( i * 10 )] + 32 ) ) )
				&& ( player[14] == 10 ) )
			{
				r_f = 1;
				player[18] = 0;	/* ���n���[�V�����Ƃق����炩���A�j���[�V�����̉��� */

			}

			if ( ( r_f == 1 ) && ( item[( i * 10 ) + 7 ] == 0 ) )
			{
				if ( ( item[( i * 10 ) + 3 ] >= 100 ) && ( item[( i * 10 ) + 3 ] <= 220 ) ) /* �Ŕ� */
				{
					rc = 1;
//					event_set( item[( i * 10 ) + 3 ] , i );
				}
				break;
			}
			
		}
	}
	return( rc );
}


//�G�쐬
void enm_set( Sint32 type, Sint32 x, Sint32 y )
{	
	Sint32 i;
	Sint32 size;
	Sint32 syu;
	Sint32 j;

	for ( i = 0; i < 20; i++ )
	{
		if ( enemy[0 + ( i * 20 )] == 0 )
		{
			for ( j = 0; j < 20 ; j++ )
			{
				enemy[j + ( i * 20 )] = 0;
			}
			
			
			enemy[0 + ( i * 20 )] = 1;
			enemy[1 + ( i * 20 )] = x;
			enemy[2 + ( i * 20 )] = y;
			size = type / 100;
			syu = type % 100;
			enemy[3 + ( i * 20 )] = size;
			if ( size >= 4 )	/* �{�X */
			{
				sprintf(string,ENM_IMAGE_PATH "boss%d.bmp", (int)syu );
				LoadBitmap(string,43,true);								//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
				syu = 1;
			}
			enemy[4 + ( i * 20 )] = syu;
			enemy[5 + ( i * 20 )] = e_hp( type ) ;
			enemy[6 + ( i * 20 )] = 0;				/* ����J�E���^ */
			enemy[7 + ( i * 20 )] = 0;				/* �ʒu�␳ */
			enemy[8 + ( i * 20 )] = 0;				/* �\���p�^�[�� */
			enemy[9 + ( i * 20 )] = 0;				/* ���� */
			enemy[10+ ( i * 20 )] = 0;				/* �\���x */
			enemy[11+ ( i * 20 )] = type;			/* ��� */
			enemy[12+ ( i * 20 )] = 0;				/* �W�����v�t���[���J�E���^ */
			enemy[13+ ( i * 20 )] = 0;				/* �W�����v�J�E���^ */
			enemy[14+ ( i * 20 )] = x;				/* �Ĕz�u���X */
			enemy[15+ ( i * 20 )] = y;				/* �Ĕz�u���Y */
			enemy[16+ ( i * 20 )] = 0;				/* ���G���� */
			enemy[17+ ( i * 20 )] = 0;
			enemy[18+ ( i * 20 )] = 0;
			enemy[19+ ( i * 20 )] = 0;				/*  */

			if ( size >= 4 )	/* �{�X */
			{
//				init_event_set_enm( enemy[11+ ( i * 20 )] , i );
				enemy[17+ ( i * 20 )] = 1;
			}
			break;
		}
	}
}

//�G�쐬
void enm_set2( Sint32 type, Sint32 x, Sint32 y ,Sint32 muki )
{	
	Sint32 i;
	Sint32 size;
	Sint32 syu;
	Sint32 j;

	for ( i = 0; i < 20; i++ )
	{
		if ( enemy[0 + ( i * 20 )] == 0 )
		{
			for ( j = 0; j < 20 ; j++ )
			{
				enemy[j + ( i * 20 )] = 0;
			}
			
			
			enemy[0 + ( i * 20 )] = 1;
			enemy[1 + ( i * 20 )] = x;
			enemy[2 + ( i * 20 )] = y;
			size = type / 100;
			syu = type % 100;
			enemy[3 + ( i * 20 )] = size;
			if ( size >= 4 )	/* �{�X */
			{
				sprintf(string,ENM_IMAGE_PATH "boss%d.bmp", (int)syu );
				LoadBitmap(string,43,true);								//�v���[���i���o�[�Q�ɃV�X�e���p�a�l�o��ǂݍ���
				syu = 1;
			}
			enemy[4 + ( i * 20 )] = syu;
			enemy[5 + ( i * 20 )] = e_hp( type ) ;
			enemy[6 + ( i * 20 )] = 0;				/* ����J�E���^ */
			enemy[7 + ( i * 20 )] = 0;				/* �ʒu�␳ */
			enemy[8 + ( i * 20 )] = 0;				/* �\���p�^�[�� */
			enemy[9 + ( i * 20 )] = muki;			/* ���� */
			enemy[10+ ( i * 20 )] = 0;				/* �\���x */
			enemy[11+ ( i * 20 )] = type;			/* ��� */
			enemy[12+ ( i * 20 )] = 0;				/* �W�����v�t���[���J�E���^ */
			enemy[13+ ( i * 20 )] = 0;				/* �W�����v�J�E���^ */
			enemy[14+ ( i * 20 )] = x;				/* �Ĕz�u���X */
			enemy[15+ ( i * 20 )] = y;				/* �Ĕz�u���Y */
			enemy[16+ ( i * 20 )] = 0;				/* ���G���� */
			enemy[17+ ( i * 20 )] = 0;
			enemy[18+ ( i * 20 )] = 0;
			enemy[19+ ( i * 20 )] = 0;				/*  */

			if ( size >= 4 )	/* �{�X */
			{
//				init_event_set_enm( enemy[11+ ( i * 20 )] , i );
				enemy[17+ ( i * 20 )] = 1;
			}
			break;
		}
	}
}


/* �G�̂g�o��ݒ�ie_set�Ŏg����j */
Sint32 e_hp( Sint32 type )
{
	Sint32 rc;
	rc = 1;
	if ( ( type / 100 ) == 1 )	/* 32 */
	{
		rc = enm1_parameter[ 10 * ( type - 100 ) + 0 ];
	}
	if ( ( type / 100 ) == 2 )	/* 48 */
	{
		rc = enm2_parameter[ 10 * ( type - 200 ) + 0 ];
	}
	if ( ( type / 100 ) == 3 )	/* 64 */
	{
		rc = enm3_parameter[ 10 * ( type - 300 ) + 0 ];
	}
	if ( ( type / 100 ) == 4 )	/* 128 */
	{
		rc = enm4_parameter[ 10 * ( type - 400 ) + 0 ];
	}
	return ( rc );
}

void enm_disp()
{	
	Sint32 i;
	Sint32 size ;
	Sint32 p_h;
	Sint32 size_hosei_x;
	Sint32 size_hosei_y;
	Sint32 isclaer;
	Sint32 disp_x_j;
	Sint32 disp_y_j;
	
	isclaer = 0; 
	size_hosei_x = 0;
	p_h = 32;	/* �v���C���[�̓����蔻�� */
	size = 0;
	
	for ( i = 0; i < 20; i++ )
	{
		if ( enemy[0 +( i * 20 )] == 1 )
		{
			isclaer = 1;
			/* ���G���Ԃ����݂���ꍇ�̓J�E���g */
			if ( enemy[16 + ( i * 20 )] > 0 )
			{
				enemy[16 + ( i * 20 )]--;
			}

			if ( enemy[3 + ( i * 20 )] == 1 )
			{
				size = 32;
			}
			if ( enemy[3 + ( i * 20 )] == 2 )
			{
				size = 48;
			}
			if ( enemy[3 + ( i * 20 )] == 3 )
			{
				size = 64;
			}
			if ( enemy[3 + ( i * 20 )] == 4 )	/* 5�{�X */
			{
				size = 128;
			}
			if ( enemy[3 + ( i * 20 )] == 5 )	/* 5�{�X */
			{
				size = 128;
			}
			if ( enemy[3 + ( i * 20 )] == 6 )	/* 6�{�X */
			{
				size = 128;
			}
			/* �{�X�̃T�C�Y�␳ */

			if ( enemy[11+ ( i * 20 )] == 401 )	
			{
//				size_hosei_x = 32;
				size_hosei_y = 20;
			} 
			else if ( enemy[11+ ( i * 20 )] == 403 )	
			{
				size_hosei_x = 32;
				size_hosei_y = 20;
			} 
			else if ( enemy[11+ ( i * 20 )] == 404 )	
			{
				size_hosei_x = 5;
				size_hosei_y = 20;
			} 
			else 
			{
				size_hosei_x = 0;
				size_hosei_y = 0;
			}

			/* ��ʓ��̓G�����`�悷�� */
			disp_x_j = enemy[1 + ( i * enm_data[2] )];
			disp_y_j = ( 0 - enemy[2 + ( i * enm_data[2] )] ) + MapInfo[3];

			if ( ( ( d_x - size ) <= ( disp_x_j ) ) 
			  && ( ( d_y - size ) <= ( disp_y_j  ) )
			  && ( ( d_x + DISPLY_WIDTH + size ) >= ( disp_x_j ) )
			  && ( ( d_y + DISPLY_HEIGHT + size ) >= ( disp_y_j ) ) )
			{
				if ( enemy[6 + ( i * 20 )] > 0 )	/* ���� */
				{
					if ( ( ( enemy[16 + ( i * 20 )] % 2 ) == 0 ) || ( sn != 1 ) )
					{
						if ( enemy[3 + ( i * 20 )] >= 4 )	/* �{�X�̂Ƃ��͕\�����@���Ⴄ */
						{
							BltRect( 39 + enemy[3 + ( i * 20 )] , enemy[1 + ( i * 20 )]  - d_x, ( 0 - enemy[2 + ( i * 20 )] ) + 480 - size  - d_y, ( size * 4 ) , size + ( size * enemy[9 + ( i * 20 )] ), size, size );
						}
						else 
						{
							BltRect( 39 + enemy[3 + ( i * 20 )] , enemy[1 + ( i * 20 )]  - d_x, ( 0 - enemy[2 + ( i * 20 )] ) + 480 - size  - d_y, ( size * 8 ) + ( enemy[9 + ( i * 20 )] *  size ) , size * ( enemy[4 + ( i * 20 )] + enemy[10+ ( i * 20 )] ), size, size );
						}
					}
				}
				else 
				{
						
					if ( ( ( enemy[16 + ( i * 20 )] % 2 ) == 0 ) || ( sn != 1 ) )
					{
						if ( enemy[3 + ( i * 20 )] >= 4 )	/* �{�X�̂Ƃ��͕\�����@���Ⴄ */
						{
							BltRect( 39 + enemy[3 + ( i * 20 )] , enemy[1 + ( i * 20 )]  - d_x, ( 0 - enemy[2 + ( i * 20 )] ) + 480 - size  - d_y, ( size * enemy[8 + ( i * 20 )] ) , size * ( enemy[4 + ( i * 20 )] + enemy[10+ ( i * 20 )] ) + ( enemy[9 + ( i * 20 )] * size ), size, size );
						}
						else 
						{
							if ( sn == 5 )
							{
								test[0] = 1;
							}
							BltRect( 39 + enemy[3 + ( i * 20 )] , enemy[1 + ( i * 20 )]  - d_x, ( 0 - enemy[2 + ( i * 20 )] ) + 480 - size  - d_y, ( size * enemy[8 + ( i * 20 )] ) + ( enemy[9 + ( i * 20 )] * ( size * 4 ) ) , size * ( enemy[4 + ( i * 20 )] + enemy[10+ ( i * 20 )] ), size, size );
						}
					}
				}
			}
			
			e_move( i );	/* �G�L������ޕʓ��� */
				

			if ( enemy[3 + ( i * 20 )] >= 4 )	/* 5�{�X */
			{
				if ( enemy[i * 20 + 1] > ( 640 - 128 ) )
				{
					enemy[i * 20 + 1] = ( 640 - 128 );
				}
				if ( enemy[i * 20 + 1] < 1 )
				{
					enemy[i * 20 + 1] = 1;
				}
			}
			
			if ( ( enemy[i * 20 + 1] >= 665 ) || ( enemy[i * 20 + 1] <= -100 ) || ( enemy[i * 20 + 2] <= -100 ) || ( enemy[i * 20 + 2] >= 500 ))
			{
				enemy[i * 20 + 0] = 0;
			}
/*
			if ( sn != 1 )
			{
				return;
			}
*/
			/* �v���C���[�Ƃ̓����蔻�� */
			if ( ( ( (enemy[1 + i * 20] + size_hosei_x ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( (player[0] + 10 ) < (enemy[1 + i * 20] + size - size_hosei_x ) )
				&& ( (enemy[2 + i * 20] + size_hosei_y ) < (player[1] + p_h) )
				&& ( (player[1] + 5 ) < ( enemy[2 + i * 20] + size - size_hosei_y ) ) ) 
				&& ( player[17] <= 0 ) 
				&& ( enemy[6 + i * 20] == 0 ) )
			{
				enemy[0 + ( i * 20 )] = 0;		/* ���� */
				soundPlaySe( EN_SE_L1 );
			}
#if false
			if ( ( ( (enemy[1 + i * 20] + size_hosei_x ) < (player[0] + player[30] + 32 ) )	/* �A���J�[�Ƃ̂����蔻�� */
				&& ( (player[0] + player[30] ) < (enemy[1 + i * 20] + size - size_hosei_x ) )
				&& ( (enemy[2 + i * 20] + size_hosei_y ) < (player[1] + player[31] + 32 ) )
				&& ( (player[1] + player[31] ) < ( enemy[2 + i * 20] + size - size_hosei_y ) ) ) 
				&& ( enemy[6 + i * 20] == 0 )
				&& ( player[23] == 1 )	/* �L�΂��Ă��� */
				&& ( player[24] == 0 )	/* �L�΂��Ă��� */
				&& ( is_tukami == 0 ) )	/* �L�΂��Ă��� */
			{
				if ( player[9] == 0 )
				{
					player[13] = 0;
					player[14] = 0;
					player[32] = enemy[1 + i * 20];
					player[33] = enemy[2 + i * 20];
					player[19] = player[21];
				}
				player[9] = 1;	/* �͂�ł��� */
				is_tukami = 1;
			}
#endif			
			/* �|���Ȃ��G�i�j���j */
//			if ( ( enemy[11 + i * 20]  == 105 )		/* �d�� */
//			  || ( enemy[11 + i * 20]  == 107 ) )	/* ���� */
//			{
//			}
//			else 
#if false
			{
				
				for ( k = 0; k < 10 ; k++ )
				{
					size2 = 32;
					/* �v���C���[�̒e�Ƃ̓����蔻�� */
					if ( ( ( (enemy[1 + i * 20]  ) < (pshot[1 + k * 10] + size2 ) )
						&& ( (pshot[1 + k * 10] ) < (enemy[1 + i * 20] + size ) )
						&& ( (enemy[2 + i * 20] ) < (pshot[2 + k * 10] + size2) )
						&& ( (pshot[2 + k * 10] ) < ( enemy[2 + i * 20] + size ) ) ) 
						&& ( pshot[0 + k * 10] == 1 ) )	/* �A�j���[�V�������R�� */
					{
						enemy[0 + ( i * 20 )] = 0;		/* ���� */
					}
					
					size2 = 32;
					/* �v���C���[�̒e�Ƃ̓����蔻�� */
					/* �v���C���[�̒e�Ƃ̓����蔻�� */
					if ( ( ( ( enemy[1 + i * 20] + size_hosei_x ) < ( ( pmshot[5 + k * 20] >> 16 )  + size2 ) )
						&& ( ( ( pmshot[5 + k * 20] >> 16 ) ) < ( enemy[1 + i * 20] + size - size_hosei_x  ) )
						&& ( ( enemy[2 + i * 20] + size_hosei_y ) < ( ( pmshot[6 + k * 20] >> 16 ) + size2 ) )
						&& ( ( ( pmshot[6 + k * 20] >> 16 ) ) < ( enemy[2 + i * 20] + size - size_hosei_y  ) ) ) 
						&& ( pmshot[2 + k * 20] == 1 ) )
					{
						/* ���G���Ԃ����݂��Ȃ� */
						if ( enemy[16 + ( i * 20 )] == 0 )
						{
							/* �U���������Ȃ� */
							if ( ( enemy[11 + ( i * 20 )] == 402 ) && ( pmshot[7 + k * 20] != 2 ) )
							{
								pmshot[2 + k * 20] = 0;		/* �e���� */
								p_shot_f_set( ( pmshot[5 + k * 20] >> 16 ) , ( pmshot[6 + k * 20] >> 16 ), 0 );
								p_shot_f2_set( ( pmshot[5 + k * 20] >> 16 ) + 15, ( pmshot[6 + k * 20] >> 16 ) + 15, 0 );
							}
							else 
							{
								
								p_shot_f2_set( ( pmshot[5 + k * 20] >> 16 ) + 15, ( pmshot[6 + k * 20] >> 16 ) + 15, 0 );
								
								if ( ( pmshot[3 + k * 20] == 2 )	/* �X�L���N���b�V���V���b�g */
								  || ( pmshot[3 + k * 20] == 4 ) )	/* �X�L���N���b�V���V���b�g */
								{
									p_shot_f_set( ( pmshot[5 + k * 20] >> 16 ) + ( rand( ) % 32 ) - 16, ( pmshot[6 + k * 20] >> 16 ) + ( rand( ) % 32 ) - 16, 0 );
									p_shot_f_set( ( pmshot[5 + k * 20] >> 16 ) + ( rand( ) % 32 ) - 16, ( pmshot[6 + k * 20] >> 16 ) + ( rand( ) % 32 ) - 16, 0 );
									p_shot_f_set( ( pmshot[5 + k * 20] >> 16 ) + ( rand( ) % 32 ) - 16, ( pmshot[6 + k * 20] >> 16 ) + ( rand( ) % 32 )- 16, 0 );
									enemy[5 + ( i * 20 )] = enemy[5 + ( i * 20 )] - 3;
								}
								else 
								{
									p_shot_f_set( ( pmshot[5 + k * 20] >> 16 ) , ( pmshot[6 + k * 20] >> 16 ), 0 );
									enemy[5 + ( i * 20 )] = enemy[5 + ( i * 20 )] - 1;
								}

								pmshot[2 + k * 20] = 0;		/* �e���� */
								if ( enemy[5 + ( i * 20 )] <= 0 )	/* �|���� */
								{

									if ( enemy[3 + ( i * 20 )] >= 4 )	/* 4�{�X */
									{
										boss_dead( enemy[1 + ( i * 20 )], enemy[2 + ( i * 20 )] );
										event_set_enm( enemy[11 + ( i * 20 )] , i );
										for ( l = 0; l < 20; l++ )
										{
											enemy[5 + ( l * 20 )] = 0;
											enemy[6 + ( l * 20 )] = 60;		/* ���� */
											enemy[16 + ( l * 20 )] = 60;
										}
									}
									
									enemy[6 + ( i * 20 )] = 60;		/* ���� */
									enemy[16 + ( i * 20 )] = 60;

								}
								else 
								{
									/* �|���ĂȂ� */
									/* ���G���Ԑݒ� */
									enemy[6 + ( i * 20 )] = 30;		/* �̂����莞�� */
	//								enemy[7 + ( i * 20 )] = 6;		/* �ʒu�␳ */
									enemy[16 + ( i * 20 )] = 60;	/* ���G���� */
									if ( enemy[3 + ( i * 20 )] >= 4 )	/* �{�X�̂Ƃ��͂̂����肪�Ȃ� */
									{
										enemy[6 + ( i * 20 )] = 0;		/* �̂����莞�� */
	//									enemy[7 + ( i * 20 )] = 0;		/* �ʒu�␳ */
									}
								}
							}
						}
					}
				}
			}
#endif
			if ( enemy[7 + ( i * 20 )] > 0 )
			{
				enemy[7 + ( i * 20 )]--;					/* �ړ��␳ */
				if ( enemy[9 + ( i * 20 )] == 0 )
				{
					if ( ( ( map1_item_j( enemy[1 + ( i * 20 )] - 4 , enemy[2 + ( i * 20 )] + 2 ) >= 100 )
					    && ( map1_item_j( enemy[1 + ( i * 20 )] - 4 , enemy[2 + ( i * 20 )] + 2 ) <= 119 ) )
					  || ( ( map1_item_j( enemy[1 + ( i * 20 )] - 4 , enemy[2 + ( i * 20 )] + 2 ) >= 200 )
					    && ( map1_item_j( enemy[1 + ( i * 20 )] - 4 , enemy[2 + ( i * 20 )] + 2 ) <= 219 ) ) )
					{
					}
					else 
					{
						enemy[1 + ( i * 20 )] = enemy[1 + ( i * 20 )] - ( enemy[7 + ( i * 20 )] * enemy[7 + ( i * 20 )] );
					}
				}
				else 
				{
					if ( ( ( map1_item_j( enemy[1 + ( i * 20 )] + size + 4, enemy[2 + ( i * 20 )] + 2 ) >= 100 )
					    && ( map1_item_j( enemy[1 + ( i * 20 )] + size + 4, enemy[2 + ( i * 20 )] + 2 ) <= 119 ) )
					  || ( ( map1_item_j( enemy[1 + ( i * 20 )] + size + 4, enemy[2 + ( i * 20 )] + 2 ) >= 200 )
					    && ( map1_item_j( enemy[1 + ( i * 20 )] + size + 4, enemy[2 + ( i * 20 )] + 2 ) <= 219 ) ) )
					{
					}
					else 
					{
						enemy[1 + ( i * 20 )] = enemy[1 + ( i * 20 )] + ( enemy[7 + ( i * 20 )] * enemy[7 + ( i * 20 )] );
					}
				}
			}
			
			if ( enemy[6 + ( i * 20 )] > 0 )
			{
				enemy[6 + ( i * 20 )]--;					/* ������܂ŃJ�E���^ */
				if ( enemy[6 + ( i * 20 )] == 0 )
				{
					if ( enemy[5 + ( i * 20 )] <= 0 )
					{
						if ( enemy[3 + ( i * 20 )] >= 4 )
						{
						}
						else 
						{
							if ( ( rand( ) % 2 ) == 0 )	/* ���C�t�o�� */
							{
								kane_set( enemy[1 + ( i * 20 )], enemy[2 + ( i * 20 )] );
							}
						}
						enemy[0 + ( i * 20 )] = 0;		/* ���� */
					}
				}
				
			}
		}
	}

	if (isclaer == 0)	
	{
		sn = 2;
		demo[0] = 0;
	} 
}

void e_move( Sint32 i )
{
	
	if ( enemy[6 + ( i * 20 )] > 0 )
	{
		return;
	}
	if ( enemy[5 + ( i * 20 )] <= 0 )	/* �|���� */
	{
		return;
	}
	
	enemy[8 + ( i * 20 )] = 0;		/* �A�j���[�V�����p�^�[�� */
	enemy[14+ ( i * 20 )] = 0;

}

/* �{�X��|�������o */
void boss_dead( Sint32 x, Sint32 y )
{
/*	
	Sint32 i;
	Sint32 xx;
	Sint32 yy;
	for ( i = 0; i < 100; i++ ) 
	{
//		BltFromBackupPlane();		//	�o�b�N�A�b�v�v���[�������Blt
		if ( ( i % 10 ) == 0 )
		{
			xx = ( rand( ) % 96 );
			yy = ( rand( ) % 96 );
			se_wav( 24 + ( rand( ) % 3 ) );
			p_shot_f_set( x + xx, y + yy, 0 );
			p_shot_f2_set(  x + xx + 15, y + yy + 15, 0 );
		}
		p_shot_f_disp( );
		p_shot_f2_disp( );
		halt;
	}
*/
}

//-----------------------------------------------------------------------------------
/* �G�V���b�g���� */
void e_shot_set( Sint32 type , Sint32 i , Sint32 x, Sint32 y )
{
	Sint32 ii;
	Sint32 j;
	Sint32 rg;
	/* See below. - Gameblabla */
	/*
	Sint32 mx;
	Sint32 my;
	*/
	
	/* �����ʒuX */
	/* �����ʒuY */
	for ( ii = 0; ii < 20; ii++ )
	{
		if ( mgc_e[ii * 10 + 0] == 0 )					/* ���݂��ĂȂ� */
		{

			for ( j = 0 ; j <= 9 ;j++ )
			{
				mgc_e[ii * 10 + j] = 0;
			}
			if ( type == 1061 )							/* ���h�J�� */
			{
				/* Same issue here. rg = funcTan2 gets unused. Therefore, mx and my also get unused. - Gameblabla*/
				//mx = ( ( 32 / 2 ) - 5 ) + player[0] - x;
				//my = ( ( 32 / 2 ) - 5 ) + player[1] - y;
				//rg = funcTan2( mx, my );
				rg = ( 90 );
				mgc_e[ii * 10 + 1] = x << 16;		/* X */
				mgc_e[ii * 10 + 2] = y << 16;		/* Y */
				mgc_e[ii * 10 + 3] = 0;		/* �A�j���[�V�����J�E���^ */
				mgc_e[ii * 10 + 4] = funcSin( rg ) * 4;		/* �ړ����� */
				mgc_e[ii * 10 + 5] = funcCos( rg ) * 4;	/* �ړ����� */
				mgc_e[ii * 10 + 6] = 6;		/* �_���[�W */
				mgc_e[ii * 10 + 7] = 2;		/* ���� */
				mgc_e[ii * 10 + 9] = type;					/* �^�C�vplayer[10] */
			}

			mgc_e[ii * 10 + 0] = 1;					/* ���݂��� */
			break;
		}
	}

}
/* �V���b�g�ړ� */
void e_shot_disp( )
{
	Sint32 i;
	Sint32 p_h;
	Sint32 size ;
	
	p_h = 22;	/* �v���C���[�̓����蔻�� */
	size = 0;
	
	/* �����ʒuX */
	/* �����ʒuY */
	for ( i = 0; i < 20; i++ )
	{
		if ( mgc_e[i * 10 + 0] == 1 )					/* ���݂��Ă��� */
		{
			if ( mgc_e[i * 10 + 9] == 4010 ) 	/* �P�{�X */
			{
				size = 32;
				BltRect( 50, ( mgc_e[i * 10 + 1] >> 16 ) - d_x , ( 0 - ( mgc_e[i * 10 + 2] >> 16 ) ) + 480 - 32 - d_y, mgc_e[i * 10 + 3] * 32, 0 , 32, 32 );
				mgc_e[i * 10 + 1] = mgc_e[i * 10 + 1] + mgc_e[i * 10 + 4];		/* X */
				mgc_e[i * 10 + 2] = mgc_e[i * 10 + 2] + mgc_e[i * 10 + 5];		/* Y */
				mgc_e[i * 10 + 3]++;					/* �A�j���[�V�����J�E���^ */
				if ( mgc_e[i * 10 + 3] >= 4 )
				{
					mgc_e[i * 10 + 3] = 0;
				}
			}
			
			/* �v���C���[�Ƃ̓����蔻�� */
			if ( ( ( ( ( mgc_e[1 + i * 10] >> 16 ) ) < (player[0] + 22 ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( (player[0] + 10 ) < ( ( mgc_e[1 + i * 10] >> 16 ) + size ) )
				&& ( ( ( mgc_e[2 + i * 10] >> 16 ) ) < (player[1] + p_h) )
				&& ( (player[1] + 5  ) < ( ( mgc_e[2 + i * 10] >> 16 ) + size ) ) ) 
				&& ( player[17] == 0 ) )
			{
				player[10] = 0;
				player[11] = 0;
				player[12] = 4;
				player[13] = 0;
				player[2] = 31;	/* �_���[�W */
				player[17] = 120;	/* ���G���� */
				player[8] = player[8] - 1;
			}


			if ( ( ( mgc_e[i * 10 + 1] >> 16 ) >= 640 ) || ( ( mgc_e[i * 10 + 1] >> 16 ) <= 0 ) || ( ( mgc_e[i * 10 + 2] >> 16 ) <= -100 )|| ( ( mgc_e[i * 10 + 2] >> 16 ) >= 480 ))
			{
				mgc_e[i * 10 + 0] = 0;
			}
		}
	}
}

/* �G�p�W�����v */
void enmjmp( Sint32 i )
{
	Sint32 y1;
	Sint32 px ;
	Sint32 px1 ;
	Sint32 px2 ;
	Sint32 py ;

	Sint32 size;
	Sint32 e10;

	if ( enemy[3 + ( i * 20 )] == 1 )
	{
		size = 16;
	}
	else if ( enemy[3 + ( i * 20 )] == 2 )
	{
		size = 24;
	}
	else
	{
		size = 16;
	}

	if ( enemy[13 + ( i * 20 )] == 10 ) 
	{
		/* ����̈ʒu */
		// Seems to not get used here - Gameblabla
		//px = ( enemy[1 + ( i * 20 )] + 32 ) / 32;
		px1 = ( enemy[1 + ( i * 20 )] + size ) / 32;
		px2 = ( enemy[1 + ( i * 20 )] + size ) / 32;
		py = ( 16 - ( ( enemy[2 + ( i * 20 )] + 28 ) / 32 ) );
		/* 100 �` 200�͒n�� */
		if ( ( !( ( map1[ px1 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 119 ) ) )
		  && ( !( ( map1[ px2 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 119 ) ) ) 
		  && ( !( ( map1[ px1 + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 219 ) ) )
		  && ( !( ( map1[ px2 + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 219 ) ) ) 
		  && ( !( ( map1[ px1 + ( ( py ) * 20 ) ] >= 600 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 619 ) ) )
		  && ( !( ( map1[ px2 + ( ( py ) * 20 ) ] >= 600 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 619 ) ) ) )
		{
			enemy[13+ ( i * 20 )] = 0;
			enemy[12+ ( i * 20 )] = 0;
		}
		else 
		{
			return;
		}
	}

	enemy[12 + ( i * 20 )]++;
	if ( enemy[12+ ( i * 20 )] >= 10 )
	{
		enemy[12+ ( i * 20 )] = 0;
		
		enemy[13 + ( i * 20 )] = enemy[13 + ( i * 20 )] - 3;
//		if ( player[5] == 0 )
//		{
//			player[5] = -3;
//		}
		if ( ( enemy[13 + ( i * 20 )] <= 2 ) && ( enemy[13 + ( i * 20 )] > -2 ) )
		{
			enemy[13 + ( i * 20 )] = -3;
		}
		if ( enemy[13 + ( i * 20 )] < -8 )
		{
			enemy[13 + ( i * 20 )] = -8;
		}
		/* �n�ʔ��� */
	}


	/* ��ʒ[�ŗ�����̂�h�~ */
	e10 = enemy[13 + ( i * 20 )];
	if (  enemy[1+ ( i * 20 )] > ( 630 - size - size ) )
	{
		if ( enemy[13+ ( i * 20 )] < 0 )	/* ���� */
		{
			e10 = 0;
		}
	}


	/* ����̈ʒu */
	y1 = ( ( 0 - e10 ) * ( 0 - e10 ) * ( 0 - e10 ) );
//	player[1] = player[1] - ( y1 / 30 );


	enemy[2 + ( i * 20 )] = enemy[2 + ( i * 20 )] - ( y1 / 25 );
	px1 = ( enemy[1 + ( i * 20 )] + size ) / 32;
	px2 = ( enemy[1 + ( i * 20 )] + size ) / 32;
	py = ( 16 - ( ( enemy[2 + ( i * 20 )] + 28 ) / 32 ) );

	if ( enemy[2 + ( i * 20 )] > 480 ) 
	{
		return;
	}

	if ( enemy[13 + ( i * 20 )] <= 0 )													/* ��蒆�͔��肵�Ȃ� */
	{
		/* �n�ʔ��� */
		if ( ( ( map1[ px1 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 119 ) )
		  || ( ( map1[ px1 + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 219 ) ) 
		  || ( ( map1[ px1 + ( ( py ) * 20 ) ] >= 600 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 619 ) )
		  || ( ( map1[ px2 + ( ( py ) * 20 ) ] >= 600 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 619 ) ) )
		{
			enemy[2 + ( i * 20 )] = ( ( 16 - py ) * 32 ) - 16; 
			enemy[13 + ( i * 20 )] = 10;
		}
	}
	else		/* �㏸�� */
	{
		if ( enemy[1+ ( i * 20 )] < ( 630 - size - size ) )
		{
			px = ( enemy[1 + ( i * 20 )] + size ) / 32;
			py = ( 16 - ( ( enemy[2 + ( i * 20 )] - ( y1 / 25 ) + 16 + 48  ) / 32 ) );

			/* �n�ʔ��� */
			if ( ( ( map1[ px + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px + ( ( py ) * 20 ) ] <= 119 ) )
			  || ( ( map1[ px + ( ( py ) * 20 ) ] >= 200 ) && ( map1[ px + ( ( py ) * 20 ) ] <= 219 ) ) )
			{
				enemy[13+ ( i * 20 )] = -3;
				enemy[12+ ( i * 20 )] = 0;
			}
		}

	}
	if ( enemy[2 + ( i * 20 )] <= -14 )
	{
		enemy[0 + ( i * 20 )] = 0;
	}

}


/* �ړ���̍��W�̃}�X��Ԃ� */
Sint32 map1_item_j( Sint32 x, Sint32 y )
{
	Sint32 px ;
	Sint32 py ;
	Sint32 i;
	Sint32 rc;
	
	px = ( x ) / 32;
	py = ( 15 - ( ( y + 16 ) / 32 ) );

	if ( py <= 0 )
	{
		py = 1;
	}
	
	rc = map1[ px + ( ( py ) * 20 ) ];
	
	/* �A�C�e���Ƃ̂����蔻�� */
	for ( i = 0; i < 50; i++ )
	{
		if ( item[( i * 10 ) + 0 ] == 1 )
		{
			/* �v���C���[�Ƃ̓����蔻�� */
			if ( ( ( (item[1 + ( i * 10 )]  ) < ( x ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( ( x ) < (item[1 + ( i * 10 )] + 31 ) )
				&& ( (item[2 + ( i * 10 )] ) < ( y ) )
				&& ( ( y ) < ( item[2 + ( i * 10 )] + 32 ) ) ) )
			{
				if ( ( item[( i * 10 ) + 3 ] >= 500 ) && ( item[( i * 10 ) + 3 ] <= 620 ) ) /* �ړ��s�A�C�e�� */
				{
					rc = 101;
					break;
				}
			}
		}

	}


	return( rc );
	
	
	
	
}


void kane_set( Sint32 x, Sint32 y )
{
	Sint32 i;
	
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
			kane[6 + ( i * 10 )] = 6;
			kane[7 + ( i * 10 )] = 0;
			kane[8 + ( i * 10 )] = 0;
			kane[9 + ( i * 10 )] = 0;
			break;
		}
	}
}

void kane_disp(  )
{
	Sint32 i;
	
	for ( i = 0; i < 20; i++ )
	{
		if ( kane[0 + ( i * 10 )] == 1 )
		{
			kane[8 + ( i * 10 )]++;
			if ( kane[8 + ( i * 10 )] >= 10 )
			{
				kane[8 + ( i * 10 )] = 0;
				kane[7 + ( i * 10 )]++;
				if ( kane[7 + ( i * 10 )] >= 4 )
				{
					kane[7 + ( i * 10 )] = 0;
				}
			} 
			if ( ( ( kane[9 + ( i * 10 )] % 2 ) == 0 ) || ( kane[9 + ( i * 10 )] < 240 ) )
			{
				BltRect( 12 , kane[1 + ( i * 10 )] - d_x, ( 0 - kane[2 + ( i * 10 )] ) + 480 - 32 - d_y, 32 * 4 , 0, 32, 32 );
			}
			
			kane[9 + ( i * 10 )]++;
			if ( kane[9 + ( i * 10 )] >= 300 )
			{
				kane[0 + ( i * 10 )] = 0;
			} 
			kane[5 + ( i * 10 )]++;
			if ( kane[5 + ( i * 10 )] >= 10 )
			{
				kane[5 + ( i * 10 )] = 0;
			} 

			k_jmp( i );
			/* �Ƃ��� */
			if ( ( ( (kane[1 + ( i * 10 )]  ) < (player[0] + 10 ) )	/* ���@�Ƃ̂����蔻�� */
				&& ( (player[0] + 22 ) < (kane[1 + ( i * 10 )] + 32 ) )
				&& ( (kane[2 + ( i * 10 )] ) < (player[1] + 32 ) )
				&& ( (player[1] + 5 ) < ( kane[2 + ( i * 10 )] + 32 ) ) ) )
			{
				player[8]++;
				if ( player[8] > save_data[6] )
				{
					player[8] = save_data[6];
				}
				kane[0 + ( i * 10 )] = 0;
			}
		}
	}
}
/* �W�����v�x�ʒu�v�Z */
void k_jmp( Sint32 i )
{
	Sint32 y1;
	Sint32 px1 ;
	Sint32 px2 ;
	Sint32 py ;
	Sint32 py3 ;

	if ( kane[6 + ( i * 10 )] == 10 ) 
	{
		return;
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
	
	/* ��ʒ[�ŗ�����̂�h�~ */
	if ( ( kane[1 + ( i * 10 )] < 0 ) || ( kane[1 + ( i * 10 )] > 610 ) )
	{
		kane[6 + ( i * 10 )] = 0;
	}
	/* ����̈ʒu */
	y1 = ( ( 0 - kane[6 + ( i * 10 )] ) * ( 0 - kane[6 + ( i * 10 )] ) * ( 0 - kane[6 + ( i * 10 )] ) );

	px1 = ( kane[1 + ( i * 10 )] + 16 ) / 32;
	px2 = ( kane[1 + ( i * 10 )] + 16 ) / 32;
	py = ( 16 - ( ( kane[2 + ( i * 10 )] + 28 ) / 32 ) );
	kane[2 + ( i * 10 )] = kane[2 + ( i * 10 )] - ( y1 / 25 );
	py3 = ( 16 - ( ( kane[2 + ( i * 10 )] + 32 + 32 + 16 ) / 32 ) );

	if ( kane[2 + ( i * 10 )] > 480 ) 
	{
		return;
	}

	if ( kane[6 + ( i * 10 )] <= 0 ) 													/* ��蒆�͔��肵�Ȃ� */
	{
		/* �n�ʔ��� */
		if ( ( ( map1[ px1 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 299 ) )
		  || ( ( map1[ px2 + ( ( py ) * 20 ) ] >= 100 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 299 ) ) 
		  || ( ( map1[ px1 + ( ( py ) * 20 ) ] >= 600 ) && ( map1[ px1 + ( ( py ) * 20 ) ] <= 619 ) )
		  || ( ( map1[ px2 + ( ( py ) * 20 ) ] >= 600 ) && ( map1[ px2 + ( ( py ) * 20 ) ] <= 619 ) ) )
		{
			kane[2 + ( i * 10 )] = ( ( 16 - py ) * 32 ) - 16; 
			kane[6 + ( i * 10 )] = 10;
		}
	}
	else		/* �㏸�� */
	{
		/* �n�ʔ��� */
		if ( ( ( map1[ px1 + ( ( py3 ) * 20 ) ] >= 100 ) && ( map1[ px1 + ( ( py3 ) * 20 ) ] <= 119 ) )
		  || ( ( map1[ px1 + ( ( py3 ) * 20 ) ] >= 200 ) && ( map1[ px1 + ( ( py3 ) * 20 ) ] <= 219 ) )
		  || ( ( map1[ px2 + ( ( py3 ) * 20 ) ] >= 100 ) && ( map1[ px2 + ( ( py3 ) * 20 ) ] <= 119 ) )
		  || ( ( map1[ px2 + ( ( py3 ) * 20 ) ] >= 200 ) && ( map1[ px2 + ( ( py3 ) * 20 ) ] <= 219 ) ) )
		{
			kane[6 + ( i * 10 )] = -1;
			kane[5 + ( i * 10 )] = 0;
		}
	}
	if ( kane[2 + ( i * 10 )] <= -14 )
	{
		kane[0 + ( i * 10 )] = 0;
	}

}

void p_jump_f_set( Sint32 x, Sint32 y )
{
	Sint32 i;
	
	for ( i = 0; i < 10; i++ )
	{
		if ( pjumpf[( i * 10 ) + 0 ] == 0 )
		{
			pjumpf[( i * 10 ) + 0 ] = 1;
			pjumpf[( i * 10 ) + 1 ] = x;
			pjumpf[( i * 10 ) + 2 ] = y;
			pjumpf[( i * 10 ) + 5 ] = 0;
			pjumpf[( i * 10 ) + 6 ] = 0;
			break;
		}
	}
}

void p_jump_f_disp( )
{
	Sint32 i;
	
	for ( i = 0; i < 10; i++ )
	{
		if ( pjumpf[( i * 10 ) + 0 ] == 1 )
		{
			BltRect( 5, pjumpf[( i * 10 ) + 1 ] - d_x, 480 - pjumpf[( i * 10 ) + 2 ] - 32 - d_y , pjumpf[( i * 10 ) + 6 ] * 32, 48, 32, 32 );
			
			pjumpf[( i * 10 ) + 5 ]++;
			if ( pjumpf[( i * 10 ) + 5 ] >= 5 )
			{
				pjumpf[( i * 10 ) + 5 ] = 0;
				pjumpf[( i * 10 ) + 6 ]++;
				if ( pjumpf[( i * 10 ) + 6 ] >= 4 )
				{
					pjumpf[( i * 10 ) + 6 ] = 0;
					pjumpf[( i * 10 ) + 0 ] = 0;
				}
			}
		}
	}
}

void screen_BackupPlane( )
{
}

/* ���j���[�̕\�� */
void window_disp( )
{
	BltRect( 61, 96, 64 + ( 32 * demo[0] ), 0, gameflag[122] * 32 , 32, 32 );
	Blt( 60, 0, 0 );
}

/* ���j���[�ł̃L�[���� */
void window_keys( )
{
	if ( IsPushKey( gameflag[0] ) )
	{
		soundPlaySe( EN_SE_SELECT );
		demo[0]--;
		if ( demo[0] < 0 )
		{
			demo[0] = 2;
		}
	}
	else if ( IsPushKey( gameflag[1] ) )
	{
		soundPlaySe( EN_SE_SELECT );
		demo[0]++;
		if ( demo[0] > 2 )
		{
			demo[0] = 0;
		}
	}
	//	�X�y�[�X�L�[���������
	if ( IsPushOKKey( ) ) 
	{
		if ( demo[0] == 0 )
		{
			sn = 1;
		}
		if ( demo[0] == 1 )
		{
			gameflag[71] = 1;
			sn = 10;
		}
		if ( demo[0] == 2 )
		{
			g_scene = EN_SN_TITLE;	/* �^�C�g���� */
//			gameflag[40] = 1;
			sn = 10;
		}
	}
	//B���������
	if ( IsPushCancelKey( ) )
	{
		sn = 1;
	}

}



/* ��Βl��Ԃ� */
Sint32 labs_gnp( Sint32 para )
{
	Sint32 rc;
	
	rc = para;
	
	if ( para < 0 )
	{
		rc = para * -1;
	}
	return( rc );
}

/* ���@���� */
void p_mgc_set( Sint32 muki , Sint32 p3 )
{
	Sint32 i;
	Sint32 k; 
	Sint32 ii;
	Sint32 rg;
	Sint32 max;
	
	rg = 0;
	
	if ( muki == 0 )
	{
		rg = ( 0 );
	}
	else if ( muki == 1 )
	{
		rg = ( 45 );
	}
	else if ( muki == 2 )
	{
		rg = ( 90 );
	}
	else if ( muki == 3 )
	{
		rg = ( 135 );
	}
	else if ( muki == 4 )
	{
		rg = ( 180 );
	}

	if ( p3 == 1 )
	{
		rg = rg * -1;
	}
 
	/* �����ʒuX */
	/* �����ʒuY */

	max = 1;
	for ( i = 0; i < max; i++ )
	{

		k = 5;

		for ( ii = 0; ii < 40; ii++ )
		{
			if ( pmshot[ii * 20 + 2] == 0 )
			{
				pmshot[ii * 20 + 0] = funcSin( rg ) * k;			/* �e��x */
				pmshot[ii * 20 + 1] = funcCos( rg ) * k;			/* �e��y */
				pmshot[ii * 20 + 2] = 1;						/* ���� */
				pmshot[ii * 20 + 3] = 0;						/* ��� */
				pmshot[ii * 20 + 4] = 0 ;						/*  */
				pmshot[ii * 20 + 5] = ( player[0] ) << 16 ;			/* �����ʒu�̐ݒ� */
				pmshot[ii * 20 + 6] = ( player[1] ) << 16 ;
				pmshot[ii * 20 + 7] = muki;						/* ���� */
				
				pmshot[ii * 20 + 8] = 0;						/*  */
				pmshot[ii * 20 + 9] = 0 ;						/*  */
				pmshot[ii * 20 + 10] = 0 ;						/*  */
				break;
			}
		}
	}


}

void p_mgc_disp( )
{
	Sint32 ii;
	
	for ( ii = 0; ii < 40; ii++ )
	{
		if ( pmshot[ii * 20 + 2] == 1 )
		{
			BltRect( 4, ( pmshot[ii * 20 + 5] >> 16 ) - d_x, 480 - ( pmshot[ii * 20 + 6] >> 16 ) - 32 - d_y, 0, 96, 32, 32 );

			pmshot[ii * 20 + 5] = pmshot[ii * 20 + 0] + pmshot[ii * 20 + 5];		/* �e��x */
			pmshot[ii * 20 + 6] = pmshot[ii * 20 + 1] + pmshot[ii * 20 + 6];		/* �e��x */
			
			if ( ( ( pmshot[ii * 20 + 5] >> 16 ) > 640 ) || ( ( pmshot[ii * 20 + 5] >> 16 ) < -32 ) )
			{
				pmshot[ii * 20 + 2] = 0;				/* ���ݖ��� */
			}
			if ( ( ( pmshot[ii * 20 + 6] >> 16 ) > 480 ) || ( ( pmshot[ii * 20 + 6] >> 16 ) < -32 ) )
			{
				pmshot[ii * 20 + 2] = 0;				/* ���ݖ��� */
			}
			
			if ( ( ( map1_item_j( ( pmshot[ii * 20 + 5] >> 16 ) + 16 , ( pmshot[ii * 20 + 6] >> 16 ) + 16 ) >= 100 )
			    && ( map1_item_j( ( pmshot[ii * 20 + 5] >> 16 ) + 16 , ( pmshot[ii * 20 + 6] >> 16 ) + 16 ) <= 220 ) ) )
			{
				pmshot[ii * 20 + 2] = 0;				/* ���ݖ��� */
				p_shot_f2_set( ( pmshot[5 + ii * 20] >> 16 ) + 15, ( pmshot[6 + ii * 20] >> 16 ) + 15, 0 );
				if ( pmshot[3 + ii * 20] == 2 )	/* �X�L���N���b�V���V���b�g */
				{
					p_shot_f_set( ( pmshot[5 + ii * 20] >> 16 ) + ( rand( ) % 32 ) - 16, ( pmshot[6 + ii * 20] >> 16 ) + ( rand( ) % 32 ) - 16, 0 );
					p_shot_f_set( ( pmshot[5 + ii * 20] >> 16 ) + ( rand( ) % 32 ) - 16, ( pmshot[6 + ii * 20] >> 16 ) + ( rand( ) % 32 ) - 16, 0 );
					p_shot_f_set( ( pmshot[5 + ii * 20] >> 16 ) + ( rand( ) % 32 ) - 16, ( pmshot[6 + ii * 20] >> 16 ) + ( rand( ) % 32 ) - 16, 0 );
				}
				else 
				{
					p_shot_f_set( ( pmshot[5 + ii * 20] >> 16 ) , ( pmshot[6 + ii * 20] >> 16 ), 0 );
				}
			}
			
			
		}
	}

}

void replay_input_save( void )
{
	Sint32 input_keys;
	input_keys = 0;
	
	/* �L�[�����͊�{�I��player[2]������Ԃ̎��ɃL�[����ꂽ��ǂ��Ȃ���L�q���Ă��� */
	if ( IsPressKey( gameflag[2] ) )
	{
		input_keys = input_keys + 1;
	}
	else if ( IsPressKey( gameflag[3] ) )
	{
		input_keys = input_keys + 10;
	}
	
	if ( IsPressKey( gameflag[0] ) )
	{
		input_keys = input_keys + 100;
	}
	else if ( IsPressKey( gameflag[1] ) )
	{
		input_keys = input_keys + 1000;
	}
	
	//	�X�y�[�X�L�[���������
	if ( IsPressKey( gameflag[4] ) )
	{
		input_keys = input_keys + 10000;
	}
	
	//B���������
	if ( IsPressKey( gameflag[5] ) )
	{
		input_keys = input_keys + 100000;
	}
	
	//B���������
	if ( IsPushKey( gameflag[5] ) )
	{
		input_keys = input_keys + 1000000;
	}
	/* 12�{�^���@���@�b�������ꂽ */
//	if ( ( ( IsPushJoyKey(6) != 0 ) || ( IsPushJoyKey(12) != 0 ) || IsPushKey( gameflag[6] ) ) )
//	{
//		input_keys = input_keys + 10000000;
//	}
	
	replay[replay_time] = input_keys;
}

void replay_input_load( void )
{
	Sint32 i;
	Sint32 input_keys;
	input_keys = 0;
	
	for ( i = 0; i < 10 ; i++ )
	{
		replay_load_key[i] = 0 ;	
	}
	
	input_keys = replay[replay_time];
	
//	if ( input_keys >= 10000000 )
//	{
//		input_keys = input_keys - 10000000;
//		replay_load_key[7] = 1 ;	
//	}
	if ( input_keys >= 1000000 )
	{
		input_keys = input_keys - 1000000;
		replay_load_key[6] = 1 ;	
	}
	if ( input_keys >= 100000 )
	{
		input_keys = input_keys - 100000;
		replay_load_key[5] = 1 ;	
	}
	if ( input_keys >= 10000 )
	{
		input_keys = input_keys - 10000;
		replay_load_key[4] = 1 ;	
	}
	if ( input_keys >= 1000 )
	{
		input_keys = input_keys - 1000;
		replay_load_key[3] = 1 ;	
	}
	if ( input_keys >= 100 )
	{
		input_keys = input_keys - 100;
		replay_load_key[2] = 1 ;	
	}
	if ( input_keys >= 10 )
	{
		input_keys = input_keys - 10;
		replay_load_key[1] = 1 ;	
	}
	if ( input_keys >= 1 )
	{
		// Never get used and it doesn't make sense anyway - Gameblabla
		//input_keys = input_keys - 1;
		replay_load_key[0] = 1 ;	
	}
}

void replay_file_save_w( void )
{
	sprintf(string,"replay/%d/replay_data_w_%d.dat", (int)stage, (int)save_data[3] );
	SaveFile( string, &replay[0], sizeof( replay ) );

}
void replay_file_save( Sint32 type )
{
	if ( type == 0 )
	{
		sprintf(string,"replay/%d/replay_data_%d.dat", (int)stage, (int)save_data[3] );
	}
	else
	{
		sprintf(string,"replay/%d/replay_data_j%d.dat", (int)stage, (int)save_data[3] );
	}
	SaveFile( string, &replay[0], sizeof( replay ) );
}

Sint32 replay_file_load( void )
{
	Sint32 rc ;
	rc = 1;
	
	if ( gameflag[125] == 0 )
	{
		sprintf( string, "replay/%d/replay_data_%d.dat", (int)stage, (int)save_data[3] );
	}
	else 
	{
		sprintf( string, "replay/%d/replay_data_j%d.dat", (int)stage, (int)save_data[3] );
	}
	if ( LoadFile( string, &replay[0], sizeof( replay ) ) )
	{
		rc = 0;
	}

	return( rc );
}

// Square root of integer
static const Sint32 ftbl[33]={0,1,1,2,2,4,5,8,11,16,22,32,45,64,90,128,181,256,362,512,724,1024,1448,2048,2896,4096,5792,8192,11585,16384,23170,32768,46340};
static const Sint32 ftbl2[32]={ 32768,33276,33776,34269,34755,35235,35708,36174,36635,37090,37540,37984,38423,38858,39287,39712,40132,40548,40960,41367,41771,42170,42566,42959,43347,43733,44115,44493,44869,45241,45611,45977};

static Sint32 int_sqrt ( Sint32 val )
{
    Sint32 cnt=0;
    Sint32 t=val;
    while (t) {cnt++;t>>=1;}
    if (6>=cnt) t=(val<<(6-cnt));
    else t=(val>>(cnt-6));
    return (ftbl[cnt]*ftbl2[t&31])>>15;
}


Sint32 GetKyori( Sint32 i1, Sint32 i2 )
{
	Sint32 rc = 0;
	
	//rc = ( Sint32 )sqrt( ( i1 * i1 ) + ( i2 * i2 ) );
	rc = ( Sint32 )int_sqrt( ( i1 * i1 ) + ( i2 * i2 ) );
	
	return ( rc );
}

void debugdisp( void )
{
#if DEBUG_DISP
	BltNumericImage( player[21], 8, 10, 10, 18, 0, 0, 10, 8 );	
	BltNumericImage( replay[replay_time], 8, 10, 20, 18, 0, 0, 10, 8 );	
	BltNumericImage( player[31], 8, 10, 30, 18, 0, 0, 10, 8 );	
	BltNumericImage( funcSin( player[20] ), 8, 10, 40, 18, 0, 0, 10, 8 );	
	BltNumericImage( funcCos( player[20] ), 8, 10, 50, 18, 0, 0, 10, 8 );	
	BltNumericImage( funcTan2( player[30],player[31] ), 8, 10, 60, 18, 0, 0, 10, 8 );	
	BltNumericImage( ( MOTatan(player[30] * 256,player[31] * 256) ), 8, 10, 70, 18, 0, 0, 10, 8 );	
#endif
}







