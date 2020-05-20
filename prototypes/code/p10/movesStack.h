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
int isDraw(BoardState board, int thread);
void push(BoardState board, int thread);            //aggiunta di una board esaminata
void pop(int thread); 
void initStack(int numThreads);
void pushPlayed(BoardState board, int numThreads);  //aggiunta di una board veramente giocata
void resetStack(int numThreads);                    //rimuove tutte le board esaminate e tiene solo quelle giocate
