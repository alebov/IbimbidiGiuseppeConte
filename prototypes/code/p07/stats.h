#pragma once
#include <time.h>
#include "moves.h"

void cutoff();
void exploredMove();
void beginExecution();
void endExecution();
void printStats(int depth, Move bestMove, int beta);
void incDepth();
void incTurn();
void setTime(int maxTime);
void printGameStats();