#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "game.h"

#define MOVE_FRAMES	10
#define die(error, ...) _die(error, __FILE__, __LINE__, __VA_ARGS__)
#define MIN(x, y) (x < y ? x : y)

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

void gfx_init(int w, int h, int fps)
{
	if (!al_init())
		die(1, "can't init allegro\n");
	if (!(al_init_primitives_addon()))
		die(1, "can't init primitives addon\n");
	if (!(al_install_keyboard()))
		die(1, "can't init keyboard\n");
	if (!(G.display = al_create_display(w, h)))
		die(2, "can't create display");
	if (!(G.ev = al_create_event_queue()))
		die(3, "can't create event queue");
	if (!(G.tick = al_create_timer(1.0 / (fps*MOVE_FRAMES))))
		die(4, "invalid fps");

	al_init_font_addon();	// initialize the font addon
	al_init_ttf_addon();	// initialize the ttf (True Type Font) addon

	G.font = al_load_ttf_font("font.ttf", 10,0 );
	assert(G.font);
	       
	/* current position is the begining */
	G.gi.cur[0] = G.gi.start[0];
	G.gi.cur[1] = G.gi.start[1];

	G.w = w;
	G.h = h;
	al_register_event_source(G.ev, al_get_keyboard_event_source());
	al_register_event_source(G.ev, al_get_display_event_source(G.display));
	al_register_event_source(G.ev, al_get_timer_event_source(G.tick));
	al_start_timer(G.tick);
}

void gfx_render(int step)
{
	static int last[2] = {0.0, 0.0};
	int i, j, scale = MIN(G.w / G.gi.map_size[0], G.h / G.gi.map_size[1]);
	float cur[2];
	ALLEGRO_COLOR colors[] = {
		al_map_rgb(  0,  0,  0),	/* x */
		al_map_rgb(  0,200,  0),	/* p */
		al_map_rgb(  0,150,  0),	/* r */
		al_map_rgb(  0,100,  0),	/* a */
		al_map_rgb(  0, 50,  0),	/* s */
	};

	al_clear_to_color(al_map_rgb(0,0,0));
	/* tiles */
	for (i=0; i<G.gi.map_size[0]; i++) {
		for (j=0; j<G.gi.map_size[1]; j++) {
			int map_index = i + j * G.gi.map_size[1];
			al_draw_filled_rectangle(scale*i, scale*j,
			                         scale*(i + 1), scale*(j + 1),
			                         colors[G.gi.mapv[map_index]]);
		}
	}

	/* prizes */
	for (i=0; i<G.gi.num_prizes_orig; i++) {
		float x = G.gi.prizes[i][0] + 0.5;
		float y = G.gi.prizes[i][1] + 0.5;
		char buff[255];

		if (!G.gi.prizes_visited[i])
			al_draw_filled_triangle(scale*(x), scale*(y-0.3),
					scale*(x+0.3), scale*(y+0.3),
					scale*(x-0.3), scale*(y+0.3),
					al_map_rgb(255, 255, 0));
		else
			al_draw_triangle(scale*(x), scale*(y-0.3),
					scale*(x+0.3), scale*(y+0.3),
					scale*(x-0.3), scale*(y+0.3),
					al_map_rgb(255, 255, 0),
					1.0);
		sprintf(buff, "%d", i+1);
		al_draw_text(G.font, al_map_rgb(0,0,0),
				scale*x, scale*(y-0.1), ALLEGRO_ALIGN_CENTRE, buff);
	}

	/* start */
	al_draw_filled_circle((float)scale * (G.gi.start[0]+0.5),
			(float)scale * (G.gi.start[1]+0.5), scale/3,
	                      al_map_rgb(0, 255, 0));
	/* end */
	al_draw_filled_circle((float)scale * (G.gi.end[0]+0.5),
	                      (float)scale * (G.gi.end[1]+0.5), scale/3,
	                      al_map_rgb(255, 0, 0));

	if (step == MOVE_FRAMES-1) {
		last[0] = G.gi.cur[0];
		last[1] = G.gi.cur[1];
	}

	cur[0] = last[0] + (step * ((float)G.gi.cur[0]-last[0]))/MOVE_FRAMES;
	cur[1] = last[1] + (step * ((float)G.gi.cur[1]-last[1]))/MOVE_FRAMES;

	/* player */
	al_draw_filled_circle((float)scale * (cur[0]+0.5),
	                      (float)scale * (cur[1]+0.5), scale/3,
	                      al_map_rgb(0, 0, 255));
	al_flip_display();
}

void gfx_step(void)
{
	int i;
	static int toogle=0;
	ALLEGRO_EVENT ev;

	for (i = 0; i < MOVE_FRAMES; i++) {
		do {
			al_wait_for_event(G.ev, &ev);
			switch (ev.type) {
			case ALLEGRO_EVENT_KEY_DOWN:
				if (toogle ^= 1) al_stop_timer(G.tick);
				else al_start_timer(G.tick);
				break;
			default:
				break;
			}
		} while (ev.type != ALLEGRO_EVENT_TIMER);
		gfx_render(i);
	}
}

void gfx_end(void)
{
	al_destroy_display(G.display);
	al_destroy_timer(G.tick);
	al_destroy_event_queue(G.ev);
}
