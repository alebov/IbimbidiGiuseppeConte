#pragma once

#include"board.h"




#define WHITE_WIN 20000		//valore della vittoria del bianco
#define BLACK_WIN (-20000)		//valore della vittoria del nero
#define DRAW 0
#define ALMOST_WHITE_WIN (WHITE_WIN - 1)

#define NORD_EST_CORNER 0U
#define NORD_OVEST_CORNER 1U
#define SUD_EST_CORNER 2U
#define SUD_OVEST_CORNER 3U

#define START_VALUE_FOR_WHITE -152
#define START_VALUE_FOR_BLACK -152

typedef int boolean;

typedef int ScoreType;

typedef struct { 
	int row;
	int column;
} Coordinate;

int evaluation(BoardState board);
int blackGoalTest(BoardState board);
int whiteGoalTest(BoardState board);
ScoreType evaluateForWhite(BoardState board);
ScoreType evaluateForBlack(BoardState board);