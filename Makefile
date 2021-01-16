CC = gcc

CFLAGS  = -Os -g3 -std=gnu90 -fomit-frame-pointer -DCLASSICMAC
LDFLAGS = -lSDL -lSDL_mixer -lm
OBJS  = src/ram.o src/util_snd.o src/refresh.o src/function.o src/dconv.o src/scene.o src/title.o src/init.o src/act.o src/option.o src/ending.o src/logo.o
SOURCES = $(patsubst %.o, %.c, $(OBJS))

OUTPUT= gnp

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm $(OBJS)
