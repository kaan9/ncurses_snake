#include <curses.h>
#include <stdlib.h>

#include "config.h"

#include "curse.h"

/*
 * numeric macros for init_pair
 * also used for grid representation as the last 2
 * bits of each [x][y] value
 */
#define SNAKE_N 1
#define FOOD_N  2
#define BACKG_N 3

static WINDOW * swin;
static WINDOW * wwin;

void curse_init()
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


	swin = newwin(HEIGHT, HRZ_SCALE * WIDTH, SWINY, SWINX);
	wwin = newwin(WHEIGHT, HRZ_SCALE * WWIDTH, WWINX, WWINY);

	box(swin,0,0);
	refresh();
}

int curse_timed_key()
{
	timeout(DELAY);
	return getch();
}

static void curse_update_score(unsigned int score)
{
	wclear(wwin);
	wprintw(wwin, "Score: %u", score);
}

static void curse_update_grid(char (* pgrid)[WIDTH][HEIGHT])
{
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			/* attribute is stored as last
			 * 2 bits of each entry
			 */
			wattron(swin, (*pgrid)[x][y] & 3);
			mvaddch(y, x, ACS_BLOCK);
			wattroff(swin, (*pgrid)[x][y] & 3);
		}
	}
}

static void curse_repaint() 
{
	wrefresh(swin);
	wrefresh(wwin);
	refresh();
}

void curse_update(unsigned int score, char (* pgrid)[WIDTH][HEIGHT])
{
	curse_update_score(score);
	curse_update_grid(pgrid);
	curse_repaint();
}

void curse_term()
{
	delwin(swin);
	delwin(wwin);
	endwin();
}
