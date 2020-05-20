#include "stats.h"
#include <stdint.h>

int cutoffs = 0;
int64_t exploredMoves = 0;
int playedTurns=0;
int depthCoutner=0;
clock_t begin;
double time_spent;
int maxTime;

void setTime(int maxtime){
  maxTime=maxtime;
}

void incDepth(){
  depthCoutner++;
}

void incTurn(){
  playedTurns++;
}

void cutoff(){
  cutoffs++;
}

void exploredMove(){
  exploredMoves++;
}

void beginExecution(){
  begin = clock();
}

void endExecution(){
  int end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}

void printGameStats(){
    int speed= exploredMoves / (playedTurns * maxTime );
    int cutSecond=cutoffs / (playedTurns * maxTime );
    double depthMedia= (double) (depthCoutner*1.0) / (playedTurns*1.0);
    printf("[STATS] \tnode/second: %d\n", speed);
    printf("[STATS] \tcut/second: %d\n", cutSecond);
    printf("[STATS] \tdepth media: %f\n", depthMedia);
}

void printStats(int depth, Move bestMove, int beta){
  printf("[STATS] Performing min-max at depth %d.\tSelected move: ", depth);
  printMove(bestMove);
}
