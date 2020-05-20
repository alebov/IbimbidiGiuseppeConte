#pragma once

#include "hash.h"
#include "moves.h"
#include "main.h"

// 500 mi sembra molto bassa considerando che esploriamo milioni di mosse
//1 gb per ogni table 2gb in totale
//#define TTDIM 67108864  //1gb / 16 bit 
#define TTDIM 16777216 //2 alla 24
#define BITMASK 0b111111111111111111111111
#define EMTPY_ENTRY 0
#define EXACT_ENTRY 1
#define UPPER_ENTRY 2
#define LOWER_ENTRY 3

typedef uint8_t Type;

typedef struct{
    Hash hash;      //8 byte
    Score value;    //2 byte
    int8_t depth;   //1 byte   
    Move bestMove;  //2 byte
    Type type;      //1  byte
} Entry;            //secondo il mio compilatore il totale fa 16 non so perchè

void initTT();
void addWhite(Hash hash, int value,int depth,Move bestMove, Type type); //to add a colmpetly evaluate node NB: you must not call it if the node is a cut-node
Entry findWhite(Hash hash);                                  //to know if the board is already been evaluate whne min or max start
void addBlack(Hash hash, int value,int depth,Move bestMove, Type type);
Entry findBlack(Hash hash);
int isValidEntry(Entry entry);
