#pragma once

#include "board.h"
#include "moves.h"

#define WHITE_WIN 20000		//valore della vittoria del bianco
#define BLACK_WIN (-20000)		//valore della vittoria del nero
#define DRAW 0
#define ALMOST_WHITE_WIN (WHITE_WIN - 1)

#define NORD_EST_CORNER 0U
#define NORD_OVEST_CORNER 1U
#define SUD_EST_CORNER 2U
#define SUD_OVEST_CORNER 3U

#define START_VALUE_FOR_WHITE 3
#define START_VALUE_FOR_BLACK 3

typedef enum { false, true } bool;

typedef struct { 
	int row;
	int column;
} Coordinate;


// --- FIRST LAYER FUNCTION ---
ScoreType newEvaluateForWhite(BoardState board);
ScoreType newEvaluateForBlack(BoardState board);

// ---  SECOND LAYER FUNCTION ---
//ScoreType isWhiteWin(BoardState board);
int throneState(BoardState board);
bool isKingCaptured(BoardState board);
bool isWhiteWin(Coordinate coordinate);
int isCheck(BoardState board,Coordinate coordinate);
ScoreType kingInDanger(BoardState board,Coordinate coordinate);
Coordinate findKing();
bool isThroneNear(Coordinate coordinate);
bool checkChannelVertical(int position,BoardState board);
bool checkChannelHorizontal(int position,BoardState board);
int pawnAlive(BoardState board,int color);
//ScoreType checkKing(BoardState board);
//ScoreType checkPawns(BoardState board,int legion);
/*
ScoreType whiteStrategy(BoardState board);
ScoreType isWhiteWin(BoardState board);
ScoreType isCheckMate(BoardState board);
ScoreType checkChannel(BoardState board);
ScoreType isCheck(BoardState board);
ScoreType checkPawns(BoardState board);
ScoreType blackStrategy(BoardState board);

// --- THIRD LAYER FUNCTION ---
ScoreType checkCorner(BoardState board, int corner)
int foundKing(BoardState board)
*/



