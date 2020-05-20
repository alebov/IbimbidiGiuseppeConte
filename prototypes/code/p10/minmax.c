#pragma once
#include "stdio.h"
#include "board.h"
#include "newEvaluation.h"
#include "moves.h"
#include "minmax.h"
#include "stats.h"
#include "movesStack.h"
#include "TTbase.h"
#include "hash.h"
#include <pthread.h>
#include "killerHeuristic.h"


Move currentMove;
int finish = 0;

void start(){
    finish = 0;
}

void stop(){
    finish = 1;
}


int isKillerMove(Move move, Move * killer, int dim){
    int i=0;
    for(i; i<dim; i++)
        if(move==killer[i])
            return 1;
        return 0;
}

// Black just performed a move, evaluate the best next white move
int max(BoardState board, int alpha, int beta, int depth, int thread){
    int boardValue;
    int maxValue = BLACK_WIN-depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    BoardState nextBoard;
    if(isKingCaptured(board))
        return BLACK_WIN -depth;
    if(isDraw(board,thread))          //draw check
        return 0;
    // If max depth reached or if black won with the last move, return.
    if(depth == 0){
        return newEvaluateForBlack(board);
	}
    if(finish)
        pthread_exit(NULL);
    push(board,thread); 
    //TT interaction---------------------------------------------------
    Hash hash=stringHash(&board);
    Entry entry=findWhite(hash);
    Move bestMove;
    if(entry.type== EXACT_ENTRY && entry.depth>=depth){
        pop(thread);
        return entry.value;
    }
    if(entry.type==UPPER_ENTRY && entry.depth>=depth){
        alpha=MAX(alpha,entry.value);
        if(alpha>=beta){
            cutoff();
            pop(thread);
            return alpha;
        }
    }
    if(entry.type!=EMTPY_ENTRY){
        //else  same entry but depth>entry.depth
        nextBoard=moveWhite(board,entry.bestMove);      //evaluate the last best move(PVS)
        boardValue=min(nextBoard,alpha,beta,depth-1,thread);
        maxValue = MAX(boardValue, maxValue);
        bestMove=entry.bestMove;
        alpha = MAX(alpha, maxValue);
        if(alpha >= beta){
            //addWhite(hash,maxValue,depth,bestMove, UPPER_ENTRY);
            pop(thread);
            cutoff(); // STATS 
            return alpha;
        }
    }
    //end of TT interaction-----------------------------------------------------------
    //killer heuristic interaction-----------------------------------------------------
    Move * killerMoves;
    int killerNumber;
    //try first the killer moves if valid
    killerMoves=getKillerMoves(depth,WHITE_TURN, &killerNumber,thread);
    for(i=0; i < killerNumber;i++)
        if(isKillerMoveValidWhite(killerMoves[i],board) && killerMoves[i]!=entry.bestMove){
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, killerMoves[i]);
            exploredMove(); // STATS
            boardValue = min(nextBoard, alpha, beta, depth-1,thread);
             maxValue = MAX(boardValue, maxValue);
             alpha = MAX(alpha, maxValue);
             if(alpha >= beta){
                //addWhite(hash,alpha,depth,killerMoves[i], UPPER_ENTRY);
                pop(thread);
                incKiller(); // STATS
                return maxValue;
             }
        }
    //end of killer heuristic iteraction----------------------------------------------
    movesNumber = getPossibleMovesWhite(board, moves);
    //sortMovesHistory(moves,movesNumber);  //SORT                           //add the board to the branch
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove /*&& !isKillerMove(moves[i],killerMoves,killerNumber)*/){
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, moves[i]);
            exploredMove(); // STATS
            // Minimum black value of the next board
            boardValue = min(nextBoard, alpha, beta, depth-1,thread);
            // White is looking for the maximum
            if(maxValue<boardValue){
                maxValue=boardValue;
                bestMove=moves[i];
            }
            alpha = MAX(alpha, maxValue);
            if(alpha >= beta){
                addWhite(hash,alpha,depth,bestMove, UPPER_ENTRY);
                addKillerMove(bestMove,depth,WHITE_TURN,thread);
                cutoff(); // STATS
                //updateHistoryValue(moves[i],depth); //SORT
                pop(thread);
                return alpha;
            }
        }  
    }
    addWhite(hash,maxValue,depth,bestMove, EXACT_ENTRY);
    pop(thread);                              //remove the board from the branch
    return maxValue;
}

// White just performed a move, evaluate the best next black move
int min(BoardState board, int alpha, int beta, int depth, int thread){
    int boardValue;
    int minValue = WHITE_WIN+depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    BoardState nextBoard;
    // If max depth reached or if white won with the last move, return.
    if(whiteGoalTest(board))
        return WHITE_WIN + depth;
    if(isDraw(board,thread) )          //draw check
        return 0;
    
    if(depth == 0){
        return newEvaluateForWhite(board);
    }
    if(finish)
        pthread_exit(NULL);
    push(board,thread);
    //TT interaction--------------------------------------------------------------------------------------
    Hash hash=stringHash(&board);
    Entry entry=findBlack(hash);
    Move bestMove; 
    if(entry.type==EXACT_ENTRY && entry.depth>=depth){  //exact node
        pop(thread);
        return entry.value;
    }
    if (entry.type==LOWER_ENTRY && entry.depth>=depth){ //lower bound node
        beta=MIN(beta,entry.value);
        if(alpha>=beta){
            cutoff();
            pop(thread);
            return beta;
        }
    }
    if(entry.type!=EMTPY_ENTRY){
        //else  same entry but depth>entry.depth
        bestMove=entry.bestMove;
        nextBoard=moveBlack(board,entry.bestMove);      //evaluate the last best move(PVS)
        exploredMove(); // STATS
        boardValue=max(nextBoard,alpha,beta,depth-1,thread);
        minValue = MIN(boardValue, minValue);
        beta = MIN(beta, minValue);
        if(alpha >= beta){
            //addBlack(hash,beta,depth,bestMove,LOWER_ENTRY);
            pop(thread);
            cutoff(); // STATS 
            return beta;
        }      
    }
    //end of TT interaction----------------------------------------------
    //killer euristic iteraction----------------------------------------
    Move *killerMoves;
    int killerNumber;
    //try first the killer moves if valid
    killerMoves=getKillerMoves(depth,BLACK_TURN,&killerNumber,thread);
    for(i=0; i < killerNumber;i++)
        if(isKillerMoveValidBlack(killerMoves[i],board) && killerMoves[i]!=entry.bestMove){
            // Apply a possible move to generate the next board
            nextBoard = moveBlack(board, killerMoves[i]);
            exploredMove(); // STATS
            boardValue = max(nextBoard, alpha, beta, depth-1,thread);
            minValue = MIN(boardValue, minValue);
            beta = MIN(beta, minValue);
            if(alpha >= beta){
                //KILLER CUTOF
                //addBlack(hash,beta,depth,bestMove,LOWER_ENTRY); 
                pop(thread);
                incKiller(); //STAT
                return beta;
             }
        }
    //end killer euristic iteraction-----------------------------------------------------------------------
    movesNumber = getPossibleMovesBlack(board, moves);
    //sortMovesHistory(moves,movesNumber);  //SORT
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove /*&& !isKillerMove(moves[i],killerMoves,killerNumber)*/){
             // Apply a possible move to generate the next board
            nextBoard = moveBlack(board, moves[i]);
            exploredMove(); // STATS
            // Maximum white value of the next board
            boardValue = max(nextBoard, alpha, beta, depth-1,thread);
            // Black is looking for the minimum
            //minValue = MIN(boardValue, minValue);
            if(minValue>boardValue){
                minValue=boardValue;
                bestMove=moves[i];
            }
            beta = MIN(beta, minValue);
            if(alpha >= beta){
                addKillerMove(bestMove,depth,BLACK_TURN,thread);
                addBlack(hash,beta,depth,bestMove,LOWER_ENTRY); 
                pop(thread);
                cutoff(); // STATS
                //updateHistoryValue(moves[i],depth); //SORT
                return beta;
            }
        }
    }
    addBlack(hash,minValue,depth,bestMove,EXACT_ENTRY);
    pop(thread); //remove the board from the branch
    return minValue;
}
