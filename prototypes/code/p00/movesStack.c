#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "board.h"
#include "movesStack.h"
   

Stack stack;    //stack definito staticamente

/*
struct Stack* createStack(unsigned capacity) 
{ 
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); 
    stack->capacity = capacity; 
    stack->top = -1; 
    stack->array = (BoardState*)malloc(stack->capacity * sizeof(BoardState)); 
    return stack; 
} 


void deleteStack(struct Stack* stack) 
{ 
    free(stack->array);
    free(stack);
} 
*/

void initStack(){
    stack.top=0;
}

int isFull() 
{ 
    return stack.top == DIM;
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

  

