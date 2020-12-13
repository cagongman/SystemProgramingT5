#include "newmap.h"
#include <ncurses.h>
#include <signal.h>

void set_up();
int main()
{
	int	c;
	set_up();

	while ( ( c = getchar()) != 'Q' ){
			
	}
	endwin();
}

void set_up()
{
	initscr();
	noecho();
	crmode();

	move(0,0);
	vline('*',24);
	hline('*',46);
	move(0,46);
	vline('*',24);
	move(24,0);
	hline('*',47);
	for(int i=0;i<260;i++){
		mvaddch(wall[i][0],wall[i][1],'*');
	}
	
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	attron(COLOR_PAIR(1));
	for(int i=0;i<6;i++){
		mvaddch(mission[i][0],mission[i][1],'M');
	}

	attroff(COLOR_PAIR(1));
	
	refresh();
}
