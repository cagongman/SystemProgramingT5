#include "move.h"
#include "minimap.h"

int main(){
	int delay;
	int ndelay;
	int c;
	void move_msg(int);
	
	symbol = 'P';
	ball_start_col = 2;
	ball_start_row = 2;
	setW();
	setM();

	initscr();
	crmode();
	noecho();
	clear();

	row = ball_start_row;
	col = ball_start_col;
	r_dir = 0;
	c_dir = 0;
	delay = 500;
	
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
	r = r-2;
	c = c-2;
	move(r,c);
	for(int i =0; i<5;i++){
		for(int j=0; j<5;j++){
			mvaddch(r+i,c+j, BLANK);
		}
	}
}

void viewM(int r, int c){
	r = r-2;
	c = c-2;
	border('*','*','*','*','*','*','*','*');
	move(0,46);
	vline('*',24);
	gauge(5);
	minimap();

	move(r,c);
	for(int i = 0; i<5;i++){
		for(int j=0;j<5;j++){
			mvaddch(r+i,c+j, MAP[r+i][c+j]);
		}
	}
}

void move_msg(int signum){
	signal(SIGALRM, move_msg);
	
	if (c_dir == -1 && MAP[row][col - 1] == '*')
		c_dir = 0;
	if(c_dir == 1 && MAP[row][col + 1] == '*')
		c_dir = 0;
	if(r_dir == -1 && MAP[row - 1][col] == '*')
		r_dir = 0;
	if(r_dir == 1 && MAP[row + 1][col] == '*')
		r_dir = 0;
	
	move(row, col);
	viewB(row,col);
	move(row,col);
	addch(BLANK);

	row += r_dir;
	col += c_dir;

	move(row,col);
	viewM(row,col);
	move(row,col);
	addch(symbol);
	refresh();
}


void setW(){
	for(int i=0;i<25;i++){
		for(int j=0;j<47;j++){
			MAP[i][j] = ' ';
		}
		MAP[i][0] = '*';
		MAP[i][46] = '*';
	}
	for(int i=0;i<47;i++){
		MAP[0][i] = '*';
		MAP[24][i] = '*';
	}
	for(int i=0;i<260;i++){
		int r = wall[i][0];
		int c = wall[i][1];
		MAP[r][c] = '*';
	}
}

void setM(){
	for(int i=0;i<6;i++){
		MAP[mission[i][0]][mission[i][1]] = 'M';
	}
}

void gauge(int mis){
	move(1,56);
	addstr("!Mission Gauge!");
	move(2,53);
	hline('+', 21);
	vline('+', 3);
	move(2,73);
	vline('+', 3);
	move(4,53);
	hline('+', 21);
	move(6,47);
	hline('*',32);
	move(3,54);
	hline('/', 2*mis);

	/*if (mis == 10) { theifwin }*/
}

void minimap(){
	for(int i=0;i<161;i++){
		mvaddch(miniwall[i][0]+5,miniwall[i][1]+48,'*');
	}
	
	for(int i=0;i<6;i++){
		mvaddch(minimisson[i][0]+5,minimisson[i][1]+48,'M');
	}
}

