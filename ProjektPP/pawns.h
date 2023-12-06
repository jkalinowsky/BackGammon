#include <iostream>
#include"conio2.h"
#include <time.h>

using namespace std;

void quarterCheck(int array[24][2], int i, int startingX, int startingY) {
    int counter, modifier = 1;
    if (array[i][0] != 0) {
        textcolor(RED);
        counter = array[i][0];
        if (i > 11) {
            i = 24 - i;
            modifier = -1;
        }
        for (int j = 0; j < counter; j++) {
            gotoxy(startingX - 5 * i, startingY - (j * modifier));
            cputs("OO");
        }
    }
    if (array[i][1] != 0) {
        textcolor(BLUE);
        counter = array[i][1];
        if (i > 11) {
            i = 24 - i;
            modifier = -1;
        }
        for (int j = 0; j < counter; j++) {
            gotoxy(startingX - 5 * i, startingY - (j * modifier));
            cputs("XX");
        }
    }
}

void pawnsPlacement(int array[24][2]) {
    for (int i = 0; i < 24; i++) {
        if (i < 12) {
            if (i < 6) {
                quarterCheck(array, i, 73, 24);
            }
            else {
                quarterCheck(array, i, 65, 24);
            }
        }
        else {
            if (i < 18) {
                quarterCheck(array, i, 70, 5);
            }
            else {
                quarterCheck(array, i, 78, 5);
            }
        }
    }
    textcolor(GREEN);
}