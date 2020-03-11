#include <curses.h>

#include "config.h"
#include "globals.h"

extern WINDOW * swin
extern WINDOW * wwin;

extern char grid[WIDTH][HEIGHT];

extern score

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

int curse_timed_key() {
	return timeout(DELAY);
}

void curse_update_score() {
	wclear(wwin);
	wprintw(wwin, "Score: %u", score);
}

void curse_update_grid() {
	
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
