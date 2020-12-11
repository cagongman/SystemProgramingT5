#include "map.h"
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
	/*
	char v = '|';
	char h = '_';
	move(0,0);
	vline(v, 20);
	hline(h, 30);
	move(4,6);
	hline(h, 4);
	vline(v, 2);
	
	move(4,9);
	vline(v, 2);
	move(4,11);
	hline(h, 2);
	move(4,12);
	vline(v, 2);
	*/
	move(0,0);
	vline('*',20);
	hline('*',30);
	move(0,30);
	vline('*',20);
	move(20,0);
	hline('*',31);
	for(int i=0;i<149;i++){
		mvaddch(wall[i][0],wall[i][1],'*');
	}
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	attron(COLOR_PAIR(1));
	for(int i=0;i<5;i++){
		mvaddch(misson[i][0],misson[i][1],'M');
	}
	attroff(COLOR_PAIR(1));
	refresh();
}
