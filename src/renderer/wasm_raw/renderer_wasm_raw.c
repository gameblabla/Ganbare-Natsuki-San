#include "../../define.h"
#include "../../renderer/renderer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__attribute__((import_module("env"), import_name("gns_canvas_init")))
void gns_canvas_init(int w, int h);

__attribute__((import_module("env"), import_name("gns_canvas_present")))
void gns_canvas_present(const unsigned char *pixels, int w, int h);

__attribute__((import_module("env"), import_name("gns_now_ms")))
double gns_now_ms(void);

__attribute__((import_module("env"), import_name("gns_asset_size")))
int gns_asset_size(const char *path);

__attribute__((import_module("env"), import_name("gns_asset_read")))
int gns_asset_read(const char *path, unsigned char *dst, int capacity);


typedef struct WasmCanvasSurface {
    int w;
    int h;
    int pitch;
    unsigned char *pixels; /* byte order is RGBA for Canvas ImageData. */
} WasmCanvasSurface;

typedef struct WasmCanvasRenderer {
    WasmCanvasSurface *screen;
    WasmCanvasSurface *bitmap[BMPBUFF_MAX];
    int initialised;
} WasmCanvasRenderer;

static WasmCanvasRenderer *g_wasm_renderer;

static uint16_t rd16(const unsigned char *p)
{
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}

static uint32_t rd32(const unsigned char *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static int32_t rds32(const unsigned char *p)
{
    return (int32_t)rd32(p);
}

static WasmCanvasSurface *surface_create(int w, int h)
{
    WasmCanvasSurface *s;
    size_t bytes;

    if (w <= 0 || h <= 0) return NULL;

    s = (WasmCanvasSurface*)calloc(1, sizeof(*s));
    if (!s) return NULL;

    s->w = w;
    s->h = h;
    s->pitch = w * 4;
    bytes = (size_t)s->pitch * (size_t)h;
    s->pixels = (unsigned char*)calloc(1, bytes);
    if (!s->pixels) {
        free(s);
        return NULL;
    }
    return s;
}

static void surface_free(WasmCanvasSurface *s)
{
    if (!s) return;
    free(s->pixels);
    free(s);
}

static void surface_fill_rgba(WasmCanvasSurface *s, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    int x;
    int y;

    if (!s || !s->pixels) return;
    for (y = 0; y < s->h; y++) {
        unsigned char *row = s->pixels + (size_t)y * (size_t)s->pitch;
        for (x = 0; x < s->w; x++) {
            row[x * 4 + 0] = r;
            row[x * 4 + 1] = g;
            row[x * 4 + 2] = b;
            row[x * 4 + 3] = a;
        }
    }
}

static unsigned char *read_entire_file(const char *path, size_t *out_size)
{
    int length;
    unsigned char *data;

    if (out_size) *out_size = 0;
    length = gns_asset_size(path);
    if (length <= 0) return NULL;

    data = (unsigned char*)malloc((size_t)length);
    if (!data) return NULL;

    if (gns_asset_read(path, data, length) != length) {
        free(data);
        return NULL;
    }

    if (out_size) *out_size = (size_t)length;
    return data;
}

static WasmCanvasSurface *load_bmp_rgba(const char *path, int use_colorkey)
{
    size_t size;
    unsigned char *file;
    const unsigned char *palette = NULL;
    const unsigned char *pixels;
    uint32_t data_offset;
    uint32_t dib_size;
    int32_t signed_w;
    int32_t signed_h;
    int width;
    int height;
    int top_down;
    uint16_t bpp;
    uint32_t compression;
    uint32_t colors_used;
    int palette_entries = 0;
    int row_stride;
    int x;
    int y;
    WasmCanvasSurface *surface;

    file = read_entire_file(path, &size);
    if (!file || size < 54) {
        free(file);
        return NULL;
    }

    if (file[0] != 'B' || file[1] != 'M') {
        free(file);
        return NULL;
    }

    data_offset = rd32(file + 10);
    dib_size = rd32(file + 14);
    if (dib_size < 40 || 14u + dib_size > size) {
        free(file);
        return NULL;
    }

    signed_w = rds32(file + 18);
    signed_h = rds32(file + 22);
    bpp = rd16(file + 28);
    compression = rd32(file + 30);
    colors_used = rd32(file + 46);

    if (signed_w <= 0 || signed_h == 0 || compression != 0) {
        free(file);
        return NULL;
    }

    width = signed_w;
    top_down = signed_h < 0;
    height = signed_h < 0 ? -signed_h : signed_h;

    if (!(bpp == 8 || bpp == 24 || bpp == 32)) {
        printf("WASM raw BMP loader only supports 8/24/32-bpp uncompressed BMP: %s\n", path);
        free(file);
        return NULL;
    }

    if (bpp == 8) {
        palette = file + 14 + dib_size;
        palette_entries = colors_used ? (int)colors_used : 256;
        if ((size_t)(palette - file) + (size_t)palette_entries * 4u > size) {
            free(file);
            return NULL;
        }
    }

    row_stride = ((width * (int)bpp + 31) / 32) * 4;
    if (data_offset >= size || (size_t)data_offset + (size_t)row_stride * (size_t)height > size) {
        free(file);
        return NULL;
    }

    surface = surface_create(width, height);
    if (!surface) {
        free(file);
        return NULL;
    }

    pixels = file + data_offset;
    for (y = 0; y < height; y++) {
        int source_y = top_down ? y : (height - 1 - y);
        const unsigned char *src = pixels + (size_t)source_y * (size_t)row_stride;
        unsigned char *dst = surface->pixels + (size_t)y * (size_t)surface->pitch;

        for (x = 0; x < width; x++) {
            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;
            unsigned char a = 255;

            if (bpp == 8) {
                unsigned char idx = src[x];
                const unsigned char *pal;
                if ((int)idx >= palette_entries) idx = 0;
                pal = palette + (int)idx * 4;
                b = pal[0];
                g = pal[1];
                r = pal[2];
            } else if (bpp == 24) {
                const unsigned char *p = src + x * 3;
                b = p[0];
                g = p[1];
                r = p[2];
            } else {
                const unsigned char *p = src + x * 4;
                b = p[0];
                g = p[1];
                r = p[2];
                a = p[3] ? p[3] : 255;
            }

            if (use_colorkey && r == 16 && g == 99 && b == 62) {
                a = 0;
            }

            dst[x * 4 + 0] = r;
            dst[x * 4 + 1] = g;
            dst[x * 4 + 2] = b;
            dst[x * 4 + 3] = a;
        }
    }

    free(file);
    return surface;
}

static void blit_surface(WasmCanvasSurface *src, WasmCanvasSurface *dst, int dst_x, int dst_y, int src_x, int src_y, int width, int height)
{
    int x;
    int y;

    if (!src || !dst || !src->pixels || !dst->pixels) return;
    if (width <= 0 || height <= 0) return;

    if (src_x < 0) { dst_x -= src_x; width += src_x; src_x = 0; }
    if (src_y < 0) { dst_y -= src_y; height += src_y; src_y = 0; }
    if (dst_x < 0) { src_x -= dst_x; width += dst_x; dst_x = 0; }
    if (dst_y < 0) { src_y -= dst_y; height += dst_y; dst_y = 0; }

    if (src_x + width > src->w) width = src->w - src_x;
    if (src_y + height > src->h) height = src->h - src_y;
    if (dst_x + width > dst->w) width = dst->w - dst_x;
    if (dst_y + height > dst->h) height = dst->h - dst_y;

    if (width <= 0 || height <= 0) return;

    for (y = 0; y < height; y++) {
        unsigned char *srow = src->pixels + (size_t)(src_y + y) * (size_t)src->pitch + (size_t)src_x * 4u;
        unsigned char *drow = dst->pixels + (size_t)(dst_y + y) * (size_t)dst->pitch + (size_t)dst_x * 4u;
        for (x = 0; x < width; x++) {
            unsigned char *sp = srow + x * 4;
            unsigned char *dp = drow + x * 4;
            if (sp[3] != 0) {
                dp[0] = sp[0];
                dp[1] = sp[1];
                dp[2] = sp[2];
                dp[3] = 255;
            }
        }
    }
}

void Renderer_Init(void)
{
    if (g_wasm_renderer) return;
    g_wasm_renderer = (WasmCanvasRenderer*)calloc(1, sizeof(*g_wasm_renderer));
    if (g_wasm_renderer) g_wasm_renderer->initialised = 1;
}

void Renderer_Shutdown(void)
{
    if (!g_wasm_renderer) return;
    Renderer_ShutdownVideo();
    free(g_wasm_renderer);
    g_wasm_renderer = NULL;
}

void Renderer_InitVideo(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (!g_wasm_renderer) Renderer_Init();
    if (!g_wasm_renderer) return;

    g_wasm_renderer->screen = surface_create(DISPLY_WIDTH, DISPLY_HEIGHT);
    if (!g_wasm_renderer->screen) return;
    surface_fill_rgba(g_wasm_renderer->screen, 0, 0, 0, 255);
    gns_canvas_init(DISPLY_WIDTH, DISPLY_HEIGHT);
}

void Renderer_ShutdownVideo(void)
{
    int i;
    if (!g_wasm_renderer) return;

    for (i = 0; i < BMPBUFF_MAX; i++) {
        if (g_wasm_renderer->bitmap[i]) {
            surface_free(g_wasm_renderer->bitmap[i]);
            g_wasm_renderer->bitmap[i] = NULL;
        }
    }

    if (g_wasm_renderer->screen) {
        surface_free(g_wasm_renderer->screen);
        g_wasm_renderer->screen = NULL;
    }
}

void Renderer_SetWindowCaption(const char *title, const char *icon)
{
    (void)title;
    (void)icon;
}

void Renderer_Flip(void)
{
}

void Renderer_RefreshScreen(void)
{
    if (!g_wasm_renderer || !g_wasm_renderer->screen) return;
    gns_canvas_present(g_wasm_renderer->screen->pixels, g_wasm_renderer->screen->w, g_wasm_renderer->screen->h);
}

int LoadBitmap(char *fname, int bmpindex, int flag)
{
    WasmCanvasSurface *surface;

    if (!g_wasm_renderer || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) return -1;

    ReleaseBitmap(bmpindex);
    surface = load_bmp_rgba(fname, flag != 0);
    if (!surface) {
        printf("WASM raw LoadBitmap failed: %s\n", fname);
        return -1;
    }

    g_wasm_renderer->bitmap[bmpindex] = surface;
    return 0;
}

void ReleaseBitmap(int bmpindex)
{
    if (!g_wasm_renderer || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) return;
    surface_free(g_wasm_renderer->bitmap[bmpindex]);
    g_wasm_renderer->bitmap[bmpindex] = NULL;
}

void Blt(int bmpindex, int dstX, int dstY)
{
    WasmCanvasSurface *src;
    if (!g_wasm_renderer || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) return;
    src = g_wasm_renderer->bitmap[bmpindex];
    if (!src) return;
    blit_surface(src, g_wasm_renderer->screen, dstX, dstY, 0, 0, src->w, src->h);
}

void BltRect(int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height)
{
    if (!g_wasm_renderer || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) return;
    blit_surface(g_wasm_renderer->bitmap[bmpindex], g_wasm_renderer->screen, dstX, dstY, srcX, srcY, width, height);
}

void ClearSecondary(void)
{
    if (!g_wasm_renderer || !g_wasm_renderer->screen) return;
    surface_fill_rgba(g_wasm_renderer->screen, 0, 0, 0, 255);
}

void CreateSurface(int bmpindex, int size_x, int size_y)
{
    if (!g_wasm_renderer || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) return;
    ReleaseBitmap(bmpindex);
    g_wasm_renderer->bitmap[bmpindex] = surface_create(size_x, size_y);
}

void SwapToSecondary(int bmpindex)
{
    WasmCanvasSurface *src;
    if (!g_wasm_renderer || bmpindex < 0 || bmpindex >= BMPBUFF_MAX) return;
    src = g_wasm_renderer->bitmap[bmpindex];
    if (!src || !g_wasm_renderer->screen) return;
    blit_surface(g_wasm_renderer->screen, src, 0, 0, 0, 0, src->w, src->h);
}

void SaveBmp(int bmpindex, char *fn)
{
    (void)bmpindex;
    (void)fn;
}

void Renderer_SetPalette(void* surface)
{
    (void)surface;
}

void SetGscreenPalette(void* surface)
{
    (void)surface;
}

uint32_t Renderer_GetTicks(void)
{
    return (uint32_t)gns_now_ms();
}

void Renderer_Delay(uint32_t ms)
{
(void)ms;
}

int Renderer_PollEvent(void* event)
{
    (void)event;
    return 0;
}

void* Renderer_GetScreen(void)
{
    return g_wasm_renderer ? g_wasm_renderer->screen : NULL;
}

void* Renderer_LoadSurface(const char* path)
{
    return load_bmp_rgba(path, 0);
}

void Renderer_FreeSurface(void* surface)
{
    surface_free((WasmCanvasSurface*)surface);
}
