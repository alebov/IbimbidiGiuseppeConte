#pragma once
#include "TTbase.h"
#include "main.h"

Entry whiteTT[TTDIM];
Entry blackTT[TTDIM];


void initTT(){
    int i;
    for(i=0;i<TTDIM;i++)
      {
        whiteTT[i].type=EMTPY_ENTRY;
        whiteTT[i].type=EMTPY_ENTRY;
      }


}

//to add a colmpetly evaluate node NB: you must not call it if the node is a cut-node
void addWhite(Hash hash, int value, int depth, Move bestMove){
  //printf("added white entry\n");
  int h = hash % TTDIM;
  if(whiteTT[h].type==EMTPY_ENTRY || whiteTT[h].depth<=depth){//always+depth
    whiteTT[h].hash = hash;
    whiteTT[h].value = value;
    whiteTT[h].depth = depth;
    whiteTT[h].bestMove = bestMove;
    whiteTT[h].type=EXACT_ENTRY;
  }
}

//to know if the board is already been evaluate whne min or max start
Entry findWhite(Hash hash){
  int h = hash % TTDIM;
  Entry result = whiteTT[h];
  if((result.hash ) == hash){  // If the entry is present
    //printf("finded white entry\n");
    return result;
  } 
  else{
    //printf("find white entry miss\n");
    result.type = EMTPY_ENTRY; // This means that the entry has not been found
    return result;
  }
}

void addBlack(Hash hash, int value, int depth, Move bestMove){
  //printf("added black entry\n");
  int h = hash % TTDIM;
  if(blackTT[h].type==EMTPY_ENTRY || blackTT[h].depth<depth){
    blackTT[h].hash = hash;
    blackTT[h].value = value;
    blackTT[h].depth = depth;
    blackTT[h].bestMove = bestMove;
    blackTT[h].type=EXACT_ENTRY;
    }
}


// Chech whether the found entry is valid or not
Bool isValidEntry(Entry entry){
  if(entry.type == EMTPY_ENTRY)
    return false;
  else
    return true;
}

Entry findBlack(Hash hash){
  
  int h = hash % TTDIM;
  Entry result = blackTT[h];
  if(result.hash  == hash){ // If the entry is present
    //printf("finded black entry\n");
    return result;
  }
  else{
    //printf("find black entry miss\n");
    result.type = EMTPY_ENTRY; // This means that the entry has not been found
    return result;
  }
}
