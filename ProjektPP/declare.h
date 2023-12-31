#include <iostream>

#ifndef DECLARE_H
#define DECLARE_H

struct s_game {
    int pawns[24][3];
    int turn, dice1, dice2, round, dubletValue, isLoaded;
};

struct s_moveCords {
    int pawnCord, destCord;
};

struct s_possibleCords {
    int cord1, cord2, cord3, cord4;
}; 

struct s_takeCords {
    int pawnCord, takeCord;
};

struct s_deadPawns {
    int playeronePawns, playertwoPawns;
};

typedef struct s_game s_game;
typedef struct s_moveCords s_moveCords;
typedef struct s_possibleCords s_possibleCords;
typedef struct s_takeCords s_takeCords;
typedef struct s_deadPawns s_deadPawns;


void diceRoll(s_game* gameData);
void startingTurn(s_game* gamedata);
void drawBoard(s_game gameData, s_deadPawns deadPawns);
void establishPossibleDest(s_game gameData, int pawnCord, s_possibleCords* possCords);
void showGraphicMove(int cord);
void showPossibleMoves(s_possibleCords possCords, s_game gameData);
void makeMove(s_game* gameData, s_moveCords moveCords);
void checkPossiblePawnTakes(s_game gameData, s_takeCords* takeCords);
void checkPossCords(s_game gameData, s_possibleCords* possCords);
void checkPawnMoves(s_game* gameData);
void getMoveCords(s_game* gameData, s_moveCords* moveCords, s_deadPawns* deadPawns);
void move(s_game* gameData, s_deadPawns* deadPawns);
void changeTurn(s_game* gameData);
void game(s_game* gameData);
s_game gameInitialize();
int menu();

void quarterCheck(int array[24][3], int i, int startingX, int startingY);
void pawnsPlacement(int array[24][3]);

void termConfig();
void drawEmptyBoard();
void menuDraw();
void playerUIDraw(int turn);
void diceUIDraw(char dice1[2], char dice2[2]);
void gameUIDraw(int dice1, int dice2, int turn);

void saveLastMove(s_game gameData, const char* filename);
void loadLastMove(s_game* gameData, const char* filename);


#endif // DECLARE_H