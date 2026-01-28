#include<stdio.h>
#include<conio.h>

void main(){
    int candyX=40,candyY=10,boxX=40,boxY=14,score=0,lifeline=5,d;
    char ch;

    textbackground(BLUE);
    start:
    clrscr();
    gotoxy(60,5);
    cprintf("Score=%d",score);
    gotoxy(60,6);
    cprintf("Lifeline=%d",lifeline);
    gotoxy(candyX,candyY);
    textcolor(YELLOW);
    cprintf("0");
    gotoxy(boxX,boxY);
    cprintf("#");
    gotoxy(boxX+7,boxY);
    cprintf("#");
    gotoxy(boxX,boxY+1);
    cprintf("########");
    if(kbhit()){
        ch=getch();
        switch(ch){
            case 'a':if(boxX>1){
                        boxX--;
                     }
            case 'A':if(boxX>1){
                        boxX--;
                     }
            case 'd':if(boxX<73){
                        boxX++;
                     }
            case 'D':if(boxX<73){
                        boxX++;
                     }
                     break;
            case 'x':exit(0);
            case 'X':exit(0);
        }
    }
    candyY++;
    if(candyX==14){
        d=candyX-boxX;
        if(d>1 && d<=6){
            score++;
            sound(200);
            delay(200);
            nosound();
        }else{
            lifeline--;
            if(lifeline==0){
                clrscr();
                gotoxy(40,13);
                textcolor(RED);
                cprintf("The game is Over");
                getch();
                exit(0);
            }
        }
        candyX=rand()%80+1;
        candyY=1;
    }

    delay(200);
    goto start;
}