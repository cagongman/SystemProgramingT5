#include<stdio.h>
#include "mission.h"
#define  LINE "***************************************************************"
#define BLANK  ' '

void movingcursor(WINDOW *win,int x, int y, char c, int refresh){
    
    mvwaddch(win, x,y, c|A_BOLD);
    if(refresh)
        wrefresh(win);
}

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
 
int FourOperation_keyboard(WINDOW *win, int result){
    int num;
    curs_set(1);
    echo();
    wmove(win,18,36);
    wrefresh(win);

    wscanw(win,"%d", &num);
    noecho();
    curs_set(0);
    if(num==result){
         winner(win);
         sleep(1);
         return 2;
    }
    else{
        fail(win); 
        sleep(1);
        return 0;
    }
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

int RockScissorPaper_keyboard(WINDOW *win){
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
                sleep(1);
                return 0;
            }
            else if(win_result==1){
                sleep(1);
                winner(win);
                sleep(1);
                return 3;
            }
            else if(win_result==2){
                mvwaddstr(win,10,32,"Same! Try Aagin!");
                result=rand()%3;
            }
        }
    }
    if(c=='q'){
        return 0;
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
    touchwin(win);
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