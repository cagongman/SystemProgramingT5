#include<stdio.h>
#include<curses.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#define  LINE "***************************************************************"
#define BLANK  ' '

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

int row=11;
int col=45;

int cursor_x=11;
int cursor_y=33;
WINDOW *win;

int main(void){
    int c;
    int result;
    
    initscr();
    keypad(stdscr, TRUE);
    crmode();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    clear();

    win=subwin(stdscr,22,70,0,0);

    AvoidX(win);
    AvoidX_keyboard(win);

    //RockScissorPaper(win);
    //RockScissorPaper_keyboard(win);

    //result=FourOperation(win);
    //FourOperation_keyboard(win,result);

    refresh();
    
    sleep(100);
    endwin();
    return 0;
} 



void AvoidX(WINDOW *win){
    int i;
    basic(win,"Avoid X", 3);
    for(i=4; i<20; i++)
        movingcursor(win,i, 68, '#', 1);
    mvwaddch(win,6,69,'F');
    mvwaddch(win,7,69,'I');
    mvwaddch(win,8,69,'N');
    mvwaddch(win,9,69,'I');
    mvwaddch(win,10,69,'S');
    mvwaddch(win,11,69,'H');
    mvwaddch(win,13,69,'L');
    mvwaddch(win,14,69,'I');
    mvwaddch(win,15,69,'N');
    mvwaddch(win,16,69,'E');
    wrefresh(win);

    x_position(win,row, col, 'X');
    movingcursor(win,cursor_x, cursor_y, 'O',1);
    signal(SIGALRM, move_x);
    set_ticker(1500);
}

void AvoidX_keyboard(WINDOW *win){
    char c;

    while(1){
        c=wgetch(win);
        if(c=='w' || c=='d' || c=='a' || c=='s')
            movingcursor(win,cursor_x, cursor_y, BLANK,1);
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
                movingcursor(win,cursor_x, cursor_y, BLANK, 1);
                winner(win);
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
            movingcursor(win,cursor_x, cursor_y, 'O',1);
      
    }
}

void x_position(WINDOW *win,int row, int col, char x){
    int i;
    static int meet=0;
    mvwaddch(win, row, col, x);                                          

    for(i=1; i<15; i=i+2){
        movingcursor(win,row-2, col+(2*i)-6, x, 0);
        if(check_meet(row-2, col+(2*i)-6)){
            meet=1;
            break;
        }
        movingcursor(win,row+2, col+(2*i)-6, x, 0);
        if(check_meet(row+2, col+(2*i)-6)){
            meet=1;
            break;
        }
        movingcursor(win,row-4, col+(2*(i+1)-6), x, 0);
        if(check_meet(row-4, col+(2*(i+1)-6))){
            meet=1;
            break;
        }

        movingcursor(win,row, col+(2*(i+1)-6), x, 0);
        if(check_meet(row, col+(2*(i+1)-6))){
            meet=1;
            break;
        }
        movingcursor(win,row+4, col+(2*(i+1)-6), x, 0);
        if(check_meet(row+4, col+(2*(i+1)-6))){
            meet=1;
            break;
        }

        if((i==5) || (i==9)){
            movingcursor(win,row+6, col+(2*(i+1)-6), x, 0);
            if(check_meet(row+6, col+(2*(i+1)-6))){
                meet=1;
                break;
            }
            movingcursor(win,row-6, col+(2*(i+1)-6), x, 0);
            if(check_meet(row-6, col+(2*(i+1)-6))){
                meet=1;
                break;
            }   
        }

        if(i==7){
            movingcursor(win,row+7, col+(2*(i+1)-6), x, 0);
            if(check_meet(row+7, col+(2*(i+1)-6))){
                meet=1;
                break;
            }
            movingcursor(win,row-7, col+(2*(i+1)-6), x, 0);
            if(check_meet(row-7, col+(2*(i+1)-6))){
                meet=1;
                break;
            }
        }
    }
    if(meet){
        fail(win);
        signal(SIGALRM, SIG_IGN);
        //return;
    }
        
    wrefresh(win);
}

// add to main code
void move_x(int signum){
    char x='X';
    static int t=1;
    signal(SIGALRM, move_x);
    x_position(win,row, col, BLANK);
    col-=1;
    if(t==1)
        row+=2;
    else
        row-=2;
    t*=-1;
    x_position(win,row, col, x);
    wrefresh(win);
}

void movingcursor(WINDOW *win,int x, int y, char c, int refresh){
    
    mvwaddch(win, x,y, c|A_BOLD);
    if(refresh)
        wrefresh(win);
}

int check_meet(int x, int y){
    int res;
    res=(x==cursor_x && y==cursor_y);
    return res;
}

/* Mission: FourOperation
*/
int FourOperation(WINDOW *win){
    char c;
    char str[3];
    char buf[BUFSIZ];
    int num[3], result,i;

    echo();
    basic(win,"The Four Fundamental Operation",2);
    mvwaddstr(win, 16,27,"*Enter to submit your answer");
    mvwaddstr(win, 18,27,"Answer :");
    mvwaddstr(win, 19,27,"--------------------------------");

    srand(time(NULL));
    for(i=0; i<3; i++)
        num[i]=(rand()%4)+1;

    switch(rand()%3){
        case 0: str[0]='+'; 
                result=num[0]+num[1];
                break;
        case 1: str[0]='-'; 
                result=num[0]-num[1];
                break;
        case 2: str[0]='x';
                result=num[0]*num[1];
                break;
    }
    switch(rand()%2){
        case 0: str[1]='+';
                result+=num[2];
                break;
        case 1: str[1]='-'; 
                result-=num[2];
                break;
    }
    sprintf(buf, "%d %c %d %c %d", num[0], str[0], num[1], str[1], num[2]);
    wmove(win, 10,35);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    for(i=0; i<strlen(buf); i++)
        waddch(win, buf[i] |COLOR_PAIR(1));
    noecho();
    wrefresh(win);
    return result;
}
 
void FourOperation_keyboard(WINDOW *win, int result){
    int num;
    curs_set(1);
    echo();
    wmove(win,18,36);
    wrefresh(win);

    wscanw(win,"%d", &num);
    noecho();
    curs_set(0);
    if(num==result)
        winner(win);
    else
        fail(win); 
}

/* 
default: SCISSOR
press 'a'(to left) or 'd'(to right)
press 'e' to submit
*/
void RockScissorPaper(WINDOW *win){
    basic(win,"ROCK SCISSOR PAPER !",1);
    draw_rcp(win);
}

void RockScissorPaper_keyboard(WINDOW *win){
    char c;
    int result, cur=1, win_result;
    srand(time(NULL));
    result=rand()%3; // 0:rock 1:scissor 2:paper
    
    keypad(win, 1);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);

    change_colors(win,cur,1,0);

    while(1){
        c=wgetch(win);
        if(c=='q')
            break;
        else if(c=='a'){
            change_colors(win, cur,2,0);
            cur=(cur-1)%3;
            if(cur<0)
                cur=2;
            change_colors(win,cur,1,0);
        }
        else if(c=='d'){
            change_colors(win,cur,2,0);
            cur=(cur+1)%3;
            change_colors(win, cur,1,0);
        }
        else if(c=='e'){
            change_colors(win, result, 3,1);
            win_result=Does_theif_win(cur, result);  
            if(win_result==-1){
                sleep(1);
                fail(win);
                break;
            }
            else if(win_result==1){
                sleep(1);
                winner(win);
                break;
            }
            else if(win_result==2){
                mvwaddstr(win,10,32,"Same! Try Aagin!");
                result=rand()%3;
            }
        }
    }
}

int Does_theif_win(int thief, int serv){
    /* if thief win return 1, bigim return 2 lose -1*/
    if(thief==0){
        if(serv==0)
            return 2;
        else if(serv==2)
            return -1;
        else if(serv==1)
            return 1;
    }
    else if(thief==1){
        if(serv==0)
            return -1;
        else if(serv==1)
            return 2;
        else if(serv==2)
            return 1;
    }
    else if(thief==2){
        if(serv==0)
            return 1;
        else if(serv==1)
            return -1;
        else if(serv==2)
            return 2;
    }
}
void change_colors(WINDOW *win, int cur, int option, int serv){
   
    int i, x,y;
    char *word;

    if(serv){
        mvwaddstr(win,10,32,"            ");
        x=10;
        y=35;
        switch(cur){
        case 0: word="ROCK"; break;
        case 1: word="SCISSOR"; break;
        case 2: word="PAPER"; break;
        }
    }
    else{
        x=18;
        switch(cur){
        case 0: word="ROCK"; y=25; break;
        case 1: word="SCISSOR"; y=35; break;
        case 2: word="PAPER"; y=48; break;
        }
    }
    for(i=0; i<strlen(word); i++)
        mvwaddch(win, x,y+i, word[i] |COLOR_PAIR(option));
    wrefresh(win);
}

void draw_rcp(WINDOW*win){
    
    mvwaddstr(win, 8,30,"The server shows ...");
    mvwaddstr(win,11,30,"=====================");
    mvwaddstr(win, 14,23,"*Use 'a', 'd' to select your r.c.p");
    mvwaddstr(win, 15,23,"*Press 'e' to submit your r.c.p");

    //ROCK
    mvwaddstr(win,17,23,"********");
    mvwaddstr(win,18,23,"*");
    mvwaddstr(win,18,25,"ROCK");
    mvwaddstr(win,18,30,"*");
    mvwaddstr(win,19,23,"********");

    //SCISSOR
    mvwaddstr(win,17,33,"***********");
    mvwaddstr(win,18,33,"*");
    mvwaddstr(win,18,35,"SCISSOR");
    mvwaddstr(win,18,43,"*");
    mvwaddstr(win,19,33,"***********");
   
    //PAPER
    mvwaddstr(win,17,46,"*********");
    mvwaddstr(win,18,46,"*");
    mvwaddstr(win,18,48,"PAPER");
    mvwaddstr(win,18,54,"*");
    mvwaddstr(win,19,46,"*********");

    wrefresh(win);
}

void basic(WINDOW *win,char *mission, int p){
    char title[]="MISSION";

    mvwaddstr(win, 1,35, title);
    mvwaddstr(win, 3,5, LINE);
    mvwaddstr(win, 21,5,LINE);

    switch (p)
    {
        case 1: 
            mvwaddstr(win,2,30,mission);
            break;
        case 2:
            mvwaddstr(win,2,24,mission);
            break;
        case 3:
            mvwaddstr(win,2,35, mission);
        default:
            break;
    }

    wrefresh(win);
}

void fail(WINDOW *win){
    int i,j;
    touchwin(win)
    wclear(win);
    //L
    for(i=8; i<=17; i++)
        movingcursor( win,i,25,'*', 0);
    for(j=25; j<=31; j++)
        movingcursor(win,17,j,'*', 0);
    //O
    for(i=10; i<=15; i++)
        movingcursor(win,i, 32,'*',0);
    movingcursor(win,9,33,'*',0);
    movingcursor(win,16,33,'*',0);
    for(j=34; j<=36; j++){
        movingcursor(win,8,j,'*', 0);
        movingcursor(win,17,j,'*',0);
    }
    movingcursor(win,9,37,'*',0);
    movingcursor(win,16,37,'*',0);
    for(i=10; i<=15; i++)
        movingcursor(win,i, 38,'*',0);
    //S
    movingcursor(win,10, 39,'*',0);
    movingcursor(win,11, 39,'*',0);
    movingcursor(win,9,40,'*',0);
    for(j=41;j<=45; j++)
        movingcursor(win,8,j,'*',0);
    for(j=40; j<=43; j++)
        movingcursor(win,12,j,'*',0);
    for(j=39; j<=43; j++)
        movingcursor(win,17,j,'*',0);
    movingcursor(win,13,44, '*',0);
    movingcursor(win,16,44,'*',0);
    for(i=14; i<=15; i++)
        movingcursor(win,i,45,'*',0);
    //E
    for(j=47; j<=53; j++)
        movingcursor(win,8,j,'*',0);
    for(i=9; i<=17; i++)
        movingcursor(win,i,47,'*',0);
    for(j=48; j<=52; j++)
        movingcursor(win,12,j,'*',0);
    for(j=48; j<=53; j++)
        movingcursor(win,17,j,'*',0);
    wrefresh(win);
}

void winner(WINDOW *win){
    int i,j;
    touchwin(win);
    wclear(win);
    //W
    for(i=8; i<=13; i++){
        movingcursor(win,i,25,'*',0);
        movingcursor(win,i,29,'*',0);
        movingcursor(win,i,30,'*',0);
        movingcursor(win,i,34,'*',0);
    }
        
    for(i=14; i<=16; i++){
        movingcursor(win,i, 26, '*',0);
        movingcursor(win,i, 28, '*',0);
        movingcursor(win,i, 31, '*',0);
        movingcursor(win,i, 33, '*',0);
    }
    movingcursor(win,17,27,'*',0);
    movingcursor(win,17,32,'*',0);

    //I
    for(j=35; j<=41; j++){
        movingcursor(win,8,j,'*',0);
        movingcursor(win,17,j,'*',0);
    }
    for(i=8;i<=17;i++){
        movingcursor(win,i,38,'*',0);
        movingcursor(win,i,43,'*',0);
        movingcursor(win,i,52,'*',0);
    }
    //N
    movingcursor(win,9,44,'*',0);
    for(i=10; i<=17; i++)
        movingcursor(win,i, i+34, '*',0);
    wrefresh(win);
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


