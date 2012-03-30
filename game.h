#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>

struct pos {
	unsigned int x, y;
};

struct game_state {
	struct pos *bonus_pos;
	int bonus_count;
};

struct game_info {
	int start[2];
	int end[2];
	int num_prizes;
	int (*prizes)[2];
	int map_size[2];
	int *mapv;	/* map of values */
	char *map;	/* map of characters */
};

struct game {
	ALLEGRO_DISPLAY *display;

	ALLEGRO_TIMER *tick;
	ALLEGRO_EVENT_QUEUE *ev;
	struct game_info gi;
	struct game_state gs;
};

extern struct game G;

void game_init(int w, int h, int fps);
void game_render(void);
void game_step(void);
void game_end(void);

#endif /* GAME_H */
