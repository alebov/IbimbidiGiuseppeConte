#pragma once
#include "stdio.h"
#include "board.h"
#include "basicEvaluation.h"
#include "moves.h"
#include "minmax.h"
#include "stats.h"
#include "movesStack.h"
#include "TT.h"
#include "sort.h"
#include "hash.h"
#include <pthread.h>

Move currentMove;
int finish = 0;

void start(){
    finish = 0;
}

void stop(){
    finish = 1;
}

// Black just performed a move, evaluate the best next white move
int max(BoardState board, int alpha, int beta, int depth, int thread){
    int boardValue;
    int maxValue = BLACK_WIN-depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    BoardState nextBoard;
    int searchDepth;
    // If max depth reached or if black won with the last move, return.
    if(finish)
        pthread_exit(NULL);
    if(depth == 0){
        return evaluateForBlack(board);
	}
    if(blackGoalTest(board))
        return BLACK_WIN -depth;
    if(isDraw(board,thread) )          //draw check
        return 0;
    
    push(board,thread);
    //TT interaction---------------------------------------------------
    Hash hash=stringHash(&board);
    Entry entry=findWhite(hash);
    Move bestMove;
    if(entry.type== EXACT_ENTRY && entry.depth>=depth){
        incTFind();
        pop(thread);
        return entry.value;
    }
    if(entry.type==UPPER_ENTRY && entry.depth>=depth){
        alpha=MAX(alpha,entry.value);
        if(alpha>=beta){
            incTcut();
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
            addWhite(hash,maxValue,depth,bestMove, UPPER_ENTRY);
            pop(thread);
            cutoff(); // STATS 
            return alpha;
        }
    }
    //end of TT interaction-----------------------------------------------------------

    movesNumber = getPossibleMovesWhite(board, moves);
    sortMovesRelativeHistory(moves,movesNumber,thread);  //SORT                           //add the board to the branch
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove){
            searchDepth= i<7 ? depth -1 : depth/2;
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, moves[i]);
            exploredMove(); // STATS
            // Minimum black value of the next board
            //if(GET_CELL_STATE(board.Board,START_ROW(moves[i]),START_COLUMN(moves[i]))==WHITE_KING)
              //  boardValue = fullMin(nextBoard, alpha, beta, depth-1,thread);
            //else
                boardValue = min(nextBoard, alpha, beta, searchDepth,thread);
            // White is looking for the maximum
            //maxValue = MAX(boardValue, maxValue);
            if(maxValue<boardValue){
                maxValue=boardValue;
                bestMove=moves[i];
            }
            alpha = MAX(alpha, maxValue);
            if(alpha >= beta){
                addWhite(hash,alpha,depth,bestMove, UPPER_ENTRY);
                cutoff(); // STATS
                updateHistoryValue(bestMove,depth,thread); //SORT
                pop(thread);
                return alpha;
            }else{
                updateRelativeHistoryValue(moves[i],thread); //SORT
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
    int searchDepth;
    BoardState nextBoard;
    // If max depth reached or if white won with the last move, return.
    if(finish)
        pthread_exit(NULL);
    if(depth == 0){
        return evaluateForWhite(board);
    }
    if(whiteGoalTest(board))
        return WHITE_WIN + depth;
    
    if(isDraw(board,thread) )          //draw check
        return 0;
    
    push(board,thread);
    //TT interaction--------------------------------------------------------------------------------------
    Hash hash=stringHash(&board);
    Entry entry=findBlack(hash);
    Move bestMove; 
    if(entry.type==EXACT_ENTRY && entry.depth>=depth){  //exact node
        incTFind();
        pop(thread);
        return entry.value;
    }
    if (entry.type==LOWER_ENTRY && entry.depth>=depth){ //lower bound node
        beta=MIN(beta,entry.value);
        if(alpha>=beta){
            incTcut();
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
            addBlack(hash,beta,depth,bestMove,LOWER_ENTRY);
            pop(thread);
            cutoff(); // STATS 
            return beta;
        }      
    }
    //end of TT interaction----------------------------------------------
    movesNumber = getPossibleMovesBlack(board, moves);
    sortMovesRelativeHistory(moves,movesNumber,thread);  //SORT
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove){
            searchDepth= i<7 ? depth -1 : depth/2;
             // Apply a possible move to generate the next board
            nextBoard = moveBlack(board, moves[i]);
            exploredMove(); // STATS
            // Maximum white value of the next board
            //if(GET_CELL_STATE(board.Board,START_ROW(moves[i]),START_COLUMN(moves[i]))==WHITE_KING)
               // boardValue = fullMax(nextBoard, alpha, beta, depth-1,thread);
            //else
                 boardValue = max(nextBoard, alpha, beta, searchDepth,thread);
            // Black is looking for the minimum
            //minValue = MIN(boardValue, minValue);
            if(minValue>boardValue){
                minValue=boardValue;
                bestMove=moves[i];
            }
            beta = MIN(beta, minValue);
            if(alpha >= beta){
                addBlack(hash,beta,depth,bestMove,LOWER_ENTRY); 
                pop(thread);
                cutoff(); // STATS
                updateHistoryValue(bestMove,depth,thread); //SORT
                return beta;
            }else{
                updateRelativeHistoryValue(moves[i],thread); //SORT
            }
        }
    }
    addBlack(hash,minValue,depth,bestMove,EXACT_ENTRY);
    pop(thread); //remove the board from the branch
    return minValue;
}





//per fare una ricerca completa senza la reduction
/*

// Black just performed a move, evaluate the best next white move
int fullMax(BoardState board, int alpha, int beta, int depth, int thread){
    int boardValue;
    int maxValue = BLACK_WIN-depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    BoardState nextBoard;
    int searchDepth;
    // If max depth reached or if black won with the last move, return.
    if(finish)
        pthread_exit(NULL);
    if(depth == 0){
        return evaluateForBlack(board);
	}
    if(blackGoalTest(board))
        return BLACK_WIN -depth;
    if(isDraw(board,thread) )          //draw check
        return 0;
    
    push(board,thread);
    //TT interaction---------------------------------------------------
    Hash hash=stringHash(&board);
    Entry entry=findWhite(hash);
    Move bestMove;
    if(entry.type== EXACT_ENTRY && entry.depth>=depth){
        incTFind();
        pop(thread);
        return entry.value;
    }
    if(entry.type==UPPER_ENTRY && entry.depth>=depth){
        alpha=MAX(alpha,entry.value);
        if(alpha>=beta){
            incTcut();
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
            addWhite(hash,maxValue,depth,bestMove, UPPER_ENTRY);
            pop(thread);
            cutoff(); // STATS 
            return alpha;
        }
    }
    //end of TT interaction-----------------------------------------------------------

    movesNumber = getPossibleMovesWhite(board, moves);
    sortMovesRelativeHistory(moves,movesNumber,thread);  //SORT                           //add the board to the branch
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove){
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, moves[i]);
            exploredMove(); // STATS
            // Minimum black value of the next board
            boardValue = fullMin(nextBoard, alpha, beta, depth-1,thread);
            // White is looking for the maximum
            //maxValue = MAX(boardValue, maxValue);
            if(maxValue<boardValue){
                maxValue=boardValue;
                bestMove=moves[i];
            }
            alpha = MAX(alpha, maxValue);
            if(alpha >= beta){
                addWhite(hash,alpha,depth,bestMove, UPPER_ENTRY);
                cutoff(); // STATS
                updateHistoryValue(bestMove,depth,thread); //SORT
                pop(thread);
                return alpha;
            }else{
                updateRelativeHistoryValue(moves[i],thread); //SORT
            }
        }  
    }
    addWhite(hash,maxValue,depth,bestMove, EXACT_ENTRY);
    pop(thread);                              //remove the board from the branch
    return maxValue;
}

// White just performed a move, evaluate the best next black move
int fullMin(BoardState board, int alpha, int beta, int depth, int thread){
    int boardValue;
    int minValue = WHITE_WIN+depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    int searchDepth;
    BoardState nextBoard;
    // If max depth reached or if white won with the last move, return.
    if(finish)
        pthread_exit(NULL);
    if(depth == 0){
        return evaluateForWhite(board);
    }
    if(whiteGoalTest(board))
        return WHITE_WIN + depth;
    
    if(isDraw(board,thread) )          //draw check
        return 0;
    
    push(board,thread);
    //TT interaction--------------------------------------------------------------------------------------
    Hash hash=stringHash(&board);
    Entry entry=findBlack(hash);
    Move bestMove; 
    if(entry.type==EXACT_ENTRY && entry.depth>=depth){  //exact node
        incTFind();
        pop(thread);
        return entry.value;
    }
    if (entry.type==LOWER_ENTRY && entry.depth>=depth){ //lower bound node
        beta=MIN(beta,entry.value);
        if(alpha>=beta){
            incTcut();
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
            addBlack(hash,beta,depth,bestMove,LOWER_ENTRY);
            pop(thread);
            cutoff(); // STATS 
            return beta;
        }      
    }
    //end of TT interaction----------------------------------------------
    movesNumber = getPossibleMovesBlack(board, moves);
    sortMovesRelativeHistory(moves,movesNumber,thread);  //SORT
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove){
             // Apply a possible move to generate the next board
            nextBoard = moveBlack(board, moves[i]);
            exploredMove(); // STATS
            // Maximum white value of the next board
            boardValue = fullMax(nextBoard, alpha, beta, depth-1,thread);
            // Black is looking for the minimum
            //minValue = MIN(boardValue, minValue);
            if(minValue>boardValue){
                minValue=boardValue;
                bestMove=moves[i];
            }
            beta = MIN(beta, minValue);
            if(alpha >= beta){
                addBlack(hash,beta,depth,bestMove,LOWER_ENTRY); 
                pop(thread);
                cutoff(); // STATS
                updateHistoryValue(bestMove,depth,thread); //SORT
                return beta;
            }else{
                updateRelativeHistoryValue(moves[i],thread); //SORT
            }
        }
    }
    addBlack(hash,minValue,depth,bestMove,EXACT_ENTRY);
    pop(thread); //remove the board from the branch
    return minValue;
}


*/