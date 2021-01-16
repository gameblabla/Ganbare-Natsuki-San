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
extern void soundLoadBuffer(int32_t num, uint8_t *fname, int loop);
extern void soundLoadBuffer2(int32_t num, uint8_t *fname1, uint8_t *fname2);
extern void soundLoadBufferSE(int32_t num, uint8_t *fname);
extern void soundStopBgm(int32_t num);
extern void soundStopBgmPlaying(void);
extern int soundIsPlayBgm(void);
extern void soundPlayBgm(int32_t num);
extern void soundPlayFadeFlag(int32_t flag, int32_t time);
extern void soundPlayCtrl(void);
extern void soundSetVolumeMaster(int32_t vol);
extern void soundSetVolumeBgm(int32_t vol, int32_t num);
extern void soundSetVolumeAll(int32_t vol);
extern void soundStopSe(int32_t num);
extern void soundPlaySe(int32_t num);
extern int soundIsPlaySe(int32_t num);
extern void soundStopSeAll(void);

extern int32_t sound_buff[];

#endif /* _UTIL_SND_ */
