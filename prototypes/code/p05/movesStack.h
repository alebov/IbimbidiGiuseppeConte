#include "board.h"


#define DIM 200 //abbastanza pessimistica direi

// A structure to represent a stack 
typedef struct{ 
    int top; 
    int playedTop;
    BoardState array[DIM]; 
} Stack;


//struct Stack* createStack(unsigned capacity); 
int isEmpty();
int isDraw(BoardState board);
void push( BoardState board);      //aggiunta di una board esaminata
void pop(); 
void initStack();
void pushPlayed(BoardState board); //aggiunta di una board veramente giocata
void resetStack();                   //rimuove tutte le board esaminate e tiene solo quelle giocate
void printDim();