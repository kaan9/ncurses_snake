#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "config.h"

#include "curse.h"

static WINDOW * swin;
static WINDOW * wwin;

void curse_init(void)
{
	initscr();
	clear();
	noecho();
	cbreak();
	nonl();

	nodelay(stdscr, TRUE);
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);

	curs_set(0);

	start_color();
	if (!has_colors() && REQ_COLOR) {
		curse_term();
		exit(0);
	}

	init_pair(SNAKE_N, SNAKE_C, SNAKE_C);
	init_pair(FOOD_N, FOOD_C, FOOD_C);
	init_pair(BACKG_N, BACKG_C, BACKG_C);

	swin = newwin(HEIGHT + 2, WIDTH + 2, SWINY, SWINX);
	wwin = newwin(WHEIGHT, WWIDTH, WWINX, WWINY);

	box(swin, 0, 0);
	refresh();
}

static void ms_wait(unsigned int ms)
{
	sleep(ms / 1000);
	usleep((ms % 1000) * 1000);
}

int curse_timed_key(void)
{
	ms_wait(DELAY);

	/* extract first valid key in buffer if exists */
	int c, key = ERR;
	while ((c = getch()), c != ERR) {
		switch(c) {
		case 'q':
			return 'q';
		case 'w':
			key = KEY_UP;
			continue;
		case 'a':
			key = KEY_LEFT;
			continue;
		case 's':
			key = KEY_DOWN;
			continue;
		case 'd':
			key = KEY_RIGHT;
			continue;
		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			key = c;
			continue;
		}
	}
	return key;
}

static void curse_update_score(unsigned int score)
{
	wclear(wwin);
	wprintw(wwin, "Score: %u", score);
}

static void curse_update_grid(unsigned char (*pgrid)[WIDTH][HEIGHT])
{
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			/* 
			 * attribute is stored as last
			 * 2 bits of each entry, corresponding to SNAKE_N, FOOD_N, BACKG_N
			 * adjust the (x,y) for the borders in mvwaddch
			 */
			wattrset(swin, COLOR_PAIR((*pgrid)[x][y] & 3));
			mvwaddch(swin, y + 1, x + 1, ACS_BLOCK);
		}
	}
}

static void curse_repaint(void)
{
	wrefresh(swin);
	wrefresh(wwin);
	refresh();
}

void curse_update(unsigned int score, unsigned char (*pgrid)[WIDTH][HEIGHT])
{
	curse_update_score(score);
	curse_update_grid(pgrid);
	curse_repaint();
}

void curse_gameover(unsigned int final_score)
{
	wclear(wwin);
	wprintw(wwin, "GAME OVER --- Final Score: %u", final_score);
	curse_repaint();
}

void curse_term(void)
{
	delwin(swin);
	delwin(wwin);
	endwin();
}
