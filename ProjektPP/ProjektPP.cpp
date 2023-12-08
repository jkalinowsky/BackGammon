#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include"conio2.h"
#include <time.h>
#include "drawingUI.h"
#include "pawns.h"
#include "saves.h"

using namespace std;

#define TRUE 1
#define FALSE 0
#define PLAYERONE 1
#define PLAYERTWO 2

void diceRoll(s_game *gameData) {
    gameData->dice1 = (rand() % 6 + 1);
    gameData->dice2 = (rand() % 6 + 1);
}

void startingTurn(s_game *gameData) {
    char diceC1[2], diceC2[2];
    diceRoll(gameData);

    textcolor(WHITE);
    gotoxy(30, 1);
    cputs("Press SPACE to roll dice");
    getch();

    while (gameData->dice1 == gameData->dice2) {
        diceRoll(gameData);
    }
    if (gameData->dice1 > gameData->dice2) {
        gameData->turn = PLAYERONE;
    }
    else {
        gameData->turn = PLAYERTWO;
    }

    sprintf(diceC1, "%c", '0' + gameData->dice1);
    sprintf(diceC2, "%c", '0' + gameData->dice2);

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
    int startingPawns[24][3] = {
        {0, 2, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0 , 0}, {5, 0, 0},   {0, 0, 0}, {3, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 5, 0}, {5, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 3, 0}, {0, 0, 0}, {0, 5, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2, 0, 0}
    };

    for (int i = 0; i < 24; i++) {
        gameData.pawns[i][0] = startingPawns[i][0];
        gameData.pawns[i][1] = startingPawns[i][1];
        gameData.pawns[i][2] = startingPawns[i][2];
    }

    startingTurn(&gameData);
    gameData.round = 1;
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

void establishPossibleDest(s_game gameData, int pawnCord, s_possibleCords *possCords) {
    int modifier = 1, index;

    if (gameData.turn == PLAYERONE) {
        index = 1;
        modifier = -1;
    }
    else {
        index = 0;
    }

    possCords->cord1 = pawnCord + (gameData.dice1 * modifier);
    possCords->cord2 = pawnCord + (gameData.dice2 * modifier);
    possCords->cord3 = pawnCord + ((gameData.dice1 + gameData.dice2) * modifier);
}

void showGraphicMove(int cord) {
    if (cord < 13) {
        if (cord < 7) {
            gotoxy((78 - 5 * cord), 25);
            cputs("##");
        }
        else {
            gotoxy((70 - 5 * cord), 25);
            cputs("##");
        }
    }
    else {
        cord = 25 - cord;
        if (cord < 7) {
            gotoxy((78 - 5 * cord), 4);
            cputs("##");
        }
        else {
            gotoxy((70 - 5 * cord), 4);
            cputs("##");
        }
    }
}

void showPossibleMoves(s_possibleCords possCords, s_game gameData) {
    textcolor(MAGENTA);
    if (possCords.cord1 > 0 && gameData.dice1 != 0) {
        showGraphicMove(possCords.cord1);
    }
    if (possCords.cord2 > 0 && gameData.dice2 != 0) {
        showGraphicMove(possCords.cord2);
    }
    if (possCords.cord3 > 0 && gameData.dice1 != 0 && gameData.dice2 != 0) {
        showGraphicMove(possCords.cord3);
    }
    textcolor(GREEN);
}

void makeMove(s_game* gameData, s_moveCords moveCords) {
    int index;
    if (gameData->turn == PLAYERONE) {
        index = 0;
    }

    else {
        index = 1;
    }
    gameData->pawns[moveCords.pawnCord - 1][index] -= 1;
    gameData->pawns[moveCords.destCord - 1][index] += 1;
}

void checkPossiblePawnTakes(s_game gameData, s_takeCords* takeCords) {
    s_possibleCords possCords = { 0, 0, 0 };
    for (int i = 0; i < 24; i++) {
        if (gameData.pawns[i][gameData.turn - 1] > 0) {
            establishPossibleDest(gameData, i+1, &possCords);
            if (gameData.turn == PLAYERONE) {
                for (int j = 1; j < 25; j++) {
                    if (gameData.pawns[j-1][PLAYERTWO - 1] == 1 && j == possCords.cord1) {
                        takeCords->pawnCord = i;
                        takeCords->takeCord = j;
                    }
                    else if (gameData.pawns[j-1][PLAYERTWO - 1] == 1 && j == possCords.cord2) {
                        takeCords->pawnCord = i;
                        takeCords->takeCord = j;
                    }
                    else if (gameData.pawns[j-1][PLAYERTWO - 1] == 1 && j == possCords.cord3) {
                        takeCords->pawnCord = i;
                        takeCords->takeCord = j;
                    }
                 
                }
            } else {
                for (int j = 23; j > -1; j--) {
                    if (gameData.pawns[j][PLAYERONE - 1] == 1 && (j == possCords.cord1 || j == possCords.cord2 || j == possCords.cord3)) {
                        takeCords->pawnCord = i;
                    }
                }
            }
        }
    }
}

void checkPossCords(s_game gameData, s_possibleCords* possCords) {
    int index;

    if (gameData.turn == PLAYERONE) {
        index = 1;
    }
    else {
        index = 0;
    }

    if (gameData.pawns[possCords->cord1-1][index] > 1 || possCords->cord1 < 1 || possCords->cord1 > 24)
        possCords->cord1 = -1;
    if (gameData.pawns[possCords->cord2-1][index] > 1 || possCords->cord2 < 1 || possCords->cord2 > 24)
        possCords->cord2 = -1;
    if (gameData.pawns[possCords->cord3-1][index] > 1 || possCords->cord3 < 1 || possCords->cord3 > 24)
        possCords->cord3 = -1;
}

void checkPawnMoves(s_game* gameData) {
    s_possibleCords possCords = { 0, 0, 0 };
    for (int i = 0; i < 24; i++) {
        if (gameData->pawns[i][gameData->turn - 1] > 0) {
            establishPossibleDest(*gameData, i + 1, &possCords);;
            checkPossCords(*gameData, &possCords);
            if ((possCords.cord1 != -1) || (possCords.cord2 != -1) || (possCords.cord3 != -1))
                gameData->pawns[i][2] = 1;
        }
    }
}

void getMoveCords(s_game *gameData, s_moveCords* moveCords) {
    char inputS[3];
    int possible = 0;
    s_possibleCords possCords = { 0, 0, 0 };
    s_takeCords takeCords = { -1, -1 };
    checkPossiblePawnTakes(*gameData, &takeCords);
    gotoxy(5, 29);
    cputs("Type pawn field: (submit with 2xenter) ");
    gotoxy(50, 29);
    cputs(":");
    if (takeCords.pawnCord != -1) {
        gameData->pawns[takeCords.pawnCord][2] = 1;
    }
    else {
        checkPawnMoves(gameData);
    }

    // showing possible pawns to choose
    for (int i = 0; i < 24; i++) { 
        if (gameData->pawns[i][2] == 1) {
            textcolor(WHITE);
            showGraphicMove(i+1);
            textcolor(GREEN);
        }
    }

    while (gameData->pawns[moveCords->pawnCord-1][2] != 1) {
        
        scanf("%d", &moveCords->pawnCord);
        sprintf(inputS, "%d", moveCords->pawnCord);
        if (gameData->pawns[moveCords->pawnCord - 1][2] != 1) {
            gotoxy(60, 29);
            textcolor(DARKGRAY);
            cputs("(Type valid pawnCord)");
            textcolor(GREEN);
        }
    }
    drawBoard(*gameData);
    gotoxy(5, 29);
    cputs("Type pawn field: (submit with 2xenter) ");
    gotoxy(50, 29);
    cputs(":");
    gotoxy(60, 29);
    cputs(inputS);

    if (takeCords.pawnCord != -1) {
        textcolor(WHITE);
        showGraphicMove(takeCords.takeCord+1);
        textcolor(GREEN);
    }
    else {
        establishPossibleDest(*gameData, moveCords->pawnCord, &possCords);
        checkPossCords(*gameData, &possCords);
        showPossibleMoves(possCords, *gameData);
    }
    
    
    if (takeCords.pawnCord != -1) {
        while (moveCords->destCord != takeCords.takeCord) {
            gotoxy(5, 30);
            cputs("Type destination field: (2xsubmit with enter) ");
            gotoxy(50, 30);
            cputs(":");
            scanf("%d", &moveCords->destCord);  
            if (moveCords->destCord != takeCords.takeCord) {
                gotoxy(60, 30);
                textcolor(DARKGRAY);
                cputs("Wrong destination field. Choose the highlited cord!");
                textcolor(GREEN);
            }
        }
    }
    else {
        while (moveCords->destCord != possCords.cord1 && moveCords->destCord != possCords.cord2 && moveCords->destCord != possCords.cord3) {
            gotoxy(5, 30);
            cputs("Type destination field: (2xsubmit with enter) ");
            gotoxy(50, 30);
            cputs(":");
            scanf("%d", &moveCords->destCord);
            if (moveCords->destCord != possCords.cord1 && moveCords->destCord != possCords.cord2 && moveCords->destCord != possCords.cord3) {
                gotoxy(60, 30);
                textcolor(DARKGRAY);
                cputs("Wrong destination field. Choose the highlited cord!");
                textcolor(GREEN);
            }
        }
    }
    sprintf(inputS, "%d", moveCords->destCord);
    gotoxy(50, 30);
    cputs(inputS); 
    makeMove(gameData, *moveCords);

    if (possCords.cord1 > 0 && moveCords->destCord == possCords.cord1) {
        gameData->dice1 = 0;
    }
    else if (possCords.cord2 > 0 && moveCords->destCord == possCords.cord2) {
        gameData->dice2 = 0;
    }
    else if (possCords.cord3 > 0 && moveCords->destCord == possCords.cord3) {
        gameData->dice1 = 0;
        gameData->dice2 = 0;
    }
    for (int i = 0; i < 24; i++) {
        gameData->pawns[i][2] = 0;
    }
}

void move(s_game *gameData) {
    while (gameData->dice1 != 0 || gameData->dice2 != 0) {
        saveLastMove(*gameData, "savenumber.txt");
        drawBoard(*gameData);
        s_moveCords moveCords = { -2, -2 };
        getMoveCords(gameData, &moveCords);
    }
}

void changeTurn(s_game *gameData) {
    if (gameData->turn == PLAYERONE)
        gameData->turn = PLAYERTWO;
    else
        gameData->turn = PLAYERONE;
    gameData->round++;
}

void game(s_game *gameData) {
    srand(time(NULL));
    getch();
    drawEmptyBoard();
    int game = TRUE;
    while (game) {
        diceRoll(gameData);
        drawBoard(*gameData);
        move(gameData);
        changeTurn(gameData);
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
    case 'L':
    case 'l': {
        clrscr();
        s_game gameData;
        s_game* gameDataPtr = &gameData;
        loadLastMove(gameDataPtr, "savenumber.txt");
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
} //debugger w getMoveCords ogarnac co tam nie gra :(
