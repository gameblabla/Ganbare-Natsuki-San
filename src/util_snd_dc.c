#ifdef DREAMCAST
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
#ifdef OPUS_MUSIC
#include <opusplay/opusplay.h>
#endif
#ifdef ADX_PLAY
#include <libadx/libadx.h> /* ADX Decoder Library */
#include <libadx/snddrv.h> /* Direct Access to Sound Driver */
#endif

#include "general.h"
#include "util_snd.h"
#include "define.h"
#include "paths.h"

enum{
	SND_RATE = 44100, 
	SND_CHANNEL = 2, 
	SND_BUFFER = 1024
};

void soundInitBuffer(void);
void soundRelease(void);
void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop);
void soundLoadBuffer2(Sint32 num, Uint8 *fname1, Uint8 *fname2);
void soundLoadBufferSE(Sint32 num, Uint8 *fname);
void soundStopBgm(Sint32 num);
void soundStopBgmPlaying(void);
int soundIsPlayBgm(void);
void soundPlayBgm(Sint32 num);
void soundPlayFadeFlag(Sint32 flag, Sint32 time);
void soundPlayCtrl(void);
void soundSetVolumeMaster(Sint32 vol);
void soundSetVolumeBgm(Sint32 vol, Sint32 num);
void soundSetVolumeAll(Sint32 vol);
void soundStopSe(Sint32 num);
void soundPlaySe(Sint32 num);
int soundIsPlaySe(Sint32 num);
void soundStopSeAll(void);


static int play_bgmvol;
static int fade_ctrl;
static int fade_vol;
static int fade_time;
static int fade_rate;
static int master_vol;

static unsigned char sound_use = FALSE;
static sfxhnd_t sfx_dc[10];

int sfx_vol = 255;

/*
static int chunkChannel[SOUND_MIXBANK];
static Mix_Music *music[SOUND_MUSBANK];
static Mix_Music *music_loop[SOUND_MUSBANK];
static int musicIntor[SOUND_MUSBANK];
static int musicLoop[SOUND_MUSBANK];
*/
static int musicPlayNum;
static int musicIsLoop;

const char sounds_dc[EN_SE_PAWA+1][32] =
{
	SE_SOUND_PATH"atk1.wav",
	SE_SOUND_PATH"damage.wav",
	SE_SOUND_PATH"l1.wav",
	SE_SOUND_PATH"landing.wav",
	SE_SOUND_PATH"msg.wav",
	SE_SOUND_PATH"select.wav",
	SE_SOUND_PATH"sw.wav",
	SE_SOUND_PATH"jump.wav",
	SE_SOUND_PATH"puwa.wav",
};

void soundInitBuffer(void)
{
	int i;
	
	snd_stream_init();
	for(i=0;i<EN_SE_PAWA+1;i++)
	{
		if (!sfx_dc[i]) sfx_dc[i] = snd_sfx_load(sounds_dc[i]);	
	}
	#ifdef OPUS_MUSIC
	opusplay_init();
	opusplay_set_volume(255);
    #elif defined(ADX_PLAY)
    adx_stop();
	#else
	cdrom_init();
	#endif
	//snd_stream_init();
	/*
#ifndef NOSOUND
	int i;
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;
    int audio_buffers;

	master_vol = 100;
	play_bgmvol = MIX_MAX_VOLUME;
	play_sevol = MIX_MAX_VOLUME;
	soundSetVolumeAll(play_bgmvol);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0){
		return;
    }

	audio_rate = SND_RATE;
	audio_format = AUDIO_S16;
	audio_channels = SND_CHANNEL;
	audio_buffers = SND_BUFFER;
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0){
		printf("Mix_Init: Failed to init required ogg and mod support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
		sound_use = FALSE;
	}else{
		sound_use = TRUE;
	}
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);

	fade_vol = 0;
	fade_ctrl = 0;
	musicPlayNum = -1;
	musicIsLoop = 0;

	for(i = 0; i < SOUND_MUSBANK; i++){
		music[i] = NULL;
		music_loop[i] = NULL;
		musicIntor[i] = 0;
		musicLoop[i] = 0;
	}

	for(i = 0; i < SOUND_CHUNKBANK; i++){
		chunk[i] = NULL;
	}

	for(i = 0; i < SOUND_MIXBANK; i++){
		chunkChannel[i] = -1;
	}
#endif
*/
}

void soundRelease(void)
{
	#ifdef OPUS_MUSIC
    if (opusplay_is_playing()){
        opusplay_stop();
    }
    opusplay_shutdown();
    #elif defined(ADX_PLAY)
    adx_stop();
	#endif
	snd_stream_shutdown();
}

void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop)
{
}

void soundLoadBuffer2(Sint32 num, Uint8 *fname1, Uint8 *fname2)
{
}

void soundLoadBufferSE(Sint32 num, Uint8 *fname)
{
	/*printf("Loading SFX %s\n", fname);
	sfx_dc[num] = snd_sfx_load(fname);*/	
}

void soundStopBgm(Sint32 num)
{
	#ifdef OPUS_MUSIC
    if (opusplay_is_playing()){
        opusplay_stop();
    }
    #elif defined(ADX_PLAY)
    adx_stop();
	#else
	cdrom_cdda_pause();
	#endif
}

void soundStopBgmPlaying(void)
{
	#ifdef OPUS_MUSIC
    if (opusplay_is_playing()){
        opusplay_stop();
    }
    #elif defined(ADX_PLAY)
    adx_stop();
	#else
	cdrom_cdda_pause();
	#endif
}

int soundIsPlayBgm(void)
{
	return 1;
}

void soundPlayBgm(Sint32 num)
{
	int track = num + 1;
	printf("BGM1 : Play track %d\n", num);
	if (num < 0 || num > 7) return;
	#ifdef OPUS_MUSIC
	char tm[64];
	snprintf(tm, sizeof(tm), BGM_SOUND_PATH"0%d.opus", track);
    if (opusplay_is_playing()){
        opusplay_stop();
    }
	opusplay_play_file(tm, 0);
	#elif defined(ADX_PLAY)
	char tm[64];
	snprintf(tm, sizeof(tm), BGM_SOUND_PATH"0%d.adx", track);
	adx_stop();
	adx_dec( tm, 1 );
	#else
	cdrom_cdda_play(track, track+1, 0xF, CDDA_TRACKS);
	#endif
}

void soundPlayBgm2(Sint32 num)
{
	int track = num + 1;
	printf("BGM2 : Play track %d\n", num);
	if (num < 0 || num > 7) return;
	#ifdef OPUS_MUSIC
	char tm[64];
	snprintf(tm, sizeof(tm), BGM_SOUND_PATH"0%d.opus", track);
    if (opusplay_is_playing()){
        opusplay_stop();
    }
	opusplay_play_file(tm, 0);
	#elif defined(ADX_PLAY)
	char tm[64];
	snprintf(tm, sizeof(tm), BGM_SOUND_PATH"0%d.adx", track);
	adx_stop();
	adx_dec( tm, 1 );
	#else
	cdrom_cdda_play(track, track+1, 0xF, CDDA_TRACKS);
	#endif
}

/* We have to do this crap otherwise it will glitch up - Gameblabla */

void Pause_Music()
{
	#if defined(ADX_PLAY)
	adx_pause();
	#else
	cdrom_cdda_pause();
	#endif
}

void Resume_Music()
{
	#if defined(ADX_PLAY)
	adx_resume();
	#else
	cdrom_cdda_resume();
	#endif
}

void Stop_Music()
{
	#if defined(ADX_PLAY)
	adx_stop();
	#else
	cdrom_spin_down ()
	#endif
}

void soundLoadPlayBgm(Uint8 *fname, Sint32 loop)
{
//	soundStopBgmPlaying();
//	if(music[BGM_TEST]){
//		Mix_FreeMusic(music[BGM_TEST]);
//		music[BGM_TEST] = NULL;
//	}
//	soundLoadBuffer(BGM_TEST, fname, -1);
//	soundPlayBgm(BGM_TEST);
}

void soundPlayFadeFlag(Sint32 flag, Sint32 time)
{
#ifndef NOSOUND
	fade_ctrl = flag;
	fade_time = time;
#endif
}

void soundPlayCtrl(void)
{
/*
#ifndef NOSOUND
	int	i;

	for(i = 0; i < SOUND_MIXBANK; i++){
		if(chunkChannel[i] != -1){
		    if(Mix_Playing(i) == 0) chunkChannel[i] = -1;
		}
	}

	if(musicPlayNum == -1){
		return;
	}

	if(musicIsLoop){
		if(musicIsLoop == 2){
			if(Mix_PlayingMusic() == SDL_TRUE) musicIsLoop--;
		}else{
			if(Mix_PlayingMusic() == SDL_FALSE){
				musicIsLoop = 0;
				soundPlayBgm2(musicPlayNum);
			}
		}
	}

	if(!fade_ctrl){
		return;
	}

	if(fade_ctrl == 1){
		fade_ctrl = 2;
		fade_vol = play_bgmvol;
		fade_rate = fade_vol / fade_time;
	}

	if(fade_ctrl == 2){
		fade_time--;
		if(fade_time){
			fade_vol -= fade_rate;
			Mix_VolumeMusic(fade_vol * master_vol / 100);
		}else{
			fade_vol = 0;
			fade_ctrl = 0;
			musicPlayNum = -1;
			musicIsLoop = 0;
			Mix_VolumeMusic(fade_vol);
		}
	}
#endif
*/
}

void soundSetVolumeMaster(Sint32 vol)
{
	master_vol = vol;
}

void soundSetVolumeBgm(Sint32 vol, Sint32 num)
{
	play_bgmvol = vol;
}

void soundSetVolumeAll(Sint32 vol)
{
	vol = (vol * 2) - 1;
	if (vol > 255) vol = 255;
	else if (vol < 0) vol = 0;
	sfx_vol = vol;
}

void soundStopSe(Sint32 num)
{
	if (num < 0 || num > EN_SE_PAWA) return;
	
	#if !defined(ADX_PLAY) && !defined(OPUS_MUSIC)
	snd_sfx_stop(num);
	#endif
}

/* Per channel muting and play but only for CDDA version */
void soundPlaySe(Sint32 num)
{	
	if (num < 0 || num > EN_SE_PAWA) return;

	snd_sfx_play_chn(num
	#if defined(ADX_PLAY) || defined(OPUS_MUSIC)
	+4
	#endif
	, sfx_dc[num], sfx_vol, 0x80);
}

int soundIsPlaySe(Sint32 num)
{
	return 0;
}

void soundStopSeAll(void)
{
	snd_sfx_stop_all();
}

#endif
