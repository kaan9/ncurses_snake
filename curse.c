#include <curses.h>

#include "config.h"
#include "globals.h"

#include "curse.h"

/* numeric macros for init_pair */
#define SNAKE_N 1
#define FOOD_N 2

extern char grid[WIDTH][HEIGHT];

extern score;

static WINDOW * swin;
static WINDOW * wwin;

void curse_init()
{
	initscr();
	clear();
	noecho();
	cbreak();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);

	start_color();
	if (!has_color && REQ_COLOR) {
		curse_term();
		exit(0);
	}

	init_pair(SNAKE_N, BACKG_C, SNAKE_C);
	init_pair(FOOD_N, BACKG_C, FOOD_C);


	swin = newwin(HEIGHT, HRZ_SCALE * WIDTH, SWINY, SWINX);
	wwin = newwin(WHEIGHT, HRZ_SCALE * WWIDTH, WWINX, WWINY);

	box(swin,0,0);
	refresh();
}

int curse_timed_key()
{
	return timeout(DELAY);
}

static void curse_update_score()
{
	wclear(wwin);
	wprintw(wwin, "Score: %u", score);
}

static void curse_update_grid()
{
        init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
}

static void curse_repaint() 
{
	wrefresh(swin);
	wrefresh(wwin);
	refresh();
}

void curse_update()
{
	curse_update_score();
	curse_update_grid();
	curse_repaint();
}

void curse_term()
{
	delwin(swin);
	delwin(wwin);
	endwin();
}
