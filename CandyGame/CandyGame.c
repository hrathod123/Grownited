#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <time.h>

#define LEFT_KEY   'a'
#define RIGHT_KEY  'd'

int candyX, candyY;
int boxX, boxY;
int score = 0, life = 5;
int speed = 90; /* lower = faster */

void drawBorder()
{
    int i;
    textcolor(WHITE);

    /* Top border */
    gotoxy(1,1);
    for(i=1;i<=80;i++) cprintf("=");

    /* Bottom border */
    gotoxy(1,25);
    for(i=1;i<=80;i++) cprintf("=");

    /* Side borders */
    for(i=2;i<=24;i++)
    {
        gotoxy(1,i);  cprintf("|");
        gotoxy(80,i); cprintf("|");
    }
}

void drawHeader()
{
    textcolor(CYAN);
    gotoxy(3,2);
    cprintf("Candy Catch Game  (A: Left, D: Right, ESC: Exit)");

    textcolor(YELLOW);
    gotoxy(55,2);
    cprintf("Score: %d", score);

    textcolor(LIGHTRED);
    gotoxy(70,2);
    cprintf("Life: %d", life);
}

void initGame()
{
    randomize();
    boxY = 22;
    boxX = 36;

    candyY = 4;
    candyX = 2 + random(76);  /* between 2..77 */
}

void drawCandy()
{
    textcolor(LIGHTGREEN);
    gotoxy(candyX, candyY);
    cprintf("O");
}

void drawBox()
{
    int i;
    textcolor(LIGHTBLUE);

    gotoxy(boxX, boxY);
    cprintf("#");
    gotoxy(boxX+7, boxY);
    cprintf("#");

    gotoxy(boxX, boxY+1);
    for(i=0;i<8;i++) cprintf("#");
}

void moveCandy()
{
    candyY++;

    /* reached box line */
    if(candyY >= boxY)
    {
        /* check catch */
        if(candyX >= boxX && candyX <= boxX+7)
        {
            score += 10;

            /* increase difficulty a bit */
            if(speed > 30) speed -= 2;
        }
        else
        {
            life--;
        }

        /* reset candy */
        candyY = 4;
        candyX = 2 + random(76);
    }
}

void handleInput()
{
    char ch;

    if(kbhit())
    {
        ch = getch();

        if(ch == 0) ch = getch(); /* handle special keys safely */

        if(ch == 27) /* ESC */
        {
            life = 0; /* force end */
            return;
        }

        if(ch == 'A' || ch == 'a')
        {
            if(boxX > 2) boxX--;
        }
        else if(ch == 'D' || ch == 'd')
        {
            if(boxX < 72) boxX++; /* 72 so box fits till 79 */
        }
    }
}

int main()
{
    clrscr();
    initGame();

    while(life > 0)
    {
        clrscr();
        drawBorder();
        drawHeader();

        drawCandy();
        drawBox();

        handleInput();
        moveCandy();

        delay(speed);
    }

    clrscr();
    textcolor(LIGHTRED);
    gotoxy(35,12);
    cprintf("GAME OVER!");
    textcolor(YELLOW);
    gotoxy(33,14);
    cprintf("Final Score: %d", score);

    textcolor(WHITE);
    gotoxy(28,16);
    cprintf("Press any key to exit...");
    getch();

    return 0;
}
