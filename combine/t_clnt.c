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
#include <ncurses.h>

#include "mission.h"
#include "move.h"
#include "minimap.h"
	
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
	int win;
	int mis_gauge;
}DATA;

DATA data;
DATA own;

// gameboard //
/*----------------------*/
int start = 0;
int mis=0;
int meet=0;
char symbol;
int ball_start_col;
int ball_start_row;

int row;
int col;
int r_dir;
int c_dir;

int mission_row=11;
int mission_col=45;
int cursor_x=11;
int cursor_y=33;
WINDOW *window;

int set_ticker(int);
void viewB(int, int);
void viewM(int, int);
void move_msg(int);
void setW();
void setM();
void gauge(int);
void minimap();
void AvoidX(WINDOW *win);
void AvoidX_keyboard(WINDOW *win);
void move_x(int signum);
void x_position(WINDOW *win,int row, int col, char x);
int check_meet(int x, int y);
void start_point();

/*----------------------*/


void * send_msg(void * arg);
void * recv_msg(void * arg);
void gameBoard();
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

	//own.win=-1;
	own.mis_gauge=0;
	start_point();

	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	  
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	gameBoard();
	//pthread_create(&gb_thread, NULL, gameBoard, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	//pthread_join(gb_thread, &thread_return);
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

void gameBoard(){
	int delay;
	int ndelay;
	int c, mission_num, result;
	void move_msg(int);

	while(1){ 
		if(start){
			printf("start!\n");
			sleep(0.5);
			break;
		}
	}
	
	symbol = 'T';

	setW();
	setM();

	initscr();
	crmode();
	noecho();
	curs_set(0);
	start_color();
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	clear();

	row=ball_start_row;
	col=ball_start_col;

	r_dir = 0;
	c_dir = 0;
	delay = 500;
	
	srand(time(NULL));
	move(row,col);
	addch(symbol);
	signal(SIGALRM, move_msg);
	signal(SIGQUIT, SIG_IGN);
	set_ticker( delay );
	
	window=subwin(stdscr,22,73,0,0);
	
	while(true){
		c = getch();
		if(c == 'w') {r_dir = -1; c_dir = 0;}
		if(c == 's') {r_dir = 1; c_dir = 0;}
		if(c == 'a') {c_dir = -1; r_dir = 0;}
		if(c == 'd') {c_dir = 1; r_dir = 0;}
		if(c == 'm'){
			if(MAP[row][col]=='M'){
				MAP[row][col]=BLANK;
				clear();
				signal(SIGALRM, SIG_IGN);
				touchwin(window);
				mission_num = rand()%3;
				switch(mission_num){
					case 0:
						mission_row=11; mission_col=45;
						cursor_x=11; cursor_y=33;
						meet=0;
						AvoidX(window);
    					AvoidX_keyboard(window);
						break;
					case 1:
						result=FourOperation(window);
    					own.mis_gauge+=FourOperation_keyboard(window,result);
						break;
					case 2:
						RockScissorPaper(window);
						own.mis_gauge+=RockScissorPaper_keyboard(window);
						break;
				}

				touchwin(window);
				wclear(window);
				wrefresh(window);
				delwin(window);
				window=subwin(stdscr,22,73,0,0);
				touchwin(stdscr);
				clear();

				signal(SIGALRM, move_msg);
				set_ticker( delay );
				wrefresh(stdscr);	
			}
		}
		if(c == 'Q') break;
	}
	endwin();

}

	
void * send_msg(void * arg)   // send thread main
{
	int sock=*((int*)arg);
	int len;
	own.who = T;
	while(1) 
	{
		own.t_col = col;
		own.t_row = row;
		//own.mis_gauge=mis;
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
		own.win=data.win;

		if(own.win!=-1){
			signal(SIGALRM,SIG_IGN);
			if(own.win==P){
				clear();
				fail(stdscr);
				break;	
			}else if(own.win==T){
				clear();
				winner(stdscr);
				break;
			}
		}
		MAP[past_r][past_c] = ' ';

		past_r = data.p_row;
		past_c = data.p_col;

		MAP[past_r][past_c] = 'P';
	}
	return NULL;
}

void start_point(){
	int n;
	srand(time(NULL));
	n=(rand()%23)+1;
	ball_start_row=n;
	ball_start_col=1;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void viewB(int r, int c){
	r = r-1;
	c = c-1;
	move(r,c);
	for(int i =0; i<3;i++){
		for(int j=0; j<3;j++){
			mvaddch(r+i,c+j, BLANK);
		}
	}
}

void viewM(int r, int c){
	r = r - 1;
	c = c - 1;
	border('*','*','*','*','*','*','*','*');
	move(0,46);
	vline('*',24);
	gauge(own.mis_gauge);
	minimap();
	move(r,c);
	for(int i = 0; i < 3;i++){
		for(int j=0;j < 3;j++){
			mvaddch(r+i,c+j, MAP[r+i][c+j]);
			if(MAP[r+i][c+j] == 'P'){
				own.win=P;
			}
				
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
	wrefresh(stdscr);
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

	if (mis >= 10)
		own.win=T;
}

void minimap(){
	for(int i=0;i<161;i++){
		mvaddch(miniwall[i][0]+5,miniwall[i][1]+48,'*');
	}
	
	for(int i=0;i<6;i++){
		mvaddch(minimisson[i][0]+5,minimisson[i][1]+48,'M');
	}
}

void AvoidX(WINDOW *window){
    int i;
    basic(window,"Avoid X", 3);
    for(i=4; i<20; i++)
        movingcursor(window,i, 68, '#', 1);
    mvwaddch(window,6,69,'F');
    mvwaddch(window,7,69,'I');
    mvwaddch(window,8,69,'N');
    mvwaddch(window,9,69,'I');
    mvwaddch(window,10,69,'S');
    mvwaddch(window,11,69,'H');
    mvwaddch(window,13,69,'L');
    mvwaddch(window,14,69,'I');
    mvwaddch(window,15,69,'N');
    mvwaddch(window,16,69,'E');
    wrefresh(window);

    x_position(window,mission_row, mission_col, 'X');
    movingcursor(window,cursor_x, cursor_y, 'O',1);
    signal(SIGALRM, move_x);
}

 void AvoidX_keyboard(WINDOW *window){
    char c;
    while(meet==0){
        c=wgetch(window);

		if(c=='w' || c=='d' || c=='a' || c=='s')
            movingcursor(window,cursor_x, cursor_y, BLANK,1);
		switch (c)
        {
			case 'q':
				break;
			case 'w':
				if(cursor_x>4)
					cursor_x-=1;  
				break;
			case 'd':
				if(cursor_y<69)
					cursor_y+=1;
				if(cursor_y>=69){
					movingcursor(window,cursor_x, cursor_y, BLANK, 1);
					winner(window);
					own.mis_gauge+=5; 
					sleep(1);
					return;
				}
				break;
			case 'a':
				if(cursor_y>5)
					cursor_y-=1;     
				break;
			case 's':
				if(cursor_x<20)
					cursor_x+=1;
				break; 
			default:
				break;
        }
        if(c=='w' || c=='d' || c=='a' || c=='s')
            movingcursor(window,cursor_x, cursor_y, 'O',1);
    }
	return;
}

void x_position(WINDOW *window,int mission_row, int mission_col, char x){
    int i; 
    mvwaddch(window, mission_row, mission_col, x);                                          

    for(i=1; i<15; i=i+2){
        movingcursor(window,mission_row-2, mission_col+(2*i)-6, x, 0);
        if(check_meet(mission_row-2, mission_col+(2*i)-6)){
            meet=1;
            break;
        }
        movingcursor(window,mission_row+2, mission_col+(2*i)-6, x, 0);
        if(check_meet(mission_row+2, mission_col+(2*i)-6)){
            meet=1;
            break;
        }
        movingcursor(window,mission_row-4, mission_col+(2*(i+1)-6), x, 0);
        if(check_meet(mission_row-4, mission_col+(2*(i+1)-6))){
            meet=1;
            break;
        }

        movingcursor(window,mission_row, mission_col+(2*(i+1)-6), x, 0);
        if(check_meet(mission_row, mission_col+(2*(i+1)-6))){
            meet=1;
            break;
        }
        movingcursor(window,mission_row+4, mission_col+(2*(i+1)-6), x, 0);
        if(check_meet(mission_row+4, mission_col+(2*(i+1)-6))){
            meet=1;
            break;
        }

        if((i==5) || (i==9)){
            movingcursor(window,mission_row+6, mission_col+(2*(i+1)-6), x, 0);
            if(check_meet(mission_row+6, mission_col+(2*(i+1)-6))){
                meet=1;
                break;
            }
            movingcursor(window,mission_row-6, mission_col+(2*(i+1)-6), x, 0);
            if(check_meet(mission_row-6, mission_col+(2*(i+1)-6))){
                meet=1;
                break;
            }   
        }

        if(i==7){
            movingcursor(window,mission_row+7, mission_col+(2*(i+1)-6), x, 0);
            if(check_meet(mission_row+7, mission_col+(2*(i+1)-6))){
                meet=1;
                break;
            }
            movingcursor(window,mission_row-7, mission_col+(2*(i+1)-6), x, 0);
            if(check_meet(mission_row-7, mission_col+(2*(i+1)-6))){
                meet=1;
                break;
            }
        }
		
    }
	if(meet){
		signal(SIGALRM, SIG_IGN);
        fail(window); //return;
		mvwaddstr(window, 19, 21, "You lose! Enter any key to go back");
		wrefresh(window);
    }
}

// add to main code
void move_x(int signum){
    char x='X';
    static int t=1;
    signal(SIGALRM, move_x);
    x_position(window, mission_row, mission_col, BLANK);
    mission_col-=1;
    if(t==1)
        mission_row+=2;
    else
        mission_row-=2;
    t*=-1;
    x_position(window,mission_row, mission_col, x);
    wrefresh(window);
}

int check_meet(int x, int y){
    int res;
    res=(x==cursor_x && y==cursor_y);
    return res;
}