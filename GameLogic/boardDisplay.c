#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "gameFunctions.h"
#include "boardDisplay.h"

int displayBoard(int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int currentPlayer) {
    // Clear the console
    printf("\033[H\033[J"); // ANSI escape code to clear the screen

    // Display the game board
    printf("Current board:\n");
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("Current player: %d\n", currentPlayer);
    printf("Moves made: ");
    for (int i = 0; i < MAX_NUMBER_OF_MOVES; i++) {
        if (moves[i] != 0) {
            printf("%d ", moves[i]);
        }
    }
    printf("\n");
    return 0;
}


// Si ens dona temps podríem fer un display gràfic guapo