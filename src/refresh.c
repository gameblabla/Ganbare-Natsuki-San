#include "define.h"
#ifdef SCALING
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "refresh.h"

extern SDL_Surface *g_screen;

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

/* Achieves a scanlike-effect. Not bad but not the intended look */
static void bitmap_scale(uint32_t startx, uint32_t starty, uint32_t viswidth, uint32_t visheight, uint32_t newwidth, uint32_t newheight,uint32_t pitchsrc,uint32_t pitchdest, uint8_t* restrict src, uint8_t* restrict dst)
{
    uint32_t W,H,ix,iy,x,y;
    x=startx<<16;
    y=starty<<16;
    W=newwidth;
    H=newheight/2;
    ix=(viswidth<<16)/W;
    iy=(visheight<<16)/H;

    do 
    {
        uint8_t* restrict buffer_mem=&src[(y>>16)*pitchsrc];
        W=newwidth; x=startx<<16;
        do 
        {
            *dst++=buffer_mem[x>>16];
            x+=ix;
        } while (--W);
        dst+=pitchdest;
        y+=iy;
    } while (--H);
}

void RefreshScreen(SDL_Surface* tmp)
{
	if (real_screen->w == 640 && real_screen->h == 480)
	{
		Simple2x((unsigned char*)g_screen->pixels, g_screen->pitch, (unsigned char*)real_screen->pixels, real_screen->pitch, 320, 240);
	}
	else
	{
		SDL_SoftStretch(g_screen,NULL,real_screen,NULL);
	}
	SDL_Flip(real_screen);
}

#endif
