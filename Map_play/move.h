#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include "newmap.h"

#define BLANK ' '

char symbol;
int ball_start_col;
int ball_start_row;

int row;
int col;
int r_dir;
int c_dir;

int set_ticker(int n_msecs);
void viewB(int r, int c);
void viewM(int r, int c);
void move_msg(int signum);