#include <curses.h>

#define HEIGHT 20
#define WIDTH  30
#define HRZ_SCALE 1.5

WINDOW * win;

void curse_init() {
	initscr();
	cbreak();

	win = newwin(HEIGHT, HRZ_SCALE * WIDTH, 0, 0);

	box(win,0,0);

}

void curse_repaint() {
	
	refresh();
	wrefresh(win);
}

void curse_term() {
	endwin();
}

int main(int argc, char ** argv)
{
	curse_init();

	curse_repaint();
		

	while(getch(), 1) curse_repaint();


	
	return 0;
}
