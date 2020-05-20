#pragma once
#include "board.h"
#include "moves.h"
#include "sort.h" 
#include <stdlib.h>
#include "main.h"

long int valuesHistoryTable[TOT_16_COMBINATIONS][THREAD];
long int valuesRelativeHistoryTable[TOT_16_COMBINATIONS][THREAD];

//HISTORY HEURISTIC
// Initializes all History Heuristic values to 0
void setHistoryValueTable(){
    int i,j;
    for(j=0;j<THREAD;j++)
        for (i=0;i<TOT_16_COMBINATIONS;i++) 
          valuesHistoryTable[i][j]=0;
}



// Returns the value of comparation between two HistoryValues for White player
int historyValuesComparator(const void * a, const void * b){
    return valuesHistoryTable[*((Move*)b)]-valuesHistoryTable[*((Move*) a)];
}
/*
// Sorts the moves array using History Heuristic
void sortMovesHistory(Move* moves, int size){
    qsort(moves,size,sizeof(Move),historyValuesComparator);
}
*/
//RELATIVE HISTORY HEURISTIC
// Initializes all History Heuristic values to 0
void setRelativeHistoryValueTable(){
    int i,j;
    for(j=0;j<THREAD;j++)
         for (i=0;i<TOT_16_COMBINATIONS;i++) 
            valuesRelativeHistoryTable[i][j]=1;
}

// Updates move History value
void updateHistoryValue(Move move, int depth, int thread){
    valuesHistoryTable[move][thread]=valuesHistoryTable[move][thread]+(depth*depth);
}

// Updates move History value
void updateRelativeHistoryValue(Move move, int thread){
    valuesRelativeHistoryTable[move][thread]=valuesRelativeHistoryTable[move][thread]+1;
}

// Returns the value of comparation between two HistoryValues for White player
int relativeHistoryValuesComparator0(const void * a, const void * b){
    return (valuesHistoryTable[*((Move*)b)][0] / valuesRelativeHistoryTable[*((Move*)b)][0] )-  valuesHistoryTable[*((Move*) a)][0] / valuesRelativeHistoryTable[*((Move*)a)][0];
}

// Returns the value of comparation between two HistoryValues for White player
int relativeHistoryValuesComparator1(const void * a, const void * b){
    return (valuesHistoryTable[*((Move*)b)][1] / valuesRelativeHistoryTable[*((Move*)b)][1] )-  valuesHistoryTable[*((Move*) a)][1] / valuesRelativeHistoryTable[*((Move*)a)][1];
}


// Returns the value of comparation between two HistoryValues for White player
int relativeHistoryValuesComparator2(const void * a, const void * b){
    return (valuesHistoryTable[*((Move*)b)][2] / valuesRelativeHistoryTable[*((Move*)b)][2] )-  valuesHistoryTable[*((Move*) a)][2] / valuesRelativeHistoryTable[*((Move*)a)][2];
}

// Returns the value of comparation between two HistoryValues for White player
int relativeHistoryValuesComparator3(const void * a, const void * b){
    return (valuesHistoryTable[*((Move*)b)][3] / valuesRelativeHistoryTable[*((Move*)b)][3] )-  valuesHistoryTable[*((Move*) a)][3] / valuesRelativeHistoryTable[*((Move*)a)][3];
}
// Sorts the moves array using Relative History Heuristic
void sortMovesRelativeHistory(Move* moves, int size, int thread){
    if(thread==0)
        qsort(moves,size,sizeof(Move),relativeHistoryValuesComparator0);
    if(thread==1)
        qsort(moves,size,sizeof(Move),relativeHistoryValuesComparator1);
    if(thread==2)
        qsort(moves,size,sizeof(Move),relativeHistoryValuesComparator2);
    if(thread==3)
        qsort(moves,size,sizeof(Move),relativeHistoryValuesComparator3);
}
