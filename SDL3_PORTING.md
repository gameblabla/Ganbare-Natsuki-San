SDL3 Porting Notes
==================

This tree now has a separate SDL3 desktop code path. It does not replace the
existing Dreamcast or SDL 1.2 code paths.

Build
-----

    make -f Makefile.SDL3

The SDL3 build expects pkg-config metadata for SDL3 and, by default, SDL3_mixer. Override the
package names if your distribution uses different names:

    make -f Makefile.SDL3 SDL3_PKG=SDL3 SDL3_MIXER_PKG=sdl3-mixer

To build without SDL3_mixer, use the no-op audio backend:

    make -f Makefile.SDL3 AUDIO_BACKEND=headless

Run from the source tree with the widescreen asset set:

    make -f Makefile.SDL3 run

Or assemble a redistributable directory:

    make -f Makefile.SDL3 package-dir

Backend layout
--------------

The gameplay code still talks to the same narrow renderer/input/audio APIs:

* src/renderer/renderer.h
* src/input/input.h
* src/audio/audio.h

The SDL3 implementation keeps SDL objects inside opaque backend structs in the
backend modules:

* src/renderer/sdl3/renderer_sdl3.c
* src/input/sdl3/input_sdl3.c
* src/audio/sdl3/audio_sdl3.c

This preserves the existing headless build model: core/gameplay code can be
linked with no-op renderer, input, and audio implementations for automated or
non-interactive runs.

Widescreen behavior
-------------------

The SDL3 Makefile defines WIDESCREEN, so the game uses the existing 400x240
coordinate system and the data/wide asset set. The SDL3 renderer presents the
software framebuffer through a streaming texture and computes an aspect-correct
centered destination rectangle for the current window size. That prevents the
400x240 framebuffer from being stretched into an incorrect aspect ratio when the
window is resized.

Legacy paths
------------

The original Makefile remains the SDL 1.2 path. Makefile.dc remains the
Dreamcast path. Their object lists were not redirected through the SDL3 backend.
