#pragma once
#include "board.h"
#include "newEvaluation.h"
#include "moves.h"
#include "minmax.h"
#include "stats.h"
#include "movesStack.h"
#include <pthread.h>

Move currentMove;
int finish = 0;

void start(){
    finish = 0;
}

void stop(){
    finish = 1;
}

// Returns the current best move to do
Move getbestMove(){
    return currentMove;
}

// Updates the current move to do with the best move found at each depth
void *iterativeDeepeningMax(void *arg){
    struct argIterativeDipening *argomenti;
    argomenti =(struct argIterativeDipening*)arg;
    int i;
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(i=1; i<MAXDEPTH; i++){
        currentMove=alphaBetaMax(argomenti->board, i);
        incDepth();
    }
}


// Updates the current move to do with the best move found at each depth
void *iterativeDeepeningMin(void *arg){
    struct argIterativeDipening *argomenti;
    argomenti =(struct argIterativeDipening*)arg;
    int i;
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(i=1; i<MAXDEPTH; i++){
        currentMove=alphaBetaMin(argomenti->board, i);
        incDepth();
    }
}

// Returns the move the white player have to do at the end of the turns
Move getMoveWhite(BoardState board, int tempo){
    Move move;
    pthread_t search;
    struct argIterativeDipening arg;
    arg.board = board;
    start();
    incTurn();
    // Creates a son process and it starts to search
    pthread_create(&search,NULL, iterativeDeepeningMax,&arg);
    // The father process wait the end of the turn and take the move found by the son
    sleep(tempo);
    move = getbestMove();
    stop();
    resetStack();
    //pthread_cancel(search);
    return move;
}

// Returns the move the black player have to do at the end of the turns
Move getMoveBlack(BoardState board, int tempo){
    Move move;
    pthread_t search;
    struct argIterativeDipening arg;
    arg.board = board;
    start();
    incTurn();
    // Creates a son process and it starts to search
    pthread_create(&search,NULL, iterativeDeepeningMin,&arg);
    // The father process wait the end of the turn and take the move found by the son
    sleep(tempo);
    move = getbestMove();
    stop();
    resetStack();
    //pthread_cancel(search);
    return move;
}

// Evaluation of the best white move
Move alphaBetaMax(BoardState board, int depth){
    Move bestMove;
    BoardState nextBoard;
    Move moves[MAXMOVES];
    int movesNumber;
    int i = 0, alpha=BLACK_WIN-depth+1, beta=WHITE_WIN+depth-1, boardValue;
    movesNumber = getPossibleMovesWhite(board, moves);                     
    for(i = 0; i < movesNumber; i++){
        // Apply a possible move to generate the next board
        nextBoard = moveWhite(board, moves[i]);
        exploredMove(); // STATS
        // Minimum black value of the next board
        boardValue = min(nextBoard, alpha, beta, depth-1);
        if(alpha < boardValue){
            alpha = boardValue;
            bestMove = moves[i];
        }
        if(alpha >= beta){
            cutoff(); // STATS
            //updateHistoryValue(bestMove,depth); //SORT
            break;
        }
    }
    printf("terminata depth %d mossa: ",depth);
    printMove(bestMove);
    printf(" valore:%d\n",alpha);
    return bestMove;
}

// Evaluation of the best black move
Move alphaBetaMin(BoardState board, int depth){
    Move bestMove;
    BoardState nextBoard;
    Move moves[MAXMOVES];
    int movesNumber;
    int i = 0, alpha=BLACK_WIN-depth+1, beta=WHITE_WIN+depth-1, boardValue;
    movesNumber = getPossibleMovesBlack(board, moves);
    
    for(i = 0; i < movesNumber; i++){
        // Apply a possible move to generate the next board
        nextBoard = moveBlack(board, moves[i]);
        exploredMove(); // STATS
        // Maximum white value of the next board
        boardValue = max(nextBoard, alpha, beta, depth-1);
        if(beta > boardValue){
            beta = boardValue;
            bestMove = moves[i];
        }
        if(alpha >= beta){
            cutoff(); // STATS
            //updateHistoryValue(bestMove,depth); //SORT
            break;
        }
    }

    printf("terminata depth %d mossa: ",depth);
    printMove(bestMove);
    printf(" valore:%d\n",beta);
    return bestMove;
}

// Black just performed a move, evaluate the best next white move
int max(BoardState board, int alpha, int beta, int depth){
    int boardValue;
    int maxValue = BLACK_WIN-depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    BoardState nextBoard;
    if(isKingCaptured(board))
        return BLACK_WIN -depth;
    if(isDraw(board)){
        //printf("pareggio a depth %d\n",depth);//draw check
        return 0;
    }    
    
    // If max depth reached or if black won with the last move, return.
    if(depth == 0){
        return newEvaluateForBlack(board);
	}
    if(finish){
        pthread_exit(NULL);
    }
        
    movesNumber = getPossibleMovesWhite(board, moves);
    //sortMovesRelativeHistory(moves,movesNumber);  //SORT
    push(board);                            //add the board to the branch
    for(i = 0; i < movesNumber; i++){
        // Apply a possible move to generate the next board
        nextBoard = moveWhite(board, moves[i]);
        exploredMove(); // STATS
        // Minimum black value of the next board
        boardValue = min(nextBoard, alpha, beta, depth-1);
        // White is looking for the maximum
        maxValue = MAX(boardValue, maxValue);
        alpha = MAX(alpha, maxValue);
        if(alpha >= beta){
            cutoff(); // STATS
            //updateHistoryValue(moves[i],depth); //SORT
            break;
        }
    }
    pop();                              //remove the board from the branch
    return maxValue;
}

// White just performed a move, evaluate the best next black move
int min(BoardState board, int alpha, int beta, int depth){
    int boardValue;
    int minValue = WHITE_WIN+depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    BoardState nextBoard;
    // If max depth reached or if white won with the last move, return.
    if(whiteGoalTest(board))
        return WHITE_WIN + depth;
        
    if(isDraw(board))  {//draw check
         return 0;
    }         
        
    if(depth == 0){
        return newEvaluateForWhite(board);
    }
    if(finish){
        pthread_exit(NULL);
    }
    movesNumber = getPossibleMovesBlack(board, moves);
    //sortMovesRelativeHistory(moves,movesNumber);  //SORT
    push(board);                            //add the board to the branch
    for(i = 0; i < movesNumber; i++){
        // Apply a possible move to generate the next board
        nextBoard = moveBlack(board, moves[i]);
        exploredMove(); // STATS
        // Maximum white value of the next board
        boardValue = max(nextBoard, alpha, beta, depth-1);
        // Black is looking for the minimum
        minValue = MIN(boardValue, minValue);
        beta = MIN(beta, minValue);
        if(alpha >= beta){
            cutoff(); // STATS
            //updateHistoryValue(moves[i],depth); //SORT
            break;
        }
    }
    pop(); //remove the board from the branch
    return minValue;
}
