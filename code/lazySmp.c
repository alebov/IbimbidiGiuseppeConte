#pragma once

#include "lazySmp.h"
#include "moves.h"
#include "board.h"
#include "minmax.h"
#include "main.h"
#include "basicEvaluation.h"
#include "sort.h"
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "TT.h"

Move currentMove;
int globalDepth=0;


// Returns the current best move to do
Move getbestMove(){
    return currentMove;
}

void putFirst(Move* moves, int dim, Move bestMove){
    int i;
     for (i = 0; i < dim; i++) {
            if (moves[i] == bestMove) {
                swap(&moves[i],&moves[0]);
            }
        }
}

// It executes the swap of tho moves
void swap(Move * move1, Move* move2){
    Move temp;
    temp=*move1;
    *move1=*move2;
    *move2=temp;
}

// It randomizes moves array
void randomizeMoves(Move moves[], int dim){
    int i;
    int j;
    srand(time(NULL)); 
    for(j=0;j<20;j++){
        for(i=0;i<dim-1;i++){
            if((rand() & 3)!=1){
                swap(&moves[i],&moves[i+1]);
            }
        }
    }
}

// Updates the current move to do with the best move found at each depth
void *iterativeDeepeningMax(void *arg){
    struct argIterativeDipening *argomenti;
    argomenti =(struct argIterativeDipening*)arg;
    int i;
    globalDepth=0;
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(i=1; i<MAXDEPTH; i++){
        alphaBetaMax(argomenti->board, i, argomenti->thread);
        incDepth();
    }
}


// Updates the current move to do with the best move found at each depth
void *iterativeDeepeningMin(void *arg){
    struct argIterativeDipening *argomenti;
    argomenti =(struct argIterativeDipening*)arg;
    int i;
    globalDepth=0;
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(i=1; i<MAXDEPTH; i++){
        alphaBetaMin(argomenti->board, i, argomenti->thread);
        incDepth();
    }
}

// Returns the move the white player have to do at the end of the turns
Move lazySmpWhite(BoardState board, int tempo, int numThreads){
    Move move;
    int i;
    pthread_t search[numThreads];
    struct argIterativeDipening arg[numThreads];
    Hash h= stringHash(&board);
    
    // Creates THREAD sons processes and it starts to search
    for(i=0;i<numThreads;i++){        
        arg[i].board = board;
        arg[i].thread = i;
        pthread_create(&search,NULL, iterativeDeepeningMax,&arg[i]);
    }
    start();
    incTurn(); //STATS
    // The father process wait the end of the turn and take the move found by the son
    sleep(tempo);
    stop();
    return currentMove;
}


// Returns the move the black player have to do at the end of the turns
Move lazySmpBlack(BoardState board, int tempo, int numThreads){
    Move move;
    int i;
    pthread_t search[numThreads];
    struct argIterativeDipening arg[numThreads];
    // Creates THREAD sons processes and it starts to search
    for(i=0;i<numThreads;i++){        
        arg[i].board = board;
        arg[i].thread = i;
        pthread_create(&search,NULL, iterativeDeepeningMin,&arg[i]);
    }
    start();
    incTurn(); //STATS
    // The father process wait the end of the turn and take the move found by the son
    sleep(tempo);
    stop();
    return currentMove;
}

// Evaluation of the best white move
void alphaBetaMax(BoardState board, int depth, int thread){
    Move bestMove;
    BoardState nextBoard;
    Move moves[MAXMOVES];
    int movesNumber;
    int i = 0, alpha=BLACK_WIN-depth+1, beta=WHITE_WIN+depth-1, boardValue;
    Hash h=stringHash(&board);
    movesNumber = getPossibleMovesWhite(board, moves);
    sortMovesRelativeHistory(moves,movesNumber,thread);
    Entry entry;
    push(board, thread); //add the board to the branch 
    //randomizeMoves(moves,movesNumber);
    for(i = 0; i < movesNumber; i++){
        //entry=findWhite(h);
        /*
        if(entry.depth>depth && entry.type!=EMTPY_ENTRY)
            return;
            */
        if(entry.depth==depth && entry.value>=alpha && entry.type!=EMTPY_ENTRY){
            alpha=entry.value;
            bestMove=entry.bestMove;
        }    
        // Apply a possible move to generate the next board
        nextBoard = moveWhite(board, moves[i]);
        exploredMove(); // STATS
        // Minimum black value of the next board
        boardValue = min(nextBoard, alpha, beta, depth-1, thread);
        if(alpha < boardValue){
            alpha = boardValue;
            bestMove = moves[i];
            addFirstWhite(h,alpha,depth,moves[i]);
        }
    }
    printf("Thread %d:terminata depth %d mossa -> ",thread,depth);
    printMove(bestMove);
    printf(" valore:%d\n",alpha);
    addWhite(h,alpha,depth,bestMove,EXACT_ENTRY);
    pop(thread); //remove the board from the branch
     if(globalDepth<=depth){
        globalDepth=depth;
        currentMove=bestMove;
        }
}

// Evaluation of the best black move
void alphaBetaMin(BoardState board, int depth, int thread){
    Move bestMove;
    BoardState nextBoard;
    Move moves[MAXMOVES];
    int movesNumber;
    int i = 0, alpha=BLACK_WIN-depth+1, beta=WHITE_WIN+depth-1, boardValue;
    Hash h=stringHash(&board); 
    movesNumber = getPossibleMovesBlack(board, moves);
    push(board, thread);                            //add the board to the branch
    randomizeMoves(moves,movesNumber);
    Entry entry;
    //sortMovesRelativeHistory(moves,movesNumber,thread);
    for(i=0; i < movesNumber; i++){
        entry=findBlack(h);
        /*
        if(entry.depth>depth && entry.type!=EMTPY_ENTRY){
            bestMove=entry.bestMove;
            return;
        }  
        */
        if(entry.depth==depth && entry.value<=beta && entry.type!=EMTPY_ENTRY){
            beta=entry.value;
            bestMove=entry.bestMove;
        }    
        // Apply a possible move to generate the next board
        nextBoard = moveBlack(board, moves[i]);
        exploredMove(); // STATS
        // Maximum white value of the next board
        boardValue = max(nextBoard, alpha, beta, depth-1, thread);
        if(beta > boardValue){
            beta = boardValue;
            bestMove = moves[i];
            addFirstBlack(h,beta,depth,moves[i]);
        }
    }
    printf("Thread %d: terminata depth %d mossa -> ",pthread_self(),depth);
    printMove(bestMove);
    printf(" valore: %d  \n",beta);
    pop(thread); //remove the board from the branch
    if(globalDepth<=depth){
        globalDepth=depth;
        currentMove=bestMove;
        }
    addBlack(h,beta,depth,bestMove,EXACT_ENTRY);
}