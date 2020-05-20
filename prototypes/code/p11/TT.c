#pragma once
#include "TT.h"
#include "main.h"
//#include <pthread.h>
#include <stdint.h>

Entry whiteTT[TTDIM];
Entry blackTT[TTDIM];
//pthread_mutex_t TABLE_LOCK_WHITE[TTDIM];
//pthread_mutex_t TABLE_LOCK_BLACK[TTDIM];

void initTT(){
  int i;
  for(i=0;i<TTDIM;i++){
    whiteTT[i].type=EMTPY_ENTRY;
    blackTT[i].type=EMTPY_ENTRY;
//    pthread_mutex_init(&TABLE_LOCK_WHITE[i],NULL);
//    pthread_mutex_init(&TABLE_LOCK_BLACK[i],NULL);
  }
}

//to add a colmpetly evaluate node NB: you must not call it if the node is a cut-node
void addWhite(Hash hash, int value, int depth, Move bestMove,Type type){
  int h = hash & BITMASK;
//  pthread_mutex_lock(&TABLE_LOCK_WHITE[h]);
  if(whiteTT[h].type==EMTPY_ENTRY || whiteTT[h].depth<=depth){
    whiteTT[h].hash = hash;
    whiteTT[h].value = value;
    whiteTT[h].depth = depth;
    whiteTT[h].bestMove = bestMove;
    whiteTT[h].type=type;
  }
//  pthread_mutex_unlock(&TABLE_LOCK_WHITE[h]);
}

//to know if the board is already been evaluate whne min or max start
Entry findWhite(Hash hash){
  int h = hash & BITMASK;
//  pthread_mutex_lock(&TABLE_LOCK_WHITE[h]);
  Entry result = whiteTT[h];
//  pthread_mutex_unlock(&TABLE_LOCK_WHITE[h]);
  if((result.hash ) == hash){
    return result;
  } 
  else{
    result.type = EMTPY_ENTRY;
    return result;
  }
}

void addBlack(Hash hash, int value, int depth, Move bestMove,Type type){
  int h = hash & BITMASK;
//  pthread_mutex_lock(&TABLE_LOCK_BLACK[h]);
  if(blackTT[h].type==EMTPY_ENTRY || blackTT[h].depth<depth){
    blackTT[h].hash = hash;
    blackTT[h].value = value;
    blackTT[h].depth = depth;
    blackTT[h].bestMove = bestMove;
    blackTT[h].type=type;
  }
//  pthread_mutex_unlock(&TABLE_LOCK_BLACK[h]);
}

// Chech whether the found entry is valid or not
bool isValidEntry(Entry entry){
  if(entry.type == EMTPY_ENTRY) return false;
  else return true;
}

Entry findBlack(Hash hash){
  int h = hash & BITMASK;
//  pthread_mutex_lock(&TABLE_LOCK_BLACK[h]);
  Entry result = blackTT[h];
//  pthread_mutex_unlock(&TABLE_LOCK_BLACK[h]);
  if(result.hash  == hash){
    return result;
  }
  else{
    result.type = EMTPY_ENTRY;
    return result;
  }
}
