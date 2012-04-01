
#include "game.h"
#include "heap.h"

typedef struct path_cost
{
	int pos[2];
	int cost;
} PathCost;

int heuristic(int pos1[], int pos2[])
{
	return abs(pos1[0]-pos2[0]) + abs(pos1[1]-pos2[1]); 
}

int compare_path_cost(const void *a, const void *b)
{
	PathCost A = (PathCost*)a;
	PathCost B = (PathCost*)B;

	return A.cost - B.cost;
}

PathCost* create_path_cost(int pos[], int cost)
{
	PathCost* path = (PathCost*)malloc(sizeof(PathCost));

	path.pos[0] = pos[0];
	path.pos[1] = pos[1];
	path.cost = cost;

	return path;
}

int a_star_search(int init[2], int goal[2])
{
	/* heap definition */
	Heap *heap = heap_cria(G.gi.map_size[0]*G.gi.map_size[1], compare_path_cost);

	/* map of path_cost */	
	PathCost *_map = (PathCost*) calloc(G.gi.map_size[0]*G.gi.map_size[1], sizeof(PathCost));

	/* directional vector */
	int dir[4][2] = {{-1,0}, {0, -1}, {1, 0}, {0, 1}};

	/* other variables */
	int i, distance;

	/* current position variable */
	int curr_pos[2];
	curr_pos[0] = init[0];
	curr_pos[1] = init[1];

	while(curr_pos[0] != goal[0] || curr_pos[1] != goal[1])
	{
		for(i = 0; i < 4; i++)
		{
			PathCost *newPath;

			/* look to the neighbors */
			int new_pos[2], map_index;
			new_pos[0] = curr_pos[0] + dir[i][0];
			new_pos[1] = curr_pos[1] + dir[i][1];

			/* skip invalid positions */
			if(new_pos[0] < 0 || new_pos[0] >= G.gi.map_size[0]) continue;
			if(new_pos[1] < 0 || new_pos[1] >= G.gi.map_size[1]) continue;

			/* get the position cost */
			/* measure distance to goal using heuristic */
			/* look at _map to see if its PathCost was created already */
			/* if dont, create PathCost to this position */
			/* if so, update its cost information */
			/* insert this information in heap */
		}

		/* pop from the heap the new position to analyse */
	}
	// map_index = ??? ;
	distance = (_map[map_index]).cost;

	heap_libera(heap, 1);
	free(_map);

	return distance;
}
