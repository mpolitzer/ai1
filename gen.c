#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "heap.h"
#include "game.h"
#include "gen.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define NUM_ELEMENTS 300

typedef struct {
	int *vet;
	float cost;
} ga_state;

static ga_state _elements[NUM_ELEMENTS];

static inline void mutation(int *state);
static inline void recomb(int *son, int *father1, int *father2);
static inline void create_random_element(ga_state *c);
static inline void ga_evolve(void);
static inline void ga_update_cost(ga_state *state);
static inline void ga_dump_vet(int *vet, int sz);
static inline void ga_assert_element(ga_state *state, char *str);

static inline void ga_dump_vet(int *vet, int sz)
{
	int i;

	for (i = 0; i < sz; i++) {
		printf("%d ", vet[i]);
	}
	printf("\n");
}

static inline void mutation(int *state)
{
	int num_prizes = G.gi.num_prizes;
	int i = (rand() % num_prizes) + 1;
	int j = (rand() % num_prizes) + 1;

	/* 0.01 chance of happening. */
	if ((rand() % 10) == 0) {
		int aux = state[i];
		state[i] = state[j];
		state[j] = aux;
	}
}

static inline void recomb(int *son_, int *father1, int *father2)
{
	int i;
	int n;
	int num_prizes = G.gi.num_prizes;
	int swap_points = rand();
	int tainted[num_prizes+1];
	int order[num_prizes];
	int son[num_prizes];

	memset(tainted, 0, sizeof(int)*(num_prizes+1));

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
	for (i=0; i<num_prizes+2; i++)
		son_[i] = son[i];
}

static inline void create_random_element(ga_state *c)
{
	int i;
	int num_rand_op = 100;
	int num_prizes = G.gi.num_prizes;

	for (i=0; i<num_prizes+2; i++) c->vet[i] = i;
	for (i = 0; i < num_rand_op; i++) mutation(c->vet);
	ga_update_cost(c);
}

static inline void ga_assert_element(ga_state *state, char *str)
{
	int i, j, k;

	for (i=0, j=0, k=0; i < G.gi.num_prizes+2; i++) {
		j += i;
		k += state->vet[i];
	}

	if (j != k) {
		for (i=0; i < G.gi.num_prizes+2; i++)
			printf("%d ", state->vet[i]);
		printf("\n");
		printf("%s j=%d k=%d\n", str, j, k);
	}
	assert(j == k);
}

static inline void ga_update_cost(ga_state *state)
{
	int i;

	state->cost=0;
	for (i=0; i < G.gi.num_prizes+1; i++) {
		state->cost += graph[mkcostidx(state->vet[i], state->vet[i+1])].cost;
	}
	assert(state->cost > 0);
}

int ga_compare(const void *_v1, const void *_v2)
{
	ga_state *v1 = (ga_state *)_v1;
	ga_state *v2 = (ga_state *)_v2;

	return v1->cost > v2->cost;
}

static inline void ga_evolve_step(int i)
{
	recomb(_elements[NUM_ELEMENTS-i-1].vet,
			_elements[i].vet,
			_elements[i+1].vet);
	ga_update_cost(&_elements[NUM_ELEMENTS-i-1]);

	mutation(_elements[i].vet);
	ga_update_cost(&_elements[i]);

	mutation(_elements[i+1].vet);
	ga_update_cost(&_elements[i+1]);
}

static inline void ga_evolve(void)
{
	int i;
	qsort(_elements, NUM_ELEMENTS, sizeof(ga_state), ga_compare);
	for (i = 0; i < NUM_ELEMENTS/3; i++) {
		ga_evolve_step(i);
	}
}

int *ga_solve_tsp(int num_iterations, float *cost)
{
	float min;
	int min_idx = 0;
	int i;

	for (i=0; i<NUM_ELEMENTS; i++)
		_elements[i].vet = malloc(sizeof(int) * (G.gi.num_prizes+2));

	/* initialize the elements. */
	for (i=0; i<NUM_ELEMENTS; i++) {
		create_random_element(&_elements[i]);
		ga_update_cost(&_elements[i]);
	}

	for (i=0; i<num_iterations; i++) ga_evolve();

	for (min=1000000, i=0; i<NUM_ELEMENTS; i++) {
		if (min < _elements[i].cost) {
			min = _elements[i].cost;
			min_idx = i;
		}
	}
	*cost = _elements[min_idx].cost;
	return _elements[min_idx].vet;
}
