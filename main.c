#include <stdio.h>

#include "game.h"
#include "astar.h"

#define INF	0x3F3F3F3F
#define NINF	0xC0C0C0C0
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

/* matrix is cost from prize to prize, from start to prizes and prizes to end.
 * index 0: start
 * 1 .. num_prizes: prizes
 * index num_prizes+1: end
 */
float *_cost;
PathCost *_path;

void calc_cost_matrix(void)
{
	int i,j;
	PathCost *goal;
	float distance;

	_cost = malloc(sizeof(float) * (G.gi.num_prizes+2) * (G.gi.num_prizes+2));
	_path = malloc(sizeof(PathCost) * (G.gi.num_prizes+2) * (G.gi.num_prizes+2));
	memset(_cost, -1, sizeof(float) * (G.gi.num_prizes+2) * (G.gi.num_prizes+2));

	for (i=0; i<G.gi.num_prizes+2; i++)
		_cost[mkcostidx(i,i)] = 0;

	for (i=0; i<G.gi.num_prizes; i++) {
		goal = a_star_search(G.gi.start, G.gi.prizes[i], &distance);
		_cost[mkcostidx(0, i+1)] = distance;
		_cost[mkcostidx(i+1, 0)] = distance;
	}

	for (i=0; i<G.gi.num_prizes; i++) {
		for (j=0; j<i; j++) {
			goal = a_star_search(G.gi.prizes[i], G.gi.prizes[j], &distance);
			_cost[mkcostidx(i+1, j+1)] = distance;
			_cost[mkcostidx(j+1, i+1)] = distance;
		}
	}
	for (i=0; i<G.gi.num_prizes; i++) {
			goal = a_star_search(G.gi.prizes[i], G.gi.end, &distance);
			_cost[mkcostidx(i+1, j+1)] = distance;
			_cost[mkcostidx(j+1, i+1)] = distance;
	}

	for (i=0; i<G.gi.num_prizes; i++) {
		for (j=0; j<G.gi.num_prizes; j++) {
			printf("% 7.1f", _cost[mkcostidx(i, j)]);
		}
		printf("\n");
	}
}

int main(int argc, const char *argv[])
{
	PathCost *goal, *ptr;
	float distance;

	game_read();
	// print_map();
	calc_cost_matrix();
	goal = a_star_search(G.gi.start, G.gi.end, &distance);

#if defined(ALLEGRO)
	gfx_init(600, 600, 10);
	for (ptr=goal; ptr; ptr = ptr->next) {
		printf("%f (%d,%d)\n", ptr->cost, ptr->pos[0], ptr->pos[1]);
	}

	while (1) {
		for (ptr=goal; ptr->next; ptr = ptr->next) {
			G.gi.cur[0] = ptr->pos[0];
			G.gi.cur[1] = ptr->pos[1];
			gfx_step();
		}
		for (; ptr->prev; ptr = ptr->prev) {
			G.gi.cur[0] = ptr->pos[0];
			G.gi.cur[1] = ptr->pos[1];
			gfx_step();
		}
	}
	gfx_end();
#else
	for (ptr=goal; ptr->prev; ptr = ptr->prev) {
		printf("%f (%d,%d)\n", ptr->cost, ptr->pos[0], ptr->pos[1]);
	}
	for (; ptr; ptr = ptr->next) {
		printf("%f (%d,%d)\n", ptr->cost, ptr->pos[0], ptr->pos[1]);
	}
#endif
	free(_cost);
	return 0;
}
