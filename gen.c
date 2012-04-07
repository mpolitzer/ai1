#include "game.h"
#include "gen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define NUM_ELEMENTS 20

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
	int aux;
	int num_prizes = G.gi.num_prizes;
	int i = (rand() % num_prizes) + 1;
	int j = (rand() % num_prizes) + 1;

	/* 10% chance of happening. */
	if ((rand() % 10)) return;

	aux = state[i];
	state[i] = state[j];
	state[j] = aux;
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

static inline void ga_evolve(void)
{
	int son, father1, father2;
	int i, n;
	int v[100];
	double total_cost;
	float max, min;

	for (total_cost=0, i=0; i < NUM_ELEMENTS; i++) {
		total_cost += 1.0/_elements[i].cost;
	}
	for (n=0, max=0, min=10000, i=0; i < NUM_ELEMENTS; i++) {
		double current = (100 * (1.0/_elements[i].cost)) / total_cost;
		int k = current;
#if 0
		printf("total: %lf current: %lf elem: %f 1/elem: %f\n",
				total_cost,
				current,
				_elements[i].cost,
				(1.0/_elements[i].cost)
				);
#endif
		min = MIN(min, current);
		max = MAX(max, current);

		while (k--) {
			v[n++] = i;
		}
	}
	for (; n<100; n++) {
		v[n] = min;
	}
	father1 = v[rand() % 100];
	father2 = v[rand() % 100];
	son = v[rand() % 100];

	recomb(_elements[son].vet,
			_elements[father1].vet,
			_elements[father2].vet);
	mutation(_elements[son].vet);
	ga_update_cost(&_elements[son]);
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

	for (i = 0; i < G.gi.num_prizes+2; i++) {
		printf(" %d", _elements[min_idx].vet[i]);
	}
	printf("\n");

	return _elements[min_idx].vet;
}
