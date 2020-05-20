#pragma once

#include "hash.h"
#include <time.h>
#include <stdlib.h>
#include "board.h"


#define A 54059 /* a prime */
#define B 76963 /* another prime */
//#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

//1-string hash(quello dell'anno scorso fatto da mattia)
Hash stringHash(BoardState* board) {
    Hash hash = FIRSTH;
    int i;
    for (i = 0; i < 9; i++) {
        hash = (hash * A) ^ (((board->Board[i]) & 0b1111111111111111U) * B);
        hash = (hash * A) ^ ((board->Board[i] >> 16U) * B);
    }
    return hash;
}


//table da 81*4
Hash table[81][4];
//inizializza la table per lo zobrist
void initZobrist(){
	srand(time(NULL));   
	int i,j;
	for(i=0;i<81;i++)
		for(j=0;j<3;j++){
			table[i][j]=rand(); 
			table[i][j]=table[i][j]<<32U;
			table[i][j]=table[i][j] ^ rand();
		}
			
}

//2-zobrist puro
Hash zobrist(BoardState* board){
	int i,j;
	int h=31;
	for(i=0; i< 9; i++)
		for(j=0;j<9;j++)
			h=h ^ table[i*9+j][GET_CELL_STATE(board->Board,i,j)];
	return h;
}

//3-zobrist forse un po' più veloce(ma non credo troppo) 
Hash fastZobrist(BoardState* board){
	int i,j;
	int h=31;
	int cell;
	for(i=0; i< 9; i++){
		for(j=0;j<9;j++){
			cell=GET_CELL_STATE(board->Board,i,j);
			if(cell!=0)
				h=h ^ table[i*9+j][cell];
		}
	}
	return h;
}

//table da 81*4

Hash fastTable[18];
//inizializza la table fast hash
int mask=0b111111111U;
void initFast(){
	srand(time(NULL));   
	int i,j;
	for(i=0;i<18;i++)
		fastTable[i]=rand(); 
		fastTable[i]=fastTable[i]<<32U;
		fastTable[i]=fastTable[i] ^ rand();		
}
//4-fast hash dovrebbe essere più veloce di zobrist(ma essendomelo inventato io non ho molte aspettative)
//fa 18 cicli al posto che 81
Hash fastHash(BoardState* board){
	int i;
	Hash hash=31;
	for(i=0;i<9;i++){
		hash=hash ^ fastTable[board->Board[i] && mask];
	}
	for(i;i<18;i++){
		hash=hash ^ fastTable[board->Board[i]>>9];
	}
	return hash;
}




//5-fake hash(maybe too fast) sicuramente da collisoni ---- serve per vedere se il test sulle collisioni funziona
inline Hash fakeHash(BoardState* board) {
    Hash hash= FIRSTH;
	hash=hash ^ board->Board[0];
	hash=hash ^ board->Board[1];
	hash=hash ^ board->Board[2];
	hash=hash ^ board->Board[3];
	hash=hash ^ board->Board[4];
	hash = (hash << 32U) ^ hash;
	hash=hash ^ board->Board[5];
	hash=hash ^ board->Board[6];
	hash=hash ^ board->Board[7];
	hash=hash ^ board->Board[8];
	hash=hash ^ board->Board[9];
    return hash;
}
