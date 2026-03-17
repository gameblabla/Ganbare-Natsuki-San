#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stddef.h>

int Filesystem_Init(void);
void Filesystem_Shutdown(void);
int Filesystem_LoadFile(const char *fn, void *buff, int size);
int Filesystem_SaveFile(const char *fn, const void *buff, int size);
void Filesystem_GetConfigPath(char *path, size_t path_size);
void Filesystem_GetSaveFolderPath(char *path, size_t path_size);
void Filesystem_CreateSaveFolder(const char *path);
void Filesystem_GetWorkSavePath(char *path, size_t path_size);
void Filesystem_GetItemSavePath(char *path, size_t path_size);

int LoadFile(char *fn, int *buff, int size);
int SaveFile(char *fn, int *buff, int size);
int LoadGameFlag(char *fn);
int SaveGameFlag(char *fn);
int LoadGameFlag2(char *fn);
int SaveGameFlag2(char *fn);

#endif
