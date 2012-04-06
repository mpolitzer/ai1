#ifndef ALGORITHM_H
#define ALGORITHM_H

typedef struct path_cost
{
	int pos[2];
	int cost;
	int distance;
	struct path_cost *next, *prev;
} PathCost;

PathCost *a_star_search(int init[2], int goal[2], int *distance);

#endif /* ALGORITHM_H */
