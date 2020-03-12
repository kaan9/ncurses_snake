#include <curses.h>

#include "config.h"
#include "curse.h"

unsigned int score = 0;

char grid[WIDTH][HEIGHT];

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
