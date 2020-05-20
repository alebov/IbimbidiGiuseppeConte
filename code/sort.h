#pragma once
#include "board.h"
#include "moves.h"

#define TOT_16_COMBINATIONS 65536

typedef struct{
    Move move;
    int value;
}HistoryValue;

typedef struct{
    Move move;
    double value;
}RelativeHistoryValue;

// Sort History Heuristic
void setHistoryValueTable();
void updateHistoryValue(Move move, int depth, int thread);


// Sort Relative History Heuristic
void setRelativeHistoryValueTable();
void updateRelativeHistoryValue(Move move, int thread);
void sortMovesRelativeHistory(Move* moves, int size, int thread);
