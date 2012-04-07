
#include<stdlib.h>
#include<stdio.h>

#include "astar.h"
#include "heap.h"
#include "game.h"

typedef struct path_cost
{
	int pos[2];
	float cost;
	float distance;
	int steps;
	struct path_cost *next, *prev;
} PathCost;

int compare_path_cost(const void *a, const void *b)
{
	PathCost *A = (PathCost*)a;
	PathCost *B = (PathCost*)b;

	return B->cost - A->cost;
}

static inline PathCost *create_path_cost(int pos[], int steps, float cost, float distance, PathCost *previous)
{
	PathCost *path = (PathCost*) malloc(sizeof(PathCost));

	path->pos[0] = pos[0];
	path->pos[1] = pos[1];
	path->steps = steps;
	path->cost = cost;
	path->distance = distance;
	path->prev = previous;

	return path;
}

static inline int manhattan_distance(int begin[2], int end[2])
{
	return abs(begin[0]-end[0]) + abs(begin[1]-end[1]);
}

static inline int pos_isvalid(int pos[2], int map_size[2])
{
	/* skip invalid positions */
	if(pos[0] < 0 || pos[0] >= map_size[0]) return 0;
	if(pos[1] < 0 || pos[1] >= map_size[1]) return 0;
	return 1;
}

int *a_star_search(int init[2], int goal[2], float *_distance, int *_steps)
{
	/* current position variable */
	PathCost *curr = create_path_cost(init, 0, 0, 0, NULL);

	/* pointer to map_size */
	int *map_size = G.gi.map_size, *mapw = G.gi.mapw;

	/* if goal is inside a wall */
	if (mapw[goal[0] + goal[1] * map_size[0]] < 0) {
		*_distance = -1;
		return NULL;
	}

	/* heap definition */
	Heap *heap = heap_cria(map_size[0]*map_size[1], compare_path_cost);

	/* map of path_cost */
	PathCost **_map = (PathCost**) calloc(map_size[0]*map_size[1], sizeof(PathCost*));
	_map[init[0]+init[1]*map_size[0]] = curr;

	/* directional vector */
	int dir[4][2] = {{-1,0}, {0, -1}, {1, 0}, {0, 1}};

	/* other variables */
	int i, map_index;
	int *final_path, final_idx;

	while(curr && (curr->pos[0] != goal[0] || curr->pos[1] != goal[1])) {
		for(i = 0; i < 4; i++) {
			float c, d;

			/* look to the neighbors */
			int new_pos[2];
			int curr_index;
			new_pos[0] = curr->pos[0] + dir[i][0];
			new_pos[1] = curr->pos[1] + dir[i][1];
			if (!pos_isvalid(new_pos, map_size)) continue;

			map_index = new_pos[0] + new_pos[1] * map_size[0];
			curr_index = curr->pos[0] + curr->pos[1] * map_size[0];
			/* this way has a wall */
			if(mapw[map_index] == -1) continue;

			/* calculating total distance */
			d = curr->distance + 0.5*mapw[map_index]
				+ 0.5*mapw[curr_index];

			/* final cost = total distance + heuristic value */
			c = d ;//+ manhattan_distance(new_pos, goal);

			/* check if that path cost already exists */
			if(_map[map_index]) {
				PathCost *path;
				path = _map[map_index];

				/* update cost */
				if(path->cost > c) {
					path->cost = c;
					path->distance = d;
					path->prev = curr;
					path->steps = curr->steps+1;

					/* BUG: se o PathCost já estiver na heap vai dar merda!!! */
					/* Talvez seja melhor usar lista de adjacencia mesmo... */

					/* insert updated path in heap */
					heap_insere(heap, path);
				}
			} else {
				PathCost *path;

				/* creating struct path_cost */
				path = create_path_cost(new_pos, curr->steps+1,c, d, curr);

				_map[map_index] = path;

				/* insert created path in heap */
				heap_insere(heap, path);
			}
		}

		/* pop next position from heap */
		curr = heap_remove(heap);
	}

	if(curr) *_distance = curr->distance;
	else *_distance = -1;

	heap_libera(heap, 0);

	if(curr)
	{
		final_idx = curr->steps;
		*_steps = curr->steps+1;

		final_path = malloc(sizeof(int)*2*(curr->steps+1));
	}

	printf("DEBUG: steps = %d ; final_idx = %d\n", curr->steps, final_idx);

	/* steal the nodes we'll need from _map */
	for(; curr; curr=curr->prev) {
		final_path[2*final_idx + 0] = curr->pos[0];
		final_path[2*final_idx + 1] = curr->pos[1];
		final_idx--;
	}

	for(i = 0; i < map_size[0]*map_size[1]; i++) {
		if(_map[i]) {
			free(_map[i]);
		}
	}
	free(_map);

	return final_path;
}
