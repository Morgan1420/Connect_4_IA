// command to execute the code
// gcc -o connect4 main.c GameLogic/gameFunctions.c GameLogic/boardDisplay.c && ./connect4

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "GameLogic/gameFunctions.h"
#include "GameLogic/boardDisplay.h"

int moves[MAX_NUMBER_OF_MOVES];
int board[BOARD_HEIGHT][BOARD_WIDTH];
int currentPlayer;
bool gameOver;
int winner;
int movesMade;
int gameMode;
bool exitGame;;

int gameMode1() { // Player vs Player
  while (!gameOver) {
    displayBoard(moves, board, currentPlayer);
  
    int column;
    printf("Player %d, choose a column (0-%d): ", currentPlayer, BOARD_WIDTH - 1);
    scanf("%d", &column);
  
    // Drop the piece
    int row = dropPiece(column, moves, board, &currentPlayer, &movesMade);
    if (row == -1) {
      printf("Invalid move. Try again.\n");
      continue; // Skip to the next iteration
    } else if (row == -2) {
      printf("Column is full. Try again.\n");
      continue; // Skip to the next iteration
    } else {
      // Check for a win
      if (checkWin(row, column, moves, board, &currentPlayer, &gameOver, &movesMade)) {
        displayBoard(moves, board, currentPlayer);
        printf("Player %d wins!\n", currentPlayer);
        break;
      }
    
      // Switch players
      currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }
  }
}

int main(int argc, char **argv) {

  exitGame = false;

  while (!exitGame){

    // Initialize the game
    startGame(moves, board, &currentPlayer, &gameOver, &winner, &movesMade, &gameMode);

    // Main game loop
    // Each mode has a the main loop inside the function
    if (gameMode == 1) {
      gameMode1(); // Player vs Player
    } else if (gameMode == 2) {
      // Implement AI vs Player logic here
    } else if (gameMode == 3) {
      // Implement AI vs AI logic here
    }
    

    // Ask if the player wants to play again
    printf("Do you want to play again? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'n' || choice == 'N') {
      exitGame = true;
      printf("Thanks for playing!\n");
    } else if (choice == 'y' || choice == 'Y') {
      // Reset the game state
      gameOver = false;
      movesMade = 0;
      currentPlayer = PLAYER1;
      winner = EMPTY;
    } else {
      printf("Invalid choice. Exiting the game.\n");
      exitGame = true;
    }
  }
  
  printf("Exiting the game...\n");

  return 0;
}
    