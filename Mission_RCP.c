#include<stdio.h>
#include<curses.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#define  LINE "***********************************************************************"
#define BLANK  ' '
#define COLOR_BLACK 0
#define COLOR_RED 1

void basic(char *mission, int p);
int set_ticker(int n_msecs);
void win();
void fail();

void RockScissorPaper();
void RockScissorPaper_keyboard();
void draw_rcp();

void FourOperation();

void AvoidX();
void AvoidX_keyboard();
void move_x(int signum);
int check_meet(int x, int y);
void x_position(int row, int col, char x);
void movingcursor(int x, int y, char c, int refresh);

int meet=0;
int row=11;
int col=45;
int moving=1;
int t=1; 
int cursor_x=11;
int cursor_y=33;

int main(void){
    int c;
    
    initscr();
    keypad(stdscr, TRUE);
    crmode();
    noecho();
    curs_set(0);
    clear();
    
    /*
    Mission: AvoidX
    */
    AvoidX();
    AvoidX_keyboard();

    //RockScissorPaper();
    //RockScissorPaper_keyboard();
    //FourOperation();
    sleep(100);
    endwin();
    return 0;
} 

/* 
default: SCISSOR
press 'a'(to left) or 'd'(to right)
press 'e' to submit
*/
/*void win(){
    move()
}*/
void RockScissorPaper_keyboard(){
    char c;
    int num;

    srand(time(NULL));
    num=rand()%3; // 0:rock 1:scissor 2:paper
    
    /*init_pair(1, COLOR_BLACK, COLOR_RED);
    attron(COLOR_PAIR(1));
    printw("what's color\n");*/
    /*while(1){
        c=getch();
        if(c=='q')
            break;
        else if(c=='a'){

        }
        else if(c=='d'){

        }
    }*/
}

void AvoidX(){
    int i;
    basic("Avoid X", 3);
    for(i=4; i<20; i++)
        movingcursor(i, 75, '#', 1);
    move(6,77); addstr("F");
    move(7,77); addstr("I");
    move(8,77); addstr("N");
    move(9,77); addstr("I");
    move(10,77); addstr("S");
    move(11,77); addstr("H");
    move(13,77); addstr("L");
    move(14,77); addstr("I");
    move(15,77); addstr("N");
    move(16,77); addstr("E");
    refresh();
    x_position(row, col, 'X');
    movingcursor(cursor_x, cursor_y, 'O',1);
    signal(SIGALRM, move_x);
    set_ticker(1500);
}

void AvoidX_keyboard(){
    char c;
    while(1){
        c=getch();
        if(c=='w' || c=='d' || c=='a' || c=='s')
            movingcursor(cursor_x, cursor_y, BLANK,1);
        switch (c)
        {
        case 'q':
            break;
        case 'w':
            if(cursor_x>4)
                cursor_x-=1;  
            break;
        case 'd':
            if(cursor_y<79)
                cursor_y+=1;
            if(cursor_y>75){
                movingcursor(cursor_x, cursor_y, BLANK, 1);
                win();
                signal(SIGALRM, SIG_IGN);
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
            movingcursor(cursor_x, cursor_y, 'O',1);
      
    }
}

void x_position(int row, int col, char x){
    int i;                                          
    move(row, col);
    addch(x);

    for(i=1; i<15; i=i+2){
        movingcursor(row-2, col+(2*i), x, 0);
        if(check_meet(row-2, col+(2*i))){
            meet=1;
            break;
        }
        movingcursor(row+2, col+(2*i), x, 0);
        if(check_meet(row+2, col+(2*i))){
            meet=1;
            break;
        }
        movingcursor(row-4, col+(2*(i+1)), x, 0);
        if(check_meet(row-4, col+(2*(i+1)))){
            meet=1;
            break;
        }

        movingcursor(row, col+(2*(i+1)), x, 0);
        if(check_meet(row, col+(2*(i+1)))){
            meet=1;
            break;
        }
        movingcursor(row+4, col+(2*(i+1)), x, 0);
        if(check_meet(row+4, col+(2*(i+1)))){
            meet=1;
            break;
        }

        if((i==5) || (i==9)){
            movingcursor(row+6, col+(2*(i+1)), x, 0);
            if(check_meet(row+6, col+(2*(i+1)))){
                meet=1;
                break;
            }
            movingcursor(row-6, col+(2*(i+1)), x, 0);
            if(check_meet(row-6, col+(2*(i+1)))){
                meet=1;
                break;
            }   
        }

        if(i==7){
            movingcursor(row+7, col+(2*(i+1)), x, 0);
            if(check_meet(row+7, col+(2*(i+1)))){
                meet=1;
                break;
            }
            movingcursor(row-7, col+(2*(i+1)), x, 0);
            if(check_meet(row-7, col+(2*(i+1)))){
                meet=1;
                break;
            }
        }
    }
    if(meet){
        fail();
        signal(SIGALRM, SIG_IGN);
        //return;
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

void movingcursor(int x, int y, char c, int refresh){
    move(x,y);
    addch(c|A_BOLD);
    if(refresh)
        refresh();
}
int check_meet(int x, int y){
    int res;
    res=(x==cursor_x && y==cursor_y);
    return res;
}

void fail(){
    int i,j;
    for(i=4; i<=20; i++)
        for(j=2; j<=80; j++)
            movingcursor(i,j,' ', 1);
    //L
    for(i=8; i<=17; i++)
        movingcursor(i,25,'*', 0);
    for(j=25; j<=31; j++)
        movingcursor(17,j,'*', 0);
    //O
    for(i=10; i<=15; i++)
        movingcursor(i, 32,'*',0);
    movingcursor(9,33,'*',0);
    movingcursor(16,33,'*',0);
    for(j=34; j<=36; j++){
        movingcursor(8,j,'*', 0);
        movingcursor(17,j,'*',0);
    }
    movingcursor(9,37,'*',0);
    movingcursor(16,37,'*',0);
    for(i=10; i<=15; i++)
        movingcursor(i, 38,'*',0);
    //S
    movingcursor(10, 39,'*',0);
    movingcursor(11, 39,'*',0);
    movingcursor(9,40,'*',0);
    for(j=41;j<=45; j++)
        movingcursor(8,j,'*',0);
    for(j=40; j<=43; j++)
        movingcursor(12,j,'*',0);
    for(j=39; j<=43; j++)
        movingcursor(17,j,'*',0);
    movingcursor(13,44, '*',0);
    movingcursor(16,44,'*',0);
    for(i=14; i<=15; i++)
        movingcursor(i,45,'*',0);
    //E
    for(j=47; j<=53; j++)
        movingcursor(8,j,'*',0);
    for(i=9; i<=17; i++)
        movingcursor(i,47,'*',0);
    for(j=48; j<=52; j++)
        movingcursor(12,j,'*',0);
    for(j=48; j<=53; j++)
        movingcursor(17,j,'*',0);
    refresh();
}

void win(){
    int i,j;
    for(i=4; i<=20; i++)
        for(j=2; j<=90; j++)
            movingcursor(i,j,' ', 1);
    //W
    for(i=8; i<=13; i++){
        movingcursor(i,25,'*',0);
        movingcursor(i,29,'*',0);
        movingcursor(i,30,'*',0);
        movingcursor(i,34,'*',0);
    }
        
    for(i=14; i<=16; i++){
        movingcursor(i, 26, '*',0);
        movingcursor(i, 28, '*',0);
        movingcursor(i, 31, '*',0);
        movingcursor(i, 33, '*',0);
    }
    movingcursor(17,27,'*',0);
    movingcursor(17,32,'*',0);

    //I
    for(j=35; j<=41; j++){
        movingcursor(8,j,'*',0);
        movingcursor(17,j,'*',0);
    }
    for(i=8;i<=17;i++){
        movingcursor(i,38,'*',0);
        movingcursor(i,43,'*',0);
        movingcursor(i,52,'*',0);
    }
    //N
    movingcursor(9,44,'*',0);
    for(i=10; i<=17; i++)
        movingcursor(i, i+34, '*',0);
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
    start_color();
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



