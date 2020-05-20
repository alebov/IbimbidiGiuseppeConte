#pragma once

#include "hash.h"
#include "moves.h"
#include "main.h"

// 500 mi sembra molto bassa considerando che esploriamo milioni di mosse
#define TTDIM 500
#define EMTPY_ENTRY 0
#define EXACT_ENTRY 1;
typedef uint8_t Type;

typedef struct{
    Hash hash;
    int value;
    int depth;
    Move bestMove;
    Type type;
} Entry;

void initTT();
void addWhite(Hash hash, int value,int depth,Move bestMove); //to add a colmpetly evaluate node NB: you must not call it if the node is a cut-node
Entry findWhite(Hash hash);                                  //to know if the board is already been evaluate whne min or max start
void addBlack(Hash hash, int value,int depth,Move bestMove);
Entry findBlack(Hash hash);
Bool isValidEntry(Entry entry);

/*example:

int max(...){
    Hash hash=HashFunction(boardState);
    Entry=findWhite(hash);
    int boardValue;
    if(entry!=null){ //avevo già valutato quella board in precedenza
        if(entry.depth>=depth) //avevo già valutato quel nodo esplorandolo ad una profondità maggiore o uguale
            return entry.value;
        boardValue=min(moveWhite(boardState,entry.bestMove),alpha,beta, depth-1);
        if(boardValue>alpha)
            alpha=boardValue
        if(alpha>=beta)
            return boardValue;
    }

    //continua con min max normale

    ....
    ....
    ....

    alla fine
    addWhite(boardState,boardValue,depth, bestMove) da fare solo se prima non ho tagliato

}



*/
