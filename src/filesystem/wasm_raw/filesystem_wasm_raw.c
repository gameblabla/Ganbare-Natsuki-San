#include "../../define.h"
#include "../../extern.h"
#include "../../filesystem/filesystem.h"
#include "../../wasm/achievements.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__attribute__((import_module("env"), import_name("gns_local_storage_load")))
int gns_local_storage_load(const char *key, unsigned char *dst, int size);

__attribute__((import_module("env"), import_name("gns_local_storage_save")))
int gns_local_storage_save(const char *key, const unsigned char *src, int size);

__attribute__((import_module("env"), import_name("gns_asset_size")))
int gns_asset_size(const char *path);

__attribute__((import_module("env"), import_name("gns_asset_read")))
int gns_asset_read(const char *path, unsigned char *dst, int capacity);


static int gns_debug_cheat_active = 0;
static int gns_debug_work_valid = 0;
static int gns_debug_item_valid = 0;
static unsigned char gns_debug_work_save[GAMEFLAG_SIZE * sizeof(Sint32)];
static unsigned char gns_debug_item_save[GAMEFLAG_SIZE * sizeof(Sint32)];

/* The title-screen Tutorial entry uses the stage-0 data set.  Keep that
   work save transient in WASM so it cannot overwrite the browser's normal
   continue slot or make the Tutorial behave like a page reload/reset. */
static int gns_tutorial_active = 0;
static int gns_tutorial_work_valid = 0;
static int gns_tutorial_item_valid = 0;
static unsigned char gns_tutorial_work_save[GAMEFLAG_SIZE * sizeof(Sint32)];
static unsigned char gns_tutorial_item_save[GAMEFLAG_SIZE * sizeof(Sint32)];

static void make_work_save_for_stage(void *buff, int size, int stage_set, int level)
{
    Sint32 *save = (Sint32*)buff;
    int count = size / (int)sizeof(Sint32);

    if (stage_set < 0) stage_set = 0;
    if (stage_set > 2) stage_set = 2;
    if (level < 1) level = 1;
    if (level > 50) level = 50;

    memset(buff, 0, size);
    if (count <= 6) {
        return;
    }

    save[0] = 8 * 32;
    save[1] = (2 * 32) - 16;
    save[2] = stage_set;
    save[3] = level;
    save[4] = 0;
    save[5] = 3;
    save[6] = 3;
}

static void make_work_save_for_level(void *buff, int size, int level)
{
    make_work_save_for_stage(buff, size, 1, level);
}

void gns_wasm_debug_start_level(int level)
{
    gns_tutorial_active = 0;
    gns_tutorial_work_valid = 0;
    gns_tutorial_item_valid = 0;

    gns_debug_cheat_active = 1;
    gns_debug_work_valid = 1;
    gns_debug_item_valid = 1;
    make_work_save_for_level(gns_debug_work_save, (int)sizeof(gns_debug_work_save), level);
    memset(gns_debug_item_save, 0, sizeof(gns_debug_item_save));
}

void gns_wasm_tutorial_start(void)
{
    gns_debug_cheat_active = 0;
    gns_debug_work_valid = 0;
    gns_debug_item_valid = 0;

    gns_tutorial_active = 1;
    gns_tutorial_work_valid = 1;
    gns_tutorial_item_valid = 1;
    make_work_save_for_stage(gns_tutorial_work_save, (int)sizeof(gns_tutorial_work_save), 0, 1);
    memset(gns_tutorial_item_save, 0, sizeof(gns_tutorial_item_save));
}

void gns_wasm_tutorial_clear(void)
{
    gns_tutorial_active = 0;
    gns_tutorial_work_valid = 0;
    gns_tutorial_item_valid = 0;
}

int gns_wasm_tutorial_is_active(void)
{
    return gns_tutorial_active;
}

int gns_wasm_debug_cheat_is_active(void)
{
    return gns_debug_cheat_active;
}

static const char *storage_key_for_path(const char *fn)
{
    if (!fn) return NULL;

    /* Keep these keys stable across browser builds, but accept both the
       platform-neutral filenames and the old Dreamcast VMU names in case an
       older experimental port wrote them. */
    if (strstr(fn, "work.sav") || strstr(fn, "gan_work.sav")) return "work.sav";
    if (strstr(fn, "item_wk.sav") || strstr(fn, "item.sav") || strstr(fn, "gan_item.sav")) return "item_wk.sav";
    if (strstr(fn, "config")) return "config";
    return NULL;
}

static int is_work_save_key(const char *storage_key)
{
    return storage_key && strcmp(storage_key, "work.sav") == 0;
}

static int is_item_save_key(const char *storage_key)
{
    return storage_key && strcmp(storage_key, "item_wk.sav") == 0;
}

static void make_default_work_save(void *buff, int size)
{
    Sint32 *save = (Sint32*)buff;
    int count = size / (int)sizeof(Sint32);

    memset(buff, 0, size);
    if (count <= 6) {
        return;
    }

    /* Mirrors title_init_save_data() for a clean stage-1 start.  This is the
       safe browser fallback when no work save exists, a stale test save is
       unreadable, or localStorage is unavailable. */
    save[0] = 8 * 32;
    save[1] = (2 * 32) - 16;
    save[2] = 1;
    save[3] = 1;
    save[4] = 0;
    save[5] = 3;
    save[6] = 3;
}

static int valid_work_save(const void *buff, int size)
{
    const Sint32 *save = (const Sint32*)buff;
    int count = size / (int)sizeof(Sint32);

    if (count <= 6) {
        return 0;
    }
    if (save[2] < 1 || save[2] > 2) {
        return 0;
    }
    if (save[3] < 1 || save[3] > 50) {
        return 0;
    }

    /* A legitimate work save can contain zero HP after a death.  Older
       browser builds treated that as corruption and silently replaced the
       save with a fresh stage-1 file, which broke normal continue and stage
       selection after dying.  Validate structure here; normalize dead HP
       separately after loading. */
    if (save[5] < -99 || save[5] > 99) {
        return 0;
    }
    if (save[6] <= 0 || save[6] > 99) {
        return 0;
    }
    return 1;
}

static void normalize_work_save_after_load(void *buff, int size)
{
    Sint32 *save = (Sint32*)buff;
    int count = size / (int)sizeof(Sint32);

    if (count <= 6) {
        return;
    }

    /* The ACT loader does not need to inherit a dead HP value.  Keep the
       selected/current stage, but restart with usable health. */
    if (save[5] <= 0) {
        save[5] = save[6] > 0 ? save[6] : 3;
    }
}

static void emit_progress_achievements_from_work_save(const void *buff, int size)
{
    const Sint32 *save = (const Sint32*)buff;
    int count = size / (int)sizeof(Sint32);

    if (count <= 6) {
        return;
    }

    /* save[2] is the stage set, save[3] is the next normal stage number.
       The direct ACT-stage hook remains the primary source, but this save-file
       fallback catches the browser path after scene transitions and avoids
       missing medals if the clear hook is bypassed by the original flow. */
    if (save[2] >= 1 && save[2] <= 2) {
        if (save[3] >= 2) {
            GNS_Achievement(GNS_ACH_FIRST_LEVEL, save[3] - 1);
        }
        if (save[3] >= 51) {
            GNS_Achievement(GNS_ACH_GAME_COMPLETE, save[3] - 1);
        }
    }
}

int Filesystem_Init(void)
{
    return 0;
}

void Filesystem_Shutdown(void)
{
}

int Filesystem_LoadFile(const char *fn, void *buff, int size)
{
    const char *storage_key = storage_key_for_path(fn);
    int asset_size;

    if (storage_key) {
        int rc;

        if (gns_tutorial_active && is_work_save_key(storage_key) && gns_tutorial_work_valid) {
            int copy = size < (int)sizeof(gns_tutorial_work_save) ? size : (int)sizeof(gns_tutorial_work_save);
            memcpy(buff, gns_tutorial_work_save, copy);
            if (copy < size) memset((unsigned char*)buff + copy, 0, size - copy);
            return 0;
        }
        if (gns_tutorial_active && is_item_save_key(storage_key) && gns_tutorial_item_valid) {
            int copy = size < (int)sizeof(gns_tutorial_item_save) ? size : (int)sizeof(gns_tutorial_item_save);
            memcpy(buff, gns_tutorial_item_save, copy);
            if (copy < size) memset((unsigned char*)buff + copy, 0, size - copy);
            return 0;
        }
        if (gns_debug_cheat_active && is_work_save_key(storage_key) && gns_debug_work_valid) {
            int copy = size < (int)sizeof(gns_debug_work_save) ? size : (int)sizeof(gns_debug_work_save);
            memcpy(buff, gns_debug_work_save, copy);
            if (copy < size) memset((unsigned char*)buff + copy, 0, size - copy);
            return 0;
        }
        if (gns_debug_cheat_active && is_item_save_key(storage_key) && gns_debug_item_valid) {
            int copy = size < (int)sizeof(gns_debug_item_save) ? size : (int)sizeof(gns_debug_item_save);
            memcpy(buff, gns_debug_item_save, copy);
            if (copy < size) memset((unsigned char*)buff + copy, 0, size - copy);
            return 0;
        }

        rc = gns_local_storage_load(storage_key, (unsigned char*)buff, size);

        if (rc == 0) {
            if (is_work_save_key(storage_key)) {
                if (!valid_work_save(buff, size)) {
                    make_default_work_save(buff, size);
                    return -1;
                }
                normalize_work_save_after_load(buff, size);
            }
            return 0;
        }

        if (is_work_save_key(storage_key)) {
            make_default_work_save(buff, size);
        } else if (is_item_save_key(storage_key)) {
            memset(buff, 0, size);
        }

        return -1;
    }

    asset_size = gns_asset_size(fn);
    if (asset_size < size) {
        printf("asset open/read error: %s\n", fn);
        return -1;
    }

    return gns_asset_read(fn, (unsigned char*)buff, size) == size ? 0 : -1;
}

int Filesystem_SaveFile(const char *fn, const void *buff, int size)
{
    const char *storage_key = storage_key_for_path(fn);

    if (storage_key) {
        if (gns_tutorial_active && is_work_save_key(storage_key)) {
            int copy = size < (int)sizeof(gns_tutorial_work_save) ? size : (int)sizeof(gns_tutorial_work_save);
            memcpy(gns_tutorial_work_save, buff, copy);
            if (copy < (int)sizeof(gns_tutorial_work_save)) memset(gns_tutorial_work_save + copy, 0, sizeof(gns_tutorial_work_save) - copy);
            gns_tutorial_work_valid = 1;
            return 0;
        }
        if (gns_tutorial_active && is_item_save_key(storage_key)) {
            int copy = size < (int)sizeof(gns_tutorial_item_save) ? size : (int)sizeof(gns_tutorial_item_save);
            memcpy(gns_tutorial_item_save, buff, copy);
            if (copy < (int)sizeof(gns_tutorial_item_save)) memset(gns_tutorial_item_save + copy, 0, sizeof(gns_tutorial_item_save) - copy);
            gns_tutorial_item_valid = 1;
            return 0;
        }
        if (gns_debug_cheat_active && is_work_save_key(storage_key)) {
            int copy = size < (int)sizeof(gns_debug_work_save) ? size : (int)sizeof(gns_debug_work_save);
            memcpy(gns_debug_work_save, buff, copy);
            if (copy < (int)sizeof(gns_debug_work_save)) memset(gns_debug_work_save + copy, 0, sizeof(gns_debug_work_save) - copy);
            gns_debug_work_valid = 1;
            return 0;
        }
        if (gns_debug_cheat_active && is_item_save_key(storage_key)) {
            int copy = size < (int)sizeof(gns_debug_item_save) ? size : (int)sizeof(gns_debug_item_save);
            memcpy(gns_debug_item_save, buff, copy);
            if (copy < (int)sizeof(gns_debug_item_save)) memset(gns_debug_item_save + copy, 0, sizeof(gns_debug_item_save) - copy);
            gns_debug_item_valid = 1;
            return 0;
        }

        int rc = gns_local_storage_save(storage_key, (const unsigned char*)buff, size);
        if (is_work_save_key(storage_key)) {
            emit_progress_achievements_from_work_save(buff, size);
        }
        return rc;
    }

    /* Browser save writes are intentionally restricted to persistent game state. */
    return -1;
}

void Filesystem_GetConfigPath(char *path, size_t path_size)
{
    snprintf(path, path_size, "config");
}

void Filesystem_GetSaveFolderPath(char *path, size_t path_size)
{
    snprintf(path, path_size, ".");
}

void Filesystem_CreateSaveFolder(const char *path)
{
    (void)path;
}

void Filesystem_GetWorkSavePath(char *path, size_t path_size)
{
    snprintf(path, path_size, "work.sav");
}

void Filesystem_GetItemSavePath(char *path, size_t path_size)
{
    snprintf(path, path_size, "item_wk.sav");
}

int LoadFile(char *fn, int *buff, int size)
{
    return Filesystem_LoadFile(fn, buff, size);
}

int SaveFile(char *fn, int *buff, int size)
{
    return Filesystem_SaveFile(fn, buff, size);
}

int LoadGameFlag(char *fn)
{
    return Filesystem_LoadFile(fn, gameflag, sizeof(gameflag));
}

int SaveGameFlag(char *fn)
{
    return Filesystem_SaveFile(fn, gameflag, sizeof(gameflag));
}

int LoadGameFlag2(char *fn)
{
    return Filesystem_LoadFile(fn, gameflag2, sizeof(gameflag2));
}

int SaveGameFlag2(char *fn)
{
    return Filesystem_SaveFile(fn, gameflag2, sizeof(gameflag2));
}
