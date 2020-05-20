#pragma once
#include "board.h"
#include "moves.h"

#define TOT_16_COMBINATIONS 65536 // All possible combination of 16-bit integer

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
void updateHistoryValue(Move move, int depth);
void sortMovesHistory(Move* moves, int size);

// Sort Relative History Heuristic
void setRelativeHistoryValueTable();
void updateRelativeHistoryValue(Move move);
void sortMovesRelativeHistory(Move* moves, int size);
