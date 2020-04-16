#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "curse.h"

typedef enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3 } SNAKE_DIR;

struct point {
	unsigned char x;
	unsigned char y;
};

struct snake_t {
	struct point head;
	struct point tail;
};

unsigned int score = 0;

/*
 * last two bits of each char for type
 * previous two bits for next direction
 * (only valid for SNAKE_N positions)
 */
unsigned char grid[WIDTH][HEIGHT];

struct snake_t snake = { { WIDTH / 2, HEIGHT / 2 },
			 { WIDTH / 2 - 2, HEIGHT / 2 } };

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

	grid[snake.head.x][snake.head.y] = (RIGHT << 2) | SNAKE_N;
	grid[snake.head.x - 1][snake.tail.y] = (RIGHT << 2) | SNAKE_N;
	grid[snake.head.x - 2][snake.tail.y] = (RIGHT << 2) | BACKG_N;

	srandom(time(NULL));

	spawn_food();

	curse_update(score, &grid);
}

SNAKE_DIR point_dir(struct point p)
{
	return (grid[p.x][p.y] >> 2) & 3;
}

int point_type(struct point p)
{
	return grid[p.x][p.y] & 3;
}

void color_point(struct point p, SNAKE_DIR d, int type)
{
	grid[p.x][p.y] = (d << 2) | type;
}

struct point next_point(SNAKE_DIR d, struct point p)
{
	switch (d) {
	case LEFT:
		p.x--;
		break;
	case RIGHT:
		p.x++;
		break;
	case UP:
		p.y--;
		break;
	case DOWN:
		p.y++;
	}
	return p;
}

int point_invalid(struct point p)
{
	return (p.x < 0 || p.x >= WIDTH || p.y < 0 || p.y >= HEIGHT);
}

SNAKE_DIR snake_next_dir(int key)
{
	SNAKE_DIR d = point_dir(snake.head);
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


void move_tail()
{
	color_point(snake.tail, point_dir(snake.tail), BACKG_N);
	snake.tail = next_point(point_dir(snake.tail), snake.tail);
}

int move_head(SNAKE_DIR d)
{
	int status = 0;
	color_point(snake.head, d, SNAKE_N);
	snake.head = next_point(d, snake.head);
	if (point_invalid(snake.head)) return -1;
	switch(point_type(snake.head)) {
	case SNAKE_N:
		return -2;
	case FOOD_N:
		status = 1;
		spawn_food();
	default:
		color_point(snake.head, d, SNAKE_N);
	}
	return status;
}

void game_loop(void)
{
	for (int key = curse_timed_key(); key != 'q'; key = curse_timed_key()) {
		switch(move_head(snake_next_dir(key))) {
		case -2:
		case -1:
			return;
		case 0:
			move_tail();
			break;
		case 1:
			score++;
		}
		curse_update(score, &grid);
	}
}

int main(int argc, char ** argv)
{
	init();

	sleep(3);

	game_loop();

	curse_gameover(score);

	while (curse_timed_key() == ERR);

	curse_term();

	return 0;
}
