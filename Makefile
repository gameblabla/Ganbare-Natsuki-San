CC := clang
CFLAGS  := -O0 -g3 -std=gnu99 -fomit-frame-pointer -DGCW0 -DMIDI_MUSIC
LDFLAGS := -lSDL -lSDL_mixer
SYSROOT = $(shell $CC $CFLAGS $LDFLAGS --print-sysroot 2)
CFLAGS += `$(SYSROOT)/usr/bin/sdl-config --cflags`

OBJS  = src/ram.o src/util_snd.o src/refresh.o src/function.o src/dconv.o src/scene.o src/title.o src/init.o src/act.o src/option.o src/ending.o src/logo.o
SOURCES = $(patsubst %.o, %.c, $(OBJS))

OUTPUT= gnp

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm $(OBJS)
