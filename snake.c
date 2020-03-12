#include <curses.h>

#include "config.h"
#include "curse.h"

unsigned int score = 0;

char grid[WIDTH][HEIGHT];

int main(int argc, char ** argv)
{
	curse_init();

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			grid[i][j] = 3;
		}
	}

	int x;
	while(x = curse_timed_key()) {
		if (x != ERR) score += x;
		curse_update(score, &grid);
	}

	curse_term();

	return 0;
}
