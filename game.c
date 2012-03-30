#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "game.h"

#define die(error, ...) _die(error, __FILE__, __LINE__, __VA_ARGS__)

void _die(int error, char* filename, int line, const char* format, ...)
{
	va_list args;

	fprintf(stderr, "[%s:%d] ", filename, line);
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(error);
}

void game_init(int w, int h, int fps)
{
	if (!al_init())
		die(1, "can't init allegro\n");
	if (!(G.display = al_create_display(w, h)))
		die(2, "can't create display");
	if (!(G.tick = al_create_timer(1.0 / fps)))
		die(3, "invalid fps");
}

void game_loop(void)
{
	int done = 0;
	while (!done){
		ALLEGRO_EVENT ev;
		al_wait_for_event(G.ev, &ev);
		game_render();
	}
}

void game_render(void)
{
	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
}

void game_end(void)
{
	al_destroy_display(G.display);
	al_destroy_timer(G.tick);
	al_destroy_event_queue(G.ev);
}
