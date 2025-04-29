#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "gameFunctions.h"


int startGame(int board[BOARD_HEIGHT][BOARD_WIDTH], int *currentPlayer, bool *gameOver, int *winner, int *movesMade, int *gameMode) {
  // Initialize the game board
  for (int i = 0; i < BOARD_HEIGHT; i++) {
      for (int j = 0; j < BOARD_WIDTH; j++) {
          board[i][j] = EMPTY;
      }
  }

  // Set default settings
  *currentPlayer = PLAYER1;
  *gameOver = false;
  *winner = EMPTY;
  *movesMade = 0;

  // Set game mode based on user input
  printf("Select game mode:\n");
  printf("1. Player vs Player\n");
  printf("2. Player vs AI\n");
  printf("3. AI vs AI\n");
  scanf("%d", gameMode);

  // Validate game mode input
  if (*gameMode < 1 || *gameMode > 3) {
      printf("Invalid game mode. Defaulting to Player vs AI.\n");
      *gameMode = 2;
  }

  return 0;
}

int dropPiece(int column, int board[BOARD_HEIGHT][BOARD_WIDTH], int *currentPlayer, int *movesMade) {
  // Check if the column is valid
  if (column < 0 || column >= BOARD_WIDTH) {
      printf("Invalid column. Please choose a column between 0 and %d.\n", BOARD_WIDTH - 1);
      return -1;
  }

  // Find the lowest empty row in the selected column
  for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
      if (board[i][column] == EMPTY) {
          board[i][column] = *currentPlayer;
          *movesMade++;
          return i; // Return the row where the piece was placed
      }
  }

  printf("Column %d is full. Please choose another column.\n", column);
  return -1;
}

int checkWin(int row, int column, int board[BOARD_HEIGHT][BOARD_WIDTH], int *currentPlayer, bool *gameOver, int *movesMade) {
  // Check horizontal
  for (int j = 0; j <= BOARD_WIDTH - WINNING_LENGTH; j++) {
      if (board[row][j] == *currentPlayer &&
          board[row][j + 1] == *currentPlayer &&
          board[row][j + 2] == *currentPlayer &&
          board[row][j + 3] == *currentPlayer) {
          return 1;
      }
  }

  // Check vertical
  for (int i = 0; i <= BOARD_HEIGHT - WINNING_LENGTH; i++) {
      if (board[i][column] == *currentPlayer &&
          board[i + 1][column] == *currentPlayer &&
          board[i + 2][column] == *currentPlayer &&
          board[i + 3][column] == *currentPlayer) {
          return 1;
      }
  }

  // Check diagonal (bottom-left to top-right)
  for (int i = WINNING_LENGTH - 1; i < BOARD_HEIGHT; i++) {
      for (int j = 0; j <= BOARD_WIDTH - WINNING_LENGTH; j++) {
          if (board[i][j] == *currentPlayer &&
              board[i - 1][j + 1] == *currentPlayer &&
              board[i - 2][j + 2] == *currentPlayer &&
              board[i - 3][j + 3] == *currentPlayer) {
              return 1;
          }
      }
  }
  // Check diagonal (top-left to bottom-right)
  for (int i = 0; i <= BOARD_HEIGHT - WINNING_LENGTH; i++) {
      for (int j = 0; j <= BOARD_WIDTH - WINNING_LENGTH; j++) {
          if (board[i][j] == *currentPlayer &&
              board[i + 1][j + 1] == *currentPlayer &&
              board[i + 2][j + 2] == *currentPlayer &&
              board[i + 3][j + 3] == *currentPlayer) {
              return 1;
          }
      }
  }
  // Check for draw
  if (*movesMade == BOARD_WIDTH * BOARD_HEIGHT) {
      *gameOver = true;
      printf("It's a draw!\n");
      return 0; // Draw
  }

  return 0; // No win found
}