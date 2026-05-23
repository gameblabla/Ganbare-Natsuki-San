#ifndef GNS_WASM_FREESTANDING_STDIO_H
#define GNS_WASM_FREESTANDING_STDIO_H

#include <stddef.h>
#include <stdarg.h>

typedef struct GNS_FILE GNS_FILE;
typedef GNS_FILE FILE;
extern FILE *stderr;
extern FILE *stdout;
extern FILE *stdin;

int printf(const char *fmt, ...);
int fprintf(FILE *stream, const char *fmt, ...);
int sprintf(char *str, const char *fmt, ...);
int snprintf(char *str, size_t size, const char *fmt, ...);
int vsnprintf(char *str, size_t size, const char *fmt, va_list ap);

#endif
