#include<stdio.h>
#include<curses.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>

//#define LINE "----------------------------------------------------------------------"
#define  LINE "***********************************************************************"
#define BLANK   ' '

void basic(char *mission, int p);
void draw_rcp();
void RockScissorPaper();
void FourOperation();
void AvoidX();
int set_ticker(int n_msecs);
void move_x(int signum);
void x_position(int row, int col, char x);

int row=11;
int col=45;
int moving=1;
int t=1;

int main(void){
    int c;
    
    initscr();
    keypad(stdscr, TRUE);
    crmode();
    noecho();
    clear();
    
    AvoidX();
    //RockScissorPaper();
    //FourOperation();
    while(1){
        c=getch();
        switch (c)
        {
        case KEY_ENTER:
            exit(1);
            break;
        
        default:
            break;
        }
        
    }
    endwin();
    return 0;
} 

void AvoidX(){
    basic("Avoid X", 3);
    x_position(row, col, 'X');
    signal(SIGALRM, move_x);
    set_ticker(1000);
}

void x_position(int row, int col, char x){
    int i;
    move(row, col);
    addch(x);

    for(i=1; i<15; i=i+2){
        move(row-2, col+(2*i));
        addch(x|A_BOLD);

        move(row+2, col+(2*i));
        addch(x|A_BOLD);

        move(row-4, col+(2*(i+1)));
        addch(x|A_BOLD);
        move(row, col+(2*(i+1)));
        addch(x|A_BOLD);
        move(row+4, col+(2*(i+1)));
        addch(x|A_BOLD);

        if((i==5) || (i==9)){
            move(row+6, col+(2*(i+1)));
            addch(x|A_BOLD);
            move(row-6, col+(2*(i+1)));
            addch(x|A_BOLD);
        }

        if(i==7){
            move(row+7, col+(2*(i+1)));
            addch(x|A_BOLD);
            move(row-7, col+(2*(i+1)));
            addch(x|A_BOLD);
        }
    }

    refresh();
}

void move_x(int signum){
    char x='X';
    signal(SIGALRM, move_x);
    x_position(row, col, BLANK);
    col-=1;
    if(t==1)
        row+=2;
    else
        row-=2;
    t*=-1;
    x_position(row, col, x);
    refresh();
}

int set_ticker(int n_msecs){
    struct itimerval timer;
    long n_sec, n_usec;
    
    n_sec=n_msecs/1000;
    n_usec=(n_msecs%1000)*1000L;
    timer.it_value.tv_sec=n_sec;
    timer.it_value.tv_usec=n_usec;
    timer.it_interval.tv_sec=n_sec;
    timer.it_interval.tv_usec=n_usec;

    return setitimer(ITIMER_REAL, &timer, NULL);
}

void FourOperation(){
    char c;
    echo();
    basic("The Four Fundamental Operation",2);
    move(18,27);
    addstr("Answer :");
    move(19,27);
    addstr("-------------------");
    move(18,36);
}

void RockScissorPaper(){
    basic("ROCK SCISSOR PAPER !",1);
    draw_rcp();
}

void draw_rcp(){
    
    //ROCK
    move(17, 23);
    addstr("********");
    move(18,23);
    addstr("*");
    move(18,25);
    addstr("ROCK");
    move(18, 30);
    addstr("*");
    move(19, 23);
    addstr("********");

    //SCISSOR
    move(17, 33);
    addstr("***********");
    move(18,33);
    addstr("*");
    move(18,35);
    addstr("SCISSOR");
    move(18, 43);
    addstr("*");
    move(19, 33);
    addstr("***********");

    //PAPER
    move(17, 46);
    addstr("*********");
    move(18,46);
    addstr("*");
    move(18,48);
    addstr("PAPER");
    move(18, 54);
    addstr("*");
    move(19, 46);
    addstr("*********");

    refresh();
}

void basic(char *mission, int p){
    char title[]="MISSION";

    move(1, 35);
    addstr(title);
    move(3, 5);
    addstr(LINE);
    move(21, 5);
    addstr(LINE);

    switch (p)
    {
    case 1: 
        move(2, 30);
        addstr(mission);
        break;
    case 2:
        move(2, 24);
        addstr(mission);
        break;
    case 3:
        move(2, 35);
        addstr(mission);
    default:
        break;
    }

    refresh();
}



