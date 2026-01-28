#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <time.h>

int candyX, candyY;
int boxX, boxY;
int score = 0, life = 5;
int speed = 90;

void drawBorder() {
    int i;
    textcolor(WHITE);

    gotoxy(1,1);
    for(i=1;i<=80;i++) cprintf("=");

    gotoxy(1,25);
    for(i=1;i<=80;i++) cprintf("=");

    for(i=2;i<=24;i++) {
        gotoxy(1,i);  cprintf("|");
        gotoxy(80,i); cprintf("|");
    }
}

void drawHeader() {
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

void initGame() {
    randomize();
    boxY = 22;
    boxX = 36;

    candyY = 4;
    candyX = 2 + random(76);
}

void drawCandy() {
    textcolor(LIGHTGREEN);
    gotoxy(candyX, candyY);
    cprintf("O");
}

void drawBox() {
    int i;
    textcolor(LIGHTBLUE);

    gotoxy(boxX, boxY);
    cprintf("#");
    gotoxy(boxX+7, boxY);
    cprintf("#");

    gotoxy(boxX, boxY+1);
    for(i=0;i<8;i++) cprintf("#");
}

void moveCandy() {
    candyY++;

    if(candyY >= boxY) {
        if(candyX >= boxX && candyX <= boxX+7) {
            score += 10;
            if(speed > 30) speed -= 2;
        } else {
            life--;
        }
        candyY = 4;
        candyX = 2 + random(76);
    }
}

void handleInput() {
    char ch;
    if(kbhit()) {
        ch = getch();
        if(ch == 0) ch = getch(); // special key safe

        if(ch == 27) { // ESC
            life = 0;
            return;
        }

        if(ch=='a' || ch=='A') {
            if(boxX > 2) boxX--;
        } else if(ch=='d' || ch=='D') {
            if(boxX < 72) boxX++;
        }
    }
}

int main() {
    clrscr();
    initGame();

    while(life > 0) {
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
    gotoxy(35,12); cprintf("GAME OVER!");
    textcolor(YELLOW);
    gotoxy(33,14); cprintf("Final Score: %d", score);
    textcolor(WHITE);
    gotoxy(28,16); cprintf("Press any key to exit...");
    getch();
    return 0;
}
