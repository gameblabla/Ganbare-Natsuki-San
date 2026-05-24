/* Minimal SDL 1.2 Cocoa entry point used by Makefile.MacOSX.
 * SDL.h remaps the game's main() to SDL_main() on classic Mac OS X builds;
 * this file provides the real process entry point and forwards to it.
 */
#import <Cocoa/Cocoa.h>

extern int SDL_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    return SDL_main(argc, argv);
}
