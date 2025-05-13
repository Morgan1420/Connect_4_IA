#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>
#include <fcntl.h>

#include "../GameLogic/gameFunctions.h"
#include "../GameLogic/boardDisplay.h"
#include "minimaxAlgo.h"


int getBestMove(int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int depth, int currentPlayer, int movesMade, int rank, int size) {
  // Function variables
  int bestMove = -1;
  int bestScore = -1000; // Initialize to a very low value for maximizing player
  int columnScores[BOARD_WIDTH] = {0, 0, 0, 0, 0, 0}; // Initialize column values to zero

  // Create some temporary variables so that we don't loose the original state
  int tempMoves[MAX_NUMBER_OF_MOVES];
  int tempMovesMade;

  // Explore all the columns
  for(int c = rank; c < BOARD_WIDTH; c += size) {
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
      int row = dropPiece(c, tempMoves, board, currentPlayer, &tempMovesMade);

      // Give a Score to the move
      bool gameOver;
      if (checkWin(row, c, board, currentPlayer, &gameOver, tempMovesMade) > 0) {
        // If we win with this move, we assign a high Score to the column
        columnScore = MAX_NUMBER_OF_MOVES - depth; // Assign a high value for winning move
      } else {
        // Call minimax to evaluate the move
        columnScore = minimax(tempMoves, board, depth - 1, currentPlayer, false, tempMovesMade); // Neutral move
      }

      // Reconstruct the original board
      reconstructBoard(moves, board, movesMade);

    } else {
      // If the column is full, assign a low Score
      columnScore = -1000; 
    }
    
    // Store the column Score
    columnScores[c] = columnScore;
  }

  // Print the column scores -- commented for debugging purposes
  /*
  printf("Column scores: ");
  for (int i = 0; i < BOARD_WIDTH; i++) {
    printf("%d ", columnScores[i]);
  }
  printf("\n");
  */

  // All the processes send the column scores to the rank 0 process
  if (rank != 0) {
    MPI_Send(columnScores, BOARD_WIDTH, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else {
    // Rank 0 process receives the column scores from all other processes
    for (int i = 1; i < size; i++) {
      int tempColumnScores[BOARD_WIDTH];
      MPI_Recv(tempColumnScores, BOARD_WIDTH, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for (int c = 0; c < BOARD_WIDTH; c++) {
        columnScores[c] += tempColumnScores[c]; // Combine the scores
      }
    }
  }

  // The rank 0 process will find the best move
  if (rank == 0) {
    // Find the best move based on the column scores
    for (int c = 0; c < BOARD_WIDTH; c++) {
      if (columnScores[c] > bestScore) {
        bestScore = columnScores[c];
        bestMove = c;
      }
    }
  }

  // Broadcast the best move to all processes
  MPI_Bcast(&bestMove, 1, MPI_INT, 0, MPI_COMM_WORLD);

  return bestMove; // Return the best move

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
