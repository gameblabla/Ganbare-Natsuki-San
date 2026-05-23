Ganbare Natsuki-San
==========================

Ganbare Natsuki-San is a game by Alpha Secret Base (now inactive).

It was officially ported to Windows, Debian (as a binary), Mac OSX, GP2X,
as well as a proprietary port for Xbox 360.

Unofficially, it was ported to the Sony PSP, Wiz, Caanoo, Dingoo and OpenPandora.
My version is based on the (partially) translated PSP version.

This source code builds and works for the GCW-Zero, RS-97, and the new Bittboy.
Other platforms are planned too.

Some bugs were fixed as well, with the aim of making the game portable.
Here are some of the changes :
  * Fixed support for 64-bits platforms
  * Loads 32-bitsz backgrounds instead of the 8-bits ones. (An error in the source code)


SDL3 desktop build
==================

This branch includes a separate SDL3 code path alongside the existing SDL 1.2 and Dreamcast paths. Build it with:

    make -f Makefile.SDL3

Run it from the source tree with the widescreen assets:

    make -f Makefile.SDL3 run

The SDL3 backend renders the existing software framebuffer through a streaming texture, keeps the 400x240 widescreen coordinate system when WIDESCREEN is enabled, and letterboxes/pillarboxes on resize instead of stretching the image. See SDL3_PORTING.md for details.

LICENSE
========

GNP is licensed under a BSD-like license. See COPYRIGHT file for more information.
