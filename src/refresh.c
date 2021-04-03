#include "define.h"
#if defined(SCALING) || defined(OPENGL_SCALING) || defined(RS90)
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#ifdef OPENGL_SCALING
#include <GL/gl.h>
#endif
#include "refresh.h"

extern SDL_Surface *g_screen;
#ifdef RS90
uint8_t drm_palette[3][256];
static uint8_t* dst_yuv[3];
#endif

#if defined(SCALING)
static void Simple2x(unsigned char *srcPtr, unsigned int srcPitch, unsigned char *dstPtr, unsigned int dstPitch, unsigned int width, unsigned int height)
{
	unsigned char *nextLine, *finish;
	nextLine = dstPtr + dstPitch;
	do
	{
		unsigned char *bP = (unsigned char *) srcPtr;
		unsigned char *dP = (unsigned char *) dstPtr;
		unsigned char *nL = (unsigned char *) nextLine;
		unsigned char	 currentPixel;
		finish		 = (unsigned char *) bP + width;
		currentPixel = *bP++;
		do
		{
			unsigned char color = currentPixel;
			*(dP)	  = color;
			*(dP + 1) = color;
			*(nL)	  = color;
			*(nL + 1) = color;
			currentPixel = *bP++;
			dP += 2;
			nL += 2;
		}
		while ((unsigned char *) bP < finish);
		srcPtr	 += srcPitch;
		dstPtr	 += dstPitch << 1;
		nextLine += dstPitch << 1;
	}
	while (--height);
}
#endif

void RefreshScreen(SDL_Surface* tmp)
{
#ifdef OPENGL_SCALING
	const float s_fDrawRate = 1.0f;
	GLuint texture;
	int dx, dy, dw, dh, sx, sy, sw, sh;
	int x, y, w, h;
	float texMinX, texMaxX, texMinY, texMaxY;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGB,
		     g_screen->w, g_screen->h,
		     0,
		     GL_RGB,
		     //GL_UNSIGNED_SHORT_5_6_5,
		     GL_UNSIGNED_SHORT,
		     g_screen->pixels);
		     
	dx = 0;
	dy = 0;
	dw = real_screen->w;
	dh = real_screen->h;
	sx = 0;
	sy = 0;
	sw = g_screen->w;
	sh = g_screen->h;
	
	x = (int)((float)dx * s_fDrawRate);
	y = (int)((float)dy * s_fDrawRate);
	w = (int)((float)(dw + dx) * s_fDrawRate) - x;
	h = (int)((float)(dh + dy) * s_fDrawRate) - y;

	texMinX = (float) sx       / (float)g_screen->w;
	texMaxX = (float)(sx + sw) / (float)g_screen->w;
	texMinY = (float) sy       / (float)g_screen->h;
	texMaxY = (float)(sy + sh) / (float)g_screen->h;

	/* Show the image on the screen */
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y  );
	glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y  );
	glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
	glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
	glEnd();
	
	SDL_GL_SwapBuffers( );
#elif defined(RS90)
	uint_fast8_t plane;
	uint8_t *srcbase = g_screen->pixels;
	dst_yuv[0] = real_screen->pixels;
	dst_yuv[1] = dst_yuv[0] + DISPLY_HEIGHT * real_screen->pitch;
	dst_yuv[2] = dst_yuv[1] + DISPLY_HEIGHT * real_screen->pitch;
    for (plane=0; plane<3; plane++) /* The three Y, U and V planes */
    {
        uint32_t y;
        register uint8_t *pal = drm_palette[plane];
        for (y=0; y < DISPLY_HEIGHT; y++)   /* The number of lines to copy */
        {
            register uint8_t *src = srcbase + (y*DISPLY_WIDTH);
            register uint8_t *end = src + DISPLY_WIDTH;
            register uint32_t *dst = (uint32_t *)&dst_yuv[plane][DISPLY_WIDTH * y];

             __builtin_prefetch(pal, 0, 1 );
             __builtin_prefetch(src, 0, 1 );
             __builtin_prefetch(dst, 1, 0 );

            while (src < end)       /* The actual line data to copy */
            {
                register uint32_t pix;
                pix  = pal[*src++];
                pix |= (pal[*src++])<<8;
                pix |= (pal[*src++])<<16;
                pix |= (pal[*src++])<<24;
                *dst++ = pix;
            }
        }
    }
	
	SDL_Flip(real_screen);
#else
	if (real_screen->w == 640 && real_screen->h == 480)
	{
		Simple2x((unsigned char*)g_screen->pixels, g_screen->pitch, (unsigned char*)real_screen->pixels, real_screen->pitch, 320, 240);
	}
	else
	{
		SDL_SoftStretch(g_screen,NULL,real_screen,NULL);
	}
	SDL_Flip(real_screen);
#endif
}

#endif
