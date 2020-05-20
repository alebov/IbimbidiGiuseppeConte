#pragma once
#include "board.h"
#include "hash.h"
#define MAXMOVES 256
#define MAXDEPTH 20
#define MAX(a,b) a>b ? a : b
#define MIN(a,b) a<b ? a : b

struct argIterativeDipening{
    BoardState board;
};

int max(BoardState board, int alpha, int beta, int depth);
int min(BoardState board, int alpha, int beta, int depth);
Move alphaBetaMax(BoardState board,int depth);
Move alphaBetaMin(BoardState board,int depth);
Move getMoveBlack(BoardState board, int tempo);
Move getMoveWhite(BoardState board, int tempo);
