#ifndef GNS_WASM_FREESTANDING_UNISTD_H
#define GNS_WASM_FREESTANDING_UNISTD_H
int access(const char *path, int mode);
int close(int fd);
int read(int fd, void *buf, unsigned int count);
int write(int fd, const void *buf, unsigned int count);
#define F_OK 0
#endif
