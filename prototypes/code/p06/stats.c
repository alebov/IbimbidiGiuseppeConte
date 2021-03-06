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
    printf("[STATS] \tplayed turn: %d\n", playedTurns);
    printf("[STATS] \tdepth counter: %d\n", depthCoutner);
    printf("[STATS] \tdepth media: %f\n", depthMedia);
}

void printStats(int depth, Move bestMove, int beta){
  printf("[STATS] Performing min-max at depth %d.\n", depth);
  printf("[STATS] \tSelected move: ");
  printMove(bestMove);
  printf(" with value %d\n",beta);
  printf("[STATS] \tExplored moves: %d\n", exploredMoves);
  printf("[STATS] \tAlpha-beta cut offs: %d\n", cutoffs);
  printf("[STATS] \tExecution time (seconds): %f\n", time_spent);
  printf("[STATS] \tExplored moves per second: %f\n", exploredMoves/time_spent);
}
