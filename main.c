#include <stdio.h>

#include "game.h"
#include "astar.h"

/* matrix is cost from prize to prize, from start to prizes and prizes to end.
 * index 0: start
 * 1 .. num_prizes: prizes
 * index num_prizes+1: end
 */
float *_cost;
PathCost *_path;

void swap_pos(int *pos1, int *pos2)
{
	int tmp;

	tmp = pos1[0];
	pos1[0] = pos2[0];
	pos2[0] = tmp;

	tmp = pos1[1];
	pos1[1] = pos2[1];
	pos2[1] = tmp;
}

void calc_cost_matrix(void)
{
	int i,j;
	PathCost *goal;
	float distance;
	int *start_distance, bkp_num_prizes = G.gi.num_prizes;

	start_distance = malloc(sizeof(float) * (G.gi.num_prizes));

	for (i=0; i<G.gi.num_prizes; i++) {
		goal = a_star_search(G.gi.start, G.gi.prizes[i], &distance);

		/* impossible to get there! */
		if(distance < 0) G.gi.prizes[i][0] = -1;

		start_distance[i] = distance;

	}

	for (i=0; i<G.gi.num_prizes; i++) {
		if(G.gi.prizes[i][0] == -1)
		{
			swap_pos(G.gi.prizes[i], G.gi.prizes[G.gi.num_prizes-1]);
			G.gi.num_prizes--;
			i--;
		}
	}

	_cost = malloc(sizeof(float) * (G.gi.num_prizes+2) * (G.gi.num_prizes+2));
	_path = malloc(sizeof(PathCost) * (G.gi.num_prizes+2) * (G.gi.num_prizes+2));
	memset(_cost, -1, sizeof(float) * (G.gi.num_prizes+2) * (G.gi.num_prizes+2));

	for (i=0; i<G.gi.num_prizes+2; i++)
		_cost[mkcostidx(i,i)] = 0;

	for (i=j=0; i<bkp_num_prizes; i++) {
		if(start_distance[i] >= 0)
		{
			_cost[mkcostidx(0, j+1)] = start_distance[i];
			_cost[mkcostidx(j+1, 0)] = start_distance[i];
			j++;
		}
	}
	free(start_distance);

	for (i=0; i<G.gi.num_prizes; i++) {
		for (j=0; j<i; j++) {
			goal = a_star_search(G.gi.prizes[i], G.gi.prizes[j], &distance);
			_cost[mkcostidx(i+1, j+1)] = distance;
			_cost[mkcostidx(j+1, i+1)] = distance;
		}
	}

	for (i=0; i<G.gi.num_prizes; i++) {
			goal = a_star_search(G.gi.prizes[i], G.gi.end, &distance);
			_cost[mkcostidx(i+1, G.gi.num_prizes+1)] = distance;
			_cost[mkcostidx(G.gi.num_prizes+1, i+1)] = distance;
	}

	for (i=0; i<G.gi.num_prizes+2; i++) {
		for (j=0; j<G.gi.num_prizes+2; j++) {
			printf("% 7.1f", _cost[mkcostidx(i, j)]);
		}
		printf("\n");
	}
}

int main(int argc, const char *argv[])
{
	int i;
	int *path;
	PathCost *goal, *ptr;
	float distance;

	game_read();
	// print_map();
	calc_cost_matrix();
	goal = a_star_search(G.gi.start, G.gi.end, &distance);

#if defined(ALLEGRO)
	gfx_init(600, 600, 10);

	path = ga_solve_tsp(100, &distance);

	for (i = 0; i < G.gi.num_prizes+2; i++) {
		printf(" %d", path[i]);
	}
	printf("\n");

	printf("distance: %f\n", distance);
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
