#pragma once

#include <stdint.h>

// 2 bit * 9 caselle -> 18 bit per riga
// unsigned long (4 byte -> 32 bit)

#define EMPTY 0U			// 00
#define BLACK 1U			// 01
#define WHITE 2U			// 10
#define WHITE_KING 3U       // 11
#define GET_CELL_STATE(board, row, column) ((board[row] >> (16 - column * 2)) & 3U)

typedef struct {
	uint32_t Board[9];
} BoardState;


void printBoard(BoardState board);
void initBoard(BoardState* board);
void initEmptyBoard(BoardState* board);
int isEqual(BoardState board1, BoardState board2);
void CreateSymmetricalHorizontal(BoardState* board, BoardState* resultBoard);
void CreateSymmetricalVertical(BoardState* board, BoardState* resultBoard);
void CreateSymmetricalUpToDown(BoardState* board, BoardState* resultBoard);
void CreateSymmetricalDownToUp(BoardState* board, BoardState* resultBoard);
void Create90degrees(BoardState* board, BoardState* resultBoard);