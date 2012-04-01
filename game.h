#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct game_info {
	int start[2];
	int end[2];
	int cur[2];
	int num_prizes;
	int (*prizes)[2];
	int map_size[2];
	int *mapv;	/* map of indexes */
	int *mapw;	/* map of values */
	char *map;	/* map of characters */
};

struct game {
	int w, h;
	struct game_info gi;
#if defined(ALLEGRO)
	ALLEGRO_DISPLAY *display;

	ALLEGRO_TIMER *tick;
	ALLEGRO_EVENT_QUEUE *ev;
#endif
};

extern struct game G;

void game_init(int w, int h, int fps);
void game_step(void);
void game_loop(void);
void game_end(void);

#endif /* GAME_H */
