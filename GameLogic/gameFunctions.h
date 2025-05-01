#define MAX_NUMBER_OF_MOVES 42
#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define WINNING_LENGTH 4
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2

int startGame(int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int *currentPlayer, bool *gameOver, int *winner, int *movesMade, int *gameMode);
int dropPiece(int column, int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int currentPlayer, int *movesMade);
int checkWin(int row, int column, int board[BOARD_HEIGHT][BOARD_WIDTH], int currentPlayer, bool *gameOver, int movesMade);
int reconstructBoard(int moves[MAX_NUMBER_OF_MOVES], int board[BOARD_HEIGHT][BOARD_WIDTH], int movesMade);
