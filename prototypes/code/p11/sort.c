#pragma once
#include "board.h"
#include "moves.h"
#include "sort.h" 
#include <stdlib.h>

long int valuesHistoryTable[TOT_16_COMBINATIONS];
long int valuesRelativeHistoryTable[TOT_16_COMBINATIONS];

//HISTORY HEURISTIC
// Initializes all History Heuristic values to 0
void setHistoryValueTable(){
    int i;
    for (i=0;i<TOT_16_COMBINATIONS;i++) valuesHistoryTable[i]=0;
}

// Updates move History value
void updateHistoryValue(Move move, int depth){
    valuesHistoryTable[move]=valuesHistoryTable[move]+(depth*depth);
}

// Returns the value of comparation between two HistoryValues for White player
int historyValuesComparator(const void * a, const void * b){
    return valuesHistoryTable[*((Move*)b)]-valuesHistoryTable[*((Move*) a)];
}


// Sorts the moves array using History Heuristic
void sortMovesHistory(Move* moves, int size){
    qsort(moves,size,sizeof(Move),historyValuesComparator);
}



//RELATIVE HISTORY HEURISTIC
// Initializes all History Heuristic values to 0
void setRelativeHistoryValueTable(){
    int i;
    for (i=0;i<TOT_16_COMBINATIONS;i++) valuesRelativeHistoryTable[i]=1;
}

// Updates move History value
void updateRelativeHistoryValue(Move move){
    valuesRelativeHistoryTable[move]=valuesRelativeHistoryTable[move]+1;
}

// Returns the value of comparation between two HistoryValues for White player
int relativeHistoryValuesComparator(const void * a, const void * b){
    return (valuesHistoryTable[*((Move*)b)] / valuesRelativeHistoryTable[*((Move*)b)] )- valuesHistoryTable[*((Move*) a)] / valuesRelativeHistoryTable[*((Move*)a)];
}

// Sorts the moves array using Relative History Heuristic
void sortMovesRelativeHistory(Move* moves, int size){
    qsort(moves,size,sizeof(Move),relativeHistoryValuesComparator);
}
