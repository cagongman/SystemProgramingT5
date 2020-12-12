#include<stdio.h>
#include<curses.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

void basic(WINDOW *win,char *mission, int p);
int set_ticker(int n_msecs);
void winner(WINDOW *win);
void fail(WINDOW *win);

void RockScissorPaper(WINDOW *win);
void RockScissorPaper_keyboard(WINDOW *win);
void draw_rcp(WINDOW *win);
void change_colors(WINDOW *win,int cur, int option, int serv);
int Does_theif_win(int thief, int serv);

int FourOperation(WINDOW *win);
void FourOperation_keyboard(WINDOW *win,int result);

void AvoidX(WINDOW *win);
void AvoidX_keyboard(WINDOW *win);
void move_x(int signum);
int check_meet(int x, int y);
void x_position(WINDOW *win,int row, int col, char x);
void movingcursor(WINDOW *win,int x, int y, char c, int refresh);


