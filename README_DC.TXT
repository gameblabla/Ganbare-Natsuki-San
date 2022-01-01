Ganbare Natsuki-San Dreamcast port
by gameblabla v1.2
====================================

This is a port of a Japanese game that was first released for the GP2X
before it got ported to other consoles like the PSP and an official X360 port.
I took over the code but mostly to port it to other platforms like OpenDingux devices
and now, the Sega Dreamcast.

It pretty much plays like Bionic Commando, albeit without enemies
aside from spiked balls and other obstacles.
Grab all of the items in a level before you can proceed to the next level !

This port was in the works for almost a year and could only be completed now
due to some technical issues that i was not aware of + toolchain improvements
that were well needed.

This port supports the VMU but only in A1 slot.
Music streaming isn't perfect : it works but i had to force it to pause
in between levels otherwise it would just poop itself and crash.

For sound, ADPCM 4-bits sound samples are used and they sound decent enough in my opinion
but they provide double the savings over 8-bits PCM.

For music, i was experimenting with OPUS and CDDA before sticking with ADX,
as it's robust enough as long as you take care of the issues related to CDROM access.


Changelog
v1.2:
- Fix issue with new music between stages not being played. (instead always playing the same tune)
- Now using stripped down SDL version.
- Switching to latest KOS with SPU DMA for ADX streaming.
- Fix up keyboard support properly. (and fix mapping)
