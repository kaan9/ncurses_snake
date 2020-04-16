#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "curse.h"

typedef enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3 } snake_dir;

struct snake_t {
	unsigned char head[2];
	unsigned char tail[2];
	snake_dir d;
};

unsigned int score = 0;

/*
 * last two bits of each char for type
 * previous two bits for next direction
 * (only valid for SNAKE_N positions)
 */
unsigned char grid[WIDTH][HEIGHT];

struct snake_t snake = { { WIDTH / 2, HEIGHT / 2 },
			 { WIDTH / 2 - 1, HEIGHT / 2 },
			 RIGHT };

void spawn_food(void)
{
	unsigned int r;
	do
		r = random() % (WIDTH * HEIGHT);
	while (grid[r / HEIGHT][r % HEIGHT] != BACKG_N);

	grid[r / HEIGHT][r % HEIGHT] = FOOD_N;
}

void init(void)
{
	curse_init();

	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			grid[i][j] = BACKG_N;

	grid[snake.head[0]][snake.head[1]] = (RIGHT << 2) | SNAKE_N;
	grid[snake.tail[0]][snake.tail[1]] = (RIGHT << 2) | SNAKE_N;

	srandom(time(NULL));

	spawn_food();
}

int movesquare(snake_dir d, unsigned char * x, unsigned char * y,
	       unsigned char tailing)
{
	unsigned char xprev = *x, yprev = *y;
	int status = 0;

	switch (d) {
	case LEFT:
		(*x)--;
		break;
	case RIGHT:
		(*x)++;
		break;
	case UP:
		(*y)--;
		break;
	case DOWN:
	default:
		(*y)++;
	}

	if ((*x) < 0 || (*x) >= WIDTH || (*y) < 0 || (*y) >= HEIGHT)
		return -1;
	if ((grid[*x][*y] & 3) == SNAKE_N)
		return -2;
	if ((grid[*x][*y] & 3) == FOOD_N)
		status = 1;

	grid[*x][*y] = grid[xprev][yprev];

	if (!tailing)
		grid[xprev][yprev] = BACKG_N;
	else
		grid[xprev][yprev] &= ((d << 2) | 3);

	return status;
}

snake_dir get_dir(int key)
{
	//	snake_dir d = (grid[snake.head[0]][snake.head[1]] >> 2) & 3;
	snake_dir d = snake.d;
	switch (key) {
	case KEY_LEFT:
		return (d != RIGHT) ? LEFT : d;
	case KEY_RIGHT:
		return (d != LEFT) ? RIGHT : d;
	case KEY_DOWN:
		return (d != UP) ? DOWN : d;
	case KEY_UP:
		return (d != DOWN) ? UP : d;
	}
	return d;
}

void game_loop(void)
{
	for (int key = curse_timed_key(); key != 'q'; key = curse_timed_key()) {
		snake_dir d = get_dir(key);
		movesquare((grid[snake.tail[0]][snake.tail[1]] >> 2) & 3,
			   &snake.tail[0], &snake.tail[1], 0);
		switch (movesquare(d, &snake.head[0], &snake.head[1], 1)) {
		case -2:
		case -1:
			return;
		case 1:
			score += 100;
			spawn_food();
		}
		snake.d = d;



		curse_update(score, &grid);
	}
}

int main(int argc, char ** argv)
{
	init();

	game_loop();

	curse_term();

	return 0;
}
