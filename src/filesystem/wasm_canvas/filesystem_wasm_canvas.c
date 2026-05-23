#include "../../define.h"
#include "../../extern.h"
#include "../../filesystem/filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

EM_JS(int, gns_local_storage_load, (const char *key_ptr, unsigned char *dst, int size), {
    var key = 'ganbare-natsuki-san:' + UTF8ToString(key_ptr);
    var value = localStorage.getItem(key);
    if (value === null) return -1;
    var bin;
    try {
        bin = atob(value);
    } catch (e) {
        return -1;
    }
    if (bin.length !== size) return -1;
    for (var i = 0; i < size; i++) Module.HEAPU8[dst + i] = bin.charCodeAt(i) & 255;
    return 0;
});

EM_JS(int, gns_local_storage_save, (const char *key_ptr, const unsigned char *src, int size), {
    var key = 'ganbare-natsuki-san:' + UTF8ToString(key_ptr);
    var chunk = 0x8000;
    var parts = [];
    for (var i = 0; i < size; i += chunk) {
        var end = Math.min(i + chunk, size);
        parts.push(String.fromCharCode.apply(null, Module.HEAPU8.subarray(src + i, src + end)));
    }
    localStorage.setItem(key, btoa(parts.join('')));
    return 0;
});
#else
static int gns_local_storage_load(const char *key, unsigned char *dst, int size) { (void)key; (void)dst; (void)size; return -1; }
static int gns_local_storage_save(const char *key, const unsigned char *src, int size) { (void)key; (void)src; (void)size; return -1; }
#endif

static const char *storage_key_for_path(const char *fn)
{
    if (!fn) return NULL;
    if (strstr(fn, "work.sav")) return "work.sav";
    if (strstr(fn, "item_wk.sav")) return "item_wk.sav";
    if (strstr(fn, "config")) return "config";
    return NULL;
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
    FILE *fp;
    const char *storage_key;

    storage_key = storage_key_for_path(fn);
    if (storage_key) {
        return gns_local_storage_load(storage_key, (unsigned char*)buff, size);
    }

    fp = fopen(fn, "rb");
    if (!fp) {
        printf("file open error!! %s\n", fn);
        return -1;
    }

    if (fread(buff, 1, (size_t)size, fp) != (size_t)size) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int Filesystem_SaveFile(const char *fn, const void *buff, int size)
{
    FILE *fp;
    const char *storage_key;

    storage_key = storage_key_for_path(fn);
    if (storage_key) {
        return gns_local_storage_save(storage_key, (const unsigned char*)buff, size);
    }

    fp = fopen(fn, "wb");
    if (!fp) {
        printf("file open error!! %s\n", fn);
        return -1;
    }

    if (fwrite(buff, 1, (size_t)size, fp) != (size_t)size) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
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
