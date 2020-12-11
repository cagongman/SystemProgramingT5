#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include "map.h"

#define BLANK ' '

char symbol;
int ball_start_col;
int ball_start_row;

int row;
int col;
int r_dir;
int c_dir;

int set_ticker(int n_msecs){
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
void viewB(int r, int c){
	r = r-1;
	c = c-1;
	move(r,c);
	for(int i =0; i<3;i++){
		for(int j=0; j<3;j++){
			mvaddch(r+i,c+i, BLANK);
		}
	}
}

void viewW(int r, int c){
	r = r-1;
	c = c-1;
	move(r,c);
	for(int i = 0; i<3;i++){
		for(int j=0;j<3;j++){
			mvaddch(r+i,c+i, WALL[r+i][c+i]);
		}
	}
}

void move_msg(int signum){
	signal(SIGALRM, move_msg);
	
	if (c_dir == -1 && WALL[row][col - 1] != ' ')
		c_dir = 0;
	if(c_dir == 1 && WALL[row][col + 1] != ' ')
		c_dir = 0;
	if(r_dir == -1 && WALL[row - 1][col] != ' ')
		r_dir = 0;
	if(r_dir == 1 && WALL[row + 1][col] != ' ')
		r_dir = 0;
	
	move(row, col);
	//viewB(row,col);
	//move(row,col);
	addch(BLANK);

	row += r_dir;
	col += c_dir;

	move(row,col);
	//viewW(row,col);
	//move(row,col);
	addch(symbol);
	refresh();
}

void setW(){
	for(int i=0;i<25;i++){
		for(int j=0;j<80;j++){
			WALL[i][j] = ' ';
		}
	}
	for(int i=0;i<149;i++){
		int r = wall[i][0];
		int c = wall[i][1];
		WALL[r][c] = '*';
	}
}

int main(){
	int delay;
	int ndelay;
	int c;
	void move_msg(int);
	
	symbol = 'k';
	ball_start_col = 1;
	ball_start_row = 1;
	setW();
	initscr();
	crmode();
	noecho();
	clear();

	row = ball_start_row;
	col = ball_start_col;
	r_dir = 0;
	c_dir = 0;
	delay = 1000;
	
	for(int i=0;i<149;i++){
		mvaddch(wall[i][0],wall[i][1],'*');
	}
	move(row,col);
	addch(symbol);
	signal(SIGALRM, move_msg);
	signal(SIGQUIT, SIG_IGN);
	set_ticker( delay );
	
	
	while(true){
		c = getch();
		if(c == 'w') {r_dir = -1; c_dir = 0;}
		if(c == 's') {r_dir = 1; c_dir = 0;}
		if(c == 'a') {c_dir = -1; r_dir = 0;}
		if(c == 'd') {c_dir = 1; r_dir = 0;}
		if(c == 'Q') break;
	}
	endwin();
	return 0;
}
