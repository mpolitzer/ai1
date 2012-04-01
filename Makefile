CC=gcc

CFLAGS=-Wall -O2 -funroll-loops -c
LDFLAGS=-O2
LDLIBS=-lm
SOURCES=main.c
HEADERS=game.h

ifeq ($(ALLEGRO),1)
SOURCES+=game.c
CFLAGS+=-DALLEGRO $(pkg-config allegro-5.0 allegro_primitives-5.0 --cflags)
LDLIBS+=`pkg-config allegro-5.0 allegro_primitives-5.0 --libs`
endif

OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))
EXECUTABLE=main
EXECUTABLE_allegro=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)

$(OBJECTS): %.o: %.c $(HEADERS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: all clean
