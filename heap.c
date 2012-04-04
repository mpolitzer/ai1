#include <stdlib.h>
#include "heap.h"

#define PAI(x) ((x - 1) >> 1)
#define ESQ(x) ((x << 1) + 1)
#define DIR(x) ((x << 1) + 2)

struct _heap {
	int max;
	int pos;
	int (*compare)(const void*, const void*);
	void **vet;
};

static void heap_troca(void** a, void** b)
{
	register void* tmp = *a;
	*a = *b;
	*b = tmp;
}

int heap_qtd(Heap* heap)
{
	if(!heap) return 0;
	else return heap->pos;
}

Heap* heap_cria(int max, int (*compare)(const void*, const void*))
{
	Heap *heap = (Heap*) malloc(sizeof(Heap));
	if(!heap) exit(1);

	heap->max = max;
	heap->pos = 0;
	heap->compare = compare;

	heap->vet = (void**) malloc(max*sizeof(void*));

	return heap;
}

void heap_libera(Heap *heap, int liberaInfo)
{
	if(heap) {
		if(liberaInfo) {
			int i;
			for(i = 0; i < heap->pos; i++) {
				free(heap->vet[i]);
			}
		}

		free(heap->vet);
		free(heap);
	}
}

void heap_insere(Heap* heap, void* info)
{
	int i = heap->pos, p = PAI(i);

	if(!heap) return;
	if(!info) return;

	if(heap->pos >= heap->max) return;

	heap->vet[i] = info;
	heap->pos++;

	while(1) {
		if(p < 0) break;

		if(heap->compare(heap->vet[p], heap->vet[i]) > 0) break;

		heap_troca(&heap->vet[p], &heap->vet[i]);

		i = p;
		p = PAI(i);
	}
}

void* heap_remove(Heap* heap)
{
	int i, e, d, aux;
	void* info;

	if(!heap) return NULL;
	if(!heap->pos) return NULL;

	info = heap->vet[0];

	if(heap->pos > 1) {
		heap_troca(&heap->vet[0], &heap->vet[heap->pos-1]);
	}
	heap->pos--;

	i = aux = 0;
	e = ESQ(i);
	d = DIR(i);
	while(1) {
		if(e < heap->pos && heap->compare(heap->vet[e], heap->vet[aux]) > 0) {
			aux = e;
		}
		if(d < heap->pos && heap->compare(heap->vet[d], heap->vet[aux]) > 0) {
			aux = d;
		}
		if(aux != i) {
			heap_troca(&heap->vet[i], &heap->vet[aux]);
		} else break;

		i = aux;
		e = ESQ(i);
		d = DIR(i);
	}

	return info;
}
