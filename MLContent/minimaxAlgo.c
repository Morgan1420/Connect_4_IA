#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>
#include <fcntl.h>

#include "../GameLogic/gameFunctions.h"
#include "../GameLogic/boardDisplay.h"
#include "minimaxAlgo.h"

int min(int a, int b) {
  return (a < b) ? a : b;
}
int max(int a, int b) {
  return (a > b) ? a : b;
}


int getBestMove(int moves[MAX_NUMBER_OF_MOVES],
                int board[BOARD_HEIGHT][BOARD_WIDTH],
                int depth,
                int currentPlayer,
                int movesMade,
                int rank,
                int size)
{
    int bestMove = -1;
    int bestScore = -1000;
    int localScores[BOARD_WIDTH] = {0};  // puntuaciones que calcula este proceso
    int globalScores[BOARD_WIDTH] = {0}; // acumulado en root

    // Copias temporales para simular movimientos
    int tempMoves[MAX_NUMBER_OF_MOVES];
    int tempMovesMade;

    // Cada proceso explora columnas c = rank, rank+size, rank+2*size, …
    for (int c = rank; c < BOARD_WIDTH; c += size) {
        // Restaurar estado inicial
        memcpy(tempMoves, moves, movesMade * sizeof(int));
        tempMovesMade = movesMade;

        int columnScore;

        if (board[0][c] == EMPTY) {
            int row = dropPiece(c, tempMoves, board, currentPlayer, &tempMovesMade);
            bool gameOver;
            if (checkWin(row, c, board, currentPlayer, &gameOver, tempMovesMade) > 0) {
                // Victoria inmediata
                columnScore = MAX_NUMBER_OF_MOVES - depth;
            } else {
                columnScore = minimax(tempMoves, board, depth - 1,
                                      currentPlayer, false, tempMovesMade);
            }
            reconstructBoard(moves, board, movesMade);
        } else {
            columnScore = -1000;
        }

        localScores[c] = columnScore;
    }

    // Sumar todas las puntuaciones locales en globalScores (en rank 0)
    MPI_Reduce(localScores, globalScores,
               BOARD_WIDTH, MPI_INT, MPI_SUM,
               0, MPI_COMM_WORLD);

    // El proceso 0 elige el mejor movimiento
    if (rank == 0) {
        for (int c = 0; c < BOARD_WIDTH; c++) {
            if (globalScores[c] > bestScore) {
                bestScore = globalScores[c];
                bestMove = c;
            }
        }
    }

    // Todos reciben el movimiento ganador
    MPI_Bcast(&bestMove, 1, MPI_INT, 0, MPI_COMM_WORLD);

    return bestMove;
}


int minimax(int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int depth, int currentPlayer, bool maximizingPlayer, int movesMade) {
  if (depth == 0){
    // If we reach the maximum depth, return the score
    return 0; // Neutral score
  }
  
  int bestScore = maximizingPlayer ? -1000 : 1000;

  // Create some temporary variables so that we don't loose the original state
  int tempMoves[MAX_NUMBER_OF_MOVES];
  int tempMovesMade;
  int tempCurrentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;

  if (maximizingPlayer == true){
    bestScore = -1000; // Initialize to a very low value for maximizing player

    for (int c = 0; c < BOARD_WIDTH; c++) {
      // Retrieve the moves made at the beginning
      for(int i = 0; i < movesMade; i++) {
        tempMoves[i] = moves[i];
      }

      // Retrieve the number of moves made at the beginning
      tempMovesMade = movesMade;

      // Create a variable to store the column value
      int columnScore;

      // See if the column is not full
      if(board[0][c] == EMPTY) {
        // Simulate the move
        int row = dropPiece(c, tempMoves, board, tempCurrentPlayer, &tempMovesMade);

        // Give a Score to the move
        bool gameOver;
        if (checkWin(row, c, board, tempCurrentPlayer, &gameOver, tempMovesMade) > 0) {
          // If we win with this move, we assign a high Score to the column
          columnScore = MAX_NUMBER_OF_MOVES - depth; // Assign a high value for winning move
        } else {
          // Call minimax to evaluate the move
          columnScore = minimax(tempMoves, board, depth - 1, tempCurrentPlayer, false, tempMovesMade); // Neutral move
        }

        // Reconstruct the original board
        reconstructBoard(moves, board, movesMade);

      } else {
        // If the column is full, assign a low Score
        columnScore = -1000; 
      }
      
      // Check if the column Score is greater than the best Score
      if (columnScore > bestScore) {
        bestScore = columnScore;
      }
    }
  }else{ // minimizing player
    bestScore = 1000; // Initialize to a very high value for minimizing player

    for (int c = 0; c < BOARD_WIDTH; c++) {
      // Retrieve the moves made at the beginning
      for(int i = 0; i < movesMade; i++) {
        tempMoves[i] = moves[i];
      }

      // Retrieve the number of moves made at the beginning
      tempMovesMade = movesMade;

      // Create a variable to store the column value
      int columnScore;

      // See if the column is not full
      if(board[0][c] == EMPTY) {
        // Simulate the move
        int row = dropPiece(c, tempMoves, board, tempCurrentPlayer, &tempMovesMade);

        // Give a Score to the move
        bool gameOver;
        if (checkWin(row, c, board, tempCurrentPlayer, &gameOver, tempMovesMade) > 0) {
          // If we win with this move, we assign a low Score to the column
          columnScore = - MAX_NUMBER_OF_MOVES + depth; // Assign a low value for winning move
        } else {
          // Call minimax to evaluate the move
          columnScore = minimax(tempMoves, board, depth - 1, tempCurrentPlayer, true, tempMovesMade); // Change maximizingPlayer to true
        }

        // Reconstruct the original board
        reconstructBoard(moves, board, movesMade);

      } else {
        // If the column is full, assign a high Score
        columnScore = 1000; 
      }
      
      // Check if the column Score is lesser than the best Score
      if (columnScore < bestScore) {
        bestScore = columnScore;
      }
    }
  }

  return bestScore; // Return the best Score
}
