#pragma once
#include "moves.h"

#define WIDTH 3 //TABLE WIDTH
#define WHITE_TURN 0
#define BLACK_TURN 1

typedef struct {
    int dim;
    Move moves[WIDTH];
    int index;
} KillerMoves;


void initKillerMoves();
void addKillerMove(Move move, int depth, int colour, int thread);   //add a killer move to the table
int isKillerMoveValidWhite(Move move, BoardState board);     //return true if the killer move is valid in the board
int isKillerMoveValidBlack(Move move, BoardState board);     //return true if the killer move is valid in the board
Move* getKillerMoves(int depth, int colour ,int* dim, int thread);
int isKiller(int depth, Move move, int colour, int thread);
void printKiller();

//NB:isValid() function of moves.h check does not cross camp
//NB isKillerMoveValid check if moves cross a checker
