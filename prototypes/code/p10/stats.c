#include "stats.h"
#include "main.h"
#include <stdint.h>

int cutoffs = 0;
int64_t exploredMoves = 0;
int playedTurns=0;
int depthCoutner=0;
clock_t begin;
double time_spent;
int maxTime;
int killerCut=0;

void incKiller(){
  killerCut++;
}
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

void printGameStats(int numThreads){
    int speed= exploredMoves / (numThreads * playedTurns * maxTime );
    int cutSecond=cutoffs / (numThreads * playedTurns * maxTime );
    double depthMedia = (double) (depthCoutner*1.0) / (numThreads* playedTurns *1.0);
    printf("[STATS] \tnode/second: %d\n", speed);
    printf("[STATS] \tcut/second: %d\n", cutSecond);
    printf("[STATS] \tplayed turn: %d\n", playedTurns);
    printf("[STATS] \tdepth counter: %f\n", (depthCoutner*1.0)/numThreads);
    printf("[STATS] \tdepth media: %f\n", depthMedia);
    printf("[STATS] \tkiller cut: %d\n", killerCut);
}
