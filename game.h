#ifndef GAME_H
#define GAME_H

#if defined(ALLEGRO)
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#endif

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

typedef struct
{
	float cost;
	int steps;
	int *path;
} TSP_Node;


#define INF	0x3F3F3F3F
#define NINF	0xC0C0C0C0
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

#define mkcostidx(x,y) ((x) + (y) * (G.gi.num_prizes+2))

extern struct game G;
extern TSP_Node *graph;

void game_read(void);
void game_print(void);
#if defined(ALLEGRO)
void gfx_init(int w, int h, int fps);
void gfx_step(void);
void gfx_loop(void);
void gfx_end(void);
#endif

#endif /* GAME_H */
