#include "board.h"
#include "newEvaluation.h"
#include "moves.h"
#include "killerMinMax.h"
#include <pthread.h>
#include "stats.h"
#include "movesStack.h"
#include "stdio.h"

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
    currentMove=0;
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(i=1; i<MAXDEPTH; i++){
        currentMove=alphaBetaMaxKiller(argomenti->board, i);
        incDepth();
    }
}


// Updates the current move to do with the best move found at each depth
void *iterativeDeepeningMin(void *arg){
    struct argIterativeDipening *argomenti;
    argomenti =(struct argIterativeDipening*)arg;
    int i;
    currentMove=0;
    // Starts searching by depth 1 and finds a move for each subseguent depth
    for(i=1; i<MAXDEPTH; i++){
        currentMove=alphaBetaMinKiller(argomenti->board, i);
        incDepth();
    }
}

// Returns the move the white player have to do at the end of the turns
Move getMoveWhiteKiller(BoardState board, int tempo){
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
    printKiller();
    stop();
    //pthread_cancel(search);
    return move;
}

// Returns the move the black player have to do at the end of the turns
Move getMoveBlackKiller(BoardState board, int tempo){
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
    printKiller();
    stop();
    //pthread_cancel(search);
    return move;
}


// Evaluation of the best white move
Move alphaBetaMaxKiller(BoardState board, int depth){
    Move bestMove;
    BoardState nextBoard;
    Move moves[MAXMOVES];
    int movesNumber;
    int i = 0, alpha=BLACK_WIN-depth+1, beta=WHITE_WIN+depth-1, boardValue;
    Move* killerMoves;
    int killerNumber;
    //try first the killer moves if valid
   killerMoves=getKillerMoves(depth,WHITE_TURN, &killerNumber);
    for(i=0; i < killerNumber;i++)
        if(isKillerMoveValid(killerMoves[i],board)){
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, killerMoves[i]);
            exploredMove(); // STATS
            boardValue = min(nextBoard, alpha, beta, depth-1);
            if(alpha < boardValue){
                    alpha = boardValue;
                    bestMove = killerMoves[i];
            }
            if(alpha >= beta){
                //KILLER CUTOF
                cutoff(); // STATS
                return bestMove;
             }
        }
    movesNumber = getPossibleMovesWhite(board, moves);
    beginExecution(); // STATS
    for(i = 0; i < movesNumber; i++){
        if(!isKiller(depth,moves[i],WHITE_TURN)){
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
            addKillerMove(bestMove,depth,WHITE_TURN);
            cutoff(); // STATS
            break;
        }

        }
    }
    printf("terminata depth %d mossa: ",depth);
    printMove(bestMove);
    printf(" valore:%d\n",alpha);
    return bestMove;
}

// Evaluation of the best black move
Move alphaBetaMinKiller(BoardState board, int depth){
    Move bestMove;
    BoardState nextBoard;
    Move moves[MAXMOVES];
    int movesNumber;
    int i = 0, alpha=BLACK_WIN-depth+1, beta=WHITE_WIN+depth-1, boardValue;
    Move * killerMoves;
    int killerNumber;
    //try first the killer moves if valid
    killerMoves=getKillerMoves(depth,BLACK_TURN,&killerNumber);
    for(i=0; i < killerNumber;i++)
        if(isKillerMoveValid(killerMoves[i],board)){
            // Apply a possible move to generate the next board
            nextBoard = moveBlack(board, killerMoves[i]);
            exploredMove(); // STATS
            boardValue = max(nextBoard, alpha, beta, depth-1);
            if(beta > boardValue){
                    beta = boardValue;
                    bestMove = killerMoves[i];
            }
            if(alpha >= beta){
                //KILLER CUTOF
                cutoff(); // STATS
                return bestMove;
             }
        }
    movesNumber = getPossibleMovesBlack(board, moves);
    beginExecution(); // STATS
    for(i = 0; i < movesNumber; i++){
         if(!isKiller(depth,moves[i],BLACK_TURN)){
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
            addKillerMove(bestMove,depth,BLACK_TURN);
            cutoff(); // STATS
            break;
            }
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
    if(isDraw(board))           //draw check
        return 0;
    // If max depth reached or if black won with the last move, return.
    if(depth == 0){
        return newEvaluateForBlack(board);
	}
    if(finish)
        pthread_exit(NULL);
    push(board);
    Move * killerMoves;
    int killerNumber;
    //try first the killer moves if valid
    killerMoves=getKillerMoves(depth,WHITE_TURN,&killerNumber);
    for(i=0; i < killerNumber;i++)
        if(isKillerMoveValid(killerMoves[i],board)){
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, killerMoves[i]);
            exploredMove(); // STATS
            boardValue = min(nextBoard, alpha, beta, depth-1);
             maxValue = MAX(boardValue, maxValue);
             alpha = MAX(alpha, maxValue);
             if(alpha >= beta){
                pop();
                cutoff(); // STATS
                return maxValue;
             }
        }
    movesNumber = getPossibleMovesWhite(board, moves);
    for(i = 0; i < movesNumber; i++){
        if(!isKiller(depth,moves[i],WHITE_TURN)){
            // Apply a possible move to generate the next board
            nextBoard = moveWhite(board, moves[i]);
            exploredMove(); // STATS
            // Minimum black value of the next board
            boardValue = min(nextBoard, alpha, beta, depth-1);
            // White is looking for the maximum
            maxValue = MAX(boardValue, maxValue);
            alpha = MAX(alpha, maxValue);
            if(alpha >= beta){
                addKillerMove(moves[i],depth,WHITE_TURN);
                cutoff(); // STATS
                break;
            }
        }
    }
    pop();
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
    // If max depth reached,the board is a draw, or if white won with the last move, return.
    if(whiteGoalTest(board))
        return WHITE_WIN + depth;
    if(isDraw(board))           //draw check
        return 0; 
    if(depth == 0){
        return newEvaluateForWhite(board);
    }
    if(finish)
        pthread_exit(NULL);
    Move *killerMoves;
    int killerNumber;
    push(board);
    //try first the killer moves if valid
    killerMoves=getKillerMoves(depth,BLACK_TURN,&killerNumber);
    for(i=0; i < killerNumber;i++)
        if(isKillerMoveValid(killerMoves[i],board)){
            // Apply a possible move to generate the next board
            nextBoard = moveBlack(board, killerMoves[i]);
            exploredMove(); // STATS
            boardValue = max(nextBoard, alpha, beta, depth-1);
            minValue = MIN(boardValue, minValue);
            beta = MIN(beta, minValue);
            if(alpha >= beta){
                //KILLER CUTOF
                pop();
                cutoff(); // STATS
                return beta;
             }
        }
    movesNumber = getPossibleMovesBlack(board, moves);
    for(i = 0; i < movesNumber; i++){
        if(!isKiller(depth,moves[i],BLACK_TURN)){
            // Apply a possible move to generate the next board
        nextBoard = moveBlack(board, moves[i]);
        exploredMove(); // STATS
        // Maximum white value of the next board
        boardValue = max(nextBoard, alpha, beta, depth-1);
        // Black is looking for the minimum
        minValue = MIN(boardValue, minValue);
        beta = MIN(beta, minValue);
        if(alpha >= beta){
            addKillerMove(moves[i],depth,BLACK_TURN);
            cutoff(); // STATS
            break;
            }
        }  
    }
    pop();
    return minValue;
}
