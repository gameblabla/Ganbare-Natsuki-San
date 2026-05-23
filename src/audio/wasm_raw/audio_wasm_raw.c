#include "../../define.h"
#include "../audio.h"
#include <stddef.h>
#include <stdint.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

__attribute__((import_module("env"), import_name("gns_audio_init")))
int gns_audio_init(void);

__attribute__((import_module("env"), import_name("gns_audio_shutdown")))
void gns_audio_shutdown(void);

__attribute__((import_module("env"), import_name("gns_audio_load_bgm")))
int gns_audio_load_bgm(int num, const char *path, int loop);

__attribute__((import_module("env"), import_name("gns_audio_load_se")))
int gns_audio_load_se(int num, const char *path);

__attribute__((import_module("env"), import_name("gns_audio_play_bgm")))
int gns_audio_play_bgm(int num);

__attribute__((import_module("env"), import_name("gns_audio_stop_bgm")))
void gns_audio_stop_bgm(void);

__attribute__((import_module("env"), import_name("gns_audio_is_bgm_playing")))
int gns_audio_is_bgm_playing(void);

__attribute__((import_module("env"), import_name("gns_audio_set_bgm_gain")))
void gns_audio_set_bgm_gain(int vol, int master);

__attribute__((import_module("env"), import_name("gns_audio_set_se_gain")))
void gns_audio_set_se_gain(int vol, int master);

__attribute__((import_module("env"), import_name("gns_audio_play_se")))
int gns_audio_play_se(int num);

__attribute__((import_module("env"), import_name("gns_audio_stop_se")))
void gns_audio_stop_se(int num);

__attribute__((import_module("env"), import_name("gns_audio_stop_se_all")))
void gns_audio_stop_se_all(void);

__attribute__((import_module("env"), import_name("gns_audio_is_se_playing")))
int gns_audio_is_se_playing(int num);

__attribute__((import_module("env"), import_name("gns_audio_pause")))
void gns_audio_pause(void);

__attribute__((import_module("env"), import_name("gns_audio_resume")))
void gns_audio_resume(void);

typedef struct WasmAudioBackend {
    int sound_use;
    int music_loaded[SOUND_MUSBANK];
    int music_loop[SOUND_MUSBANK];
    int music_play_num;
    int play_bgmvol;
    int play_sevol;
    int master_vol;
    int fade_ctrl;
    int fade_vol;
    int fade_time;
    int fade_rate;
} WasmAudioBackend;

static WasmAudioBackend g_audio;
Sint32 sound_buff[SOUND_MUSBANK + SOUND_CHUNKBANK];

static int clamp_vol(int vol)
{
    if (vol < 0) return 0;
    if (vol > VOL_MAX) return VOL_MAX;
    return vol;
}

void soundInitBuffer(void)
{
    int i;
    g_audio.sound_use = FALSE;
    g_audio.music_play_num = -1;
    g_audio.play_bgmvol = VOL_MAX;
    g_audio.play_sevol = VOL_MAX;
    g_audio.master_vol = 100;
    g_audio.fade_ctrl = 0;
    g_audio.fade_vol = 0;
    g_audio.fade_time = 0;
    g_audio.fade_rate = 0;
    for (i = 0; i < SOUND_MUSBANK; i++) {
        g_audio.music_loaded[i] = FALSE;
        g_audio.music_loop[i] = 0;
    }
    g_audio.sound_use = gns_audio_init() == 0 ? TRUE : FALSE;
    gns_audio_set_bgm_gain(g_audio.play_bgmvol, g_audio.master_vol);
    gns_audio_set_se_gain(g_audio.play_sevol, g_audio.master_vol);
}

void soundRelease(void)
{
    gns_audio_shutdown();
    g_audio.sound_use = FALSE;
}

void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop)
{
    if (num < 0 || num >= SOUND_MUSBANK || !fname) return;
    if (gns_audio_load_bgm((int)num, (const char *)fname, loop) == 0) {
        g_audio.music_loaded[num] = TRUE;
        g_audio.music_loop[num] = loop;
    }
}

void soundLoadBuffer2(Sint32 num, Uint8 *fname1, Uint8 *fname2)
{
    /* The browser backend keeps the same public API but treats intro+loop BGM
       as the loop file. The stock data path only uses soundLoadBuffer(). */
    if (fname2) soundLoadBuffer(num, fname2, -1);
    else soundLoadBuffer(num, fname1, -1);
}

void soundLoadBufferSE(Sint32 num, Uint8 *fname)
{
    if (num < 0 || num >= SOUND_CHUNKBANK || !fname) return;
    (void)gns_audio_load_se((int)num, (const char *)fname);
}

void soundStopBgm(Sint32 num)
{
    (void)num;
    g_audio.fade_ctrl = 0;
    g_audio.fade_vol = 0;
    g_audio.music_play_num = -1;
    gns_audio_stop_bgm();
}

void soundStopBgmPlaying(void)
{
    if (g_audio.music_play_num != -1) soundStopBgm(g_audio.music_play_num);
}

int soundIsPlayBgm(void)
{
    return gns_audio_is_bgm_playing();
}

void soundPlayBgm(Sint32 num)
{
    if (num < 0 || num >= SOUND_MUSBANK) return;
    if (!g_audio.music_loaded[num]) return;
    g_audio.fade_ctrl = 0;
    g_audio.music_play_num = num;
    gns_audio_set_bgm_gain(g_audio.play_bgmvol, g_audio.master_vol);
    (void)gns_audio_play_bgm((int)num);
}

void soundPlayFadeFlag(Sint32 flag, Sint32 time)
{
    g_audio.fade_ctrl = flag;
    g_audio.fade_time = time;
}

void soundPlayCtrl(void)
{
    if (!g_audio.fade_ctrl) return;

    if (g_audio.fade_ctrl == 1) {
        g_audio.fade_ctrl = 2;
        g_audio.fade_vol = g_audio.play_bgmvol;
        g_audio.fade_rate = g_audio.fade_time > 0 ? g_audio.fade_vol / g_audio.fade_time : g_audio.fade_vol;
        if (g_audio.fade_rate <= 0) g_audio.fade_rate = 1;
    }

    if (g_audio.fade_ctrl == 2) {
        g_audio.fade_time--;
        if (g_audio.fade_time > 0 && g_audio.fade_vol > 0) {
            g_audio.fade_vol -= g_audio.fade_rate;
            if (g_audio.fade_vol < 0) g_audio.fade_vol = 0;
            gns_audio_set_bgm_gain(g_audio.fade_vol, g_audio.master_vol);
        } else {
            g_audio.fade_ctrl = 0;
            g_audio.fade_vol = 0;
            g_audio.music_play_num = -1;
            gns_audio_set_bgm_gain(0, g_audio.master_vol);
            gns_audio_stop_bgm();
        }
    }
}

void soundSetVolumeMaster(Sint32 vol)
{
    if (vol < 0) vol = 0;
    g_audio.master_vol = vol;
    gns_audio_set_bgm_gain(g_audio.play_bgmvol, g_audio.master_vol);
    gns_audio_set_se_gain(g_audio.play_sevol, g_audio.master_vol);
}

void soundSetVolumeBgm(Sint32 vol, Sint32 num)
{
    (void)num;
    g_audio.play_bgmvol = clamp_vol(vol);
    gns_audio_set_bgm_gain(g_audio.play_bgmvol, g_audio.master_vol);
}

void soundSetVolumeAll(Sint32 vol)
{
    g_audio.play_bgmvol = clamp_vol(vol);
    g_audio.play_sevol = clamp_vol(vol);
    gns_audio_set_bgm_gain(g_audio.play_bgmvol, g_audio.master_vol);
    gns_audio_set_se_gain(g_audio.play_sevol, g_audio.master_vol);
}

void soundStopSe(Sint32 num)
{
    if (num < 0 || num >= SOUND_CHUNKBANK) return;
    gns_audio_stop_se((int)num);
}

void soundPlaySe(Sint32 num)
{
    if (num < 0 || num >= SOUND_CHUNKBANK) return;
    (void)gns_audio_play_se((int)num);
}

int soundIsPlaySe(Sint32 num)
{
    if (num < 0 || num >= SOUND_CHUNKBANK) return 0;
    return gns_audio_is_se_playing((int)num);
}

void soundStopSeAll(void)
{
    gns_audio_stop_se_all();
}

void Pause_Music(void)
{
    gns_audio_pause();
}

void Resume_Music(void)
{
    gns_audio_resume();
}

void Stop_Music(void)
{
    gns_audio_stop_bgm();
}
