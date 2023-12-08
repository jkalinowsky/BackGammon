#ifndef SAVES_H
#define SAVES_H

#include <iostream>
#include "declare.h"

void saveLastMove(s_game gameData, const char* filename) {
    FILE* file = fopen(filename, "w");  // Open the file in write mode to overwrite existing content
    if (file != NULL) {
        // Save the last move information in the first line
        fprintf(file, "%d %d %d\n", gameData.dice1, gameData.dice2, gameData.turn);

        // Save the pawns information in the subsequent lines
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 3; j++) {
                fprintf(file, "%d ", gameData.pawns[i][j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
    }
}

void loadLastMove(s_game* gameData, const char* filename) {
     FILE* file = fopen(filename, "r");
    if (file != NULL) {
        // Load the last move information from the first line
        fscanf(file, "%d %d %d\n", &gameData->dice1, &gameData->dice2, &gameData->turn);

        // Load the pawn array information from the subsequent lines
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 3; j++) {
                fscanf(file, "%d\n", &gameData->pawns[i][j]);
            }
        }

        fclose(file);
    }
}


#endif // SAVES_H