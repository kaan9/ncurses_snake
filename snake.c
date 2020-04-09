#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "curse.h"

typedef enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3 } snake_dir;

struct snake_t {
	int head[2];
	int tail[2];
};

unsigned int score = 0;

/*
 * last two bits of each char for type
 * previous two bits for next direction
 * (only valid for SNAKE_N positions)
 */
char grid[WIDTH][HEIGHT];

struct snake_t snake = { { WIDTH / 2, HEIGHT / 2 }, { WIDTH / 2 - 1, HEIGHT / 2 } };

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

void movetile(snake_dir d, int * pos)
{
	switch (d) {
	case LEFT:
		pos[0]--;
		break;
	case RIGHT:
		pos[0]++;
		break;
	case DOWN:
		pos[1]++;
		break;
	case UP:
		pos[1]--;
		break;
	}
}

snake_dir get_dir(int key)
{
		snake_dir d = grid[snake.head[0]][snake.head[1]] >> 2;
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
	FILE * fuk =  fopen("debug", "w+");
	for (int key = curse_timed_key(); key != 'q'; key = curse_timed_key()) {
		snake_dir d = get_dir(key);

		int head[2] = { snake.head[0], snake.head[1] };
		int tail[2] = {snake.tail[0], snake.tail[1]};

		fprintf(fuk, "%d %d -- %d %d\n", head[0], head[1], tail[0], tail[1]);
		movetile(d, head);
		fprintf(fuk, "%d %d -- %d %d\n", head[0], head[1], tail[0], tail[1]);

		if (head[0] < 0 || head[0] >= WIDTH || head[1] < 0 ||
		    head[1] >= HEIGHT)
			break;

		movetile(grid[tail[0]][tail[1]] >> 2, tail);


		switch (grid[head[0]][head[1]] & 0x3) {
		case SNAKE_N:
			printf("snake\n");
			return;
		case BACKG_N:
			printf("backg\n");
			grid[snake.tail[0]][snake.tail[1]] = BACKG_N;
			
			snake.tail[0] = tail[0];
			snake.tail[1] = tail[1];
		case FOOD_N:
			printf("food\n");
			grid[head[0]][head[1]] = (d << 2) | SNAKE_N;
			snake.head[0] = head[0];
			snake.head[1] = head[1];
			score += 100;
		}
	//	score += 2000;
		curse_update(score, &grid);
	}
	fclose(fuk);
}

int main(int argc, char ** argv)
{
	init();

	game_loop();

	curse_term();

	return 0;
}
