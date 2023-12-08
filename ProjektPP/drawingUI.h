#ifndef DRAWINGUI_H
#define DRAWINGUI_H

#include <iostream>
#include"conio2.h"
#include <time.h>
#include "declare.h"

using namespace std;

#define PLAYERONE 1
#define PLAYERTWO 2

void termConfig() {
    settitle("BackGammon");
    textbackground(BLACK);
    textcolor(GREEN);
    _setcursortype(_NOCURSOR);
}

void drawEmptyBoard() {
    clrscr();
    textcolor(GREEN);
    gotoxy(10, 3);
    cputs("13   14   15   16   17   18           19   20   21   22   23   24");
    gotoxy(6, 4);
    cputs("<===##===##===##===##===##===##====+++====##===##===##===##===##===##===>");
    for (int i = 5; i < 12; i++) {
        gotoxy(6, i);
        cputs("::  ()   \\/   ()   \\/   ()   \\/    |||    ()   \\/   ()   \\/   ()   \\/  ::");
    }
    for (int i = 12; i < 18; i++) {
        gotoxy(6, i);
        cputs("::                                 |||                                 ::");
    }
    for (int i = 18; i < 25; i++) {
        gotoxy(6, i);
        cputs("::  ()   /\\   ()   /\\   ()   /\\    |||    ()   /\\   ()   /\\   ()   /\\  ::");
    }
    gotoxy(6, 25);
    cputs("<===##===##===##===##===##===##====+++====##===##===##===##===##===##===>");
    gotoxy(10, 26);
    cputs("12   11   10    9    8    7            6    5    4    3    2    1");
}

void menuDraw() {
    cout << "     (P)lay     (L)oad game     (Q)uit" << endl;
}

void playerUIDraw(int turn) {
    textcolor(RED);
    gotoxy(85, 10);
    cputs("|PLAYER ONE|");
    textcolor(BLUE);
    gotoxy(85, 18);
    cputs("|PLAYER TWO|");
    textcolor(WHITE);
    if (turn == PLAYERONE) {
        gotoxy(90, 13);
        cputs("||");
        gotoxy(90, 12);
        cputs("/\\");
    }
    else if (turn == PLAYERTWO) {
        gotoxy(90, 15);
        cputs("||");
        gotoxy(90, 16);
        cputs("\\/");
    }
}

void diceUIDraw(char dice1[2], char dice2[2]) {
    textcolor(GREEN);
    gotoxy(88, 14);
    cputs("[");
    cputs(dice1);
    cputs("]");
    cputs("[");
    cputs(dice2);
    cputs("]"); 
}

void gameUIDraw(int dice1, int dice2, int turn) {
    char diceC1[2], diceC2[2];
    sprintf(diceC1, "%d", dice1);
    sprintf(diceC2, "%d", dice2);
    if (dice1 < 0)
        diceC1[0] = 'X';
    if (dice2 < 0)
        diceC2[0] = 'X';

    textcolor(WHITE);
    gotoxy(34, 2);
    cputs("Game in progress...");

    playerUIDraw(turn);
    diceUIDraw(diceC1, diceC2);
}

#endif // DRAWINGUI_H



