
#include<stdlib.h>

#include "heap.h"
#include "game.h"

typedef struct path_cost
{
	int pos[2];
	int cost;
	int distance;
} PathCost;

int compare_path_cost(const void *a, const void *b)
{
	PathCost *A = (PathCost*)a;
	PathCost *B = (PathCost*)B;

	return A->cost - B->cost;
}

PathCost* create_path_cost(int pos[], int cost, int distance)
{
	PathCost* path = (PathCost*)malloc(sizeof(PathCost));

	path->pos[0] = pos[0];
	path->pos[1] = pos[1];
	path->cost = cost;
	path->distance = distance;

	return path;
}

int heuristic(int pos1[], int pos2[])
{
	return abs(pos1[0]-pos2[0]) + abs(pos1[1]-pos2[1]); 
}

int a_star_search(int init[2], int goal[2])
{
	/* current position variable */
	PathCost *curr = create_path_cost(init, 0);

	/* pointer to map_size */
	int *map_size = G.gi.map_size, *mapw = G.gi.mapw;

	/* heap definition */
	Heap *heap = heap_cria(map_size[0]*map_size[1], compare_path_cost);

	/* map of path_cost */	
	PathCost **_map = (PathCost**) calloc(map_size[0]*map_size[1], sizeof(PathCost*));

	/* directional vector */
	int dir[4][2] = {{-1,0}, {0, -1}, {1, 0}, {0, 1}};

	/* other variables */
	int i, distance, map_index;

	while(curr && (curr->pos[0] != goal[0] || curr->pos[1] != goal[1]))
	{
		for(i = 0; i < 4; i++)
		{
			PathCost *path;
			int h, c, d;

			/* look to the neighbors */
			int new_pos[2];
			new_pos[0] = curr->pos[0] + dir[i][0];
			new_pos[1] = curr->pos[1] + dir[i][1];

			map_index = new_pos[0] + new_pos[1] * map_size[0];

			/* skip invalid positions */
			if(new_pos[0] < 0 || new_pos[0] >= map_size[0]) continue;
			if(new_pos[1] < 0 || new_pos[1] >= map_size[1]) continue;

			/* this way has a wall */
			if(mapw[map_index] == -1) continue;

			/* calc distance total cost and measure distance using heuristic */
			d = curr->distance + mapw[map_index];
			h = heuristic(new_pos, goal);
			c = curr->cost + mapw[map_index] + h;

			/* check if that path cost already exists */
			if(_map[map_index])
			{
				path = _map[map_index];

				/* update cost */
				if(path->cost > c) 
				{
					path->cost = c;

					/* BUG: se o PathCost já estiver na heap vai dar merda!!! */
					/* Talvez seja melhor usar lista de adjacencia mesmo... */

					/* insert updated path in heap */
					heap_insere(heap, path);
				}
			}
			else 
			{
				path = create_path_cost(new_pos, c, d);

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
	for(i = 0; i < map_size[0]*map_size[1]; i++)
	{
		if(_map[i]) 
		{
			free(_map[i]);
		}
	}
	free(_map);

	return distance;
}
