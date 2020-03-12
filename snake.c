#include <curses.h>

#include "config.h"
#include "curse.h"

#define HRZ_SCALE 1.5

/* snake window */
#define HEIGHT 20
#define WIDTH  30
#define SWINX 0
#define SWINY 1

/* score window */
#define WHEIGHT SWINY
#define WWIDTH  WIDTH
#define WWINX 0
#define WWINY 0

WINDOW * swin, * wwin;

unsigned int score = 0;

char grid[WIDTH][HEIGHT];

void curse_init() {
	initscr();
	clear();
	noecho();
	cbreak();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);

	swin = newwin(HEIGHT, HRZ_SCALE * WIDTH, SWINY, SWINX);
	wwin = newwin(WHEIGHT, HRZ_SCALE * WWIDTH, WWINX, WWINY);

	box(swin,0,0);
	refresh();
}

void curse_update_score(unsigned int score) {
	wclear(wwin);
	wprintw(wwin, "Score: %u", score);
}

void curse_repaint() {
	
	wrefresh(swin);
	wrefresh(wwin);
	refresh();
}

void curse_term() {
	delwin(swin);
	delwin(wwin);
	endwin();
}

int main(int argc, char ** argv)
{
	curse_init();

	curse_repaint();
		
	int x;
	while(x = wgetch(swin)) {
		score += x;
		curse_update_score(score);
		wprintw(swin, "--got %d--", x);
		curse_repaint();
	}

	curse_term();

	return 0;
}
