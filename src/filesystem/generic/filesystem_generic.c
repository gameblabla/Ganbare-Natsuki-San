#include "../../define.h"
#include "../../extern.h"
#include "../../filesystem/filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

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
	int rc = 0;

	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;	
	}
	else 
	{
		fread( buff, 1, size, fp );
		fclose( fp );	
	}
	
	return rc;
}

int Filesystem_SaveFile(const char *fn, const void *buff, int size)
{
	FILE *fp;
	int rc = 0;

	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
		fwrite( buff, 1, size, fp );
		fclose( fp );
	}
	
	return rc;
}

void Filesystem_GetConfigPath(char *path, size_t path_size)
{
	snprintf(path, path_size, "%s/.ganbare/config", getenv("HOME"));
}

void Filesystem_GetSaveFolderPath(char *path, size_t path_size)
{
	snprintf(path, path_size, "%s/.ganbare", getenv("HOME"));
}

void Filesystem_CreateSaveFolder(const char *path)
{
	mkdir(path, 0755);
}

void Filesystem_GetWorkSavePath(char *path, size_t path_size)
{
	snprintf(path, path_size, "%s/.ganbare/work.sav", getenv("HOME"));
}

void Filesystem_GetItemSavePath(char *path, size_t path_size)
{
	snprintf(path, path_size, "%s/.ganbare/item_wk.sav", getenv("HOME"));
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
