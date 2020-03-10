#include <curses.h>

#define HEIGHT 20
#define WIDTH  30


int main(int argc, char ** argv)
{
	initscr();
	cbreak();

	refresh();
	
	WINDOW * win = newwin(HEIGHT, 1.5 * WIDTH, 0, 0);
	refresh();

	box(win,0,0);

	wrefresh(win);

	while(getch(), 1);

	endwin();
	
	return 0;
}
