#include "game.h"

#include <stdlib.h>
#include <string.h>

#define NUM_ELEMENTS 20

typedef struct {
	int *vet;
	float cost;
} ga_state;

static ga_state _elements[NUM_ELEMENTS];

static inline void mutation(int *state)
{
	int aux;
	int num_prizes = G.gi.num_prizes;
	int i = (rand() % num_prizes) + 1;
	int j = (rand() % num_prizes) + 1;

	aux = state[i];
	state[i] = state[j];
	state[j] = aux;
}

static inline void recomb(int *son, int *father1, int *father2)
{
	int i;
	int n;
	int num_prizes = G.gi.num_prizes;
	int swap_points = rand();
	int tainted[num_prizes+1];
	int order[num_prizes];

	memset(tainted, 0, sizeof(int)*num_prizes);

	son[0] = 0;
	son[num_prizes+1] = num_prizes+1;

	for (i=1; i<num_prizes+1; i++) {
		if (swap_points & (1 << i)) {
			son[i] = father1[i];
		} else {
			tainted[father1[i]] = 1;
		}
	}
	for (n=0, i=1; i<num_prizes+1; i++) {
		if (tainted[father2[i]]) {
			order[n++] = father2[i];
		}
	}
	for (n=0, i=1; i<num_prizes+1; i++) {
		if (!(swap_points & (1 << i))) {
			son[i] = order[n++];
		}
	}
}

void create_random_element(ga_state *c)
{
	int i;
	int num_rand_op = 100;
	int num_prizes = G.gi.num_prizes;
	int orig[num_prizes];

	for (i=0; i<num_prizes; i++) orig[i] = i;
	for (i = 0; i < num_rand_op; i++) mutation(orig);
}

void ga_select(void)
{

}

void ga_calc_cost(ga_state *state)
{
	int i;

	state->cost = 0;
	for (i = 0; i < G.gi.num_prizes+1; i++) {
		state->cost += _cost[mkcostidx(state->vet[i], state->vet[i+1])];
	}
}

void ga_calc(void)
{
	int i;

	/* initialize it. */
	for (i = 0; i < NUM_ELEMENTS; i++) {
		create_random_element(&_elements[i]);
		ga_calc_cost(&_elements[i]);
	}
}
