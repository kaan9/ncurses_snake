#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "config.h"
#include "curse.h"

unsigned int score = 0;

char grid[WIDTH][HEIGHT];

int main(int argc, char ** argv)
{
	curse_init();

	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			grid[i][j] = BACKG_N;

	grid[5][5] = FOOD_N;
	grid[6][5] = FOOD_N;
	int i = 6;
	grid[7][3] = SNAKE_N;

	for (int key; (key = curse_timed_key());) {
		if (key == 'q')
			break;
		if (key != ERR)
			score += key;
		grid[++i][5] = FOOD_N;
		grid[i - 2][5] = BACKG_N;
		curse_update(score, &grid);
	}

	curse_term();

	return 0;
}
