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


// Updates the current move to do with the best move found at each depth
void *iterativeDeepeningMax(void *arg){
    struct argIterativeDipening *argomenti;
    argomenti =(struct argIterativeDipening*)arg;
    int depth;
    int value;
    Hash h= hash(&argomenti->board);
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(depth=1; depth<MAXDEPTH; depth++){
        value=max(argomenti->board,BLACK_WIN-depth,WHITE_WIN+depth,depth);
        printf("terminata depth %d value: %d bestMove:",depth,value);
        currentMove=findWhite(h).bestMove;
        printMove(currentMove);
        printf("\n");
        incDepth();
    }
}


// Updates the current move to do with the best move found at each depth
void *iterativeDeepeningMin(void *arg){
    struct argIterativeDipening *argomenti;
    argomenti =(struct argIterativeDipening*)arg;
    int depth;
    int value;
    Hash h= hash(&argomenti->board);
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(depth=1; depth<MAXDEPTH; depth++){
       value= min(argomenti->board,BLACK_WIN-depth,WHITE_WIN+depth,depth);
        printf("terminata depth %d value: %d bestMove " ,depth,value);
        currentMove=findBlack(h).bestMove;
        printMove(currentMove);
        printf("\n");
        incDepth();
    }
}

// Returns the move the white player have to do at the end of the turns
Move getMoveWhite(BoardState board, int tempo){
    pthread_t search;
    struct argIterativeDipening arg;
    arg.board = board;
    start();
    incTurn();
    // Creates a son process and it starts to search
    pthread_create(&search,NULL, iterativeDeepeningMax,&arg);
    // The father process wait the end of the turn and take the move found by the son
    sleep(tempo);
    stop();
    //pthread_cancel(search);
    return currentMove;
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
    stop();
    //pthread_cancel(search);
    return currentMove;
}





// Black just performed a move, evaluate the best next white move
int max(BoardState board, int alpha, int beta, int depth){
    int boardValue;
    int maxValue = BLACK_WIN-depth;
    Move moves[MAXMOVES];
    int i;
    int movesNumber;
    BoardState nextBoard;
    if(depth == 0){
        return evaluation(board);
	}
    if(blackGoalTest(board))
        return BLACK_WIN -depth;
    // If max depth reached or if black won with the last move, return.
    if(isDraw(board) )          //draw check
        return 0;
    if(finish)
        pthread_exit(NULL);
    push(board);
    //TT interaction---------------------------------------------------
    Hash h=hash(&board);
    Entry entry=findWhite(h);
    Move bestMove;
    if(entry.type== EXACT_ENTRY && entry.depth>=depth){
        pop();
        return entry.value;
    }
    if(entry.type==UPPER_ENTRY && entry.depth>=depth){
        alpha=MAX(alpha,entry.value);
        if(alpha>=beta){
            cutoff();
            pop();
            return alpha;
        }
    }
    if(entry.type!=EMTPY_ENTRY){
        //else  same entry but depth>entry.depth
        nextBoard=moveWhite(board,entry.bestMove);      //evaluate the last best move(PVS)
        boardValue=min(nextBoard,alpha,beta,depth-1);
        maxValue = MAX(boardValue, maxValue);
        bestMove=entry.bestMove;
        alpha = MAX(alpha, maxValue);
        if(alpha >= beta){
            addWhite(h,maxValue,depth,bestMove, UPPER_ENTRY);
            pop();
            cutoff(); // STATS 
            return alpha;
        }
    }
    //end of TT interaction-----------------------------------------------------------
    int searchDepth;
    movesNumber = getPossibleMovesWhite(board, moves);
    sortMovesRelativeHistory(moves,movesNumber);  //SORT                           //add the board to the branch
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove){
            //if(i<4)
                searchDepth=depth-1;
           // else
           // {
            //    searchDepth=depth/2;
            //}
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, moves[i]);
            exploredMove(); // STATS
            // Minimum black value of the next board
            boardValue = min(nextBoard, alpha, beta, searchDepth);
            // White is looking for the maximum
            //maxValue = MAX(boardValue, maxValue);
            if(maxValue<boardValue){
                maxValue=boardValue;
                bestMove=moves[i];
            }
            alpha = MAX(alpha, maxValue);
            if(alpha >= beta){
                addWhite(h,alpha,depth,bestMove, UPPER_ENTRY);
                cutoff(); // STATS
                updateHistoryValue(bestMove,depth); //SORT
                pop();
                return alpha;
            }else{
                updateRelativeHistoryValue(moves[i]); //SORT
            }
        }  
    }
    addWhite(h,maxValue,depth,bestMove, EXACT_ENTRY);
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
    if(depth == 0){
        return evaluation(board);
    }
    if(whiteGoalTest(board))
        return WHITE_WIN + depth;
    
    if(isDraw(board) )          //draw check
        return 0;
    if(finish)
        pthread_exit(NULL);
    push(board);
    //TT interaction--------------------------------------------------------------------------------------
    Hash h=hash(&board);
    Entry entry=findBlack(h);
    Move bestMove; 
    if(entry.type==EXACT_ENTRY && entry.depth>=depth){  //exact node
        pop();
        return entry.value;
    }
    if (entry.type==LOWER_ENTRY && entry.depth>=depth){ //lower bound node
        beta=MIN(beta,entry.value);
        if(alpha>=beta){
            cutoff();
            pop();
            return beta;
        }
    }
    if(entry.type!=EMTPY_ENTRY){
        //else  same entry but depth>entry.depth
        bestMove=entry.bestMove;
        nextBoard=moveBlack(board,entry.bestMove);      //evaluate the last best move(PVS)
        exploredMove(); // STATS
        boardValue=max(nextBoard,alpha,beta,depth-1);
        minValue = MIN(boardValue, minValue);
        beta = MIN(beta, minValue);
        if(alpha >= beta){
            //addBlack(hash,beta,depth,bestMove,LOWER_ENTRY);
            pop();
            cutoff(); // STATS 
            return beta;
        }      
    }
    //end of TT interaction----------------------------------------------
    int searchDepth;
    movesNumber = getPossibleMovesBlack(board, moves);
    sortMovesRelativeHistory(moves,movesNumber);  //SORT
    for(i = 0; i < movesNumber; i++){
        if(moves[i]!=entry.bestMove){
           // if(i<4)
                searchDepth=depth-1;
            //else
            //{
            //    searchDepth=depth/2;
            //}
             // Apply a possible move to generate the next board
            nextBoard = moveBlack(board, moves[i]);
            exploredMove(); // STATS
            // Maximum white value of the next board
            boardValue = max(nextBoard, alpha, beta, searchDepth);
            // Black is looking for the minimum
            //minValue = MIN(boardValue, minValue);
            if(minValue>boardValue){
                minValue=boardValue;
                bestMove=moves[i];
            }
            beta = MIN(beta, minValue);
            if(alpha >= beta){
                addBlack(h,beta,depth,bestMove,LOWER_ENTRY); 
                pop();
                cutoff(); // STATS
                updateHistoryValue(bestMove,depth); //SORT
                return beta;
            }else{
                updateRelativeHistoryValue(moves[i]); //SORT
            }
        }
    }
    addBlack(h,minValue,depth,bestMove,EXACT_ENTRY);
    pop(); //remove the board from the branch
    return minValue;
}
