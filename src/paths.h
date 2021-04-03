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
#endif

#endif
