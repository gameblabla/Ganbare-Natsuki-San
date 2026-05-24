CC := gcc
CFLAGS  := -O0 -g3 -std=gnu99 -fomit-frame-pointer -DSDL_AVAILABLE -DWIDESCREEN
CFLAGS += -Isrc
LDFLAGS := -lSDL -lSDL_mixer -lGL -lm
SYSROOT = $(shell $CC $CFLAGS $LDFLAGS --print-sysroot 2)
CFLAGS += `$(SYSROOT)/usr/bin/sdl-config --cflags`

OBJS  = src/ram.o src/refresh.o src/function.o src/dconv.o src/scene.o src/title.o src/init.o src/act.o src/option.o src/ending.o src/logo.o
OBJS += src/audio/generic/audio_sdl.o
OBJS += src/renderer/sdl12generic/renderer_sdl12.o
OBJS += src/input/generic/input_generic.o
OBJS += src/filesystem/generic/filesystem_generic.o
OBJS += src/platform/generic/platform_generic.o
SOURCES = $(patsubst %.o, %.c, $(OBJS))

OUTPUT= gnp

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm $(OBJS)
