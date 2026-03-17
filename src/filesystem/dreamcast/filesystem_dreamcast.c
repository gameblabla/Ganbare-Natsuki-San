#include "../../define.h"
#include "../../filesystem/filesystem.h"
#include "vmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kos.h>

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
	snprintf(path, path_size, "/ram/config");
}

void Filesystem_GetSaveFolderPath(char *path, size_t path_size)
{
	snprintf(path, path_size, "/ram");
}

void Filesystem_CreateSaveFolder(const char *path)
{
	(void)path;
}

void Filesystem_GetWorkSavePath(char *path, size_t path_size)
{
	snprintf(path, path_size, "/ram/work.sav");
}

void Filesystem_GetItemSavePath(char *path, size_t path_size)
{
	snprintf(path, path_size, "/ram/item_wk.sav");
}
