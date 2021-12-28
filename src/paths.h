#ifndef PATHS_H
#define PATHS_H

#include "define.h"

/* We are now using SDL's own file functions as to avoid using Mac's specific ":" separator */
#if defined (CLASSICMAC)
#define SYS_IMAGE_PATH ":image:sys:"
#define IMAGE_PATH ":image:"
#define TITLE_IMAGE_PATH ":image:title:"
#define BAK_IMAGE_PATH ":image:bak:"
#define END_IMAGE_PATH ":image:end:"
#define ENM_IMAGE_PATH ":image:enm:"
#define PLAYER_IMAGE_PATH ":image:player:"
#define MAP_IMAGE_PATH ":image:map:"
#define MAP_DATA_PATH ":data:%d:"
#define OPTION_IMAGE_PATH ":image:option:"
#define BGM_SOUND_PATH ":sound:bgm:"
#define SE_SOUND_PATH ":sound:se:"
#define CONFIG_FIG_PATH "::config.fig"
#elif defined(MINGW)
#define SYS_IMAGE_PATH "image\\sys\\"
#define IMAGE_PATH "image\\"
#define TITLE_IMAGE_PATH "image\\title\\"
#define BAK_IMAGE_PATH "image\\bak\\"
#define END_IMAGE_PATH "image\\end\\"
#define ENM_IMAGE_PATH "image\\enm\\"
#define PLAYER_IMAGE_PATH "image\\player\\"
#define MAP_IMAGE_PATH "image\\map\\"
#define MAP_DATA_PATH "data\\%d\\"
#define OPTION_IMAGE_PATH "image\\option\\"
#define BGM_SOUND_PATH "sound\\bgm\\"
#define SE_SOUND_PATH "sound\\se\\"
#define CONFIG_FIG_PATH "config.fig"
#elif defined(DREAMCAST)

#ifdef NAOMI
#define PREFIX_FILE "/rd/"
#else
#define PREFIX_FILE "/cd/"
#endif

#define SYS_IMAGE_PATH PREFIX_FILE"image/sys/"
#define IMAGE_PATH PREFIX_FILE"image/"
#define TITLE_IMAGE_PATH PREFIX_FILE"image/title/"
#define BAK_IMAGE_PATH PREFIX_FILE"image/bak/"
#define END_IMAGE_PATH PREFIX_FILE"image/end/"
#define ENM_IMAGE_PATH PREFIX_FILE"image/enm/"
#define PLAYER_IMAGE_PATH PREFIX_FILE"image/player/"
// Map Image and Data is loaded from romdisk for faster disk access, on a NAOMI, all of this would be ROMDISK
#define MAP_IMAGE_PATH "/rd/image/map/"
#define MAP_DATA_PATH "/rd/data/%d/"
#define OPTION_IMAGE_PATH PREFIX_FILE"image/option/"
#define BGM_SOUND_PATH PREFIX_FILE"sound/bgm/"
#define SE_SOUND_PATH PREFIX_FILE"sound/se/"
#define CONFIG_FIG_PATH "/ram/config.fig"
#else
#define SYS_IMAGE_PATH "image/sys/"
#define IMAGE_PATH "image/"
#define TITLE_IMAGE_PATH "image/title/"
#define BAK_IMAGE_PATH "image/bak/"
#define END_IMAGE_PATH "image/end/"
#define ENM_IMAGE_PATH "image/enm/"
#define PLAYER_IMAGE_PATH "image/player/"
#define MAP_IMAGE_PATH "image/map/"
#define MAP_DATA_PATH "data/%d/"
#define OPTION_IMAGE_PATH "image/option/"
#define BGM_SOUND_PATH "sound/bgm/"
#define SE_SOUND_PATH "sound/se/"
#define CONFIG_FIG_PATH "config/config.fig"
#endif

#endif
