#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "gameFunctions.h"


int startGame(int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int *currentPlayer, bool *gameOver, int *winner, int *movesMade, int *gameMode) {
  // Initialize the game board
  for (int r = 0; r < BOARD_HEIGHT; r++) {
    for (int c = 0; c < BOARD_WIDTH; c++) {
      board[r][c] = EMPTY;
    }
  }

  // Initialize moves array
  for (int i = 0; i < MAX_NUMBER_OF_MOVES; i++) {
    moves[i] = 0;
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

int dropPiece(int column, int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int currentPlayer, int *movesMade) {
  // Validate column index
  if (column < 0 || column >= BOARD_WIDTH) {
    printf("Invalid column number. Please choose between 0 and %d.\n", BOARD_WIDTH - 1);
    return -1; // Indicate invalid column
  }

  // Check if the column is full
  if (board[0][column] != EMPTY) {
    printf("Column %d is full. Please choose another column.\n", column);
    return -2; // Indicate column is full
  }
  // Find the lowest empty row in the column
  int row;
  for (row = BOARD_HEIGHT - 1; row >= 0; row--) {
    if (board[row][column] == EMPTY) {
      break;
    }
  }
  // Place the piece in the board
  board[row][column] = currentPlayer;
  // Store the move
  moves[*movesMade] = column + 1; // Store the move (1-indexed)
  (*movesMade)++;

  return row; // Return the row where the piece was placed
  // Note: The row is returned to check for a win in the checkWin function
}



int checkWin(int row, int column, int board[BOARD_HEIGHT][BOARD_WIDTH], int currentPlayer, bool *gameOver, int movesMade) {
  int player = currentPlayer;
  int count = 0;

  // Check horizontal
  for (int c = 0; c < BOARD_WIDTH; c++) {
    if (board[row][c] == player) {
      count++;
      if (count == WINNING_LENGTH) {
        *gameOver = true;
        return player;
      }
    } else {
      count = 0;
    }
  }

  // Check vertical
  count = 0;
  for (int r = 0; r < BOARD_HEIGHT; r++) {
    if (board[r][column] == player) {
      count++;
      if (count == WINNING_LENGTH) {
        *gameOver = true;
        return player;
      }
    } else {
      count = 0;
    }
  }

  // Check diagonal (bottom-left to top-right)
  count = 0;
  for (int i = -WINNING_LENGTH + 1; i < WINNING_LENGTH; i++) {
    int r = row + i;
    int c = column + i;
    if (r >= 0 && r < BOARD_HEIGHT && c >= 0 && c < BOARD_WIDTH) {
      if (board[r][c] == player) {
        count++;
        if (count == WINNING_LENGTH) {
          *gameOver = true;
          return player;
        }
      } else {
        count = 0;
      }
    }
  }

  // Check diagonal (top-left to bottom-right)
  count = 0;
  for (int i = -WINNING_LENGTH + 1; i < WINNING_LENGTH; i++) {
    int r = row - i;
    int c = column + i;
    if (r >= 0 && r < BOARD_HEIGHT && c >= 0 && c < BOARD_WIDTH) {
      if (board[r][c] == player) {
        count++;
        if (count == WINNING_LENGTH) {
          *gameOver = true;
          return player;
        }
      } else {
        count = 0;
      }
    }
  }

  // Check for a draw
  if (movesMade == MAX_NUMBER_OF_MOVES) {
    *gameOver = true;
    return EMPTY; // Indicate a draw
  }

  return EMPTY; // No winner yet
  
}

int reconstructBoard( int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int movesMade) {
  // Empty the board 
  for (int r = 0; r < BOARD_HEIGHT; r++) {
    for (int c = 0; c < BOARD_WIDTH; c++) {
      board[r][c] = EMPTY;
    }
  }

  // Reconstruct the board based on moves
  for (int i = 0; i < movesMade; i++) {
    int column = moves[i] - 1; // Convert to 0-indexed
    // Find the lowest empty row in the column
    int row;
    for (row = BOARD_HEIGHT - 1; row >= 0; row--) {
      if (board[row][column] == EMPTY) {
        break;
      }
    }
    // Place the piece in the board
    board[row][column] = (i % 2 == 0) ? PLAYER1 : PLAYER2; // Alternate players
  }
  return 0;
}