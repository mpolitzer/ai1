
#include<stdlib.h>
#include<stdio.h>

#include "astar.h"
#include "heap.h"
#include "game.h"

int compare_path_cost(const void *a, const void *b)
{
	PathCost *A = (PathCost*)a;
	PathCost *B = (PathCost*)b;

	return B->cost - A->cost;
}

static inline PathCost *create_path_cost(int pos[], float cost, float distance, PathCost *previous)
{
	PathCost *path = (PathCost*) malloc(sizeof(PathCost));

	path->pos[0] = pos[0];
	path->pos[1] = pos[1];
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

PathCost *a_star_search(int init[2], int goal[2], float *_distance)
{
	/* current position variable */
	PathCost *aux, *curr = create_path_cost(init, 0, 0, NULL);

	/* pointer to map_size */
	int *map_size = G.gi.map_size, *mapw = G.gi.mapw;

	/* heap definition */
	Heap *heap = heap_cria(map_size[0]*map_size[1], compare_path_cost);

	/* map of path_cost */
	PathCost **_map = (PathCost**) calloc(map_size[0]*map_size[1], sizeof(PathCost*));

	/* directional vector */
	int dir[4][2] = {{-1,0}, {0, -1}, {1, 0}, {0, 1}};

	/* other variables */
	int i, map_index;
	float distance;

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
			c = d + manhattan_distance(new_pos, goal);

			/* check if that path cost already exists */
			if(_map[map_index]) {
				PathCost *path;
				path = _map[map_index];

				/* update cost */
				if(path->cost > c) {
					path->cost = c;
					path->distance = d;
					path->prev = curr;

					/* BUG: se o PathCost já estiver na heap vai dar merda!!! */
					/* Talvez seja melhor usar lista de adjacencia mesmo... */

					/* insert updated path in heap */
					heap_insere(heap, path);
				}
			} else {
				PathCost *path;

				/* creating struct path_cost */
				path = create_path_cost(new_pos, c, d, curr);

				_map[map_index] = path;

				/* insert created path in heap */
				heap_insere(heap, path);
			}
		}

		/* pop next position from heap */
		curr = heap_remove(heap);
	}

	if(curr) distance = curr->distance;
	else distance = -1;

	heap_libera(heap, 0);

	/* steal the nodes we'll need from _map */
	for(aux=NULL; curr; curr=curr->prev) {
		curr->next = aux;
		aux = curr;
		_map[aux->pos[0] + aux->pos[1] * map_size[0]] = NULL;


	}

	for(i = 0; i < map_size[0]*map_size[1]; i++) {
		if(_map[i]) {
			free(_map[i]);
		}
	}
	free(_map);
	*_distance = distance;
	return aux;
}
