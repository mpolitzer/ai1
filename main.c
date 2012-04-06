#include <stdio.h>

#include "game.h"
#include "astar.h"

int main(int argc, const char *argv[])
{
	PathCost *goal, *ptr;
	float distance;

	game_read();
	// print_map();

	goal = a_star_search(G.gi.start, G.gi.end, &distance);
	printf("Distance: %f\n", distance);
	goal = a_star_search(G.gi.end, G.gi.start, &distance);
	printf("Distance: %f\n", distance);

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
	return 0;
}
