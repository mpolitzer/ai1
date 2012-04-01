#ifndef _HEAP
#define _HEAP

typedef struct _heap Heap;

int heap_qtd(Heap* heap);

Heap* heap_cria(int max, int (*compare)(const void*, const void*));

void heap_insere(Heap* heap, void* info);

void* heap_remove(Heap* heap);

void heap_libera(Heap *heap, int liberaInfo);

#endif
