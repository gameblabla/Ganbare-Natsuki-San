#include "../../define.h"
#include "../audio.h"
#include <stdint.h>

Sint32 sound_buff[SOUND_MUSBANK + SOUND_CHUNKBANK];

void soundInitBuffer(void)
{
}

void soundRelease(void)
{
}

void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop)
{
	(void)num;
	(void)fname;
	(void)loop;
}

void soundLoadBuffer2(Sint32 num, Uint8 *fname1, Uint8 *fname2)
{
	(void)num;
	(void)fname1;
	(void)fname2;
}

void soundLoadBufferSE(Sint32 num, Uint8 *fname)
{
	(void)num;
	(void)fname;
}

void soundStopBgm(Sint32 num)
{
	(void)num;
}

void soundStopBgmPlaying(void)
{
}

int soundIsPlayBgm(void)
{
	return 0;
}

void soundPlayBgm(Sint32 num)
{
	(void)num;
}

void soundPlayFadeFlag(Sint32 flag, Sint32 time)
{
	(void)flag;
	(void)time;
}

void soundPlayCtrl(void)
{
}

void soundSetVolumeMaster(Sint32 vol)
{
	(void)vol;
}

void soundSetVolumeBgm(Sint32 vol, Sint32 num)
{
	(void)vol;
	(void)num;
}

void soundSetVolumeAll(Sint32 vol)
{
	(void)vol;
}

void soundStopSe(Sint32 num)
{
	(void)num;
}

void soundPlaySe(Sint32 num)
{
	(void)num;
}

int soundIsPlaySe(Sint32 num)
{
	(void)num;
	return 0;
}

void soundStopSeAll(void)
{
}

void Pause_Music(void)
{
}

void Resume_Music(void)
{
}

void Stop_Music(void)
{
}
