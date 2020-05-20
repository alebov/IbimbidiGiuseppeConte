#pragma once

#include "moves.h"
#include "board.h"

struct argIterativeDipening{
    BoardState board;
    int thread;
};

Move lazySmpWhite(BoardState board, int tempo, int numThreads);
Move lazySmpBlack(BoardState board, int tempo, int numThreads);
Move alphaBetaMax(BoardState board, int depth, int thread);
Move alphaBetaMin(BoardState board, int depth, int thread);