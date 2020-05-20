#pragma once

#include "board.h"

typedef uint16_t Move;
typedef int16_t ScoreType;

//FUNZIONI
int getPossibleMovesWhite(BoardState currentState, Move* moves);
int getPossibleMovesBlack(BoardState currentState, Move* moves);
void printMove(Move move);
BoardState moveWhite(BoardState board, Move move);
BoardState moveBlack(BoardState board, Move move);


//FUNZIONI DI APPOGGIO
void getMovesForBlackPiece(BoardState currentState, int row, int column, Move* moves, int* count);
void getMovesForWhitePiece(BoardState currentState, int row, int column, Move* moves, int* count);
int isMoveValid(int startRow, int startColumn, int targetRow, int targetColumn);
void checkWhiteCaptured(BoardState * state, int targetRow, int targetColumn);
void checkBlackCaptured(BoardState * state, int targetRow, int targetColumn);
void fakeBoardBlack(BoardState * state);
void fakeBoardWhite(BoardState * state);

//MACRO
#define START_ROW(m) (m >> 12U)
#define START_COLUMN(m) ((m >> 8U) & 0b0000000000001111U)
#define TARGET_ROW(m) ((m >> 4U) & 0b0000000000001111U)
#define TARGET_COLUMN(m) ((m) & 0b0000000000001111U)
#define GET_CELL_STATE(board, row, column) ((board[row] >> (16 - column * 2)) & 3U)

//MACRO DI APPOGGIO
#define APPLY_MOVE_WHITE(board, move) board[START_ROW(move)] ^= (1U << (17U - (START_COLUMN(move) << 1U))); board[TARGET_ROW(move)] ^= (1U << (17U - (TARGET_COLUMN(move) << 1U)))
#define APPLY_MOVE_BLACK(board, move) board[START_ROW(move)] ^= (1U << (16U - (START_COLUMN(move) << 1U))); board[TARGET_ROW(move)] ^= (1U << (16U - (TARGET_COLUMN(move) << 1U)))
#define APPLY_MOVE_KING(board, move) board[START_ROW(move)] ^= (3U << (16U - (START_COLUMN(move) * 2U))); board[TARGET_ROW(move)] ^= (3U << (16U - (TARGET_COLUMN(move) * 2U)))
#define CREATE_MOVE(startRow, startColumn, targetRow, targetColumn) ((((startRow << 12U) | startColumn << 8U) | targetRow << 4U) | targetColumn)
#define GETSTART(board,move) GET_CELL_STATE(board,((move >> 12U) & 0b0000000000001111U), ((move >> 8U) & 0b0000000000001111U))
#define DELETE_WHITE(board, row, column) board[row] ^= (1U << (17 - (column << 1)))
#define DELETE_BLACK(board, row, column) board[row] ^= (1U << (16 - (column << 1)))
#define DELETE_WHITE_KING(board, row, column) board[row] ^= (3U << (16 - (column << 1)))

