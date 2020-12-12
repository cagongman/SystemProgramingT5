#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <curses.h>

#include "newmap.h"
	
#define BLANK ' '
#define T 0
#define P 1
#define ADMIN 2


typedef struct{
	int p_row;
	int p_col;
	int t_row;
	int t_col;
	int who;
}DATA;

DATA data;
DATA own;

// gameboard //
/*----------------------*/
int start = 0;

char symbol;
int ball_start_col = 20;
int ball_start_row = 2;

int row;
int col;
int r_dir;
int c_dir;

int set_ticker(int);
void viewB(int, int);
void viewM(int, int);
void move_msg(int);
void setW();
void setM();

/*----------------------*/


void * send_msg(void * arg);
void * recv_msg(void * arg);
void * gameBoard(void * arg);
void * signal_msg(void *arg);
void error_handling(char * msg);
	
	
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread, gb_thread, signal_thread;
	void * thread_return;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	 }

	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	  
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_create(&gb_thread, NULL, gameBoard, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	pthread_join(gb_thread, &thread_return);
	close(sock);  
	return 0;
}

void *signal_msg(void *arg){
	struct sigaction act;
	sigset_t newmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	act.sa_handler=move_msg;
	sigaction(SIGALRM, &act, NULL);
	pthread_sigmask(SIG_UNBLOCK, &newmask, NULL);
	return NULL;
}

void * gameBoard(void * arg){
	int delay;
	int ndelay;
	int c;
	void move_msg(int);

	while(1){ 
		if(start){
			printf("start!\n");
			sleep(0.5);
			break;
		}
	}
	
	symbol = 'T';
	ball_start_col = data.p_col;
	ball_start_row = data.p_row;
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
	delay = 300;
	
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
	
void * send_msg(void * arg)   // send thread main
{
	int sock=*((int*)arg);
	int len;


	own.who = T;
    col = ball_start_col;
    row = ball_start_row;

	while(1) 
	{
		own.t_col = col;
		own.t_row = row;
		write(sock, (void*)&own, sizeof(own));
		sleep(0.5);
		
	}
	return NULL;
}
	
void * recv_msg(void * arg)   // read thread main
{
	int sock=*((int*)arg);
	char result[500];
	int str_len;
	int past_r, past_c;

	while(1)
	{
		str_len = read(sock, (void*)&data, sizeof(data));
		start = 1;
		own.p_col = data.p_col;
		own.p_row = data.p_row;

		move(past_r, past_c);
		addch(BLANK);

		past_r = data.p_row;
		past_c = data.p_col;

		move(data.p_row, data.p_col);
		addch('P');
	}
	return NULL;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}


/*-----------------------------------------------------------------------------------*/


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