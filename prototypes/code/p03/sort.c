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

// Returns move History value
int getHistoryValue(Move move){
    return valuesHistoryTable[move];
}

// Returns the value of comparation between two HistoryValues for White player
int historyValuesComparator(const void * a, const void * b){
    return ((HistoryValue*)b)->value-((HistoryValue*)a)->value;
}

// Sorts the moves array using History Heuristic
void sortMovesHistory(Move* moves, int size){
    HistoryValue values[size];
    int i;
    // Creates a vector with move and History value
    for(i=0;i<size;i++){
        values[i].move=moves[i];
        values[i].value=getHistoryValue(moves[i]);
    }
    // Sorts the vector
    qsort(values, size, sizeof(HistoryValue), historyValuesComparator);
    
    // Updates moves vector
    for(i=0;i<size;i++){
        moves[i]=values[i].move;
    }
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

// Returns move History value
int getRelativeHistoryValue(Move move){
    return valuesRelativeHistoryTable[move];
}

// Returns the value of comparation between two RelativeHistoryValues for White player
int relativeHistoryValuesComparator(const void * a, const void * b){
    double val_a=(double)getHistoryValue(((RelativeHistoryValue*)a)->move)/(double)(((RelativeHistoryValue*)a)->value);
    double val_b=(double)getHistoryValue(((RelativeHistoryValue*)b)->move)/(double)(((RelativeHistoryValue*)b)->value);
    double res=val_b-val_a;
    if(res<0){
        return -1;
    }else{
        if(res==0){
            return 0;
        }else{
            return 1;
        }
    }
}

// Sorts the moves array using Relative History Heuristic
void sortMovesRelativeHistory(Move* moves, int size){
    RelativeHistoryValue values[size];

    int i;
    // Creates a vector with move and RelativeHistory value
    for(i=0;i<size;i++){
        values[i].move=moves[i];
        values[i].value=getRelativeHistoryValue(moves[i]);
    }

    // Sorts the vector
    qsort(values, size, sizeof(RelativeHistoryValue), relativeHistoryValuesComparator);
    
    double res;
    // Updates moves vector
    for(i=0;i<size;i++){
        moves[i]=values[i].move;
    }
}