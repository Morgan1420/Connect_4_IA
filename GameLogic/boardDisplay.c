#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "gameFunctions.h"
#include "boardDisplay.h"

int displayBoard(int board[BOARD_HEIGHT][BOARD_WIDTH], int currentPlayer) {
    // Clear the console
    //printf("\033[H\033[J"); // ANSI escape code to clear the screen

    // Display the game board
    printf("Current board:\n");
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("Current player: %d\n", currentPlayer);
    return 0;
}
