CC = gcc
PKG_CONFIG = pkg-config

OUTDIR = build
PROJ = c-sdl-sample
EXE = $(OUTDIR)/$(PROJ)

CFLAGS = -std=c11 -Wall -Wextra -O2
LDFLAGS = 

ifdef EMSCRIPTEN
	CC = emcc
	EXE = $(OUTDIR)/$(PROJ).html
	
	USES = -sUSE_SDL=2 -sUSE_SDL_IMAGE=2

	CFLAGS += $(USES)
	LDFLAGS += $(USES) -sWASM=1 -sSDL2_IMAGE_FORMATS='["png"]' --preload-file assets
else
	DEPS = sdl2 SDL2_image

	CFLAGS += `$(PKG_CONFIG) --cflags $(DEPS)`
	LDFLAGS += `$(PKG_CONFIG) --libs $(DEPS)`
endif 

SRC = main.c
OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

$(EXE): $(OBJ)
	mkdir -p $(OUTDIR)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OUTDIR)
	rm -f $(OBJ)

.PHONY: clean
