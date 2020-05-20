#pragma once

#include "hash.h"
#include "moves.h"
#include "main.h"

#define TTDIM 67108864 
#define EMTPY_ENTRY 0
#define EXACT_ENTRY 1
#define UPPER_ENTRY 2
#define LOWER_ENTRY 3

typedef uint8_t Type;

typedef struct{} EmptyStruct;

typedef struct{
    Hash hash;      //8 byte
    Score value;    //2 byte
    int8_t depth;   //1 byte   
    Move bestMove;  //2 byte
    Type type;      //1  byte
} Entry;            //secondo il mio compilatore il totale fa 16 non so perchè

void initTT();
void addWhite(Hash hash, int value,int depth,Move bestMove, Type type);
void addBlack(Hash hash, int value,int depth,Move bestMove, Type type);
Entry findWhite(Hash hash);
Entry findBlack(Hash hash);
bool isValidEntry(Entry entry);
