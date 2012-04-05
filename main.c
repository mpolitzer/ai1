#include <stdio.h>

#include "game.h"
#include "algorithm.h"

int main(int argc, const char *argv[])
{
	PathCost *goal, *ptr;
	int distance;

	game_read();
	// print_map();

	goal = a_star_search(G.gi.start, G.gi.end, &distance);
	printf("Distance: %d\n", distance);

#if defined(ALLEGRO)
	gfx_init(600, 600, 10);
	while (1) {
		for (ptr=goal; ptr; ptr = ptr->prev) {
			G.gi.cur[0] = ptr->pos[0];
			G.gi.cur[1] = ptr->pos[1];
			gfx_step();
		}
	}
	gfx_end();
#else
	for (ptr=goal; ptr; ptr = ptr->prev) {
		printf("(%d,%d)\n", ptr->pos[0], ptr->pos[1]);
	}
#endif
	return 0;
}
