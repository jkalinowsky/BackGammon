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

    if (gameData->dice1 == gameData->dice2)
        gameData->dubletValue = 2;
}

void diceWhileNotEven(s_game* gameData) {
    while (gameData->dice1 == gameData->dice2) {
        diceRoll(gameData);
    }
    if (gameData->dice1 > gameData->dice2) {
        gameData->turn = PLAYERONE;
    }
    else {
        gameData->turn = PLAYERTWO;
    }
}

void startingTurn(s_game *gameData) {
    char diceC1[2], diceC2[2];
    diceRoll(gameData);
    diceWhileNotEven(gameData);

    textcolor(WHITE);
    gotoxy(30, 1);
    cputs("Press any KEY to roll dice");
    getch();

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
        {0, 2, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {5, 0, 0}, {0, 0, 0}, {3, 0, 0},
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
    gameData.dubletValue = 0;
    gameData.isLoaded = 0;

    return gameData;
}

void drawDeadPawns(s_game gameData, s_deadPawns deadPawns) {
    char deadS[3];
    if (deadPawns.playertwoPawns != 0) {
        gotoxy(19, 14);
        cputs("XX  -  ");
        sprintf(deadS, "%d", deadPawns.playertwoPawns);
        cputs(deadS);
    }
    if (deadPawns.playeronePawns != 0) {
        gotoxy(55, 14);
        cputs("OO  -  ");
        sprintf(deadS, "%d", deadPawns.playeronePawns);
        cputs(deadS);
    }
}

void drawBoard(s_game gameData, s_deadPawns deadPawns) {
    clrscr();
    drawEmptyBoard();
    pawnsPlacement(gameData.pawns);
    gameUIDraw(gameData.dice1, gameData.dice2, gameData.turn);
    if (deadPawns.playeronePawns != 0 || deadPawns.playertwoPawns != 0)
        drawDeadPawns(gameData, deadPawns);
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

    possCords->cord1 = (gameData.dice1 != 0) ? (pawnCord + (gameData.dice1 * modifier)) : 0;
    possCords->cord2 = (gameData.dice2 != 0) ? (pawnCord + (gameData.dice2 * modifier)) : 0;
    possCords->cord3 = (gameData.dice1 != 0 && gameData.dice2 != 0) ? (pawnCord + ((gameData.dice1 + gameData.dice2) * modifier)) : 0;
    if (gameData.dubletValue != 0)
        possCords->cord4 = (gameData.dice1 != 0) ? (pawnCord + (gameData.dice1 * 4 * modifier)) : 0;
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

void makeNormalMove(s_game* gameData, s_moveCords moveCords) {
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

void checkPawnTakesCords(s_game gameData, s_takeCords* takeCords, s_possibleCords possCords, int index, int j, int i) {
     if (gameData.pawns[j][index - 1] == 1 && j == possCords.cord1) {
        takeCords->pawnCord = (i + 1);
        takeCords->takeCord = (j + 1);
    }
    else if (gameData.pawns[j][index - 1] == 1 && j == possCords.cord2) {
        takeCords->pawnCord = (i + 1);
        takeCords->takeCord = (j + 1);
    }
    else if (gameData.pawns[j][index - 1] == 1 && j == possCords.cord3) {
        takeCords->pawnCord = (i + 1);
        takeCords->takeCord = (j + 1);
    }
}

void checkPossiblePawnTakes(s_game gameData, s_takeCords* takeCords) {
    s_possibleCords possCords = { 0, 0, 0 };
    for (int i = 0; i < 24; i++) {
        if (gameData.pawns[i][gameData.turn - 1] > 0) {
            establishPossibleDest(gameData, i, &possCords);
            if (gameData.turn == PLAYERONE) {
                for (int j = 0; j < 24; j++) {
                    checkPawnTakesCords(gameData, takeCords, possCords, PLAYERTWO, j, i);
                    if (takeCords->pawnCord != -1)
                        return;
                }
            } else {
                for (int j = 23; j > -1; j--) {
                    checkPawnTakesCords(gameData, takeCords, possCords, PLAYERONE, j, i);
                    if (takeCords->pawnCord != -1)
                        return;
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

void showPossPawns(s_game gameData) {
    for (int i = 0; i < 24; i++) {
        if (gameData.pawns[i][2] == 1) {
            textcolor(WHITE);
            showGraphicMove(i + 1);
            textcolor(GREEN);
        }
    }
}

void pawnFieldText() {
    gotoxy(5, 29);
    cputs("Type pawn field: (submit with 2xenter) ");
    gotoxy(50, 29);
    cputs(":");
}

void getPawnCord(s_game *gameData, s_moveCords* moveCords, s_takeCords takeCords, s_deadPawns deadPawns) {
    
    pawnFieldText();
    if (takeCords.pawnCord != -1) {
        gameData->pawns[takeCords.pawnCord - 1][2] = 1;
    }
    else {
        checkPawnMoves(gameData);
    }



    showPossPawns(*gameData);
    while (gameData->pawns[moveCords->pawnCord - 1][2] != 1) {
        scanf("%d", &moveCords->pawnCord);
        if (gameData->pawns[moveCords->pawnCord - 1][2] != 1) {
            gotoxy(60, 29);
            textcolor(DARKGRAY);
            cputs("(Type valid pawnCord)");
            textcolor(GREEN);
        }
    }
    drawBoard(*gameData, deadPawns);
    pawnFieldText();
    
}

void destFieldText() {
    gotoxy(5, 30);
    cputs("Type destination field: (2xsubmit with enter) ");
    gotoxy(50, 30);
    cputs(":");
}

void getDestCord_take(s_game* gameData, s_moveCords* moveCords, s_takeCords takeCords) {
    while (moveCords->destCord != takeCords.takeCord) {
        destFieldText();
        scanf("%d", &moveCords->destCord);
        if (moveCords->destCord != takeCords.takeCord) {
            gotoxy(60, 30);
            textcolor(DARKGRAY);
            cputs("Wrong destination field. Choose the highlited cord!");
            textcolor(GREEN);
        }
    }
}

void getDestCord_normal(s_game* gameData, s_moveCords* moveCords, s_possibleCords possCords) {
    while (moveCords->destCord != possCords.cord1 && moveCords->destCord != possCords.cord2 && moveCords->destCord != possCords.cord3) {
        destFieldText();
        scanf("%d", &moveCords->destCord);
        if (moveCords->destCord != possCords.cord1 && moveCords->destCord != possCords.cord2 && moveCords->destCord != possCords.cord3) {
            gotoxy(60, 30);
            textcolor(DARKGRAY);
            cputs("Wrong destination field. Choose the highlited cord!");
            textcolor(GREEN);
        }
    }
}

void preparePossMoves(s_game gameData, s_moveCords* moveCords, s_possibleCords* possCords, s_takeCords takeCords) {
    if (takeCords.pawnCord != -1) {
        textcolor(MAGENTA);
        showGraphicMove(takeCords.takeCord);
        textcolor(GREEN);
    }
    else {
        establishPossibleDest(gameData, moveCords->pawnCord, possCords);
        checkPossCords(gameData, possCords);
        showPossibleMoves(*possCords, gameData);
    }
}

void resetDicesAndMoves(s_game* gameData, s_moveCords moveCords, s_possibleCords possCords) {
    if (gameData->dubletValue != 0) {
        gameData->dubletValue--;
    }
    else {
        if (possCords.cord1 > 0 && moveCords.destCord == possCords.cord1) {
            gameData->dice1 = 0;
        }
        else if (possCords.cord2 > 0 && moveCords.destCord == possCords.cord2) {
            gameData->dice2 = 0;
        }
        else if (possCords.cord3 > 0 && moveCords.destCord == possCords.cord3) {
            gameData->dice1 = 0;
            gameData->dice2 = 0;
        }
    }
    for (int i = 0; i < 24; i++) {
        gameData->pawns[i][2] = 0;
    }
}

void makeTakeMove(s_game* gameData, s_moveCords moveCords, s_deadPawns* deadPawns) {
    int indexFriendly, indexEnemy;
    if (gameData->turn == PLAYERONE) {
        indexFriendly = 0;
        indexEnemy = 1;
        deadPawns->playertwoPawns += 1;
    }

    else {
        indexFriendly = 1;
        indexEnemy = 0;
        deadPawns->playeronePawns += 1;
    }
    gameData->pawns[moveCords.pawnCord - 1][indexFriendly] -= 1;
    gameData->pawns[moveCords.destCord - 1][indexEnemy] -= 1;
    gameData->pawns[moveCords.destCord - 1][indexFriendly] += 1;
}

void getMoveCords(s_game *gameData, s_moveCords* moveCords, s_deadPawns* deadPawns) {
    char inputS[3];
    int possible = 0;
    s_possibleCords possCords = { 0, 0, 0, 0 };
    s_takeCords takeCords = { -1, -1 };
    checkPossiblePawnTakes(*gameData, &takeCords);

    if ((deadPawns->playeronePawns != 0 && gameData->turn == PLAYERONE) || (deadPawns->playertwoPawns != 0 && gameData->turn == PLAYERTWO)) {
        int index = gameData->turn == PLAYERONE ? PLAYERONE : PLAYERTWO;
        moveCords->pawnCord = gameData->turn == PLAYERONE ? 25 : 0;
        takeCords.pawnCord = gameData->turn == PLAYERONE ? 25 : 0;
        gotoxy(5, 29);
        cputs("You have to leave the bar FIRST! Press KEY");
        getch();
        preparePossMoves(*gameData, moveCords, &possCords, takeCords);
        gameData->turn == PLAYERONE ? deadPawns->playeronePawns -= 1 : deadPawns->playertwoPawns -= 1;
    }
    else {
        getPawnCord(gameData, moveCords, takeCords, *deadPawns);
        gotoxy(60, 29);
        sprintf(inputS, "%d", moveCords->pawnCord);
        cputs(inputS);
        preparePossMoves(*gameData, moveCords, &possCords, takeCords);
    }
    
    if (takeCords.pawnCord != -1)
        getDestCord_take(gameData, moveCords, takeCords);
    else 
        getDestCord_normal(gameData, moveCords, possCords);

    sprintf(inputS, "%d", moveCords->destCord);
    gotoxy(50, 30);
    cputs(inputS); 
    if (takeCords.pawnCord != -1) {
        int modifier = 1;
        makeTakeMove(gameData, *moveCords, deadPawns);
        if (gameData->turn == PLAYERONE)
            modifier = -1;

        if (gameData->dubletValue != 0) {
            gameData->dubletValue--;
        }
        else {
            if (takeCords.takeCord == takeCords.pawnCord + (gameData->dice1 * modifier))
                gameData->dice1 = 0;
            else if (takeCords.takeCord == takeCords.pawnCord + (gameData->dice2 * modifier))
                gameData->dice2 = 0;
            else if (takeCords.takeCord == takeCords.pawnCord + ((gameData->dice1 + gameData->dice2) * modifier)) {
                gameData->dice1 = 0;
                gameData->dice2 = 0;
            }
        }
        for (int i = 0; i < 24; i++) {
            gameData->pawns[i][2] = 0;
        }
    }
    else {
        makeNormalMove(gameData, *moveCords);
        resetDicesAndMoves(gameData, *moveCords, possCords);
    }

}

void move(s_game* gameData, s_deadPawns* deadPawns) {
    while (gameData->dice1 != 0 || gameData->dice2 != 0) {
        saveLastMove(*gameData, "savenumber.txt");
        drawBoard(*gameData, *deadPawns);
        s_moveCords moveCords = { -2, -2 };
        getMoveCords(gameData, &moveCords, deadPawns);
        /*if ((deadPawns->playeronePawns != 0 && gameData->turn == PLAYERONE) || (deadPawns->playertwoPawns != 0 && gameData->turn == PLAYERTWO)) {
            removePawnFromBar(gameData, &moveCords,deadPawns);
        } 
        else*/
            
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
    s_deadPawns deadPawns = { 0, 0 };
    getch();
    drawEmptyBoard();
    int game = TRUE;
    while (game) {
        if (gameData->isLoaded == 1) {
            gameData->isLoaded = 0;
        } else
            diceRoll(gameData);
        drawBoard(*gameData, deadPawns);
        move(gameData, &deadPawns);
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
        gameData.isLoaded = 1;
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
    srand(time(NULL));
    #ifndef __cplusplus
        Conio2_Init();
    #endif  

    termConfig();
    menu();
} // naprawic removeformbarpawns bo powinna brac pod uwage bicie, restartowac kostki
