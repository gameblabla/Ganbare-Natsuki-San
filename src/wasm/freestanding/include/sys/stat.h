#ifndef GNS_WASM_FREESTANDING_SYS_STAT_H
#define GNS_WASM_FREESTANDING_SYS_STAT_H
#define S_IRWXU 0700
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
int mkdir(const char *path, unsigned int mode);
#endif
