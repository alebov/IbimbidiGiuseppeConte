#include "board.h"


#define DIM 200 //abbastanza pessimistica direi

// A structure to represent a stack 
typedef struct{ 
    int top; 
    BoardState array[DIM]; 
} Stack;


//struct Stack* createStack(unsigned capacity); 
int isEmpty();
int isDraw(BoardState board);
void push( BoardState board); 
void pop(); 
void initStack();