#ifndef GNS_WASM_FREESTANDING_STDLIB_H
#define GNS_WASM_FREESTANDING_STDLIB_H

#include <stddef.h>

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void exit(int status) __attribute__((noreturn));
int abs(int value);
int rand(void);
void srand(unsigned int seed);
int atoi(const char *s);
long atol(const char *s);

#define RAND_MAX 32767

#endif
