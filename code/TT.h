
#include "hash.h"
#include "moves.h"
#include "main.h"

#define TTDIM 16777216
#define BITMASK 0b111111111111111111111111
#define EMTPY_ENTRY 0
#define EXACT_ENTRY 1
#define UPPER_ENTRY 2
#define LOWER_ENTRY 3

typedef uint8_t Type;


typedef struct{
    Hash hash;
    Score value;
    int8_t depth; 
    Move bestMove;
    Type type;
} Entry;

void initTT();
void addWhite(Hash hash, int value,int depth,Move bestMove, Type type);
void addBlack(Hash hash, int value,int depth,Move bestMove, Type type);
Entry findWhite(Hash hash);
Entry findBlack(Hash hash);
bool isValidEntry(Entry entry);
void printEntry(Entry entry);
void addFirstWhite(Hash hash, int value,int depth,Move bestMove);
void addFirstBlack(Hash hash, int value,int depth,Move bestMove);