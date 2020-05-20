#include "board.h"
#include "moves.h"
#include <stdio.h>
#include <stdlib.h>


void printBoard(BoardState board) {
	unsigned int i, j, cell;
	printf("\n\n      ");
	for (i = 0; i < 9; i++) {
		printf(" %c  ", 65+i);
	}
	printf("\n      ");
	for (i = 0; i < 9; i++) {
		printf("----");
	}
	printf("\n");

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (j == 0)
				printf("%d  |   ", i+1);

			cell = GET_CELL_STATE(board.Board, i, j);

			switch(cell) {
				case 0: printf("-   "); break;
				case 2: printf("O   "); break;
				case 3: printf("K   "); break;
				case 1: printf("X   "); break;
				default: printf("-   "); break;
			}

			if (j == 8)
				printf("|  %d", i+1);
		}
		printf("\n");
		if (i < 8)
			printf("\n");
	}

	printf("      ");
	for (i = 0; i < 9; i++) {
		printf("----");
	}
	printf("\n      ");
	for (i = 0; i < 9; i++) {
		printf(" %c  ", 65+i);
	}
	printf("\n");

	printf("\n\n");
}





void initBoard(BoardState* board) {
	board->Board[0] = 0b00000000000000000000010101000000U;
	board->Board[1] = 0b00000000000000000000000100000000U;
	board->Board[2] = 0b00000000000000000000001000000000U;
	board->Board[3] = 0b00000000000000010000001000000001U;
	board->Board[4] = 0b00000000000000010110101110100101U;
	board->Board[5] = 0b00000000000000010000001000000001U;
	board->Board[6] = 0b00000000000000000000001000000000U;
	board->Board[7] = 0b00000000000000000000000100000000U;
	board->Board[8] = 0b00000000000000000000010101000000U;
}


void initEmptyBoard(BoardState* board) {
	board->Board[0] = 0;
	board->Board[1] = 0;
	board->Board[2] = 0;
	board->Board[3] = 0;
	board->Board[4] = 0;
	board->Board[5] = 0;
	board->Board[6] = 0;
	board->Board[7] = 0;
	board->Board[8] = 0;
}


int isEqual(BoardState board1, BoardState board2) {
    return
        board1.Board[0] == board2.Board[0] &&
        board1.Board[1] == board2.Board[1] &&
        board1.Board[2] == board2.Board[2] &&
        board1.Board[3] == board2.Board[3] &&
        board1.Board[4] == board2.Board[4] &&
        board1.Board[5] == board2.Board[5] &&
        board1.Board[6] == board2.Board[6] &&
        board1.Board[7] == board2.Board[7] &&
        board1.Board[8] == board2.Board[8];
}