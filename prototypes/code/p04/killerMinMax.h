#pragma once


#include "moves.h"
#include "killerHeuristic.h"

#define MAXMOVES 256
#define MAXDEPTH 20
#define MAX(a,b) a>b ? a : b
#define MIN(a,b) a<b ? a : b

struct argIterativeDipening{
    BoardState board;
};



Move alphaBetaMaxKiller(BoardState board,int depth);
Move alphaBetaMinKiller(BoardState board,int depth);
Move getMoveBlackKiller(BoardState board, int tempo);
Move getMoveWhiteKiller(BoardState board, int tempo);