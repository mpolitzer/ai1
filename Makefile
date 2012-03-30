CC=gcc
CFLAGS=-O2 -funroll-loops -c $(pkg-config allegro-5.0 --cflags)
LDFLAGS=-O2
LDLIBS=-lm `pkg-config allegro-5.0 --libs`
SOURCES=main.c game.c
HEADERS=game.h
OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))
EXECUTABLE=main

all: $(EXECUTABLE)

zip: Makefile $(SOURCES) $(HEADERS)
	zip -r mypackage.zip $^

$(EXECUTABLE): $(OBJECTS)

$(OBJECTS): %.o: %.c $(HEADERS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: all clean
