#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "game.h"

struct game G;

int read_map(void)
{
	int i, j;

	assert(scanf(" %d %d", &G.gi.start[0], &G.gi.start[1]) == 2);
	assert(scanf(" %d %d", &G.gi.end[0], &G.gi.end[1]) == 2);
	assert(scanf(" %d", &G.gi.num_prizes) == 1);

	G.gi.prizes = malloc(sizeof(int) * 2 * G.gi.num_prizes);

	for (i = 0; i < G.gi.num_prizes; i++) 
	{
		int x, y;
		assert(scanf(" %d %d", &x, &y) == 2);

		G.gi.prizes[i][0] = x;
		G.gi.prizes[i][1] = y;
	}

	assert(scanf(" %d %d", &G.gi.map_size[0], &G.gi.map_size[1]) == 2);

	G.gi.map = malloc(sizeof(char) * G.gi.map_size[0] * G.gi.map_size[1]);
	G.gi.mapv = malloc(sizeof(int) * G.gi.map_size[0] * G.gi.map_size[1]);
	G.gi.mapw = malloc(sizeof(int) * G.gi.map_size[0] * G.gi.map_size[1]);

	/* read tile values */
	for (i = 0; i < G.gi.map_size[0]; i++) 
	{
		for (j = 0; j < G.gi.map_size[1]; j++) 
		{
			int map_index = i + j * G.gi.map_size[1];
			char c;
			
			do 
			{
				c = getchar();
			} 
			while (!isalnum(c));

			c = tolower(c);

			G.gi.map[map_index] = c;
			switch (c){
			case 'x':
				G.gi.mapv[map_index] = 0;
				G.gi.mapw[map_index] = -1;
				break;
			case 's':
				G.gi.mapv[map_index] = 1;
				G.gi.mapw[map_index] = 1;
				break;
			case 'a':
				G.gi.mapv[map_index] = 2;
				G.gi.mapw[map_index] = 4;
				break;
			case 'r':
				G.gi.mapv[map_index] = 3;
				G.gi.mapw[map_index] = 10;
				break;
			case 'p':
				G.gi.mapv[map_index] = 4;
				G.gi.mapw[map_index] = 20;
				break;
			}
		}
	}
	return 0;
}

void print_map(void)
{
	int i, j;

	printf("start (%2d,%2d)\n", G.gi.start[0], G.gi.start[1]);
	printf("end (%2d,%2d)\n", G.gi.end[0], G.gi.end[1]);
	printf("num prizes %2d\n", G.gi.num_prizes);

	for (i = 0; i < G.gi.num_prizes; i++)
	{
		printf("\t(%2d,%2d)\n", G.gi.prizes[i][0], G.gi.prizes[i][1]);
	}
	printf("map_size (%2d,%2d)\n", G.gi.map_size[0], G.gi.map_size[1]);

	for (i = 0; i < G.gi.map_size[0]; i++)
	{
		for (j = 0; j < G.gi.map_size[1]; j++)
		{
			int map_index = i + j * G.gi.map_size[1];
			printf("%c", G.gi.map[map_index]);
		}
		printf(" |");
		for (j = 0; j < G.gi.map_size[1]; j++)
		{
			int map_index = i + j * G.gi.map_size[1];
			printf("%3d", G.gi.mapv[map_index]);
		}
		printf("\n");
	}
}

int main(int argc, const char *argv[])
{
	read_map();
	print_map();

	game_init(600, 600, 60);
	game_loop();
	game_end();

	return 0;
}
