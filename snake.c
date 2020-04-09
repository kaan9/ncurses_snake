#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "curse.h"

enum snake_dir { LEFT = -1, RIGHT = 1, UP = 2, DOWN = -2 };

struct snake_t {
	enum snake_dir dir;
	int head[2];
	int tail[2];
};

unsigned int score = 0;

char grid[WIDTH][HEIGHT];

struct snake_t snake = { RIGHT,
			 { WIDTH / 2, HEIGHT / 2 },
			 { WIDTH / 2, HEIGHT / 2 } };

void spawn_food(void)
{
	unsigned int r;
	do
		r = random() % (WIDTH * HEIGHT);
	while (grid[r / HEIGHT][r % HEIGHT] == SNAKE_N);

	grid[r / HEIGHT][r % HEIGHT] = FOOD_N;
}

void inc_pos(enum snake_dir d, int * pos)
{
	switch (d) {
	case LEFT:
		pos[0]--;
		break;
	case RIGHT:
		pos[0]++;
		break;
	case DOWN:
		pos[1]--;
		break;
	case UP:
		pos[1]++;
		break;
	}
}

void init(void)
{
	curse_init();

	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			grid[i][j] = BACKG_N;

	srandom(time(NULL));

	spawn_food();
}

void update(void)
{
	curse_update(score, &grid);
}

void game_loop(void)
{
	for (int key; (key = curse_timed_key());) {
		if (key == 'q')
			return;
		enum snake_dir d = snake.dir;
		switch (key) {
		case KEY_LEFT:
			d = d != RIGHT ? LEFT : d;
			break;
		case KEY_RIGHT:
			d = d != LEFT ? RIGHT : d;
			break;
		case KEY_DOWN:
			d = d != UP ? DOWN : d;
			break;
		case KEY_UP:
			d = d != DOWN ? UP : d;
			break;
		}

		int pos[2] = { snake.head[0], snake.head[1] };

		inc_pos(d, &pos);
		
		

	}
}

int main(int argc, char ** argv)
{
	init();

	grid[5][5] = SNAKE_N;
	grid[6][5] = SNAKE_N;
	int i = 6;
	//grid[7][3] = SNAKE_N;
	for (int key; (key = curse_timed_key());) {
		if (key == 'q')
			break;
		if (key != ERR)
			score += key;
		grid[++i][5] = SNAKE_N;
		grid[i - 2][5] = BACKG_N;
		update();
	}

	curse_term();

	return 0;
}
