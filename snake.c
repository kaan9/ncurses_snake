#include <curses.h>

#define HEIGHT 20
#define WIDTH  30
#define HRZ_SCALE 1.5
#define WINX 0
#define WINY 0

WINDOW * swin;

char grid[WIDTH][HEIGHT];

void curse_init() {
	initscr();
	clear();
	noecho();
	cbreak();

	swin = newwin(HEIGHT, HRZ_SCALE * WIDTH, WINY, WINX);

	keypad(swin, TRUE);
	box(swin,0,0);
	refresh();
}

void curse_repaint() {
	
	refresh();
	wrefresh(swin);
}

void curse_term() {
	delwin(swin);
	endwin();
}

int main(int argc, char ** argv)
{
	curse_init();

	curse_repaint();
		
	int x;
	while(x = wgetch(swin), 1) printw("--got %x--", x), curse_repaint();


	
	return 0;
}
