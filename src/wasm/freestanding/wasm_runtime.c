#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

__attribute__((import_module("env"), import_name("gns_host_log")))
void gns_host_log(const char *text);

__attribute__((import_module("env"), import_name("gns_host_exit")))
void gns_host_exit(int code);

__attribute__((import_module("env"), import_name("gns_now_ms")))
double gns_now_ms(void);

struct GNS_FILE { int fd; };
static struct GNS_FILE g_stderr_file = { 2 };
static struct GNS_FILE g_stdout_file = { 1 };
static struct GNS_FILE g_stdin_file = { 0 };
FILE *stderr = &g_stderr_file;
FILE *stdout = &g_stdout_file;
FILE *stdin = &g_stdin_file;

extern unsigned char __heap_base;
static uintptr_t g_heap_next = 0;

typedef struct AllocHeader {
    size_t size;
} AllocHeader;

static uintptr_t align_up_uintptr(uintptr_t v, uintptr_t align)
{
    return (v + align - 1u) & ~(align - 1u);
}

static uintptr_t memory_bytes(void)
{
    return (uintptr_t)__builtin_wasm_memory_size(0) * 65536u;
}

static int grow_to_fit(uintptr_t end)
{
    uintptr_t current = memory_bytes();
    if (end <= current) return 1;
    uintptr_t needed = end - current;
    uintptr_t pages = (needed + 65535u) / 65536u;
    return __builtin_wasm_memory_grow(0, pages) != (size_t)-1;
}

void *malloc(size_t size)
{
    uintptr_t base;
    uintptr_t data;
    uintptr_t end;
    AllocHeader *header;

    if (size == 0) size = 1;
    if (!g_heap_next) g_heap_next = align_up_uintptr((uintptr_t)&__heap_base, 16u);

    base = align_up_uintptr(g_heap_next, 16u);
    data = base + sizeof(AllocHeader);
    data = align_up_uintptr(data, 16u);
    end = data + align_up_uintptr((uintptr_t)size, 16u);
    if (end < data) return 0;
    if (!grow_to_fit(end)) return 0;

    header = (AllocHeader *)(data - sizeof(AllocHeader));
    header->size = size;
    g_heap_next = end;
    return (void *)data;
}

void free(void *ptr)
{
    (void)ptr;
}

void *calloc(size_t nmemb, size_t size)
{
    size_t bytes;
    void *ptr;
    if (size != 0 && nmemb > ((size_t)-1) / size) return 0;
    bytes = nmemb * size;
    ptr = malloc(bytes);
    if (ptr) memset(ptr, 0, bytes);
    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    void *out;
    size_t old_size = 0;
    if (!ptr) return malloc(size);
    if (size == 0) return 0;
    old_size = ((AllocHeader *)((unsigned char *)ptr - sizeof(AllocHeader)))->size;
    out = malloc(size);
    if (out) memcpy(out, ptr, old_size < size ? old_size : size);
    return out;
}

void *memset(void *dst, int c, size_t n)
{
    unsigned char *d = (unsigned char *)dst;
    while (n--) *d++ = (unsigned char)c;
    return dst;
}

void *memcpy(void *dst, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    while (n--) *d++ = *s++;
    return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    if (d == s || n == 0) return dst;
    if (d < s) {
        while (n--) *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n--) *--d = *--s;
    }
    return dst;
}

int memcmp(const void *a, const void *b, size_t n)
{
    const unsigned char *x = (const unsigned char *)a;
    const unsigned char *y = (const unsigned char *)b;
    while (n--) {
        if (*x != *y) return (int)*x - (int)*y;
        x++; y++;
    }
    return 0;
}

size_t strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

char *strcpy(char *dst, const char *src)
{
    char *out = dst;
    while ((*dst++ = *src++) != 0) {}
    return out;
}

char *strncpy(char *dst, const char *src, size_t n)
{
    char *out = dst;
    while (n && *src) { *dst++ = *src++; n--; }
    while (n) { *dst++ = 0; n--; }
    return out;
}

char *strcat(char *dst, const char *src)
{
    strcpy(dst + strlen(dst), src);
    return dst;
}

int strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char *a, const char *b, size_t n)
{
    while (n && *a && *a == *b) { a++; b++; n--; }
    if (!n) return 0;
    return (unsigned char)*a - (unsigned char)*b;
}

char *strchr(const char *s, int c)
{
    while (*s) {
        if (*s == (char)c) return (char *)s;
        s++;
    }
    return c == 0 ? (char *)s : 0;
}

char *strstr(const char *haystack, const char *needle)
{
    size_t nlen;
    if (!*needle) return (char *)haystack;
    nlen = strlen(needle);
    while (*haystack) {
        if (*haystack == *needle && strncmp(haystack, needle, nlen) == 0) return (char *)haystack;
        haystack++;
    }
    return 0;
}

int abs(int value) { return value < 0 ? -value : value; }

static unsigned int g_rand_state = 1u;
void srand(unsigned int seed) { g_rand_state = seed ? seed : 1u; }
int rand(void)
{
    g_rand_state = g_rand_state * 1103515245u + 12345u;
    return (int)((g_rand_state >> 16) & 0x7fffu);
}

static long parse_long(const char *s)
{
    long sign = 1;
    long value = 0;
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    if (*s == '-') { sign = -1; s++; }
    else if (*s == '+') s++;
    while (*s >= '0' && *s <= '9') {
        value = value * 10 + (*s - '0');
        s++;
    }
    return sign * value;
}

int atoi(const char *s) { return (int)parse_long(s); }
long atol(const char *s) { return parse_long(s); }

time_t time(time_t *out)
{
    time_t t = (time_t)(gns_now_ms() / 1000.0);
    if (out) *out = t;
    return t;
}

clock_t clock(void)
{
    return (clock_t)(gns_now_ms() * 1000.0);
}

static void out_char(char **dst, size_t *remaining, int *count, char c)
{
    if (*remaining > 1) {
        **dst = c;
        (*dst)++;
        (*remaining)--;
    }
    (*count)++;
}

static void out_str(char **dst, size_t *remaining, int *count, const char *s)
{
    if (!s) s = "(null)";
    while (*s) out_char(dst, remaining, count, *s++);
}

static void out_uint(char **dst, size_t *remaining, int *count, unsigned long value, unsigned base, int neg, int width, int zero_pad)
{
    char tmp[32];
    int pos = 0;
    int digits;
    if (value == 0) tmp[pos++] = '0';
    while (value) {
        unsigned d = (unsigned)(value % base);
        tmp[pos++] = (char)(d < 10 ? '0' + d : 'a' + d - 10);
        value /= base;
    }
    digits = pos + (neg ? 1 : 0);
    while (width > digits) { out_char(dst, remaining, count, zero_pad ? '0' : ' '); width--; }
    if (neg) out_char(dst, remaining, count, '-');
    while (pos--) out_char(dst, remaining, count, tmp[pos]);
}

int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    char *dst = str;
    size_t remaining = size;
    int count = 0;

    if (!str || size == 0) {
        static char dummy;
        dst = &dummy;
        remaining = 0;
    }

    while (*fmt) {
        if (*fmt != '%') {
            out_char(&dst, &remaining, &count, *fmt++);
            continue;
        }
        fmt++;
        if (*fmt == '%') { out_char(&dst, &remaining, &count, *fmt++); continue; }

        int zero_pad = 0;
        int width = 0;
        if (*fmt == '0') { zero_pad = 1; fmt++; }
        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }
        while (*fmt == 'l' || *fmt == 'h' || *fmt == 'z') fmt++;

        switch (*fmt) {
        case 's':
            out_str(&dst, &remaining, &count, va_arg(ap, const char *));
            break;
        case 'c':
            out_char(&dst, &remaining, &count, (char)va_arg(ap, int));
            break;
        case 'd':
        case 'i': {
            long v = (long)va_arg(ap, int);
            int neg = v < 0;
            unsigned long uv = neg ? (unsigned long)(-v) : (unsigned long)v;
            out_uint(&dst, &remaining, &count, uv, 10u, neg, width, zero_pad);
            break;
        }
        case 'u':
            out_uint(&dst, &remaining, &count, (unsigned long)va_arg(ap, unsigned int), 10u, 0, width, zero_pad);
            break;
        case 'x':
        case 'X':
            out_uint(&dst, &remaining, &count, (unsigned long)va_arg(ap, unsigned int), 16u, 0, width, zero_pad);
            break;
        default:
            out_char(&dst, &remaining, &count, '%');
            if (*fmt) out_char(&dst, &remaining, &count, *fmt);
            break;
        }
        if (*fmt) fmt++;
    }

    if (size > 0) {
        if (remaining > 0) *dst = 0;
        else str[size - 1] = 0;
    }
    return count;
}

int snprintf(char *str, size_t size, const char *fmt, ...)
{
    va_list ap;
    int rc;
    va_start(ap, fmt);
    rc = vsnprintf(str, size, fmt, ap);
    va_end(ap);
    return rc;
}

int sprintf(char *str, const char *fmt, ...)
{
    va_list ap;
    int rc;
    va_start(ap, fmt);
    rc = vsnprintf(str, (size_t)-1, fmt, ap);
    va_end(ap);
    return rc;
}

int printf(const char *fmt, ...)
{
    char buf[512];
    va_list ap;
    int rc;
    va_start(ap, fmt);
    rc = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    gns_host_log(buf);
    return rc;
}

int fprintf(FILE *stream, const char *fmt, ...)
{
    char buf[512];
    va_list ap;
    int rc;
    (void)stream;
    va_start(ap, fmt);
    rc = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    gns_host_log(buf);
    return rc;
}

void exit(int status)
{
    gns_host_exit(status);
    for (;;) {}
}

int access(const char *path, int mode) { (void)path; (void)mode; return -1; }
int close(int fd) { (void)fd; return -1; }
int read(int fd, void *buf, unsigned int count) { (void)fd; (void)buf; (void)count; return -1; }
int write(int fd, const void *buf, unsigned int count) { (void)fd; (void)buf; (void)count; return -1; }
int mkdir(const char *path, unsigned int mode) { (void)path; (void)mode; return 0; }
