#ifndef _UTIL_SND_
#define _UTIL_SND_

#include <stdint.h>

enum{
	SOUND_MUSBANK = 32,
	SOUND_CHUNKBANK = 32,
	SOUND_MIXBANK = 8
};

enum {
	BGM_GAME01 = 0,

	SE_SEL = 0,
	SE_COLLECT,
	SE_SHOT,
	SE_SHIPBRK,
	SE_CHIP,
	SE_DMG,
	SE_ENEEXP,
	SE_ENEBRK,
	SE_EXTEND,
	SE_ALERT,
};

extern void soundInitBuffer(void);
extern void soundRelease(void);
extern void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop);
extern void soundLoadBuffer2(Sint32 num, Uint8 *fname1, Uint8 *fname2);
extern void soundLoadBufferSE(Sint32 num, Uint8 *fname);
extern void soundStopBgm(Sint32 num);
extern void soundStopBgmPlaying(void);
extern int soundIsPlayBgm(void);
extern void soundPlayBgm(Sint32 num);
extern void soundPlayFadeFlag(Sint32 flag, Sint32 time);
extern void soundPlayCtrl(void);
extern void soundSetVolumeMaster(Sint32 vol);
extern void soundSetVolumeBgm(Sint32 vol, Sint32 num);
extern void soundSetVolumeAll(Sint32 vol);
extern void soundStopSe(Sint32 num);
extern void soundPlaySe(Sint32 num);
extern int soundIsPlaySe(Sint32 num);
extern void soundStopSeAll(void);

extern Sint32 sound_buff[];

#endif /* _UTIL_SND_ */
