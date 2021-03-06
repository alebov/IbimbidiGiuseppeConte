#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "board.h"
#include "movesStack.h"
   

Stack stack;    //stack definito staticamente


void pushPlayed(BoardState board){
    stack.array[stack.playedTop]=board;
    stack.top++;
    stack.playedTop++;
}

void initStack(){
    stack.playedTop=0;
    stack.top=0;
}

int isFull() 
{ 
    return stack.top == DIM;
}  
  
void resetStack(){
    stack.top=stack.playedTop;
}

int isEmpty() 
{ 
    return stack.top  == 0;
} 
  

void push(BoardState board) 
{ 
    if (isFull(stack)) 
        return; 
        
    stack.array[stack.top]=board;
    stack.top++;
    //printf("Board pushed to stack\n", board); 
} 


void pop() 
{ 
    /*
    if (isEmpty(stack)) 
        return;
        */
    stack.top--;
    //printf("Top board removed from stack\n"); 
} 
  

int isDraw(BoardState board) {
    int i;
    int found = 0;
    for(i=0; i < stack.top && !found; i++)
        if(isEqual(stack.array[i],board))
            found = 1;
    return found;
}

  
void printDim(){

    printf("totale: %d",stack.top);
    printf("giocate: %d",stack.playedTop);
}