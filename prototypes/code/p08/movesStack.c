#include "board.h"
#include "movesStack.h"
#include "main.h"
#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 

Stack *stack;

void initStack(int numThreads){
    int i;
    stack=(Stack*) malloc(sizeof(Stack)*numThreads);
    for(i=0;i<numThreads;i++){
        stack[i].playedTop=0;
        stack[i].top=0;
    }
}

void pushPlayed(BoardState board, int numThreads){
    int i;
    for(i=0;i<numThreads;i++){
        stack[i].array[stack[i].playedTop]=board;
        stack[i].top++;
        stack[i].playedTop++;
    }
}

void resetStack(int numThreads){
    int i;
    for(i=0;i<numThreads;i++){
        stack[i].top=stack[i].playedTop;
    }
}

void push(BoardState board, int thread) 
{ 
    stack[thread].array[stack[thread].top]=board;
    stack[thread].top++;
} 

void pop(int thread) 
{ 
    stack[thread].top--;
} 

int isDraw(BoardState board, int thread) {
    int i;
    int found = 0;
    for(i=0; i < stack[thread].top && !found; i++)
        if(isEqual(stack[thread].array[i],board))
            found = 1;
    return found;
}

  
