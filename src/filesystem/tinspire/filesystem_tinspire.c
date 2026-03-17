#include "../../define.h"
#include "../../filesystem/filesystem.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	char buf[256];

	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "rb" ) ) == NULL )
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
	char buf[256];

	snprintf(buf, sizeof(buf), "%s.tns", fn);
	if ( ( fp = fopen( buf, "wb" ) ) == NULL )
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
	snprintf(path, path_size, "./save/config.tns");
}

void Filesystem_GetSaveFolderPath(char *path, size_t path_size)
{
	snprintf(path, path_size, "./save");
}

void Filesystem_CreateSaveFolder(const char *path)
{
	mkdir(path, 0755);
}

void Filesystem_GetWorkSavePath(char *path, size_t path_size)
{
	snprintf(path, path_size, "./save/work.sav.tns");
}

void Filesystem_GetItemSavePath(char *path, size_t path_size)
{
	snprintf(path, path_size, "./save/item_wk.sav.tns");
}
