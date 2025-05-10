// command to execute the code
// gcc -o connect4 main.c GameLogic/gameFunctions.c GameLogic/boardDisplay.c MLContent/minimaxAlgo.c MLContent/alphaBetaAlgo.c && ./connect4

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/time.h>
#include <mpi.h>

#include "GameLogic/gameFunctions.h"
#include "GameLogic/boardDisplay.h"
#include "MLContent/minimaxAlgo.h"
#include "MLContent/alphaBetaAlgo.h"

//#define minmaxDepth 10 // Depth for Minimax algorithm (change as needed)

int moves[MAX_NUMBER_OF_MOVES];
int board[BOARD_HEIGHT][BOARD_WIDTH];
int currentPlayer;
bool gameOver;
int winner;
int movesMade;
int gameMode;
bool exitGame;

int minmaxDepth = 8; // Set depth for Minimax algorithm from command line argument or default to 10



// Player vs Player
int gameMode1() { 
  while (!gameOver) {
    displayBoard(moves, board, currentPlayer);
  
    int column;
    printf("Player %d, choose a column (0-%d): ", currentPlayer, BOARD_WIDTH - 1);
    scanf("%d", &column);
  
    // Drop the piece
    int row = dropPiece(column, moves, board, currentPlayer, &movesMade);
    if (row == -1) {
      printf("Invalid move. Try again.\n");
      continue; // Skip to the next iteration
    } else if (row == -2) {
      printf("Column is full. Try again.\n");
      continue; // Skip to the next iteration
    } else {
      // Check for a win
      if (checkWin(row, column, board, currentPlayer, &gameOver, movesMade)) {
        displayBoard(moves, board, currentPlayer);
        printf("Player %d wins!\n", currentPlayer);
        break;
      }
    
      // Switch players
      currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }
  }
}

// Player vs AI
int gameMode2(int rank, int size) {
  while (!gameOver) {
    displayBoard(moves, board, currentPlayer);
  
    int column;
    if (currentPlayer == PLAYER1) {
      printf("Player %d, choose a column (0-%d): ", currentPlayer, BOARD_WIDTH - 1);
      scanf("%d", &column);
    } else {
      // AI's turn
      column = getBestMove(moves, board, minmaxDepth, currentPlayer, movesMade, rank, size);
      printf("AI chooses column %d\n", column);
    }
  
    // Drop the piece
    int row = dropPiece(column, moves, board, currentPlayer, &movesMade);
    if (row == -1) {
      printf("Invalid move. Try again.\n");
      continue; // Skip to the next iteration
    } else if (row == -2) {
      printf("Column is full. Try again.\n");
      continue; // Skip to the next iteration
    } else {
      // Check for a win
      if (checkWin(row, column, board, currentPlayer, &gameOver, movesMade)) {
        displayBoard(moves, board, currentPlayer);
        printf("Player %d wins!\n", currentPlayer);
        break;
      }
    
      // Switch players
      currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }
  }
}

// AI vs AI
int gameMode3(int rank, int size) {
  // Implement AI vs AI logic here
  while (!gameOver) {
    //displayBoard(moves, board, currentPlayer);
  
    // AI's turn
    int column = getBestMove(moves, board, minmaxDepth, currentPlayer, movesMade, rank, size);
    // printf("AI chooses column %d\n", column); -- comented for debugging purposes
  
    // Drop the piece
    int row = dropPiece(column, moves, board, currentPlayer, &movesMade);
    if (row == -1) {
      printf("Invalid move. Try again.\n");
      continue; // Skip to the next iteration
    } else if (row == -2) {
      printf("Column is full. Try again.\n");
      continue; // Skip to the next iteration
    } else {
      // Check for a win
      if (checkWin(row, column, board, currentPlayer, &gameOver, movesMade)) {
        if (rank == 0) {
          displayBoard(moves, board, currentPlayer);
          printf("Player %d wins!\n", currentPlayer);
        }
        
        break;
      }
    
      // Switch players
      currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }
  }
}


int main(int argc, char **argv) {

  // Process the command line arguments
  minmaxDepth = argc > 1 ? atoi(argv[1]) : 8; // Set depth for Minimax algorithm from command line argument or default to 10
  if (minmaxDepth < 1 || minmaxDepth > MAX_NUMBER_OF_MOVES) {
    printf("Invalid depth. Setting to default value of 8.\n");
    minmaxDepth = 8;
  }
  // Eliminate the depth from the command line arguments
  argc--;
  argv++;

  // MPI initialization
  int rank, size, err; // Variables for MPI

  // Initialize the MPI code
  err = MPI_Init(&argc, &argv);
  if (err != MPI_SUCCESS) {
      fprintf(stderr, "MPI initialization failed!\n");
      return 1;
  }

  // Collect the rank and size of each process
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Rank %d out of %d processes\n", rank, size);

  // Start the game loop
  exitGame = false;
  while (!exitGame) {

    // Initialize the game
    startGame(moves, board, &currentPlayer, &gameOver, &winner, &movesMade, &gameMode);

    // Each mode has a loop inside the function
    if (gameMode == 1) {
      gameMode1(); // Player vs Player
    } else if (gameMode == 2) {
      gameMode2(rank, size); // Player vs AI
    } else if (gameMode == 3) {
      // Start measuring time
      struct timeval begin;
      gettimeofday(&begin, 0);
      


      // Run the AI vs AI game
      gameMode3(rank, size); // AI vs AI

      // Stop measuring time and calculate the elapsed time
      if (rank == 0) {
        struct timeval end;
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long micros = end.tv_usec - begin.tv_usec;
        long elapsed = seconds * 1000000 + micros; // Total elapsed time in microseconds
        // printf("Elapsed time: %ld microseconds\n", elapsed);
        printf("Elapsed time: %ld seconds\n", elapsed / 1000000);

        // Print the time in seconds with 5 decimal places
        printf("Elapsed time: %.5f seconds\n", (float)elapsed / 1000000);
      }
      
    }


    // Ask if the player wants to play again
    printf("Do you want to play again? (y/n): ");
    char choice = 'n';
    //scanf(" %c", &choice); // -- comented for debugging purposes
    
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

  // Finalize the MPI code
  if (MPI_Finalize() != MPI_SUCCESS) {
      fprintf(stderr, "MPI finalization failed!\n");
      return 1;
  }

  return 0;
}
    