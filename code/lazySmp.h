#pragma once

#include "moves.h"
#include "board.h"

struct argIterativeDipening{
    BoardState board;
    int thread;
};

Move lazySmpWhite(BoardState board, int tempo, int numThreads);
Move lazySmpBlack(BoardState board, int tempo, int numThreads);
void randomizeMoves(Move moves[], int dim);