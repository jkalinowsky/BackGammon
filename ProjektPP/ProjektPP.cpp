#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include"conio2.h"
#include <time.h>
#include "drawingUI.h"
#include "pawns.h"

using namespace std;

#define TRUE 1
#define FALSE 0
#define PLAYERONE 1
#define PLAYERTWO 2

struct s_game {
    int pawns[24][2];
    int turn, dice1, dice2, round;
};

struct s_moveCords {
    int pawnCord, destCord;
};

typedef struct s_game s_game;
typedef struct s_moveCords s_moveCords;

int diceRoll() {
    return rand() % 6 + 1;
}

void startingTurn(s_game *gameData) {
    int dice1 = diceRoll(), dice2 = diceRoll();
    char diceC1[2], diceC2[2];

    textcolor(WHITE);
    gotoxy(30, 1);
    cputs("Press SPACE to roll dice");
    getch();

    while (dice1 == dice2) {
        dice1 = diceRoll();
        dice2 = diceRoll();
    }
    if (dice1 > dice2) {
        gameData->turn = PLAYERONE;
    }
    else {
        gameData->turn = PLAYERTWO;
    }

    sprintf(diceC1, "%d", dice1);
    sprintf(diceC2, "%d", dice2);

    textcolor(GREEN);
    drawEmptyBoard();

    diceUIDraw(diceC1, diceC2);

    textcolor(WHITE);
    gotoxy(30, 1);
    cputs("Player ");
    if (gameData->turn == PLAYERONE) {
        textcolor(RED);
        cputs("One");
    }
    else {
        textcolor(BLUE);
        cputs("Two");
    }
    textcolor(WHITE);
    cputs(" starts the Game.");

    gotoxy(28, 2);
    cputs("Press SPACE to start the Game!");
}

s_game gameInitialize() {
    s_game gameData;
    int startingPawns[24][2] = {
        {0, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {5, 0},   {0, 0}, {3, 0},
        {0, 0}, {0, 0}, {0, 0}, {0, 5}, {5, 0}, {0, 0}, {0, 0}, {0, 0},
        {0, 3}, {0, 0}, {0, 5}, {0, 0}, {0, 0}, {0, 0}, {2, 0}
    };

    for (int i = 0; i < 24; i++) {
        gameData.pawns[i][0] = startingPawns[i][0];
        gameData.pawns[i][1] = startingPawns[i][1];
    }

    startingTurn(&gameData);
    gameData.round = 0;
    gameData.dice1 = 0;
    gameData.dice2 = 0;

    return gameData;
}

void drawBoard(s_game gameData) {
    clrscr();
    drawEmptyBoard();
    pawnsPlacement(gameData.pawns);
    gameUIDraw(gameData.dice1, gameData.dice2, gameData.turn);
}

void showMove(int move) {
    if (move < 12) {
        if (move < 6) {
            
        }
        else {
            
        }
    }
    else {
        if (move < 18) {
            
        }
        else {
        ;
        }
    }
}

int checkPossiblePawn(s_game gameData, int pawnCord) {
    int index;
    if (gameData.turn == PLAYERONE)
        index = 0;
    else
        index = 1;
    if (gameData.pawns[pawnCord-1][index] != 0)
        return 1;
    gotoxy(68, 29);
    cputs("Invalid pawn cord! Press enter and type again.");
    getch();
    gotoxy(68, 29);
    cputs("                                                            ");
    return 0;
}

void getMoveCords(s_game gameData, s_moveCords* moveCords) {
    char inputS[3];
    int possible = 0;;

    gotoxy(30, 29);
    cputs("Type pawn field: (submit with enter) ");
    while (possible == 0) {
        scanf("%d", &moveCords->pawnCord);
        sprintf(inputS, "%d", moveCords->pawnCord);
        possible = checkPossiblePawn(gameData, moveCords->pawnCord);
    }
    gotoxy(85, 29);
    cputs(inputS);
    

    gotoxy(30, 30);
    cputs("Type destination field: (submit with enter) ");
    scanf("%d", &moveCords->destCord);
    sprintf(inputS, "%d", moveCords->destCord);
    gotoxy(85, 30);
    cputs(inputS);
}

void makeMove(s_game* gameData, s_moveCords moveCords) {
    int index;
    if (gameData->turn == PLAYERONE) {
        index = 0;
        gameData->turn = PLAYERTWO;
    }
        
    else {
        index = 1;
        gameData->turn = PLAYERONE;
    }
    gameData->pawns[moveCords.pawnCord - 1][index] -= 1;
    gameData->pawns[moveCords.destCord - 1][index] += 1;
}

void move(s_game *gameData) {
    s_moveCords moveCords = { 0, 0 };
    getMoveCords(*gameData, &moveCords);
    makeMove(gameData, moveCords);
}


void game(s_game *gameData) {
    srand(time(NULL));
    getch();
    drawEmptyBoard();
    int game = TRUE;
    while (game) {
        gameData->dice1 = diceRoll(), gameData->dice2 = diceRoll();
        drawBoard(*gameData);
        move(gameData);
        drawBoard(*gameData);
    }
}

int menu() {
    menuDraw();
    char gameOption = getch();
    
    switch (gameOption) {
    case 'P':
    case 'p': {
        clrscr();
        s_game gameData = gameInitialize();
        s_game* gameDataPtr = &gameData;
        game(gameDataPtr);
        break;
    }
    case 'Q':
    case 'q':
        return 0;
    }
}


int main()
{
    #ifndef __cplusplus
        Conio2_Init();
    #endif  

    termConfig();
    menu();
} // gdzies mzoe byc dodatkowy clear ktory sprawia ze caly ekran przeskakuje w gore? trzeba to naprawic i bedzie naprawde git :)
