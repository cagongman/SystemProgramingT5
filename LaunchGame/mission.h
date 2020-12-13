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
void winner(WINDOW *win);
void fail(WINDOW *win);

void RockScissorPaper(WINDOW *win);
int RockScissorPaper_keyboard(WINDOW *win);
void draw_rcp(WINDOW *win);
void change_colors(WINDOW *win,int cur, int option, int serv);
int Does_theif_win(int thief, int serv);

int FourOperation(WINDOW *win);
int FourOperation_keyboard(WINDOW *win,int result);

void movingcursor(WINDOW *win,int x, int y, char c, int refresh);
int set_ticker(int n_msecs);


