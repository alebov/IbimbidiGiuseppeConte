#pragma once

#include"board.h"


#define WHITE_WIN 2000
#define BLACK_WIN -2000
int evaluation(BoardState board);
int blackGoalTest(BoardState board);
int whiteGoalTest(BoardState board);
