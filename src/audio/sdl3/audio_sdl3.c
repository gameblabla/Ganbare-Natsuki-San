#include "../../define.h"
#include "../audio.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef struct SDL3AudioBackend {
	MIX_Mixer *mixer;
	MIX_Track *bgm_track;
	MIX_Track *se_track[SOUND_MIXBANK];
	MIX_Audio *music[SOUND_MUSBANK];
	MIX_Audio *music_loop[SOUND_MUSBANK];
	MIX_Audio *chunk[SOUND_CHUNKBANK];
	int chunkChannel[SOUND_MIXBANK];
	int musicIntro[SOUND_MUSBANK];
	int musicLoop[SOUND_MUSBANK];
	int musicPlayNum;
	int musicIsLoop;
	int play_bgmvol;
	int play_sevol;
	int fade_ctrl;
	int fade_vol;
	int fade_time;
	int fade_rate;
	int master_vol;
	int sound_use;
} SDL3AudioBackend;

static SDL3AudioBackend s_audio;
Sint32 sound_buff[SOUND_MUSBANK + SOUND_CHUNKBANK];

static float gain_from_volume(int vol, int master)
{
	float gain;
	if (vol < 0) vol = 0;
	if (vol > VOL_MAX) vol = VOL_MAX;
	if (master < 0) master = 0;
	gain = ((float)vol / (float)VOL_MAX) * ((float)master / 100.0f);
	if (gain < 0.0f) gain = 0.0f;
	return gain;
}

static SDL_PropertiesID make_play_options(int loops)
{
	SDL_PropertiesID props;

	if (loops == 0) {
		return 0;
	}

	props = SDL_CreateProperties();
	if (props) {
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
	}
	return props;
}

static void play_track(MIX_Track *track, MIX_Audio *audio, int loops)
{
	SDL_PropertiesID props;

	if (!track || !audio) {
		return;
	}

	MIX_StopTrack(track, 0);
	MIX_SetTrackAudio(track, audio);
	props = make_play_options(loops);
	MIX_PlayTrack(track, props);
	if (props) {
		SDL_DestroyProperties(props);
	}
}

void soundInitBuffer(void)
{
	int i;
	memset(&s_audio, 0, sizeof(s_audio));

	s_audio.master_vol = 100;
	s_audio.play_bgmvol = VOL_MAX;
	s_audio.play_sevol = VOL_MAX;
	s_audio.musicPlayNum = -1;
	s_audio.sound_use = FALSE;

	for (i = 0; i < SOUND_MIXBANK; i++) {
		s_audio.chunkChannel[i] = -1;
	}

	if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
		fprintf(stderr, "SDL3 audio init failed: %s\n", SDL_GetError());
		return;
	}

	s_audio.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!s_audio.mixer) {
		fprintf(stderr, "SDL3_mixer init failed: %s\n", SDL_GetError());
		return;
	}

	s_audio.bgm_track = MIX_CreateTrack(s_audio.mixer);
	if (!s_audio.bgm_track) {
		fprintf(stderr, "SDL3_mixer BGM track creation failed: %s\n", SDL_GetError());
		return;
	}

	for (i = 0; i < SOUND_MIXBANK; i++) {
		s_audio.se_track[i] = MIX_CreateTrack(s_audio.mixer);
		if (!s_audio.se_track[i]) {
			fprintf(stderr, "SDL3_mixer SE track creation failed: %s\n", SDL_GetError());
		}
	}

	s_audio.sound_use = TRUE;
	soundSetVolumeAll(s_audio.play_bgmvol);
}

void soundRelease(void)
{
	int i;

	if (s_audio.bgm_track) {
		MIX_DestroyTrack(s_audio.bgm_track);
		s_audio.bgm_track = NULL;
	}

	for (i = 0; i < SOUND_MIXBANK; i++) {
		if (s_audio.se_track[i]) {
			MIX_DestroyTrack(s_audio.se_track[i]);
			s_audio.se_track[i] = NULL;
		}
	}

	for (i = 0; i < SOUND_MUSBANK; i++) {
		if (s_audio.music[i]) MIX_DestroyAudio(s_audio.music[i]);
		if (s_audio.music_loop[i]) MIX_DestroyAudio(s_audio.music_loop[i]);
		s_audio.music[i] = NULL;
		s_audio.music_loop[i] = NULL;
	}

	for (i = 0; i < SOUND_CHUNKBANK; i++) {
		if (s_audio.chunk[i]) MIX_DestroyAudio(s_audio.chunk[i]);
		s_audio.chunk[i] = NULL;
	}

	if (s_audio.mixer) {
		MIX_DestroyMixer(s_audio.mixer);
		s_audio.mixer = NULL;
	}

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	s_audio.sound_use = FALSE;
}

void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop)
{
	if (!s_audio.mixer || num < 0 || num >= SOUND_MUSBANK || s_audio.music[num]) {
		return;
	}

	s_audio.music[num] = MIX_LoadAudio(s_audio.mixer, (char*)fname, false);
	if (!s_audio.music[num]) {
		fprintf(stderr, "Can't load music %s: %s\n", fname, SDL_GetError());
		return;
	}
	s_audio.musicIntro[num] = 0;
	s_audio.musicLoop[num] = loop;
}

void soundLoadBuffer2(Sint32 num, Uint8 *fname1, Uint8 *fname2)
{
	if (!s_audio.mixer || num < 0 || num >= SOUND_MUSBANK) {
		return;
	}
	if (!s_audio.music[num]) {
		s_audio.music[num] = MIX_LoadAudio(s_audio.mixer, (char*)fname1, false);
		s_audio.musicIntro[num] = 1;
	}
	if (!s_audio.music_loop[num]) {
		s_audio.music_loop[num] = MIX_LoadAudio(s_audio.mixer, (char*)fname2, false);
	}
}

void soundLoadBufferSE(Sint32 num, Uint8 *fname)
{
	if (!s_audio.mixer || num < 0 || num >= SOUND_CHUNKBANK) {
		return;
	}

	if (s_audio.chunk[num]) {
		MIX_DestroyAudio(s_audio.chunk[num]);
	}
	s_audio.chunk[num] = MIX_LoadAudio(s_audio.mixer, (char*)fname, true);
	if (!s_audio.chunk[num]) {
		fprintf(stderr, "Can't load SE %s: %s\n", fname, SDL_GetError());
	}
}

void soundStopBgm(Sint32 num)
{
	(void)num;
	if (!s_audio.sound_use || !s_audio.bgm_track) {
		return;
	}
	s_audio.fade_vol = 0;
	s_audio.fade_ctrl = 0;
	s_audio.musicPlayNum = -1;
	s_audio.musicIsLoop = 0;
	MIX_StopTrack(s_audio.bgm_track, 0);
}

void soundStopBgmPlaying(void)
{
	if (s_audio.musicPlayNum != -1) {
		soundStopBgm(s_audio.musicPlayNum);
	}
}

int soundIsPlayBgm(void)
{
	if (!s_audio.sound_use || !s_audio.bgm_track) {
		return 0;
	}
	return MIX_TrackPlaying(s_audio.bgm_track) ? 1 : 0;
}

void soundPlayBgm(Sint32 num)
{
	if (!s_audio.sound_use || !s_audio.bgm_track || num < 0 || num >= SOUND_MUSBANK || !s_audio.music[num]) {
		return;
	}

	s_audio.fade_ctrl = 0;
	s_audio.musicPlayNum = num;
	MIX_SetTrackGain(s_audio.bgm_track, gain_from_volume(s_audio.play_bgmvol, s_audio.master_vol));
	if (s_audio.musicIntro[num] == 0) {
		play_track(s_audio.bgm_track, s_audio.music[num], s_audio.musicLoop[num]);
		s_audio.musicIsLoop = 0;
	} else {
		play_track(s_audio.bgm_track, s_audio.music[num], 0);
		s_audio.musicIsLoop = 2;
	}
}

void soundPlayBgm2(Sint32 num)
{
	if (!s_audio.sound_use || !s_audio.bgm_track || num < 0 || num >= SOUND_MUSBANK || !s_audio.music_loop[num]) {
		return;
	}
	play_track(s_audio.bgm_track, s_audio.music_loop[num], -1);
}

void soundPlayFadeFlag(Sint32 flag, Sint32 time)
{
	s_audio.fade_ctrl = flag;
	s_audio.fade_time = time;
}

void soundPlayCtrl(void)
{
	int i;

	if (!s_audio.sound_use) {
		return;
	}

	for (i = 0; i < SOUND_MIXBANK; i++) {
		if (s_audio.chunkChannel[i] != -1 && s_audio.se_track[i] && !MIX_TrackPlaying(s_audio.se_track[i])) {
			s_audio.chunkChannel[i] = -1;
		}
	}

	if (s_audio.musicPlayNum == -1 || !s_audio.bgm_track) {
		return;
	}

	if (s_audio.musicIsLoop) {
		if (s_audio.musicIsLoop == 2) {
			if (MIX_TrackPlaying(s_audio.bgm_track)) s_audio.musicIsLoop--;
		} else {
			if (!MIX_TrackPlaying(s_audio.bgm_track)) {
				s_audio.musicIsLoop = 0;
				soundPlayBgm2(s_audio.musicPlayNum);
			}
		}
	}

	if (!s_audio.fade_ctrl) {
		return;
	}

	if (s_audio.fade_ctrl == 1) {
		s_audio.fade_ctrl = 2;
		s_audio.fade_vol = s_audio.play_bgmvol;
		s_audio.fade_rate = s_audio.fade_time > 0 ? s_audio.fade_vol / s_audio.fade_time : s_audio.fade_vol;
	}

	if (s_audio.fade_ctrl == 2) {
		s_audio.fade_time--;
		if (s_audio.fade_time > 0) {
			s_audio.fade_vol -= s_audio.fade_rate;
			MIX_SetTrackGain(s_audio.bgm_track, gain_from_volume(s_audio.fade_vol, s_audio.master_vol));
		} else {
			s_audio.fade_vol = 0;
			s_audio.fade_ctrl = 0;
			s_audio.musicPlayNum = -1;
			s_audio.musicIsLoop = 0;
			MIX_SetTrackGain(s_audio.bgm_track, 0.0f);
			MIX_StopTrack(s_audio.bgm_track, 0);
		}
	}
}

void soundSetVolumeMaster(Sint32 vol)
{
	s_audio.master_vol = vol;
}

void soundSetVolumeBgm(Sint32 vol, Sint32 num)
{
	(void)num;
	s_audio.play_bgmvol = vol;
	if (s_audio.bgm_track) {
		MIX_SetTrackGain(s_audio.bgm_track, gain_from_volume(s_audio.play_bgmvol, s_audio.master_vol));
	}
}

void soundSetVolumeAll(Sint32 vol)
{
	int i;
	s_audio.play_sevol = vol;
	if (s_audio.bgm_track) {
		MIX_SetTrackGain(s_audio.bgm_track, gain_from_volume(s_audio.play_bgmvol, s_audio.master_vol));
	}
	for (i = 0; i < SOUND_MIXBANK; i++) {
		if (s_audio.se_track[i]) {
			MIX_SetTrackGain(s_audio.se_track[i], gain_from_volume(s_audio.play_sevol, s_audio.master_vol));
		}
	}
}

void soundStopSe(Sint32 num)
{
	int i;
	for (i = 0; i < SOUND_MIXBANK; i++) {
		if (s_audio.chunkChannel[i] == num && s_audio.se_track[i]) {
			MIX_StopTrack(s_audio.se_track[i], 0);
			s_audio.chunkChannel[i] = -1;
		}
	}
}

void soundPlaySe(Sint32 num)
{
	int i;
	if (!s_audio.sound_use || num < 0 || num >= SOUND_CHUNKBANK || !s_audio.chunk[num]) {
		return;
	}

	soundStopSe(num);
	for (i = 0; i < SOUND_MIXBANK; i++) {
		if (s_audio.chunkChannel[i] == -1 && s_audio.se_track[i]) {
			MIX_SetTrackGain(s_audio.se_track[i], gain_from_volume(s_audio.play_sevol, s_audio.master_vol));
			play_track(s_audio.se_track[i], s_audio.chunk[num], 0);
			s_audio.chunkChannel[i] = num;
			return;
		}
	}
}

int soundIsPlaySe(Sint32 num)
{
	int i;
	for (i = 0; i < SOUND_MIXBANK; i++) {
		if (s_audio.chunkChannel[i] == num && s_audio.se_track[i] && MIX_TrackPlaying(s_audio.se_track[i])) {
			return 1;
		}
	}
	return 0;
}

void soundStopSeAll(void)
{
	int i;
	for (i = 0; i < SOUND_MIXBANK; i++) {
		s_audio.chunkChannel[i] = -1;
		if (s_audio.se_track[i]) {
			MIX_StopTrack(s_audio.se_track[i], 0);
		}
	}
}

void Pause_Music(void)
{
	if (s_audio.bgm_track) {
		MIX_PauseTrack(s_audio.bgm_track);
	}
}

void Resume_Music(void)
{
	if (s_audio.bgm_track) {
		MIX_ResumeTrack(s_audio.bgm_track);
	}
}

void Stop_Music(void)
{
	if (s_audio.bgm_track) {
		MIX_StopTrack(s_audio.bgm_track, 0);
	}
}
